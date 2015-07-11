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

package com.rapplogic.xbee;

import gnu.io.CommPortIdentifier;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
import gnu.io.UnsupportedCommOperationException;

import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.TooManyListenersException;

import org.apache.log4j.Logger;

import com.rapplogic.xbee.api.XBeeException;

/** 
 * This class encapsulates a RXTX serial port, providing access to input/output streams,
 * and notifying the subclass of new data events via the handleSerialData method.
 * 
 * @author andrew
 * 
 */
public class RxTxSerialComm implements XBeeConnection, SerialPortEventListener {

	private final static Logger log = Logger.getLogger(RxTxSerialComm.class);
	
	private InputStream inputStream;
	private OutputStream outputStream;

	private SerialPort serialPort;
	
	public RxTxSerialComm() {
	
	}
	
	public void openSerialPort(String port, int baudRate) throws PortInUseException, UnsupportedCommOperationException, TooManyListenersException, IOException, XBeeException {
		this.openSerialPort(port, "XBee", 0, baudRate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE, SerialPort.FLOWCONTROL_NONE);
	}
	
	public void openSerialPort(String port, String appName, int timeout, int baudRate) throws PortInUseException, UnsupportedCommOperationException, TooManyListenersException, IOException, XBeeException {
		this.openSerialPort(port, appName, timeout, baudRate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE, SerialPort.FLOWCONTROL_NONE);
	}
	
	@SuppressWarnings("unchecked")
	public void openSerialPort(String port, String appName, int timeout, int baudRate, int dataBits, int stopBits, int parity, int flowControl) throws PortInUseException, UnsupportedCommOperationException, TooManyListenersException, IOException, XBeeException {
		// Apparently you can't query for a specific port, but instead must iterate
		Enumeration<CommPortIdentifier> portList = CommPortIdentifier.getPortIdentifiers();
		
		CommPortIdentifier portId = null;

		boolean found = false;
		
		while (portList.hasMoreElements()) {

			portId = portList.nextElement();

			if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {

				//log.debug("Found port: " + portId.getName());

				if (portId.getName().equals(port)) {
					//log.debug("Using Port: " + portId.getName());
					found = true;
					break;
				}
			}
		}

		if (!found) {
			throw new XBeeException("Could not find port: " + port);
		}
		
		serialPort = (SerialPort) portId.open(appName, timeout);
		
		serialPort.setSerialPortParams(baudRate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
		serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);

		// activate the DATA_AVAILABLE notifier
		serialPort.notifyOnDataAvailable(true);
		
		// activate the OUTPUT_BUFFER_EMPTY notifier
		//serialPort.notifyOnOutputEmpty(true);
		
		serialPort.addEventListener(this);
		
		inputStream = serialPort.getInputStream();
		outputStream = new BufferedOutputStream(serialPort.getOutputStream());
	}

	/**
	 * Shuts down RXTX
	 */
	public void close() {
		try {
			serialPort.getInputStream().close();
		} catch (Exception e) {
			log.warn("Exception while closing input stream", e);
		}

		try {
			serialPort.getOutputStream().close();
		} catch (Exception e) {
			log.warn("Exception while closing output stream", e);
		}
		
		try {
			// this call blocks while thread is attempting to read from inputstream
			serialPort.close();
		} catch (Exception e) {
			log.warn("Exception while closing serial port");
		}
	}
	
	public OutputStream getOutputStream() {
		return outputStream;
	}

	public InputStream getInputStream() {
		return inputStream;
	}
	
	public void serialEvent(SerialPortEvent event) {
		
		switch (event.getEventType()) {	
			case SerialPortEvent.DATA_AVAILABLE:

				try {
					if (this.getInputStream().available() > 0) {
						try {
							log.debug("serialEvent: " + serialPort.getInputStream().available() + " bytes available");
							
							synchronized (this) {
								this.notify();										
							}
						} catch (Exception e) {
							log.error("Error in handleSerialData method", e);
						}				
					} else {
						log.warn("We were notified of new data but available() is returning 0");
					}
				} catch (IOException ex) {
					// it's best not to throw the exception because the RXTX thread may not be prepared to handle
					log.error("RXTX error in serialEvent method", ex);
				}
			default:
				log.debug("Ignoring serial port event type: " + event.getEventType());
		}		
	}
}