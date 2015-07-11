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

package com.rapplogic.xbee.util;

/**
 *  //TODO replace with nio.IntBuffer
 */
public class IntArrayInputStream implements IIntArrayInputStream {

	private int[] source;
	private int pos;
	
	public IntArrayInputStream(int[] source) {
		this.source = source;
	}
	
	public int read() {
		if (pos >= source.length) {
			throw new IllegalStateException("end of input stream");
		}
		
		return source[pos++];
	}
	
	/**
	 * Reads <i>size</i> bytes from the input stream and returns the bytes in an array
	 *  
	 * @param size
	 * @return
	 * Apr 13, 2009
	 */
	public int[] read(int size) {
		int[] block = new int[size];
		System.arraycopy(source, pos, block, 0, size);
		// index pos
		pos+=size;
		return block;
	}
	
	public int read(String s) {
		return read();
	}
}
