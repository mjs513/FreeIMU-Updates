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

import java.util.EnumSet;
import java.util.HashMap;
import java.util.Map;

import org.apache.log4j.Logger;

import com.rapplogic.xbee.util.ByteUtils;

/**
 * Packages a frame data array into an XBee packet.
 * <p/>
 * @author andrew
 *
 */
public class XBeePacket {

	public enum SpecialByte {
		START_BYTE (0x7e), // ~
		ESCAPE (0x7d), // }
		XON (0x11),
		XOFF (0x13);
		
		private static final Map<Integer,SpecialByte> lookup = new HashMap<Integer,SpecialByte>();
		
		static {
			for(SpecialByte s : EnumSet.allOf(SpecialByte.class)) {
				lookup.put(s.getValue(), s);
			}
		}
		
		public static SpecialByte get(int value) { 
			return lookup.get(value); 
		}
		
	    private final int value;
	    
	    SpecialByte(int value) {
	        this.value = value;
	    }

		public int getValue() {
			return value;
		}
	}
	
	private final static Logger log = Logger.getLogger(XBeePacket.class);
		
	private int[] packet;
	
	/**
	 * Performs the necessary activities to construct an XBee packet from the frame data.
	 * This includes: computing the checksum, escaping the necessary bytes, adding the start byte and length bytes.
	 * 
	 * The format of a packet is as follows:
	 * 
	 * start byte - msb length byte - lsb length byte - frame data - checksum byte
	 * 
	 * @param frameData
	 */
	public XBeePacket(int[] frameData) {
		
		// checksum is always computed on pre-escaped packet
		Checksum checksum = new Checksum();

        for (int aFrameData : frameData) {
            checksum.addByte(aFrameData);
        }
		
		checksum.compute();
		
		// packet size is frame data + start byte + 2 length bytes + checksum byte
		packet = new int[frameData.length + 4];
		packet[0] = SpecialByte.START_BYTE.getValue();
		
		// Packet length does not include escape bytes or start, length and checksum bytes
		XBeePacketLength length = new XBeePacketLength(frameData.length);
		
		// msb length (will be zero until maybe someday when > 255 bytes packets are supported)
		packet[1] = length.getMsb();
		// lsb length
		packet[2] = length.getLsb();
		
		for (int i = 0; i < frameData.length; i++) {
			if (frameData[i] > 255) {
				throw new RuntimeException("Packet values must not be greater than one byte (255): " + frameData[i]);
			}
			
			packet[3 + i] = frameData[i];
		}
		
		// set last byte as checksum
		// note: if checksum is not correct, XBee won't send out packet or return error.  ask me how I know.

		packet[packet.length - 1] = checksum.getChecksum();
		
//		for (int i = 0; i < packet.length; i++) {
//			log.debug("XBeeApi pre-escape packet byte " + i + " is " + ByteUtils.toBase16(packet[i]));
//		}
		
		int preEscapeLength = packet.length;
		
		// TODO save escaping for the serial out method. this is an unnecessary operation
		packet = escapePacket(packet);
		
		if (log.isDebugEnabled()) {
			StringBuilder stringBuilder = new StringBuilder();
			stringBuilder.append("Packet: ");
			for (int i = 0; i < packet.length; i++) {
				stringBuilder.append(ByteUtils.toBase16(packet[i]));
				if (i < packet.length - 1) {
					stringBuilder.append(" ");
				}
			}	
			log.debug(stringBuilder);

			log.debug("pre-escape packet size is " + preEscapeLength + ", post-escape packet size is " + packet.length);
		}
		
		// 10/25/08 testing znet tx packet: max packet size that succeeded is 96.  when sending all escape bytes, pre-escape max is 94
//		final int maxPacketSize = 94;
		
		// This seems like a nice idea but it depends on which radio type is involved and possibly several other factors.
		// The packet size validation in the TX Request class should suffice.
		// if you don't get an ACK response back, one explanation is your packet is too large

		//		if (preEscapeLength > maxPacketSize) {
//			throw new RuntimeException("This packet (pre escape) exceeds the max size of " + maxPacketSize);
////			log.warn("p.48 indicates DI can handle 100 bytes: RF transmission will also commence after 100 Bytes (maximum packet size) are received in the DI buffer.");
////			// WTF p.48 indicates DI can handle 100 bytes: RF transmission will also commence after 100 Bytes (maximum packet size) are received in the DI buffer.
////			throw new RuntimeException("This packet exceeds the DI buffer limit of 34 bytes this API does not use Hardware Flow Control (XBee manual p.11)");
////			log.warn("packet exceeds max length of 100");
//		}
	}
	
