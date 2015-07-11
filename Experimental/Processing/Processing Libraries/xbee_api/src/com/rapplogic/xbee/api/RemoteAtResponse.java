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


//TODO Now supported by series 1 XBee. parseIoSample now needs to handle series 1 and 2

/**
 * Supported by both series 1 (10C8 firmware and later) and series 2.  To be moved into common package in the future.
 * Represents a response, corresponding to a RemoteAtRequest.
 * <p/>
 * API ID: 0x97
 */
public class RemoteAtResponse extends AtCommandResponse {
		
	private XBeeAddress64 remoteAddress64;
	private XBeeAddress16 remoteAddress16;
	
	public RemoteAtResponse() {

	}

	public boolean isSixteenBitAddressUnknown() {
		return remoteAddress16.getMsb() == 0xff && remoteAddress16.getLsb() == 0xfe;
	}

	public XBeeAddress64 getRemoteAddress64() {
		return remoteAddress64;
	}

	public void setRemoteAddress64(
			XBeeAddress64 sixtyFourBitResponderAddress) {
		this.remoteAddress64 = sixtyFourBitResponderAddress;
	}

	public XBeeAddress16 getRemoteAddress16() {
		return remoteAddress16;
	}

	public void setRemoteAddress16(
			XBeeAddress16 sixteenBitResponderAddress) {
		this.remoteAddress16 = sixteenBitResponderAddress;
	}

	/**
	 * @deprecated use getCommand instead
	 * @return
	 * Mar 4, 2009
	 */
	public String getCommandName() {
		return super.getCommand();
	}
	
	/**
	 * @deprecated use getValue instead
	 * @return
	 * Mar 4, 2009
	 */
	public int[] getCommandData() {
		return super.getValue();
	}
	
	public String toString() {
		return super.toString() +
			",remoteAddress64=" + this.remoteAddress64 +
			",remoteAddress16=" + this.remoteAddress16;
	}
}