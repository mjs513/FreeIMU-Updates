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

package com.rapplogic.xbee.examples.wpan;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import com.rapplogic.xbee.api.ApiId;
import com.rapplogic.xbee.api.ErrorResponse;
import com.rapplogic.xbee.api.XBee;
import com.rapplogic.xbee.api.XBeeResponse;
import com.rapplogic.xbee.api.wpan.IoSample;
import com.rapplogic.xbee.api.wpan.RxResponseIoSample;
import com.rapplogic.xbee.util.ByteUtils;

/**
 * Series 1 XBee -- receive IO samples from remote radio
 * In this example we are going to set pin 20 to analog input, pins 11 and 12 to digital input, and configure change detect for pin 12.
 * Change detect sends a sample whenever the remote XBee detects a change from or to ground; 
 * it does not detect the transition from open circuit to high or vice versa.
 *  
 * Configuration:
 * 
 * This assumes we are starting with the factory default settings and the XBee is in API mode (AP = 2)
 * 
 * Remote radio:
 * 
 * MY = 1 necessary?
 * sets pin 20 to analog input
 * D0 = 2
 * sets pin 11 to digital input
 * D4 = 3 
 * sets pin 12 to digital input for change detect
 * D7 = 3 
 * turn on change detect for D7.  we need a bit mask of 10000000 (if you can't convert base 2 to hex in your head either, it's 0x80)
 * why 10000000?  we place a 1 in the position of each input we want to enable, so if you want to enable for D4 and D7 it would be 10010000
 * IC = 80
 * we want 1 sample per packet
 * IT = 1
 * receive samples every 5 seconds
 * IR = 1388
 * (coordinator address) necessary? doesn't it always send to coordinator?
 * DL = 0
 * (save configuration) 
 * WR 
 * 
 * Coordinator:
 * 
 * (set as coordinator)
 * CE = 1 
 * (set address)
 * MY = 0 
 * (end device) necessary?
 * DL = 1 
 * 
 * 
 * @author andrew
 * 
 */
public class IoSamplesExample {

	private final static Logger log = Logger.getLogger(IoSamplesExample.class);
	
	private IoSamplesExample() throws Exception {
		XBee xbee = new XBee();		
		
		try {			
			xbee.open("/dev/tty.usbserial-A6005v5M", 9600);
	
			while (true) {

				try {
					XBeeResponse response = xbee.getResponse();
					
					log.info("Received i/o response: " + response);
					log.info("packet bytes is " + ByteUtils.toBase16(response.getPacketBytes()));
					
					if (response.isError()) {
						log.info("response contains errors", ((ErrorResponse)response).getException());
						continue;
					}
					
					if (response.getApiId() == ApiId.RX_16_IO_RESPONSE) {
						RxResponseIoSample ioSample = (RxResponseIoSample)response;
						
						// optionally output the rssi strength
						//log.info("rssi is " + ioSample.getRssi());
						
						// loops just once since IT = 1
						for (IoSample sample: ioSample.getSamples()) {		
							if (ioSample.containsAnalog()) {
								log.info("Analog pin 20 10-bit reading is " + sample.getAnalog0());
								log.info("Digital pin 11 is " + (sample.isD4On() ? "on" : "off"));
								log.info("Digital pin 12 is " + (sample.isD7On() ? "on" : "off"));
							} else {
								// we know it's change detect since analog was not sent
								log.info("Received change detect for Digital pin 12: " + (sample.isD7On() ? "on" : "off"));
							}
						}
					} else {
						// not what we expected
						log.info("Ignoring mystery packet " + response.toString());
					}
				} catch (Exception e) {
					log.error(e);
				}
			}
		} finally {
			xbee.close();
		}
	}
	
	public static void main(String[] args) throws Exception {
		// init log4j
		PropertyConfigurator.configure("log4j.properties");
		new IoSamplesExample();
	}
}
