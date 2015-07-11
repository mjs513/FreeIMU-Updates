package com.rapplogic.xbee.api;

public class XBeeConfiguration {

	private boolean startupChecks = true;
	private int maxQueueSize = 100;
	private int sendSynchronousTimeout = 5000;
	private ResponseQueueFilter responseQueueFilter;
	
	private final ResponseQueueFilter noRequestResponseQueueFilter = new ResponseQueueFilter() {
		public boolean accept(XBeeResponse response) {
			return response instanceof NoRequestResponse;
		}
	};
	
	public XBeeConfiguration() {

	}

	// response queue filter
	
	/**
	 * Controls is a startup check is performed when connecting to the XBee.
	 * The startup check attempts to determine the firmware type and if it is 
	 * configured correctly for use with this software.  Default is true.
	 *  
	 * @param startupChecks
	 */
	public XBeeConfiguration withStartupChecks(boolean startupChecks) {
		this.startupChecks = startupChecks;
		return this;
	}

	/**
	 * Sets the maximum size of the internal queue that supports the getResponse(..) method.
	 * Packets are removed from the head of the queue once this limit is reached.  The default is 100
	 * 
	 * @param size
	 */
	public XBeeConfiguration withMaxQueueSize(int size) {	
		if (size <= 0) {
			throw new IllegalArgumentException("Size must be > 0");
		}
		
		this.maxQueueSize = size;
		return this;
	}
	
	public XBeeConfiguration withResponseQueueFilter(ResponseQueueFilter filter) {
		this.responseQueueFilter = filter;
		return this;
	}

	public XBeeConfiguration withSendSynchronousTimeout(int sendSynchronousTimeout) {
		this.sendSynchronousTimeout = sendSynchronousTimeout;
		return this;
	}
	
	/**
	 * Only adds responses that implement NoRequestResponse
	 * 
	 * @return
	 */
	public XBeeConfiguration withNoRequestResponseQueueFilter() {
		this.responseQueueFilter = this.noRequestResponseQueueFilter;
		return this;
	}	

	public boolean isStartupChecks() {
		return startupChecks;
	}

	public int getMaxQueueSize() {
		return maxQueueSize;
	}

	public ResponseQueueFilter getResponseQueueFilter() {
		return responseQueueFilter;
	}

	public int getSendSynchronousTimeout() {
		return sendSynchronousTimeout;
	}
}