	/**
	 * Escape all bytes in packet after start byte, and including checksum
	 * 
	 * @param packet
	 * @return
	 */
	private static int[] escapePacket(int[] packet) {
		int escapeBytes = 0;
		
		// escape packet.  start at one so we don't escape the start byte 
		for (int i = 1; i < packet.length; i++) {
			if (isSpecialByte(packet[i])) {
				log.debug("escapeFrameData: packet byte requires escaping byte " + ByteUtils.toBase16(packet[i]));
				escapeBytes++;
			}
		}

		if (escapeBytes == 0) {
			return packet;
		} else {
			log.debug("packet requires escaping");
			
			int[] escapePacket = new int[packet.length + escapeBytes];
			
			int pos = 1;
			
			escapePacket[0] = SpecialByte.START_BYTE.getValue();
				
			for (int i = 1; i < packet.length; i++) {
				if (isSpecialByte(packet[i])) {
					escapePacket[pos] = SpecialByte.ESCAPE.getValue();
					escapePacket[++pos] = 0x20 ^ packet[i];
					
					log.debug("escapeFrameData: xor'd byte is 0x" + Integer.toHexString(escapePacket[pos]));
				} else {
					escapePacket[pos] = packet[i];
				}
				
				pos++;
			}
			
			return escapePacket;
		}
	}
	
	/**
	 * @deprecated use getByteArray
	 * @return
	 */
	public int[] getPacket() {
		return this.getByteArray();
	}
	
	public int[] getByteArray() {
		return packet;
	}
	
	public static boolean isSpecialByte(int b) {
        return b == SpecialByte.START_BYTE.getValue() || b == SpecialByte.ESCAPE.getValue() || b == SpecialByte.XON.getValue() ||
                b == SpecialByte.XOFF.getValue();

    }
	
	public String toString() {
		return ByteUtils.toBase16(this.packet);
	}
	
	/**
	 * Returns true if the packet is valid
     *
	 * @param packet
	 * @return
	 */
	public static boolean verify(int[] packet) {
		boolean valid = true;
		
		try {
			if (packet[0] != SpecialByte.START_BYTE.getValue()) {
				return false;
			}
					
			// first need to unescape packet
			int[] unEscaped = unEscapePacket(packet);

			XBeePacketLength len = new XBeePacketLength(unEscaped[1], unEscaped[2]);
			
			// stated packet length does not include start byte, length bytes, or checksum and is calculated before escaping
			
			int[] frameData = new int[len.get16BitValue()];
			
			Checksum checksum = new Checksum();
			
			for (int i = 3; i < unEscaped.length - 1; i++) {
				frameData[i - 3] = unEscaped[i];
				checksum.addByte(frameData[i - 3]);
			}
			
			// add checksum byte to verify -- the last byte
			checksum.addByte(unEscaped[unEscaped.length - 1]);
			
			if (!checksum.verify()) {
				valid = false;
			}
		} catch (Exception e) {
			throw new RuntimeException("Packet verification failed with error: ", e);
		}

		return valid;
	}
	
	/**
	 * 
	 * @param packet
	 * @return
	 */
	public static int[] unEscapePacket(int[] packet) {
	
		int escapeBytes = 0;

        for (int aPacket : packet) {
            if (aPacket == SpecialByte.ESCAPE.getValue()) {
                escapeBytes++;
            }
        }
		
		if (escapeBytes == 0) {
			return packet;
		}
		
		int[] unEscapedPacket = new int[packet.length - escapeBytes];
		
		int pos = 0;
		
		for (int i = 0; i < packet.length; i++) {
			if (packet[i] == SpecialByte.ESCAPE.getValue()) {
				// discard escape byte and un-escape following byte
				unEscapedPacket[pos] = 0x20 ^ packet[++i];
			} else {
				unEscapedPacket[pos] = packet[i];
			}
			
			pos++;
		}
		
		return unEscapedPacket;
	}
}