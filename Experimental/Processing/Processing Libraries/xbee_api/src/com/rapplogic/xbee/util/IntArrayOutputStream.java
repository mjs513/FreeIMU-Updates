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

import java.util.ArrayList;
import java.util.List;

/**
 * TODO replace with nio.IntBuffer 
 */
public class IntArrayOutputStream implements IIntArray {

	private List<Integer> intList = new ArrayList<Integer>();
	
	public IntArrayOutputStream() {

	}
	
	public void write (int val) {
		intList.add(val);
	}
	
	public void write(int[] val) {
        for (int aVal : val) {
            this.write(aVal);
        }
	}
	
	public int[] getIntArray() {
		//int[] integer = (int[]) intList.toArray(new int[0]);
		// TODO there has got to be a better way -- how to convert list to int[] array?
		int[] intArr = new int[intList.size()];
		
		int i = 0;
		
		for (Integer integer : intList) {
			intArr[i++] = integer;
		}
		
		return intArr;
	}

	public List<Integer> getInternalList() {
		return intList;
	}	
}
