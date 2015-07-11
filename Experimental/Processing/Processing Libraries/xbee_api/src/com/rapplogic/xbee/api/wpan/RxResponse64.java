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

package com.rapplogic.xbee.api.wpan;

import com.rapplogic.xbee.api.XBeeAddress64;

/**
 * Series 1 XBee. 64-bit address Receive packet.
 * This packet is received when a remote radio transmits a TxRequest64
 * packet to this radio's SH + SL address.
 * <p/>
 * Note: MY address must be set to 0xffff to receive this packet type.
 * <p/>
 * API ID: 0x80
 * 
 * @author andrew
 */
public class RxResponse64 extends RxResponse {
	
	public XBeeAddress64 getRemoteAddress() {
		return (XBeeAddress64) this.getSourceAddress();
	}
}