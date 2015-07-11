package com.rapplogic.xbee.test;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import com.rapplogic.xbee.api.AtCommand;
import com.rapplogic.xbee.api.XBee;
import com.rapplogic.xbee.api.XBeeException;

/**
 * Tests opening and closing connections to the radio
 * 
 * @author andrew
 *
 */
public class OpenCloseConnectionsTest {

	private final static Logger log = Logger.getLogger(OpenCloseConnectionsTest.class);
	
	private XBee xbee = new XBee();
	
	public static void main(String[] args) throws XBeeException, InterruptedException {
		PropertyConfigurator.configure("log4j.properties");
		new OpenCloseConnectionsTest();
	}

	public OpenCloseConnectionsTest() throws XBeeException, InterruptedException {
		
		// series 1
		String device = "/dev/tty.usbserial-A4004Rim";
		// series 2
//		String device = "/dev/tty.usbserial-A6005v5M";
		
		log.info("opening connection");
		
//		xbee.setStartupChecks(false);
		
		// first connect directly to end device and configure.  then comment out configureXXX methods and connect to coordinator
		xbee.open(device, 9600);
		
		if (!xbee.isConnected()) throw new RuntimeException("Should be connected");
		
		try {
			log.info("attempting duplicate open");
			xbee.open(device, 9600);
			throw new RuntimeException("fail -- already open");
		} catch (Exception e) {
			log.debug("Expected", e);
		}
		
		log.info("sending channel command");
		
		if (!xbee.sendAtCommand(new AtCommand("CH")).isOk()) throw new RuntimeException("fail");
		log.info("closing connection");
		xbee.close();
		
		if (xbee.isConnected()) {
			throw new RuntimeException("Should be disconnected");
		}
		
		try {
			log.info("sending at command, but we're disconnected");
			xbee.sendAtCommand(new AtCommand("CH")).isOk();
			throw new RuntimeException("Should be disconnected");
		} catch (Exception e) {
			log.debug("Expected", e);
		}
		
		log.info("reconnecting");
		xbee.open(device, 9600);
		if (!xbee.sendAtCommand(new AtCommand("CH")).isOk()) throw new RuntimeException("fail");
		
		log.info("closing conn");
		xbee.close();
		
		try {
			log.info("try duplicate close");
			xbee.close();
			throw new RuntimeException("Already closed");
		} catch (Exception e) {
			log.debug("Expected", e);
		}
	}	
}
