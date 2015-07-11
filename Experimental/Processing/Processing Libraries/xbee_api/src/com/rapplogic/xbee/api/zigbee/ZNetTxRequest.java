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

import com.rapplogic.xbee.api.ApiId;
import com.rapplogic.xbee.api.XBeeAddress16;
import com.rapplogic.xbee.api.XBeeAddress64;
import com.rapplogic.xbee.api.XBeeRequest;
import com.rapplogic.xbee.util.ByteUtils;
import com.rapplogic.xbee.util.IntArrayOutputStream;

/**
 * Series 2 XBee.  Sends a packet to a remote radio.  The remote radio
 * receives the data as a ZNetRxResponse packet.
 * <p/>
 * API ID: 0x10
 * <p/>
 * @author andrew
 *
 */
public class ZNetTxRequest extends XBeeRequest {

	// 10/28/08 the datasheet states 72 is maximum payload size but I was able to push 75 through successfully, 
	// even with all bytes as escape bytes (a total post-escape packet size of 169!).
	
	/**
	 * This is the maximum payload size for ZNet firmware, as specified in the datasheet.
	 * This value is provided for reference only and is not enforced by this software unless
	 * max size unless specified in the setMaxPayloadSize(int) method.
	 * Note: this size refers to the packet size prior to escaping the control bytes.
	 * Note: ZB Pro firmware provides the ATNP command to determine max payload size.
	 * For ZB Pro firmware, the TX Status will return a PAYLOAD_TOO_LARGE (0x74) delivery status 
	 * if the payload size is exceeded
	 */
	public final static int ZNET_MAX_PAYLOAD_SIZE = 72;
	public final static int DEFAULT_BROADCAST_RADIUS = 0;
				
	private XBeeAddress64 destAddr64;
	private XBeeAddress16 destAddr16;
	private int broadcastRadius;
	private Option option;
	private int[] payload;
	private int maxPayloadSize;
	
	//TODO frameId should go last in all Request constructors since it is not specific to any one request
	
	public enum Option {
		
		UNICAST (0),
		BROADCAST (8);
		
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
	
	/**
	 * From manual p. 33:
	 * The ZigBee Transmit Request API frame specifies the 64-bit Address and the network address (if
	 * known) that the packet should be sent to. By supplying both addresses, the module will forego
	 * network address Discovery and immediately attempt to route the data packet to the remote. If the
	 * network address of a particular remote changes, network address and route discovery will take
	 * place to establish a new route to the correct node. Upon successful
	 * 
	 * Key points:
	 * 	- always specify the 64-bit address and also specify the 16-bit address, if known.  Set
	 * 	the 16-bit network address to 0xfffe if not known.
	 *  - check the 16-bit address of the tx status response frame as it may change.  
	 *  - keep a hash table mapping of 64-bit address to 16-bit network address.
	 *  
	 *  
	 * @param frameId
	 * @param dest64
	 * @param dest16
	 * @param broadcastRadius
	 * @param option
	 * @param payload
	 */
	public ZNetTxRequest(int frameId, XBeeAddress64 dest64, XBeeAddress16 dest16, int broadcastRadius, Option option, int[] payload) {
		this.setFrameId(frameId);
		this.destAddr64 = dest64;
		this.destAddr16 = dest16;
		this.broadcastRadius = broadcastRadius;
		this.option = option;
		this.payload = payload;
	}
	
	/**
	 * Abbreviated constructor for sending a unicast TX packet
	 * 
	 * @param dest64
	 * @param payload
	 */
	public ZNetTxRequest(XBeeAddress64 dest64, int[] payload) {
		this(XBeeRequest.DEFAULT_FRAME_ID, dest64, XBeeAddress16.ZNET_BROADCAST, ZNetTxRequest.DEFAULT_BROADCAST_RADIUS, Option.UNICAST, payload);
	}
	
	protected IntArrayOutputStream getFrameDataAsIntArrayOutputStream() {

		if (this.getMaxPayloadSize() > 0 && payload.length > this.getMaxPayloadSize()) {
			throw new IllegalArgumentException("Payload exceeds user-defined maximum payload size of " + this.getMaxPayloadSize() + " bytes.  Please package into multiple packets");
		}
		
		IntArrayOutputStream out = new IntArrayOutputStream();
		
		// api id
		out.write(this.getApiId().getValue()); 
		
		// frame id (arbitrary byte that will be sent back with ack)
		out.write(this.getFrameId());
		
		// add 64-bit dest address
		out.write(destAddr64.getAddress());
		
		// add 16-bit dest address
		out.write(destAddr16.getAddress());
		
		// write broadcast radius
		out.write(broadcastRadius);
		
		// write options byte
		out.write(option.getValue());
		
		out.write(payload);
		
		return out;
	}
	
	public int[] getFrameData() {
		return this.getFrameDataAsIntArrayOutputStream().getIntArray();
	}
	
	public ApiId getApiId() {
		return ApiId.ZNET_TX_REQUEST;
	}
	
	public XBeeAddress64 getDestAddr64() {
		return destAddr64;
	}

	public void setDestAddr64(XBeeAddress64 destAddr64) {
		this.destAddr64 = destAddr64;
	}

	public XBeeAddress16 getDestAddr16() {
		return destAddr16;
	}

	public void setDestAddr16(XBeeAddress16 destAddr16) {
		this.destAddr16 = destAddr16;
	}

	public int getBroadcastRadius() {
		return broadcastRadius;
	}

	public void setBroadcastRadius(int broadcastRadius) {
		this.broadcastRadius = broadcastRadius;
	}

	public Option getOption() {
		return option;
	}

	public void setOption(Option option) {
		this.option = option;
	}

	public int[] getPayload() {
		return payload;
	}

	public void setPayload(int[] payload) {
		this.payload = payload;
	}
	
	public String toString() {
		return super.toString() + 
			",destAddr64=" + this.destAddr64 +
			",destAddr16=" + this.destAddr16 +
			",broadcastRadius=" + this.broadcastRadius + 
			",option=" + this.option +
			",payload=" + ByteUtils.toBase16(this.payload);
	}

	public int getMaxPayloadSize() {
		return maxPayloadSize;
	}

	public void setMaxPayloadSize(int maxPayloadSize) {
		this.maxPayloadSize = maxPayloadSize;
	}
}
