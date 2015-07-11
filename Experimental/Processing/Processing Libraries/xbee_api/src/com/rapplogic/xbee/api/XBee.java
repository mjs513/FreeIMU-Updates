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
import java.util.concurrent.TimeUnit;

import org.apache.log4j.Logger;

import com.rapplogic.xbee.RxTxSerialComm;
import com.rapplogic.xbee.XBeeConnection;
import com.rapplogic.xbee.util.ByteUtils;

/**
 * This is an API for communicating with Digi XBee 802.15.4 and ZigBee radios
 * via the serial port
 * <p/>
 * @author Andrew Rapp <andrew.rapp at gmail>
 * 
 */
public class XBee implements IXBee {

	private final static Logger log = Logger.getLogger(XBee.class);
	
	// object to synchronize on to protect access to sendPacket
	private Object sendPacketBlock = new Object();
	private XBeeConnection xbeeConnection;
	private InputStreamThread parser;	
	private XBeeConfiguration conf;
	
	public XBee() {
		this.conf = new XBeeConfiguration().withMaxQueueSize(100).withStartupChecks(true);
		
		Runtime.getRuntime().addShutdownHook(new Thread() {
		    public void run() { 
		    	if (isConnected()) {
		    		log.info("ShutdownHook is closing connection");
		    		close();
		    	}
		    }
		});
	}

	public XBee(XBeeConfiguration conf) {
		this.conf = conf;
	}
	
	private void doStartupChecks() throws XBeeException {
		// Perform startup checks
		try {				
			AtCommandResponse ap = this.sendAtCommand(new AtCommand("AP"));
			
			if (!ap.isOk()) {
				throw new XBeeException("Attempt to query AP parameter failed");
			}
			
			if (ap.getValue()[0] != 2) {
				log.warn("XBee radio is in API mode without escape characters (AP=1).  The radio must be configured in API mode with escape bytes (AP=2) for use with this library.");
				
				log.info("Attempting to set AP to 2");
				ap = this.sendAtCommand(new AtCommand("AP", 2));
				
				if (ap.isOk()) {
					log.info("Successfully set AP mode to 2.  This setting will not persist a power cycle without the WR (write) command");	
				} else {
					throw new XBeeException("Attempt to set AP=2 failed");
				}
			} else {
				log.info("Radio is in correct AP mode (AP=2)");
			}

			ap = this.sendAtCommand(new AtCommand("HV"));
			
			if (!ap.isOk()) {
				throw new XBeeException("Attempt to query HV parameter failed");
			}
			
			if (ap.isOk()) {
				switch (ap.getValue()[0]) {
				case 0x17:
					log.info("XBee radio is Series 1");
					break;
				case 0x18:
					log.info("XBee radio is Series 1 Pro");
					break;
				case 0x19:
					log.info("XBee radio is Series 2");
					break;
				case 0x1a:
					log.info("XBee radio is Series 2 Pro");
					break;
				default:
					log.warn("Unknown radio type (HV): " + ap.getValue()[0]);
				}
			}	
			
			AtCommandResponse vr = this.sendAtCommand(new AtCommand("VR"));
			
			if (vr.isOk()) {
				log.info("Firmware version is " + ByteUtils.toBase16(vr.getValue()));
			}
			
			this.clearResponseQueue();
		} catch (XBeeTimeoutException ex) {
			throw new XBeeException("AT command timed-out while attempt to set/read in API mode.  The XBee radio must be in API mode (AP=2) to use with this library");
		}		
	}
	
	/**
	 * If XBeeConnection.startUpChecks is set to true (default), this method will check if the AP parameter
	 * is set correctly and attempt to update if AP=1.  If AP=0 (Transparent mode), an
	 * exception will be thrown.
	 */
	public void open(String port, int baudRate) throws XBeeException {
		try {
			if (this.isConnected()) {
				throw new IllegalStateException("Cannot open new connection -- existing connection is still open.  Please close first");
			}
			
			RxTxSerialComm serial = new RxTxSerialComm(); 
			serial.openSerialPort(port, baudRate);
			
			this.initConnection(serial);
		} catch (XBeeException e) {
			throw e;
		} catch (Exception e) {		
			throw new XBeeException(e);
		}
	}
	
	/**
	 * Allows a protocol specific implementation of XBeeConnection to be used instead of the default RXTX connection.
	 * The connection must already be established as the interface has no means to do so.
	 */
	public void initProviderConnection(XBeeConnection connection) throws XBeeException {
		if (this.isConnected()) {
			throw new IllegalStateException("Cannot open new connection -- existing connection is still open.  Please close first");
		}
		
		initConnection(connection);
	}
	
	private void initConnection(XBeeConnection conn) throws XBeeException {
		try {			
			this.xbeeConnection = conn;
			
			parser = new InputStreamThread(this.xbeeConnection, conf);
			
			// startup heuristics
			if (conf.isStartupChecks()) {
				this.doStartupChecks();
			}
		} catch (XBeeException e) {
			throw e;
		} catch (Exception e) {		
			throw new XBeeException(e);
		}
	}

