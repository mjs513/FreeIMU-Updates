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

package com.rapplogic.xbee.api.zigbee;

import com.rapplogic.xbee.api.XBeeAddress16;
import com.rapplogic.xbee.api.XBeeAddress64;
import com.rapplogic.xbee.api.XBeeRequest;
import com.rapplogic.xbee.api.RemoteAtRequest;

/**
 * Uses Remote AT to send a Force Sample (IS) AT command to a remote XBee
 * 
 * @author andrew
 *
 */
public class ZBForceSampleRequest extends RemoteAtRequest {
		
	/**
	 * Creates a Force Sample Remote AT request
	 * 
	 * @param dest64
	 * @param command
	 */
	public ZBForceSampleRequest(XBeeAddress64 dest64) {
		super(XBeeRequest.DEFAULT_FRAME_ID, dest64, XBeeAddress16.ZNET_BROADCAST, false, "IS", null);
	}
}
