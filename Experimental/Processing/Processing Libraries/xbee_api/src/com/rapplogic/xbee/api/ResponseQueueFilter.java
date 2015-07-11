package com.rapplogic.xbee.api;

public interface ResponseQueueFilter {
	public boolean accept(XBeeResponse response);
}
