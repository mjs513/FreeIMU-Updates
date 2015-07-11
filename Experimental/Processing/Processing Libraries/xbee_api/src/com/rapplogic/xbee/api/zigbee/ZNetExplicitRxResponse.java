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

import com.rapplogic.xbee.util.ByteUtils;
import com.rapplogic.xbee.util.DoubleByte;

// TODO deprecate all ZNet* classes and replace with ZigBee*
/**
 * Series 2 XBee. This packet is received when a remote XBee sends a ZNetExplicitTxRequest
 * <p/>
 * Radio must be configured for explicit frames to use this class (AO=1)
 * <p/>
 * API ID: 0x91
 */
public class ZNetExplicitRxResponse extends ZNetRxResponse {
	
	private int sourceEndpoint;
	private int destinationEndpoint;
	private DoubleByte clusterId;
	private DoubleByte profileId;
	
	public ZNetExplicitRxResponse() {
		super();
	}

	public int getSourceEndpoint() {
		return sourceEndpoint;
	}

	public void setSourceEndpoint(int sourceEndpoint) {
		this.sourceEndpoint = sourceEndpoint;
	}

	public int getDestinationEndpoint() {
		return destinationEndpoint;
	}

	public void setDestinationEndpoint(int destinationEndpoint) {
		this.destinationEndpoint = destinationEndpoint;
	}

	public DoubleByte getClusterId() {
		return clusterId;
	}

	public void setClusterId(DoubleByte clusterId) {
		this.clusterId = clusterId;
	}

	public DoubleByte getProfileId() {
		return profileId;
	}

	public void setProfileId(DoubleByte profileId) {
		this.profileId = profileId;
	}
	
	public String toString() {
		return super.toString() + 
			",sourceEndpoint=" + ByteUtils.toBase16(this.getSourceEndpoint()) +
			",destinationEndpoint=" + ByteUtils.toBase16(this.getDestinationEndpoint()) +
			",clusterId(msb)=" + ByteUtils.toBase16(this.getClusterId().getMsb()) +
			",clusterId(lsb)=" + ByteUtils.toBase16(this.getClusterId().getLsb()) +
			",profileId(msb)=" + ByteUtils.toBase16(this.getProfileId().getMsb()) +
			",profileId(lsb)=" + ByteUtils.toBase16(this.getProfileId().getLsb());
	}
}