	public void addPacketListener(PacketListener packetListener) { 
		if (parser == null) {
			throw new IllegalStateException("No connection");
		}
		
		synchronized (parser.getPacketListenerList()) {
			this.parser.getPacketListenerList().add(packetListener);	
		}
	}

	public void removePacketListener(PacketListener packetListener) {
		if (parser == null) {
			throw new IllegalStateException("No connection");
		}
		
		synchronized (parser.getPacketListenerList()) {
			this.parser.getPacketListenerList().remove(packetListener);
		}
	}
	
	public void sendRequest(XBeeRequest request) throws IOException {
		log.info("Sending request to XBee: " + request);
		this.sendPacket(request.getXBeePacket());
	}
	
	/** 
	 * It's possible for packets to get interspersed if multiple threads send simultaneously.  
	 * This method is not thread-safe because doing so would introduce a synchronized performance penalty 
	 * for the vast majority of users that will not never need thread safety.
	 * That said, it is responsibility of the user to provide synchronization if multiple threads are sending.
	 * 
	 * Not thread safe.
	 *  
	 * @param packet
	 * @throws IOException
	 */
	public void sendPacket(XBeePacket packet) throws IOException {	
		this.sendPacket(packet.getByteArray());
	}
	
	/**
	 * This exists solely for the XMPP project.  Use sendRequest instead
	 * 
	 * Not Thread Safe
	 * 
	 * @param packet
	 * @throws RuntimeException when serial device is disconnected
	 */
	public void sendPacket(int[] packet)  throws IOException {
		// TODO should we synchronize on read lock so we are sending/recv. simultaneously?
		// TODO call request listener with byte array
		
		if (!this.isConnected()) {
			throw new RuntimeException("XBee is not connected");
		}
		
		if (log.isInfoEnabled()) {
			log.info("sending packet to XBee " + ByteUtils.toBase16(packet));	
		}

        for (int aPacket : packet) {
        	xbeeConnection.getOutputStream().write(aPacket);
        }

        xbeeConnection.getOutputStream().flush();
	}

	/**
	 * Sends an XBeeRequest though the XBee interface in an asynchronous manner, such that
	 * it will return immediately, without waiting for a response.
	 * Refer to the getResponse method for obtaining a response
	 * 
	 * Not thread safe
	 * 
	 * @param request
	 * @throws XBeeException
	 */
	public void sendAsynchronous(XBeeRequest request) throws XBeeException {

		try {
			this.sendRequest(request);			
		} catch (Exception e) {
			throw new XBeeException(e);
		}
	}

	/**
	 * Uses sendSynchronous to send an AtCommand and collect the response
	 * <p/>
	 * Timeout value is fixed at 5 seconds
	 * 
	 * @deprecated Use this.sendSynchronous(command, timeout);
	 * @param command
	 * @return
	 * @throws XBeeException
	 */
	public AtCommandResponse sendAtCommand(AtCommand command) throws XBeeException {
		return (AtCommandResponse) this.sendSynchronous(command, 5000);
	}
	
	/**
	 * Synchronous method for sending an XBeeRequest and obtaining the 
	 * corresponding response (response that has same frame id).
	 * <p/>
	 * This method returns the first response object with a matching frame id, within the timeout
	 * period, so it is important to use a unique frame id (relative to previous subsequent requests).
	 * <p/>
	 * This method must only be called with requests that receive a response of
	 * type XBeeFrameIdResponse.  All other request types will timeout.
	 * <p/>
	 * Keep in mind responses received here will also be available through the getResponse method
	 * and the packet listener.  If you would prefer to not have these responses added to the response queue,
	 * you can add a ResponseQueueFilter via XBeeConfiguration to ignore packets that are sent in response to
	 * a request.  Another alternative is to call clearResponseQueue prior to calling this method.
	 * <p/>
	 * It is recommended to use a timeout of at least 5 seconds, since some responses can take a few seconds or more
	 * (e.g. if remote radio is not powered on).
	 * <p/>
	 * This method is thread-safe 
	 * 
	 * @param xbeeRequest
	 * 
	 * @return
	 * @throws XBeeException 
	 * @throws XBeeTimeoutException thrown if no matching response is identified
	 */
	public XBeeResponse sendSynchronous(final XBeeRequest xbeeRequest, int timeout) throws XBeeTimeoutException, XBeeException {		
		if (xbeeRequest.getFrameId() == XBeeRequest.NO_RESPONSE_FRAME_ID) {
			throw new XBeeException("Frame Id cannot be 0 for a synchronous call -- it will always timeout as there is no response!");
		}		

		PacketListener pl = null;
		
		try {
			final List<XBeeResponse> container = new LinkedList<XBeeResponse>();

			// this makes it thread safe -- prevents multiple threads from writing to output stream simultaneously
			synchronized (sendPacketBlock) {
				this.sendRequest(xbeeRequest);	
			}
			
			pl = new PacketListener() {
				// TODO handle error response as well
				public void processResponse(XBeeResponse response) {
					if (response instanceof XBeeFrameIdResponse && ((XBeeFrameIdResponse)response).getFrameId() == xbeeRequest.getFrameId()) {
						// frame id matches -- yay we found it
						container.add(response);
						
						synchronized(container) {
							container.notify();	
						}
						
					}
				}
			};
			
			this.addPacketListener(pl);
			
			synchronized (container) {
				try {
					container.wait(timeout);
				} catch (InterruptedException e) { }
			}
			
			if (container.size() == 0) {
				// we didn't find a matching packet
				throw new XBeeTimeoutException();
			}
			
			return (XBeeResponse) container.get(0);
		} catch (IOException io) {
			throw new XBeeException(io);
		} finally {
			if (pl != null) {
				this.removePacketListener(pl);
			}
		}
	}
	
