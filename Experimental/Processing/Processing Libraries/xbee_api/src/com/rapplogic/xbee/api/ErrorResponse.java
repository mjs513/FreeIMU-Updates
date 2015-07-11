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
 * Represents a Java error during packet parsing.
 * This is the only class that extends XBeeResponse and does not map
 * to a XBee API ID
 * <p/>
 * @author andrew
 *
 */
public class ErrorResponse extends XBeeResponse {

	private String errorMsg;
	private Exception exception;
	
	public ErrorResponse() {
		super();
		this.setApiId(ApiId.ERROR_RESPONSE);
		this.setError(true);
	}

	/**
	 * A bit redundant in that it is the same as getException.getMessage()
	 * 
	 * @return
	 */
	public String getErrorMsg() {
		return errorMsg;
	}

	public void setErrorMsg(String errorMsg) {
		this.errorMsg = errorMsg;
	}
	
	public Exception getException() {
		return exception;
	}

	public void setException(Exception exception) {
		this.exception = exception;
	}

	public String toString() {
		return super.toString() + ",errorMsg=" + this.errorMsg + ",exception=" + this.exception;
	}
}