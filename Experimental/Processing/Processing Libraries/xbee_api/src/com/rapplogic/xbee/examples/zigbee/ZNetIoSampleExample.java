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

package com.rapplogic.xbee.examples.zigbee;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import com.rapplogic.xbee.api.ApiId;
import com.rapplogic.xbee.api.PacketListener;
import com.rapplogic.xbee.api.XBee;
import com.rapplogic.xbee.api.XBeeResponse;
import com.rapplogic.xbee.api.wpan.IoSample;
import com.rapplogic.xbee.api.wpan.RxResponseIoSample;
import com.rapplogic.xbee.api.zigbee.ZNetRxIoSampleResponse;

/**
 * Series 2 XBees -- Example of receiving I/O samples.  To configure your radio for this example, connect 
 * your end device to your serial connection and run the configureIOSamples() method
 * in ZNetApiAtTest.
 *  
 * @author andrew
 *
 */
public class ZNetIoSampleExample implements PacketListener {

	private final static Logger log = Logger.getLogger(ZNetIoSampleExample.class);
	
	private ZNetIoSampleExample() throws Exception {
		XBee xbee = new XBee();		

		try {			
			// replace with the com port of your XBee coordinator
			xbee.open("/dev/tty.usbserial-A6005v5M", 9600);
			xbee.addPacketListener(this);
			
			// wait forever
			synchronized(this) { this.wait(); }
		} finally {
			xbee.close();
		}
	}
	
	/**
	 * Called by XBee API thread when a packet is received
	 */
	public void processResponse(XBeeResponse response) {
		// This is a I/O sample response.  You will only get this is you are connected to a Coordinator that is configured to
		// receive I/O samples from a remote XBee.
		
		if (response.getApiId() == ApiId.ZNET_IO_SAMPLE_RESPONSE) {
			ZNetRxIoSampleResponse ioSample = (ZNetRxIoSampleResponse) response;
			
			log.debug("received i/o sample packet.  contains analog is " + ioSample.containsAnalog() + ", contains digital is " + ioSample.containsDigital());
			
			// check the value of the input pins
			log.debug("pin 20 (DO) digital is " + ioSample.isD0On());
			log.debug("pin 19 (D1) analog is " + ioSample.getAnalog1());
		}
	}
	
	public static void main(String[] args) throws Exception {
		// init log4j
		PropertyConfigurator.configure("log4j.properties");
		new ZNetIoSampleExample();
	}
}
