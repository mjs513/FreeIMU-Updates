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

import com.rapplogic.xbee.api.NoRequestResponse;
import com.rapplogic.xbee.api.XBeeAddress16;

/**
 * Series 1 XBee. 16-bit address Receive packet.
 * This packet is received when a remote radio transmits a TxRequest16
 * packet to this radio's MY address
 * <p/>
 * API ID: 0x81
 * 
 * @author andrew
 *
 */
public class RxResponse16 extends RxResponse implements NoRequestResponse {
	
	public XBeeAddress16 getRemoteAddress() {
		return (XBeeAddress16) this.getSourceAddress();
	}
}