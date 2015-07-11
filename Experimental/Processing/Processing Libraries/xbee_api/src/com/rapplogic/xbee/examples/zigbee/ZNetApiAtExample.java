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

import com.rapplogic.xbee.api.AtCommand;
import com.rapplogic.xbee.api.AtCommandResponse;
import com.rapplogic.xbee.api.XBee;
import com.rapplogic.xbee.api.XBeeException;
import com.rapplogic.xbee.api.XBeeResponse;
import com.rapplogic.xbee.api.zigbee.AssociationStatus;
import com.rapplogic.xbee.util.ByteUtils;

/** 
 * This class contains AtCommand examples that are specific to ZNet radios
 * 
 * @author andrew
 *
 */
public class ZNetApiAtExample {

	private final static Logger log = Logger.getLogger(ZNetApiAtExample.class);
	
	private XBee xbee = new XBee();
	
	public ZNetApiAtExample() throws XBeeException {
		try {
			
			// replace with port and baud rate of your XBee
			xbee.open("COM6", 9600);	
			
			// get the 8 byte SH/SL address
			log.debug("SH is " + ByteUtils.toBase16(((AtCommandResponse)xbee.sendAtCommand(new AtCommand("SH"))).getValue()));
			log.debug("SL is " + ByteUtils.toBase16(((AtCommandResponse)xbee.sendAtCommand(new AtCommand("SL"))).getValue()));
			
			// uncomment to run
//			this.configureIOSamples(xbee);
//			this.associationStatus(xbee);
//			this.nodeDiscover(xbee);
//			this.configureCoordinator(xbee);
//			this.configureEndDevice(xbee);
		} finally {
			xbee.close();
		}
	}

	private void associationStatus(XBee xbee) throws XBeeException {
		// get association status - success indicates it is associated to another XBee
		AtCommandResponse response = (AtCommandResponse) xbee.sendAtCommand(new AtCommand("AI"));
		log.debug("Association Status is " + AssociationStatus.get(response));		
	}
	
	private void configureEndDevice(XBee xbee) throws XBeeException {
				
		// basic end device configuration (assumes ZNet radio flashed with end device API firmware)
		XBeeResponse response = null;
		
		// reset to factory settings
		response = xbee.sendAtCommand(new AtCommand("RE"));
		log.debug("RE is " + response);

		// set PAN id to arbitrary value
		response = xbee.sendAtCommand(new AtCommand("ID", new int[] {0x1a, 0xaa}));
		log.debug("ID is " + response);
		
		// set NI -- can be any arbitrary sequence of chars
		response = xbee.sendAtCommand(new AtCommand("NI", new int[] {'E','N','D','_','D','E','V','I','C','E','_','2' }));
		log.debug("NI is " + response);

		// set API mode to 2.  factory setting is 1
		response = xbee.sendAtCommand(new AtCommand("AP", 2));
		log.debug("AP is " + response);			

		// save to settings to survive power cycle
		response = xbee.sendAtCommand(new AtCommand("WR"));
		log.debug("WR is " + response);			

		// software reset
		response = xbee.sendAtCommand(new AtCommand("FR"));
		log.debug("FR is " + response);	
	}
	
	private void configureCoordinator(XBee xbee) throws XBeeException {			
		// basic coordinator configuration (assumes ZNet radio flashed with COORDINATOR API firmware)
		XBeeResponse response = null;
		
		// reset to factory settings
		response = xbee.sendAtCommand(new AtCommand("RE"));
		log.debug("RE is " + response);

		// set PAN id to arbitrary value
		response = xbee.sendAtCommand(new AtCommand("ID", new int[] {0x1a, 0xaa}));
		log.debug("RE is " + response);
		
		// set NI
		response = xbee.sendAtCommand(new AtCommand("NI", new int[] {'C','O','O','R','D','I','N','A','T','O','R' }));
		log.debug("NI is " + response);

		// set API mode to 2.  factory setting is 1
		response = xbee.sendAtCommand(new AtCommand("AP", 2));
		log.debug("AP is " + response);			

		// save to settings to survive power cycle
		response = xbee.sendAtCommand(new AtCommand("WR"));
		log.debug("WR is " + response);			

		// software reset
		response = xbee.sendAtCommand(new AtCommand("FR"));
		log.debug("FR is " + response);	
	}

	/**
	 * This assumes an end device that is already has the configureEndDevice configuration
	 * Does not save configuration! -- use WR if you want this configure to survive power on/off.
	 * 
	 * @param xbee
	 * @throws XBeeException
	 */
	private void configureIOSamples(XBee xbee) throws XBeeException {			
		// basic coordinator configuration (assumes ZNet radio flashed with COORDINATOR API firmware)
		XBeeResponse response = null;
		
		// set IR to 1 sample every 10 seconds.  Set to 0 to disable
		response = xbee.sendAtCommand(new AtCommand("IR", new int[] {0x27, 0x10}));
		log.debug("IR is " + response);

		// set pin 20 to monitor digital input
		response = xbee.sendAtCommand(new AtCommand("DO", 0x3));
		log.debug("DO is " + response);

		// set pin 19 to monitor analog input
		response = xbee.sendAtCommand(new AtCommand("D1", 0x2));
		log.debug("D1 is " + response);
		
		// set pin 18 to monitor analog input
		response = xbee.sendAtCommand(new AtCommand("D2", 0x2));
		log.debug("D2 is " + response);
		
		// set pin 16 to monitor digital input
		response = xbee.sendAtCommand(new AtCommand("D6", 0x3));
		log.debug("D6 is " + response);
		
		// optionally configure DH + DL; if set to zero (default), samples will be sent to coordinator
	}
	
	public static void main(String[] args) throws XBeeException {
		PropertyConfigurator.configure("log4j.properties");
		new ZNetApiAtExample();
	}
}
