/**
 * Copyright (c) 2009 Andrew Rapp. All rights reserved.
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

package com.rapplogic.xbee;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

/**
 * Represents a configurable XBee pin and associated name, pin number, AT command, default capability and list of supported 
 * capabilities.
 * 
 * @author andrew
 *
 */
public class XBeePin {

//TODO add pin direction
//TODO methods to filter list by Capability
	 
	private String name;
	private Integer pin;
	private String atCommand;
	private Integer atPin;
	// TODO add logical pin e.g. getDigital(pin)
	
	private Capability defaultCapability;
	private String description;

	private List<Capability> capabilities = new ArrayList<Capability>();
	
	/**
	 * Contains all possible pin configurations and the associated AT command value
	 * 
	 * @author andrew
	 *
	 */
	public enum Capability {
		DISABLED (0),
		RTS_FLOW_CTRL(1),
		CTS_FLOW_CTRL(1),
		RSSI_PWM (1),
		ASSOC_LED (1),
		ANALOG_INPUT (2),
		PWM_OUTPUT (2),
		DIGITAL_INPUT (3),
		DIGITAL_OUTPUT_LOW (4),
		DIGITAL_OUTPUT_HIGH (5),
		UNMONITORED_INPUT (0), // only zigbee
		NODE_ID_ENABLED (1), // only zigbee
		RS485_TX_LOW(6), // only zigbee
		RS485_TX_HIGH(7); // only zigbee		
	
	    private final int value;
	    
	    Capability(int value) {
	        this.value = value;
	    }

		public int getValue() {
			return value;
		}
		
		public static String printAll(String delimiter) {
			StringBuffer sb = new StringBuffer();
			
			for (Capability cap : Capability.values()) {
				sb.append(System.getProperty("line.separator"));
				sb.append(delimiter);
				sb.append(cap);
				sb.append(delimiter);
				sb.append("0x" + cap.getValue());
				sb.append(cap.getValue());
				sb.append(delimiter);
			}
			
			return sb.toString();
		}
	}
	
