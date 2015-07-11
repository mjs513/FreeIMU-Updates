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

import java.util.EnumSet;
import java.util.HashMap;
import java.util.Map;

import com.rapplogic.xbee.util.ByteUtils;

public enum ApiId {
	/**
	 * API ID: 0x0
	 */
	TX_REQUEST_64 (0x0),
	/**
	 * API ID: 0x1
	 */
	TX_REQUEST_16 (0x1),
	/**
	 * API ID: 0x08
	 */
	AT_COMMAND (0x08),
	/**
	 * API ID: 0x09
	 */
	AT_COMMAND_QUEUE (0x09),
	/**
	 * API ID: 0x17
	 */
	REMOTE_AT_REQUEST (0x17),
	/**
	 * API ID: 0x10
	 */	
	ZNET_TX_REQUEST (0x10),
	/**
	 * API ID: 0x11
	 */
	ZNET_EXPLICIT_TX_REQUEST (0x11),
	/**
	 * API ID: 0x80
	 */	
	RX_64_RESPONSE (0x80),
	/**
	 * API ID: 0x81
	 */	
	RX_16_RESPONSE (0x81),
	/**
	 * API ID: 0x82
	 */	
	RX_64_IO_RESPONSE (0x82),
	/**
	 * API ID: 0x83
	 */	
	RX_16_IO_RESPONSE (0x83),
	/**
	 * API ID: 0x88
	 */	
	AT_RESPONSE (0x88),
	/**
	 * API ID: 0x89
	 */	
	TX_STATUS_RESPONSE (0x89),
	/**
	 * API ID: 0x8a
	 */	
	MODEM_STATUS_RESPONSE (0x8a),
	/**
	 * API ID: 0x90
	 */	
	ZNET_RX_RESPONSE (0x90),
	/**
	 * API ID: 0x91
	 */	
	ZNET_EXPLICIT_RX_RESPONSE (0x91),
	/**
	 * API ID: 0x8b
	 */	
	ZNET_TX_STATUS_RESPONSE (0x8b),
	/**
	 * API ID: 0x97
	 */	
	REMOTE_AT_RESPONSE (0x97),
	/**
	 * API ID: 0x92
	 */	
	ZNET_IO_SAMPLE_RESPONSE (0x92),
	/**
	 * API ID: 0x95
	 */	
	ZNET_IO_NODE_IDENTIFIER_RESPONSE (0x95),
	/**
	 * Indicates that we've parsed a packet for which we didn't know how to handle the API type.  This will be parsed into a GenericResponse
	 */
	UNKNOWN (0xff),
	/**
	 * This is returned if an error occurs during packet parsing and does not correspond to a XBee API ID.
	 */
	ERROR_RESPONSE (-1);
	
	private static final Map<Integer,ApiId> lookup = new HashMap<Integer,ApiId>();
	
	static {
		for(ApiId s : EnumSet.allOf(ApiId.class)) {
			lookup.put(s.getValue(), s);
		}
	}
	
	public static ApiId get(int value) { 
		return lookup.get(value); 
	}
	
    private final int value;
    
    ApiId(int value) {
        this.value = value;
    }

	public int getValue() {
		return value;
	}
	
	public String toString() {
		return this.name() + " (" + ByteUtils.toBase16(this.getValue()) + ")";
	}
}
