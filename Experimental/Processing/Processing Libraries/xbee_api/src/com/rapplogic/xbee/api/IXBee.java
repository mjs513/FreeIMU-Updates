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

import java.io.IOException;

/**
 * XBee interface
 * 
 * @author andrew
 *
 */
public interface IXBee {
	public void open(String port, int baudRate) throws XBeeException;

	public void addPacketListener(PacketListener packetListener);

	public void removePacketListener(PacketListener packetListener);

	public void sendPacket(XBeePacket packet) throws IOException;

	public void sendPacket(int[] packet)  throws IOException;

	public void sendAsynchronous(XBeeRequest xbeeRequest) throws XBeeException;

	public XBeeResponse sendSynchronous(final XBeeRequest xbeeRequest, int timeout) throws XBeeTimeoutException, XBeeException;

	public XBeeResponse getResponse() throws XBeeException;

	public XBeeResponse getResponse(int timeout) throws XBeeException, XBeeTimeoutException;

	public void close();

	public int getCurrentFrameId();

	public int getNextFrameId();

	public void updateFrameId(int val);

	public boolean isConnected();
	
	public void clearResponseQueue();
}