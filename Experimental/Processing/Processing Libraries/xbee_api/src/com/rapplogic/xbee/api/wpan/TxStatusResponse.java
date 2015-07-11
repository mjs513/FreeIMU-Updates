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

import java.util.EnumSet;
import java.util.HashMap;
import java.util.Map;

import com.rapplogic.xbee.api.XBeeFrameIdResponse;

/**
 * Series 1 XBee.  This is sent out the UART of the transmitting XBee immediately following
 * a Transmit packet.  Indicates if the Transmit (TxRequest16 or TxRequest64)
 * was successful.
 * <p/>
 * API ID: 0x89
 * <p/>
 * @author andrew
 *
 */
public class TxStatusResponse extends XBeeFrameIdResponse {
	
	public enum Status {
	
		SUCCESS (0),
		NO_ACK (1),
		CCA_FAILURE(2),
		PURGED(3);
		
		private static final Map<Integer,Status> lookup = new HashMap<Integer,Status>();
		
		static {
			for(Status s : EnumSet.allOf(Status.class)) {
				lookup.put(s.getValue(), s);
			}
		}
		
		public static Status get(int value) { 
			return lookup.get(value); 
		}
		
	    private final int value;
	    
	    Status(int value) {
	        this.value = value;
	    }

		public int getValue() {
			return value;
		}
	}
	
	private Status status;
	
	public TxStatusResponse() {

	}

	public Status getStatus() {
		return status;
	}

	public void setStatus(Status status) {
		this.status = status;
	}
	
	/**
	 * Returns true if the delivery status is SUCCESS
	 */
	public boolean isSuccess() {
		return this.status == Status.SUCCESS;
	}

	// isError() was overridding XBeeResponse isError()
		
	public boolean isAckError() {
		return this.status == Status.NO_ACK;
	}
	
	public boolean isCcaError() {
		return this.status == Status.CCA_FAILURE;
	}
	
	public boolean isPurged() {
		return this.status == Status.PURGED;
	}
	
	public String toString() {
		return super.toString() + ",status=" + this.getStatus();
	}
}