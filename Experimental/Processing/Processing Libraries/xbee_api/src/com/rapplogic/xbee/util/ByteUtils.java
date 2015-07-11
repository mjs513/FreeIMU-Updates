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

import java.io.IOException;

public class ByteUtils {

	// not to be instantiated
	private ByteUtils() {
		
	}
	
	/**
	 * There is a slight problem with this method that you might have noticed;  a Java int is signed, so we can't make
	 * use of the 32nd bit.  This means this method does not support a four byte value with msb greater than 01111111 ((2^7-1) or 127)
     * and will throw a runtime exception if it encounters this situation.
	 * 
	 * TODO use long instead of int to support 4 bytes values.  note that long assignments are not atomic.
	 * 
	 * Not Used
	 */
	public static int convertMultiByteToInt(int[] bytes) {
		
		if (bytes.length > 4) {
			throw new RuntimeException("too big");
		} else if (bytes.length == 4 && ((bytes[0] & 0x80) == 0x80)) {
			// 0x80 == 10000000, 0x7e == 01111111
			throw new IllegalArgumentException("Java int can't support a four byte value with msb byte greater than 7e");
		}
		
		int val = 0;
		
		for (int i = 0; i < bytes.length; i++) {
			
			if (bytes[i] > 0xFF) {
				throw new IllegalArgumentException("Values exceeds byte range: " + bytes[i]);
			}
			
			if (i == (bytes.length - 1)) {
				val+= bytes[i];
			} else {
				val+= bytes[i] << ((bytes.length - i - 1) * 8);	
			}
		}
		
		return val;
	}

    /**
     * Works for positive values only
     *
     * @param val
     * @return
     */
	public static int[] convertInttoMultiByte(int val) {

        if (val < 0) {
            throw new IllegalArgumentException("Negative values are not supported");
        }
        
		// must decompose into a max of 4 bytes
		// b1		b2		 b3		  b4
		// 01111111 11111111 11111111 11111111
		// 127      255      255      255
		
		int size = 0;
		
		if ((val >> 24) > 0) {
			size = 4;
		} else if ((val >> 16) > 0) {
			size = 3;
		} else if ((val >> 8) > 0) {
			size = 2;
		} else {
			size = 1;
		}
		
		int[] data = new int[size];
		
		for (int i = 0; i < size; i++) {
			data[i] = (val >> (size - i - 1) * 8) & 0xFF;
		}
		
		return data;	
	}

	public static String toBase16(int[] arr) {
		return toBase16(arr, ",");
	}

	public static String toBase16(int[] arr, String delimiter) {
		
		if (arr == null) {
			return "";
		}
		
		StringBuffer sb = new StringBuffer();
		
		for (int i = 0; i < arr.length; i++) {
			sb.append(toBase16(arr[i]));
			
			if (i < arr.length - 1) {
				sb.append(delimiter);
			}
		}
		
		return sb.toString();
	}
	
	public static String toBase2(int[] arr) {

		if (arr == null) {
			return "";
		}
		
		StringBuffer sb = new StringBuffer();
		
		for (int i = 0; i < arr.length; i++) {
			sb.append(toBase2(arr[i]));
			
			if (i < arr.length - 1) {
				sb.append(" ");
			}
		}
		
		return sb.toString();
	}

	public static String toBase10(int[] arr) {
		
		if (arr == null) {
			return "";
		}
		
		StringBuffer sb = new StringBuffer();
		
		for (int i = 0; i < arr.length; i++) {
			sb.append((arr[i]));
			
			if (i < arr.length - 1) {
				sb.append(" ");
			}
		}
		
		return sb.toString();
	}
	
	/**
	 * @deprecated use toString(int[] arr)
	 */
	public static String toChar(int[] arr) {
		return toString(arr);
	}
	
	/**
	 * Converts an int array to string.  
	 * Note: this method does not validate that int values map
	 * to valid characters
	 * 
	 * @param arr
	 * @return
	 */
	public static String toString(int[] arr) {
		
		if (arr == null) {
			return "";
		}
		
		StringBuffer sb = new StringBuffer();

        for (int anArr : arr) {
            sb.append((char) anArr);
        }
		
		return sb.toString();
	}	
	
	private static String padBase2(String s) {
		
		for (int i = s.length(); i < 8; i++) {
			s = "0" + s;
		}
		
		return s;
	}
	
//	/**
//	 * Determines the bit value of a single byte
//	 * 
//	 * @param b
//	 * @return
//	 */
//	public static boolean[] parseBits(int b) {
//		
//		boolean[] results = new boolean[8];
//		
//		for (int i = 0; i < 8; i++) {
//			if (((b >> i) & 0x1) == 0x1) {
//				results[i] = true;
//			} else {
//				results[i] = false;
//			}
//		}
//	
//		return results;
//	}
	
	/**
	 * Returns true if the bit is on (1) at the specified position
	 * Position range: 1-8
	 */
	public static boolean getBit(int b, int position) {
		
		if (position < 1 || position > 8) {
			throw new IllegalArgumentException("Position is out of range");
		}
		
		if (b > 0xff) {
			throw new IllegalArgumentException("input value [" + b + "] is larger than a byte");
		}

        return ((b >> (--position)) & 0x1) == 0x1;

    }
	
	public static String toBase16(int b) {
		
		if (b > 0xff) {
			throw new IllegalArgumentException("input value [" + b + "] is larger than a byte");
		}
		
		if (b < 0x10) {
			return "0x0" + Integer.toHexString(b);
		} else {
			return "0x" + Integer.toHexString(b);
		}
	}
	
	public static String toBase2(int b) {
		
		if (b > 0xff) {
			throw new IllegalArgumentException("input value [" + b + "] is larger than a byte");
		}
		
		return padBase2(Integer.toBinaryString(b));
	}
	
	public static String formatByte(int b) {
		return "base10=" + Integer.toString(b) + ",base16=" + toBase16(b) + ",base2=" + toBase2(b);
	}
	
	public static int[] stringToIntArray(String s) {
		int[] intArr = new int[s.length()];
		
		for (int i = 0; i < s.length(); i++) {
			intArr[i] = (char)s.charAt(i);
		}
		
		return intArr;
	}
	
	/**
	 * Parses a 10-bit analog value from the input stream
	 * 
	 * @return
	 * @throws IOException
	 */
	public static int parse10BitAnalog(int msb, int lsb) throws IOException {	
		msb = msb & 0xff;
		
		// shift up bits 9 and 10 of the msb
		msb = (msb & 0x3) << 8;
		
//		log.debug("shifted msb is " + msb);
		
		lsb = lsb & 0xff;
		
		return msb + lsb;
	}
	
	public static int parse10BitAnalog(IIntArrayInputStream in, int pos) throws IOException {
		int adcMsb = in.read("Analog " + pos + " MSB");
		int adcLsb = in.read("Analog " + pos + " LSB");
		
		return ByteUtils.parse10BitAnalog(adcMsb, adcLsb);
	}
}
