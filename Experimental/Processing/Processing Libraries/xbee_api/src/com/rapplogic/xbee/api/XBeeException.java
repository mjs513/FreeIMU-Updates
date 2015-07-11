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

/**
 * I usually detest checked exceptions but given this is a public api, it is reasonable to
 * notify users what they can expect.
 * 
 * @author andrew
 *
 */
public class XBeeException extends Exception {

	private static final long serialVersionUID = -5501299728920565639L;
	private Exception cause;
	
	public XBeeException(String message) {
		super(message);
	}

	public XBeeException(String message, Exception e) {
		super(message, e);
	}
	
	public XBeeException() {
		super();
	}
	
	public XBeeException(Exception cause) {
		super();
		this.setCause(cause);
	}

	public Exception getCause() {
		return cause;
	}

	public void setCause(Exception cause) {
		this.cause = cause;
	}
}