	public XBeePin(String name, Integer pin, String atCommand, Integer atPin, Capability defaultCapability, String description, Capability... capabilityArr) {
		this.setName(name);
		this.setPin(pin);
		this.setAtCommand(atCommand);
		this.setAtPin(atPin);
		this.setDefaultCapability(defaultCapability);
		this.setDescription(description);
		
		if (capabilityArr != null) {
			for (Capability capability: capabilityArr) {
				this.getCapabilities().add(capability);
			}			
		}
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public Integer getPin() {
		return pin;
	}

	public void setPin(Integer pin) {
		this.pin = pin;
	}

	public String getAtCommand() {
		return atCommand;
	}

	public void setAtCommand(String atCommand) {
		this.atCommand = atCommand;
	}

	public Capability getDefaultCapability() {
		return defaultCapability;
	}

	public void setDefaultCapability(Capability defaultCapability) {
		this.defaultCapability = defaultCapability;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public List<Capability> getCapabilities() {
		return capabilities;
	}

	public void setCapabilities(List<Capability> capabilities) {
		this.capabilities = capabilities;
	}
	
	public Integer getAtPin() {
		return atPin;
	}

	public void setAtPin(Integer atPin) {
		this.atPin = atPin;
	}
	
	private final static List<XBeePin> zigBeePins = new ArrayList<XBeePin>();
	
	static {
		// notes: DIO13/DIO8/DIO9 not supported
		// TODO P0/P1 is missing pwm output option?? could it be 0x2?
		zigBeePins.add(new XBeePin("PWM0/RSSI/DIO10", 6, "P0", 10, Capability.RSSI_PWM, "PWM Output 0 / RX Signal Strength Indicator / Digital IO", 
				Capability.DISABLED, Capability.RSSI_PWM, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, 
				Capability.DIGITAL_OUTPUT_HIGH));
		zigBeePins.add(new XBeePin("PWM/DIO11", 7, "P1", 1, Capability.UNMONITORED_INPUT, "Digital I/O 11", Capability.UNMONITORED_INPUT, 
				Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, Capability.DIGITAL_OUTPUT_HIGH));
		zigBeePins.add(new XBeePin("DIO12", 4, "P2", 2, Capability.UNMONITORED_INPUT, "Digital I/O 12", Capability.UNMONITORED_INPUT, 
				Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, Capability.DIGITAL_OUTPUT_HIGH));
		zigBeePins.add(new XBeePin("AD0/DIO0/Commissioning Button", 20, "D0", 0, Capability.NODE_ID_ENABLED, 
				"Analog Input 0, Digital IO 0, or Commissioning Button", Capability.DISABLED, Capability.NODE_ID_ENABLED, 
				Capability.ANALOG_INPUT, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, Capability.DIGITAL_OUTPUT_HIGH));
		zigBeePins.add(new XBeePin("AD1/DIO1", 19, "D1", 1, Capability.DISABLED, "Analog Input 1 or Digital I/O 1", Capability.DISABLED, 
				Capability.ANALOG_INPUT, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, Capability.DIGITAL_OUTPUT_HIGH));
		zigBeePins.add(new XBeePin("AD2/DIO2", 18, "D2", 2, Capability.DISABLED, "Analog Input 2 or Digital I/O 2", Capability.DISABLED, 
				Capability.ANALOG_INPUT, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, Capability.DIGITAL_OUTPUT_HIGH));
		zigBeePins.add(new XBeePin("AD3/DIO3", 17, "D3", 3, Capability.DISABLED, "Analog Input 3 or Digital I/O 3", Capability.DISABLED, 
				Capability.ANALOG_INPUT, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, Capability.DIGITAL_OUTPUT_HIGH));
		zigBeePins.add(new XBeePin("DIO4", 11, "D4", 4, Capability.DISABLED, "Digital I/O 4", Capability.DISABLED, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, Capability.DIGITAL_OUTPUT_HIGH));
		zigBeePins.add(new XBeePin("Associate/DIO5", 15, "D5", 5, Capability.ASSOC_LED, "Associated Indicator, Digital I/O 5", 
				Capability.DISABLED, Capability.ASSOC_LED, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, 
				Capability.DIGITAL_OUTPUT_HIGH));
		zigBeePins.add(new XBeePin("CTS/DIO7", 12, "D7", 7, Capability.CTS_FLOW_CTRL, "Clear-to-Send Flow Control or Digital I/O 7", 
				Capability.DISABLED, Capability.CTS_FLOW_CTRL, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, 
				Capability.DIGITAL_OUTPUT_HIGH, Capability.RS485_TX_LOW, Capability.RS485_TX_HIGH));
		
		// TODO manual lists only RTS and disabled but x-ctu lists all digital capabilities
		zigBeePins.add(new XBeePin("RTS/DIO6", 16, "D6", 6, Capability.DISABLED, "Request-to-Send Flow Control, Digital I/O 6", 
				Capability.DISABLED, Capability.RTS_FLOW_CTRL, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, 
				Capability.DIGITAL_OUTPUT_HIGH));
		
		
		
		// other pins
		zigBeePins.add(new XBeePin("VCC", 1, null, null, null, "Power Supply", (Capability[])null));
		zigBeePins.add(new XBeePin("DOUT", 2, null, null, null, "UART Data Out", (Capability[])null));
		zigBeePins.add(new XBeePin("DIN", 3, null, null, null, "UART Data In", (Capability[])null));
		zigBeePins.add(new XBeePin("RESET", 5, null, null, null, "Module Reset (reset pulse must be at least 200 ns)", (Capability[])null));
		zigBeePins.add(new XBeePin("[reserved]", 8, null, null, null, "Do not connect", (Capability[])null));
		// DIO8 not supported according to manual
		zigBeePins.add(new XBeePin("DTR/SLEEP_RQ", 9, null, null, null, "Pin Sleep Control Line", (Capability[])null));
		zigBeePins.add(new XBeePin("GND", 10, null, null, null, "Ground", (Capability[])null));
		// DIO9 not supported according to manual
		zigBeePins.add(new XBeePin("ON/SLEEP", 13, null, null, null, "Module Status Indicator", (Capability[])null));
		zigBeePins.add(new XBeePin("VREF", 14, null, null, null, "Not used on this module. For compatibility with other XBee modules, we recommend connecting this pin to a voltage reference if Analog sampling is desired. Otherwise, connect to GND", (Capability[])null));
	
		Collections.sort(zigBeePins, new PinSorter());
	}
	
	/**
	 * Sorts by pin number
	 * 
	 * @author andrew
	 *
	 */
	private static class PinSorter implements Comparator {
		public int compare(Object o1, Object o2) {
			return ((XBeePin)o1).getPin().compareTo(((XBeePin)o2).getPin());
		}		
	}
	
	public static List<XBeePin> getZigBeePins() {
		return zigBeePins;
	}
	
	private final static List<XBeePin> wpanPins = new ArrayList<XBeePin>();
	
	static { 
		wpanPins.add(new XBeePin("DTR/SLEEP_RQ/DI8", 9, "D8", 8, Capability.DISABLED, "Pin Sleep Control Line or Digital Input 8", Capability.DISABLED, Capability.DIGITAL_INPUT));
		wpanPins.add(new XBeePin("CTS/DIO7", 12, "D7", 7, Capability.CTS_FLOW_CTRL, "Clear-to-Send Flow Control or Digital I/O 7", Capability.DISABLED, Capability.CTS_FLOW_CTRL, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, Capability.DIGITAL_OUTPUT_HIGH));
		wpanPins.add(new XBeePin("RTS/AD6/DIO6", 16, "D6", 6, Capability.DISABLED, "Request-to-Send Flow Control, Analog Input 6 or Digital I/O 6", Capability.DISABLED, Capability.RTS_FLOW_CTRL, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, Capability.DIGITAL_OUTPUT_HIGH));
		wpanPins.add(new XBeePin("Associate/AD5/DIO5", 15, "D5", 5, Capability.ASSOC_LED, "Associated Indicator, Analog Input 5 or Digital I/O 5", Capability.DISABLED, Capability.ASSOC_LED, Capability.ANALOG_INPUT, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, Capability.DIGITAL_OUTPUT_HIGH));
		wpanPins.add(new XBeePin("AD4/DIO4", 11, "D4", 4, Capability.DISABLED, "Analog Input 4 or Digital I/O 4", Capability.DISABLED, Capability.ANALOG_INPUT, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, Capability.DIGITAL_OUTPUT_HIGH));
		wpanPins.add(new XBeePin("AD3/DIO3", 17, "D3", 3, Capability.DISABLED, "Analog Input 3 or Digital I/O 3", Capability.DISABLED, Capability.ANALOG_INPUT, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, Capability.DIGITAL_OUTPUT_HIGH));
		wpanPins.add(new XBeePin("AD2/DIO2", 18, "D2", 2, Capability.DISABLED, "Analog Input 2 or Digital I/O 2", Capability.DISABLED, Capability.ANALOG_INPUT, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, Capability.DIGITAL_OUTPUT_HIGH));
		wpanPins.add(new XBeePin("AD1/DIO1", 19, "D1", 1, Capability.DISABLED, "Analog Input 1 or Digital I/O 1", Capability.DISABLED, Capability.ANALOG_INPUT, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, Capability.DIGITAL_OUTPUT_HIGH));
		wpanPins.add(new XBeePin("AD0/DIO0", 20, "D0", 0, Capability.DISABLED, "Analog Input 0 or Digital I/O 0", Capability.DISABLED, Capability.ANALOG_INPUT, Capability.DIGITAL_INPUT, Capability.DIGITAL_OUTPUT_LOW, Capability.DIGITAL_OUTPUT_HIGH));
		wpanPins.add(new XBeePin("PWM0/RSSI", 6, "P0", null, Capability.RSSI_PWM, "PWM Output 0 / RX Signal Strength Indicator", Capability.DISABLED, Capability.RSSI_PWM, Capability.PWM_OUTPUT));
		wpanPins.add(new XBeePin("PWM1", 7, "P1", null, Capability.DISABLED, "PWM Output 1", Capability.DISABLED,  Capability.RSSI_PWM, Capability.PWM_OUTPUT));
		
	
		// other pins
		wpanPins.add(new XBeePin("VCC", 1, null, null, null, "Power Supply", (Capability[])null));
		wpanPins.add(new XBeePin("DOUT", 2, null, null, null, "UART Data Out", (Capability[])null));
		wpanPins.add(new XBeePin("DIN/CONFIG", 3, null, null, null, "UART Data In", (Capability[])null));
		wpanPins.add(new XBeePin("DO8", 4, null, null, null, "Digital Output 8 (not supported as of 2/28/09)", (Capability[])null));
		wpanPins.add(new XBeePin("RESET", 5, null, null, null, "Module Reset (reset pulse must be at least 200 ns)", (Capability[])null));
		wpanPins.add(new XBeePin("[reserved]", 8, null, null, null, "Do not connect", (Capability[])null));
		wpanPins.add(new XBeePin("GND", 10, null, null, null, "Ground", (Capability[])null));
		wpanPins.add(new XBeePin("ON/SLEEP", 13, null, null, null, "Module Status Indicator", (Capability[])null));
		wpanPins.add(new XBeePin("VREF", 14, null, null, null, "Voltage Reference for A/D Inputs", (Capability[])null));
		
		Collections.sort(wpanPins, new PinSorter());
	}
	
	public static String printAll(List<XBeePin> pins, String delimiter) {
		
		StringBuffer sb = new StringBuffer();
		
		sb.append(delimiter);
		sb.append("*Pin #*");
		sb.append(delimiter);
		sb.append("*AT Command*");
		sb.append(delimiter);
		sb.append("*Description*");
		sb.append(delimiter);
		sb.append("*Default Configuration*");
		sb.append(delimiter);
		sb.append("*Supported Configurations (Value)*");
		sb.append(delimiter);
		sb.append("*Analog I/O Method*");
		sb.append(delimiter);
		sb.append("*Digital I/O Method*");
		sb.append(delimiter);
		
		for (XBeePin pin : pins) {
			sb.append(System.getProperty("line.separator"));
			
			sb.append(delimiter);
			sb.append(pin.getPin());
			sb.append(delimiter);
			sb.append(pin.getAtCommand() == null ? "n/a" : pin.getAtCommand());
			sb.append(delimiter);
			sb.append(pin.getDescription());
			sb.append(delimiter);
			sb.append(pin.getDefaultCapability() == null ? "n/a" : pin.getDefaultCapability());
			sb.append(delimiter);
			
			boolean first = false;
			
			for (Capability cap : pin.getCapabilities()) {
				if (!first) {
					first = true;
				} else {
					sb.append(", ");
				}
				
				sb.append(cap + " (0x" + cap.getValue() + ")");
				//sb.append(cap);
			}
		
			sb.append(delimiter);
			
			// analog methods
			if (pin.getCapabilities().contains(Capability.ANALOG_INPUT)) {
				sb.append("getAnalog" + pin.getAtPin() + "()");
			} else {
				sb.append("n/a");
			}

			sb.append(delimiter);
			
			// digital methods
			if (pin.getCapabilities().contains(Capability.DIGITAL_INPUT)) {
				sb.append("isD" + pin.getAtPin() + "On()");
			} else {
				sb.append("n/a");
			}
			
			sb.append(delimiter);
		}
		
		return sb.toString();
	}
	
	public static List<XBeePin> getWpanPins() {
		return wpanPins;
	}
	
	public static void main(String[] args) {
		// print for wiki table
		System.out.println(Capability.printAll("|| "));
		
		System.out.println("Series 1:");
		System.out.println(printAll(wpanPins, "|| "));
		System.out.println("Series 2:");
		System.out.println(printAll(zigBeePins, "|| "));
	}
}
