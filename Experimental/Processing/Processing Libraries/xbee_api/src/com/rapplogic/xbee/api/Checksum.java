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

import org.apache.log4j.Logger;

import com.rapplogic.xbee.util.ByteUtils;

/**
 * Computes and verifies packet checksums
 * <p/>
 * @author andrew
 *
 */
public class Checksum {
	
	private final static Logger log = Logger.getLogger(Checksum.class);
	
	public int checksum = 0;
	
	/**
	 * Don't add Checksum byte when computing checksum!!
	 * 
	 * @param val
	 */
	public void addByte(int val) {
		checksum+= val;
	}
	
	/**
	 * Computes checksum and stores in checksum instance variable
	 * 
	 * @return
	 */
	public void compute() {
		// discard values > 1 byte
		checksum = 0xff & checksum;
		// perform 2s complement
		checksum = 0xff - checksum;
		
		log.debug("computed checksum is " + ByteUtils.formatByte(checksum));
	}
	
	/**
	 * First add all relevant bytes, including checksum
	 * 
	 * @return
	 */
	public boolean verify() {
		checksum = checksum & 0xff;
		log.debug("verify checksum is " + checksum);
		return 0xff == checksum;
	}
	
	public int getChecksum() {
		return checksum;
	}
	
	public static void main(String[] args) {
		//83 56 78 24 00 01 02 00 03 ff 85
		Checksum ck = new Checksum();

		ck.addByte(0x83);
		ck.addByte(0x56);
		ck.addByte(0x78);
		ck.addByte(0x26);
		ck.addByte(0x00);
		ck.addByte(0x01);
		ck.addByte(0x02);
		ck.addByte(0x00);
		ck.addByte(0x03);
		ck.addByte(0xff);
		// checksum
		ck.addByte(0x83);
		
		// checksum is 0x83
		//ck.compute();
		ck.verify();
		
		System.out.println("checksum is " + ByteUtils.formatByte(ck.getChecksum()));
	}
}