	/**
	 * Uses sendSynchronous timeout defined in XBeeConfiguration (default is 5000ms)
	 */
	public XBeeResponse sendSynchronous(final XBeeRequest request) throws XBeeTimeoutException, XBeeException {		
		return this.sendSynchronous(request, conf.getSendSynchronousTimeout());
	}
	
		
	/**
	 * Same as getResponse(int) but does not timeout.
	 * It's highly recommend that you always use a timeout because
	 * if the serial connection dies under certain conditions, you will end up waiting forever!
	 * <p/>
	 * Consider using the PacketListener for asynchronous (non-blocking) behavior
	 * 
	 * @return
	 * @throws XBeeException 
	 */
	public XBeeResponse getResponse() throws XBeeException {
		return getResponseTimeout(null);
	}
	
	/**
	 * This method returns an XBeeResponse from the queue, if available, or
	 * waits up to "timeout" milliseconds for a response.
	 * <p/>
	 * There are three possible outcomes:
	 * <p/>
	 * 1.  A packet is returned within "timeout" milliseconds <br/>
	 * 2.  An XBeeTimeoutException is thrown (i.e. queue was empty for duration of timeout) <br/>
	 * 3.  Null is returned if timeout is 0 and queue is empty. <br/>
	 * <p/>
	 * @param timeout milliseconds to wait for a response.  A value of zero disables the timeout
	 * @return
	 * @throws XBeeException
	 * @throws XBeeTimeoutException if timeout occurs before a response is received
	 */
	public XBeeResponse getResponse(int timeout) throws XBeeException, XBeeTimeoutException {
		return this.getResponseTimeout(timeout);
	}
	
	private XBeeResponse getResponseTimeout(Integer timeout) throws XBeeException, XBeeTimeoutException {
		XBeeResponse response;
		try {
			if (timeout != null) {
				response = parser.getResponseQueue().poll(timeout, TimeUnit.MILLISECONDS);	
			} else {
				response = parser.getResponseQueue().take();
			}
		} catch (InterruptedException e) {
			throw new XBeeException("Error while attempting to remove packet from queue", e);
		}
		
		if (response == null && timeout > 0) {
			throw new XBeeTimeoutException();
		}
		
		return response;
	}
	
	
	/**
	 * Shuts down RXTX and packet parser thread
	 */
	public void close() {		

		if (!this.isConnected()) {
			throw new IllegalStateException("XBee is not connected");
		}
		
		// shutdown parser thread
		if (parser != null) {
			parser.setDone(true);
			// interrupts thread, if waiting.  does not interrupt thread if blocking on read
			// serial port close will be closed prior to thread exit
			parser.interrupt();
		}
		
		try {
			this.xbeeConnection.getOutputStream().close();
		} catch (IOException e) {
			log.warn("Failed to close output stream", e);
		}
		
		parser = null;
		xbeeConnection = null;
	}

	/**
	 * Indicates if serial port connection has been established.
	 * The open method may be called if true it returned
	 * 
	 * @return
	 */
	public boolean isConnected() {
		try {
			if (parser.getXBeeConnection().getInputStream() != null && parser.getXBeeConnection().getOutputStream() != null) {
				return true;
			}
			
			return false;
		} catch (Exception e) {
			return false;
		}
	}
	
	// TODO move to its own class
	private int sequentialFrameId = 0xff;
	
	public int getCurrentFrameId() {
		// TODO move to separate class (e.g. FrameIdCounter)
		return sequentialFrameId;
	}
	
	/**
	 * This is useful for obtaining a frame id when composing your XBeeRequest.
	 * It will return frame ids in a sequential manner until the maximum is reached (0xff)
	 * and it flips to 1 and starts over.
	 * 
	 * Not Thread-safe
	 * 
	 * @return
	 */
	public int getNextFrameId() {
		if (sequentialFrameId == 0xff) {
			// flip
			sequentialFrameId = 1;
		} else {
			sequentialFrameId++;
		}
		
		return sequentialFrameId;
	}
	
	/**
	 * Updates the frame id.  Any value between 1 and ff is valid
	 * 
	 * @param val
	 * Jan 24, 2009
	 */
	public void updateFrameId(int val) {
		if (val <=0 || val > 0xff) {
			throw new IllegalArgumentException("invalid frame id");
		}
		
		this.sequentialFrameId = val;
	}		
	
	/**
	 * Removes all packets off of the response queue
	 */
	public void clearResponseQueue() {
		parser.getResponseQueue().clear();
	}
}
