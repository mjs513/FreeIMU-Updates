package com.rapplogic.xbee;

import gnu.io.SerialPortEvent;

public interface RxTxSerialEventListener {
	public void handleSerialEvent(SerialPortEvent event);
}
