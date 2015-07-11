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

import java.util.EnumSet;
import java.util.HashMap;
import java.util.Map;

import com.rapplogic.xbee.api.XBeeAddress16;
import com.rapplogic.xbee.api.XBeeAddress64;
import com.rapplogic.xbee.api.XBeeResponse;
import com.rapplogic.xbee.util.DoubleByte;

public class ZNetNodeIdentificationResponse extends XBeeResponse {

	public enum Option {
		PACKET_ACKNOWLEDGED (0x01),
		BROADCAST_PACKET (0x02);
		
		private static final Map<Integer,Option> lookup = new HashMap<Integer,Option>();
		
		static {
			for(Option s : EnumSet.allOf(Option.class)) {
				lookup.put(s.getValue(), s);
			}
		}
		
		public static Option get(int value) { 
			return lookup.get(value); 
		}
		
	    private final int value;
	    
	    Option(int value) {
	        this.value = value;
	    }

		public int getValue() {
			return value;
		}
	}

	// TODO this is repeated in NodeDiscover
	public enum DeviceType  {
		COORDINATOR (0x1),
		ROUTER (0x2),
		END_DEVICE (0x3);
		
		private static final Map<Integer,DeviceType> lookup = new HashMap<Integer,DeviceType>();
		
		static {
			for(DeviceType s: EnumSet.allOf(DeviceType.class)) {
				lookup.put(s.getValue(), s);
			}
		}
		
		public static DeviceType get(int value) { 
			return lookup.get(value); 
		}
		
	    private final int value;
	    
	    DeviceType(int value) {
	        this.value = value;
	    }

		public int getValue() {
			return value;
		}
	}

	public enum SourceAction {
		PUSHBUTTON (0x1),
		JOINING (0x2);

		private static final Map<Integer,SourceAction> lookup = new HashMap<Integer,SourceAction>();
		
		static {
			for(SourceAction s: EnumSet.allOf(SourceAction.class)) {
				lookup.put(s.getValue(), s);
			}
		}
		
		public static SourceAction get(int value) { 
			return lookup.get(value); 
		}
		
	    private final int value;
	    
	    SourceAction(int value) {
	        this.value = value;
	    }

		public int getValue() {
			return value;
		}
	}
	
	private XBeeAddress64 remoteAddress64;
	private XBeeAddress16 remoteAddress16;
	private Option option;
	// TODO Digi WTF why duplicated?? p.70
	private XBeeAddress64 remoteAddress64_2;
	private XBeeAddress16 remoteAddress16_2;
	
	private String nodeIdentifier;
	private XBeeAddress16 parentAddress;
	private DeviceType deviceType;
	private SourceAction sourceAction;
	private DoubleByte profileId;
	private DoubleByte mfgId;
	
	public ZNetNodeIdentificationResponse() {

	}

	public XBeeAddress64 getRemoteAddress64() {
		return remoteAddress64;
	}

	public void setRemoteAddress64(XBeeAddress64 remoteAddress64) {
		this.remoteAddress64 = remoteAddress64;
	}

	public XBeeAddress16 getRemoteAddress16() {
		return remoteAddress16;
	}

	public void setRemoteAddress16(XBeeAddress16 remoteAddress16) {
		this.remoteAddress16 = remoteAddress16;
	}

	public Option getOption() {
		return option;
	}

	public void setOption(Option option) {
		this.option = option;
	}

	public XBeeAddress64 getRemoteAddress64_2() {
		return remoteAddress64_2;
	}

	public void setRemoteAddress64_2(XBeeAddress64 remoteAddress64_2) {
		this.remoteAddress64_2 = remoteAddress64_2;
	}

	public XBeeAddress16 getRemoteAddress16_2() {
		return remoteAddress16_2;
	}

	public void setRemoteAddress16_2(XBeeAddress16 remoteAddress16_2) {
		this.remoteAddress16_2 = remoteAddress16_2;
	}

	public String getNodeIdentifier() {
		return nodeIdentifier;
	}

	public void setNodeIdentifier(String nodeIdentifier) {
		this.nodeIdentifier = nodeIdentifier;
	}

	public XBeeAddress16 getParentAddress() {
		return parentAddress;
	}

	public void setParentAddress(XBeeAddress16 parentAddress) {
		this.parentAddress = parentAddress;
	}

	public DeviceType getDeviceType() {
		return deviceType;
	}

	public void setDeviceType(DeviceType deviceType) {
		this.deviceType = deviceType;
	}

	public SourceAction getSourceAction() {
		return sourceAction;
	}

	public void setSourceAction(SourceAction sourceAction) {
		this.sourceAction = sourceAction;
	}

	public DoubleByte getProfileId() {
		return profileId;
	}

	public void setProfileId(DoubleByte profileId) {
		this.profileId = profileId;
	}

	public DoubleByte getMfgId() {
		return mfgId;
	}

	public void setMfgId(DoubleByte mfgId) {
		this.mfgId = mfgId;
	}

	@Override
	public String toString() {
		return "ZNetNodeIdentificationResponse [deviceType=" + deviceType
				+ ", mfgId=" + mfgId + ", nodeIdentifier=" + nodeIdentifier
				+ ", option=" + option + ", parentAddress=" + parentAddress
				+ ", profileId=" + profileId + ", remoteAddress16="
				+ remoteAddress16 + ", remoteAddress16_2=" + remoteAddress16_2
				+ ", remoteAddress64=" + remoteAddress64
				+ ", remoteAddress64_2=" + remoteAddress64_2
				+ ", sourceAction=" + sourceAction + "]" +
				super.toString();
	}
}