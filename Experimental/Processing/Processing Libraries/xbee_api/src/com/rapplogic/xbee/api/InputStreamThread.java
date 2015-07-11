/**
 * Copyright (c) 2008 Andrew Rapp. All rights reserved.
 *  
 * This file is part of XBee-API.
 *  
 * XBee-API is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *  
 * XBee-API is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with XBee-API.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.rapplogic.xbee.api;

import java.io.IOException;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingQueue;

import org.apache.log4j.Logger;

import com.rapplogic.xbee.XBeeConnection;
import com.rapplogic.xbee.util.ByteUtils;

/**
 * Reads data from the input stream and hands off to PacketParser for packet parsing.
 * Notifies XBee class when a new packet is parsed
 * <p/>
 * @author andrew
 *
 */
public class InputStreamThread implements Runnable {
	
	private final static Logger log = Logger.getLogger(InputStreamThread.class);
	
	private Thread thread;
	private ExecutorService listenerPool;
	private volatile boolean done = false;
	private final XBeeConnection connection;
	private XBeeConfiguration conf;
	
	public XBeeConnection getXBeeConnection() {
		return connection;
	}

	private final BlockingQueue<XBeeResponse> responseQueue = new LinkedBlockingQueue<XBeeResponse>();
	
	// TODO use weak references
	private final List<PacketListener> packetListenerList = new LinkedList<PacketListener>();
	
	public List<PacketListener> getPacketListenerList() {
		return packetListenerList;
	}

	public BlockingQueue<XBeeResponse> getResponseQueue() {
		return responseQueue;
	}

	public InputStreamThread(final XBeeConnection connection, XBeeConfiguration conf) {
		this.connection = connection;
		this.conf = conf;
		
//		executor = Executors.newFixedThreadPool(1);
//		executor.submit(this);

        // Create an executor to deliver incoming packets to listeners. We'll use a single
        // thread with an unbounded queue.
		listenerPool = Executors.newSingleThreadExecutor();
		
		thread = new Thread(this);
		thread.setName("InputStreamThread");
		thread.start();
		
		log.debug("starting packet parser thread");
	}
	
	private void addResponse(final XBeeResponse response) throws InterruptedException {
		
		// trim the queue
		while (responseQueue.size() >= (conf.getMaxQueueSize() - 1)) {
			log.debug("Response queue has reached the maximum size of " + conf.getMaxQueueSize() + " packets.  Trimming a packet from head of queue to make room");
			responseQueue.poll();
		}
		
		if (conf.getResponseQueueFilter() != null) {
			if (conf.getResponseQueueFilter().accept(response)) {
				responseQueue.put(response);
			}
		} else {
			responseQueue.put(response);	
		}
		
		listenerPool.submit(new Runnable() {
			public void run() {
				// must synchronize to avoid  java.util.ConcurrentModificationException at java.util.AbstractList$Itr.checkForComodification(Unknown Source)
				// this occurs if packet listener add/remove is called while we are iterating
				synchronized (packetListenerList) {
					for (PacketListener pl : packetListenerList) {
						try {
							if (pl != null) {
								pl.processResponse(response);	
							} else {
								log.warn("PacketListener is null, size is " + packetListenerList.size());
							}
						} catch (Throwable th) {
							log.warn("Exception in packet listener", th);
						}
					}			
				}				
			}
		});
	}
	
	public void run() {

		int val = -1;
		
		XBeeResponse response = null;
		PacketParser packetStream = null;

		try {
			while (!done) {
				try {
					if (connection.getInputStream().available() > 0) {
						log.debug("About to read from input stream");
						val = connection.getInputStream().read();
						log.debug("Read " + ByteUtils.formatByte(val) + " from input stream");
						
						if (val == XBeePacket.SpecialByte.START_BYTE.getValue()) {
							packetStream = new PacketParser(connection.getInputStream());
							response = packetStream.parsePacket();
							
							if (log.isInfoEnabled()) {
								log.info("Received packet from XBee: " + response);	
								log.debug("Received packet: int[] packet = {" + ByteUtils.toBase16(response.getRawPacketBytes(), ", ") + "};");	
							}
							
							// success
							this.addResponse(response);
						} else {
							log.warn("expected start byte but got this " + ByteUtils.toBase16(val) + ", discarding");
						}
					} else {
						log.debug("No data available.. waiting for new data event");
						
						// we will wait here for RXTX to notify us of new data
						synchronized (this.connection) {
							// There's a chance that we got notified after the first in.available check
							if (connection.getInputStream().available() > 0) {
								continue;
							}
							
							// wait until new data arrives
							this.connection.wait();
						}	
					}				
				} catch (Exception e) {
					if (e instanceof InterruptedException) throw ((InterruptedException)e);
					
					log.error("Error while parsing packet:", e);
					
					if (e instanceof IOException) {
						// this is thrown by RXTX if the serial device unplugged while we are reading data; if we are waiting then it will waiting forever
						log.error("Serial device IOException.. exiting");
						break;
					}
				}
			}
		} catch(InterruptedException ie) {
			// We've been told to stop -- the user called the close() method			
			log.info("Packet parser thread was interrupted.  This occurs when close() is called");
		} finally {
			if (connection != null) {
				connection.close();
			}
			
			if (listenerPool != null) {
				try {
					listenerPool.shutdownNow();
				} catch (Throwable t) {
					log.warn("Failed to shutdown listner thread pool", t);
				}
			}
		}
		
		log.info("InputStreamThread is exiting");
	}

	public void setDone(boolean done) {
		this.done = done;
	}
	
	public void interrupt() {
		if (thread != null) {
			try {
				thread.interrupt();	
			} catch (Exception e) {
				log.warn("Error interrupting parser thread", e);
			}
		}
	}
}