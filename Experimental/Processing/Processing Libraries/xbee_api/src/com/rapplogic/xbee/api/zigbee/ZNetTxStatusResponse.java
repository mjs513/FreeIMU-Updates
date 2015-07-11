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
import com.rapplogic.xbee.api.XBeeFrameIdResponse;

/**
 * Series 2 XBee.  This is sent out the UART of the transmitting XBee immediately following
 * a Transmit packet.  Indicates if the Transmit packet (ZNetTxRequest)
 * was successful.
 * <p/>
 * API ID: 0x8b
 * <p/>
 * @author andrew
 */
public class ZNetTxStatusResponse extends XBeeFrameIdResponse {
	
	public enum DeliveryStatus {
		SUCCESS (0),
		CCA_FAILURE (0x02),
		INVALID_DESTINATION_ENDPOINT (0x15),
		NETWORK_ACK_FAILURE (0x21),
		NOT_JOINED_TO_NETWORK (0x22),
		SELF_ADDRESSED (0x23),
		ADDRESS_NOT_FOUND (0x24),
		ROUTE_NOT_FOUND (0x25),
		PAYLOAD_TOO_LARGE(0x74); // ZB Pro firmware only

		private static final Map<Integer,DeliveryStatus> lookup = new HashMap<Integer,DeliveryStatus>();
		
		static {
			for(DeliveryStatus s : EnumSet.allOf(DeliveryStatus.class)) {
				lookup.put(s.getValue(), s);
			}
		}
		
	    private final int value;
	    
	    DeliveryStatus(int value) {
	        this.value = value;
	    }

		public static DeliveryStatus get(int value) { 
			return lookup.get(value); 
		}
		
		public int getValue() {
			return value;
		}
	}

	public enum DiscoveryStatus {
		NO_DISCOVERY (0),
		ADDRESS_DISCOVERY (1),
		ROUTE_DISCOVERY (2),
		ADDRESS_AND_ROUTE_DISCOVERY(3);

		private static final Map<Integer,DiscoveryStatus> lookup = new HashMap<Integer,DiscoveryStatus>();
		
		static {
			for(DiscoveryStatus s : EnumSet.allOf(DiscoveryStatus.class)) {
				lookup.put(s.getValue(), s);
			}
		}
		
	    private final int value;
	    
	    DiscoveryStatus(int value) {
	        this.value = value;
	    }

		public static DiscoveryStatus get(int value) { 
			return lookup.get(value); 
		}
		
		public int getValue() {
			return value;
		}
	}

	private XBeeAddress16 remoteAddress16;
	private int retryCount;
	private DeliveryStatus deliveryStatus;
	private DiscoveryStatus discoveryStatus;
	
	
	public ZNetTxStatusResponse() {

	}

	public XBeeAddress16 getRemoteAddress16() {
		return remoteAddress16;
	}


	public void setRemoteAddress16(XBeeAddress16 remoteAddress) {
		this.remoteAddress16 = remoteAddress;
	}


	public int getRetryCount() {
		return retryCount;
	}


	public void setRetryCount(int retryCount) {
		this.retryCount = retryCount;
	}

	public DeliveryStatus getDeliveryStatus() {
		return deliveryStatus;
	}

	public void setDeliveryStatus(DeliveryStatus deliveryStatus) {
		this.deliveryStatus = deliveryStatus;
	}

	public DiscoveryStatus getDiscoveryStatus() {
		return discoveryStatus;
	}

	public void setDiscoveryStatus(DiscoveryStatus discoveryStatus) {
		this.discoveryStatus = discoveryStatus;
	}
	
	/**
	 * Returns true if the delivery status is SUCCESS
	 * 
	 * @return
	 */
	public boolean isSuccess() {
		return this.getDeliveryStatus() == DeliveryStatus.SUCCESS;
	}
	
	public String toString() {
		return super.toString() + 
		",remoteAddress16=" + this.remoteAddress16 +
		",retryCount=" + this.retryCount +
		",deliveryStatus=" + this.deliveryStatus + 
		",discoveryStatus=" + this.discoveryStatus;
	}
}