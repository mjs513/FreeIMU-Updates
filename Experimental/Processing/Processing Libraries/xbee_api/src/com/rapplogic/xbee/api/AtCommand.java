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

import com.rapplogic.xbee.util.ByteUtils;
import com.rapplogic.xbee.util.IntArrayOutputStream;

/** 
 * API technique to set/query commands
 * <p/>
 * WARNING: Any changes made will not survive a power cycle unless written to memory with WR command
 * According to the manual, the WR command can only be written so many times.. however many that is.
 * <p/>
 * API ID: 0x8
 * <p/>
 * Determining radio type with HV:<br/>
 * Byte 1, Part Number<br/>
 * x17, XB24 (series 1)<br/>
 * x18, XBP24 (series 1)<br/>
 * x19, XB24-B (series 2)<br/>
 * x1A, XBP24-B (series 2)<br/>
 * <p/>
 * XB24-ZB<br/>
 * XBP24-ZB<br/>
 * @author andrew
 */
public class AtCommand extends XBeeRequest {
	
	private String command;
	private int[] value;
	
	public AtCommand(String command) {
		this(command, null, DEFAULT_FRAME_ID);
	}
	
	public AtCommand(String command, int value) {
		this(command, new int[] {value}, DEFAULT_FRAME_ID);		
	}

	public AtCommand(String command, int value[]) {
		this(command, value, DEFAULT_FRAME_ID);
	}
	
	/**
	 * Warning: frameId must be > 0 for a response
	 * 
	 * @param command
	 * @param value
	 * @param frameId
	 */
	public AtCommand(String command, int[] value, int frameId) {
		this.command = command;
		this.value = value;
		this.setFrameId(frameId);
	}

	public int[] getFrameData() {
		if (command.length() > 2) {
			throw new IllegalArgumentException("Command should be two characters.  Do not include AT prefix");
		}
		
		IntArrayOutputStream out = new IntArrayOutputStream();
		
		// api id
		out.write(this.getApiId().getValue());
		// frame id
		out.write(this.getFrameId());
		// at command byte 1
		out.write((int) command.substring(0, 1).toCharArray()[0]);
		// at command byte 2
		out.write((int) command.substring(1, 2).toCharArray()[0]);

		// int value is up to four bytes to represent command value
		if (value != null) {
			out.write(value);
		}
		
		return out.getIntArray();
	}

	public ApiId getApiId() {
		return ApiId.AT_COMMAND;
	}
	
	public String getCommand() {
		return command;
	}

	public void setCommand(String command) {
		this.command = command;
	}

	public int[] getValue() {
		return value;
	}

	public void setValue(int[] value) {
		this.value = value;
	}

	public String toString() {
		return super.toString() +
			",command=" + this.command +
			",value=" + (value == null ? "null" : ByteUtils.toBase16(value));
	}
}
