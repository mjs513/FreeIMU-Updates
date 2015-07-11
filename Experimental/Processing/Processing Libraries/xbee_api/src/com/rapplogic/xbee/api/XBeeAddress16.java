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

import com.rapplogic.xbee.util.DoubleByte;

/**
 * Represents a 16-bit XBee Address.
 * <p/>
 * @author andrew
 *
 */
public class XBeeAddress16 extends XBeeAddress {

	public static final XBeeAddress16 BROADCAST = new XBeeAddress16(0xFF, 0xFF);
	public static final XBeeAddress16 ZNET_BROADCAST = new XBeeAddress16(0xFF, 0xFE);

	private DoubleByte doubleByte = new DoubleByte();
	
	/**
	 * Provide address as msb byte and lsb byte
	 * 
	 * @param msb
	 * @param lsb
	 */
	public XBeeAddress16(int msb, int lsb) {
		this.doubleByte.setMsb(msb);
		this.doubleByte.setLsb(lsb);
	}

	public XBeeAddress16(int[] arr) {
		this.doubleByte.setMsb(arr[0]);
		this.doubleByte.setLsb(arr[1]);
	}
	
	public XBeeAddress16() {
		
	}
	
	public int get16BitValue() {
		return this.doubleByte.get16BitValue();
	}
	
	public int getMsb() {
		return this.doubleByte.getMsb();
	}

	public void setMsb(int msb) {
		this.doubleByte.setMsb(msb);
	}

	public int getLsb() {
		return this.doubleByte.getLsb();
	}

	public void setLsb(int lsb) {
		this.doubleByte.setLsb(lsb);
	}

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        XBeeAddress16 that = (XBeeAddress16) o;

        if (doubleByte != null ? !doubleByte.equals(that.doubleByte) : that.doubleByte != null) return false;

        return true;
    }

    @Override
    public int hashCode() {
        return doubleByte != null ? doubleByte.hashCode() : 0;
    }

    @Override
	public int[] getAddress() {
		return new int[] { this.doubleByte.getMsb(), this.doubleByte.getLsb() };
	}
}
