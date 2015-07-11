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

package com.rapplogic.xbee.api.zigbee;

import java.io.IOException;

import org.apache.log4j.Logger;

import com.rapplogic.xbee.api.AtCommandResponse;
import com.rapplogic.xbee.api.NoRequestResponse;
import com.rapplogic.xbee.api.XBeeParseException;
import com.rapplogic.xbee.util.ByteUtils;
import com.rapplogic.xbee.util.IIntArrayInputStream;
import com.rapplogic.xbee.util.IntArrayInputStream;

/**
 * Series 2 XBee.  Represents an I/O Sample resposne sent from a remote radio.
 * Provides access to the XBee's 4 Analog (0-4), 11 Digital (0-7,10-12), and 1 Supply Voltage pins
 * <p/>
 * Note: Series 2 XBee does not support multiple samples (IT) per packet
 * <p/>
 * @author andrew
 *
 */
public class ZNetRxIoSampleResponse extends ZNetRxBaseResponse implements NoRequestResponse {
	
	private final static Logger log = Logger.getLogger(ZNetRxIoSampleResponse.class);
	
	private int digitalChannelMaskMsb;
	private int digitalChannelMaskLsb;
	private int analogChannelMask;

	// all values that may not be in the packet use Integer to distinguish between null and non-null
	private Integer dioMsb;
	private Integer dioLsb;
	
	private final static int SUPPLY_VOLTAGE_INDEX = 4;
	private Integer[] analog = new Integer[5];
	
	public ZNetRxIoSampleResponse() {
		
	}

	public static ZNetRxIoSampleResponse parseIsSample(AtCommandResponse response) throws IOException {
		
		if (!response.getCommand().equals("IS")) {
			throw new RuntimeException("This is only applicable to the \"IS\" AT command");
		}
		
		IntArrayInputStream in = new IntArrayInputStream(response.getValue());
		ZNetRxIoSampleResponse sample = new ZNetRxIoSampleResponse();
		sample.parse(in);
		
		return sample;
	}
	
	/**
	 * This method is a bit non standard since it needs to parse an IO sample
	 * from either a RX response or a Remote AT/Local AT response (IS).
	 * 
	 * @param ps
	 * @throws IOException
	 */
	public void parse(IIntArrayInputStream ps) throws IOException {
		// eat sample size.. always 1
		int size = ps.read("ZNet RX IO Sample Size");
		
		if (size != 1) {
			throw new XBeeParseException("Sample size is not supported if > 1 for ZNet I/O");
		}
		
		this.setDigitalChannelMaskMsb(ps.read("ZNet RX IO Sample Digital Mask 1"));
		this.setDigitalChannelMaskLsb(ps.read("ZNet RX IO Sample Digital Mask 2"));
		this.setAnalogChannelMask(ps.read("ZNet RX IO Sample Analog Channel Mask"));
		
		// zero out n/a bits
		this.analogChannelMask = this.analogChannelMask & 0x8f; //10001111
		// zero out all but bits 3-5
		// TODO apparent bug: channel mask on ZigBee Pro firmware has DIO10/P0 as enabled even though it's set to 01 (RSSI).  Digital value reports low. 
		this.digitalChannelMaskMsb = this.digitalChannelMaskMsb & 0x1c; //11100
				
		if (this.containsDigital()) {
			log.info("response contains digital data");
			// next two bytes are digital
			this.setDioMsb(ps.read("ZNet RX IO DIO MSB"));
			this.setDioLsb(ps.read("ZNet RX IO DIO LSB"));
		} else {
			log.info("response does not contain digital data");
		}
		
		// parse 10-bit analog values
		
		int enabledCount = 0;
		
		for (int i = 0; i < 4; i++) {
			if (this.isAnalogEnabled(i)) {
				log.info("response contains analog[" + i + "]");
				analog[i] = ByteUtils.parse10BitAnalog(ps, enabledCount);
				enabledCount++;
			}			
		}
		
		if (this.isSupplyVoltageEnabled()) {
			analog[SUPPLY_VOLTAGE_INDEX] = ByteUtils.parse10BitAnalog(ps, enabledCount);
			enabledCount++;
		}
		
		log.debug("There are " + analog + " analog inputs in this packet");
	}
	
	public int getDigitalChannelMaskMsb() {
		return digitalChannelMaskMsb;
	}

	private void setDigitalChannelMaskMsb(int digitalChannelMask1) {
		this.digitalChannelMaskMsb = digitalChannelMask1;
	}

	public int getDigitalChannelMaskLsb() {
		return digitalChannelMaskLsb;
	}

	private void setDigitalChannelMaskLsb(int digitalChannelMask2) {
		this.digitalChannelMaskLsb = digitalChannelMask2;
	}

	public int getAnalogChannelMask() {
		return analogChannelMask;
	}

	private void setAnalogChannelMask(int analogChannelMask) {
		this.analogChannelMask = analogChannelMask;
	}

	/**
	 * Consider using isDigitalEnabled(pin) instead
	 * 
	 * @return
	 */
	public boolean isD0Enabled() {
		return this.isDigitalEnabled(0);
	}

	public boolean isD1Enabled() {
		return this.isDigitalEnabled(1);
	}

	public boolean isD2Enabled() {
		return this.isDigitalEnabled(2);
	}

	public boolean isD3Enabled() {
		return this.isDigitalEnabled(3);
	}

	public boolean isD4Enabled() {
		return this.isDigitalEnabled(4);
	}

	public boolean isD5Enabled() {
		return this.isDigitalEnabled(5);
	}

	public boolean isD6Enabled() {
		return this.isDigitalEnabled(6);
	}

	public boolean isD7Enabled() {
		return this.isDigitalEnabled(7);
	}
	
	public boolean isD10Enabled() {
		return this.isDigitalEnabled(10);
	}
	
	public boolean isD11Enabled() {
		return this.isDigitalEnabled(11);
	}
	
	public boolean isD12Enabled() {
		return this.isDigitalEnabled(12);
	}
	
	/**
	 * Consider using isAnalogEnable(pin) instead
	 * 
	 * @return
	 */
	public boolean isA0Enabled() {
		return this.isAnalogEnabled(0);
	}

	public boolean isA1Enabled() {
		return this.isAnalogEnabled(1);
	}
	
	public boolean isA2Enabled() {
		return this.isAnalogEnabled(2);
	}
	
	public boolean isA3Enabled() {
		return this.isAnalogEnabled(3);
	}	
	
	public boolean isDigitalEnabled(int pin) {
		if (pin >=0 && pin <= 7) {
			return ByteUtils.getBit(this.digitalChannelMaskLsb, pin + 1);
		} else if (pin >=10 && pin <= 12) {
			return ByteUtils.getBit(this.digitalChannelMaskMsb, pin - 7);
		} else {
			throw new IllegalArgumentException("Unsupported pin: " + pin);
		}
	}

	public boolean isAnalogEnabled(int pin) {
		if (pin >=0 && pin <= 3) {
			return ByteUtils.getBit(this.analogChannelMask, pin + 1);
		} else {
			throw new IllegalArgumentException("Unsupported pin: " + pin);
		}
	}

	/**
	 * (from the spec) The voltage supply threshold is set with the V+ command.  If the measured supply voltage falls 
	 * below or equal to this threshold, the supply voltage will be included in the IO sample set.  V+ is 
	 * set to 0 by default (do not include the supply voltage). 

	 * @return
	 */
	public boolean isSupplyVoltageEnabled() {
		return ByteUtils.getBit(this.analogChannelMask, 8);
	}

	/**
	 * Consider using isDigitalOn(pin)
	 * 
	 * @return
	 */
	public Boolean isD0On() {
		if (isDigitalEnabled(0)) {
			return isDigitalOn(0);
		}
		
		return null;
	}

	// consider using underscore for readability (isD1_On)
	public Boolean isD1On() {
		if (isDigitalEnabled(1)) {
			return isDigitalOn(1);
		}
		
		return null;
	}
	
	public Boolean isD2On() {
		if (isDigitalEnabled(2)) {
			return isDigitalOn(2);
		}
		
		return null;
	}
	
	public Boolean isD3On() {
		if (isDigitalEnabled(3)) {
			return isDigitalOn(3);
		}
		
		return null;
	}
	
	public Boolean isD4On() {
		if (isDigitalEnabled(4)) {
			return isDigitalOn(4);
		}
		
		return null;
	}

	public Boolean isD5On() {
		if (isDigitalEnabled(5)) {
			return isDigitalOn(5);
		}
		
		return null;
	}
	
	public Boolean isD6On() {
		if (isDigitalEnabled(6)) {
			return isDigitalOn(6);
		}
		
		return null;
	}
	
	public Boolean isD7On() { 
		if (isDigitalEnabled(7)) {
			return isDigitalOn(7);
		}
		
		return null;
	}
	
	public Boolean isD10On() {
		if (isDigitalEnabled(10)) {
			return isDigitalOn(10);
		}
		
		return null;
	}

	public Boolean isD11On() {
		if (isDigitalEnabled(11)) {
			return isDigitalOn(11);
		}
		
		return null;
	}
	
	public Boolean isD12On() {
		if (isDigitalEnabled(12)) {
			return isDigitalOn(12);
		}
		
		return null;
	}	
		
	/**
	 * If digital I/O line (DIO0) is enabled: returns true if digital 0 is HIGH (ON); false if it is LOW (OFF).
	 * If digital I/O line is not enabled this method returns null as it has no value.
	 * <p/>
	 * Important: the pin number corresponds to the logical pin (e.g. D4), not the physical pin number.
	 * <p/>
	 * Digital I/O pins seem to report high when open circuit (unconnected)
	 * 
	 * @return
	 */	
	public Boolean isDigitalOn(int pin) {
		if (this.isDigitalEnabled(pin)) {
			if (pin >=0 && pin <= 7) {
				return ByteUtils.getBit(dioLsb, pin + 1);
			} else if (pin >=10 && pin <= 12) {
				return ByteUtils.getBit(dioMsb, pin - 7);
			}			
		}
		
		return null;
	}

	/**
	 * Returns true if this sample contains data from digital inputs
	 * 
	 * See manual page 68 for byte bit mapping
	 * 
	 * @return
	 */
	public boolean containsDigital() {
        return this.getDigitalChannelMaskMsb() > 0 || this.getDigitalChannelMaskLsb() > 0;
    }
	
	/**
	 * Returns true if this sample contains data from analog inputs or supply voltage
	 * 
	 * How does supply voltage get enabled??
	 * 
	 * See manual page 68 for byte bit mapping
	 * 
	 * @return
	 */
	public boolean containsAnalog() {
        return this.getAnalogChannelMask() > 0;
    }

	/**
	 * Returns the DIO MSB, only if sample contains digital; null otherwise
	 * 
	 * @return
	 */
	public Integer getDioMsb() {
		return dioMsb;
	}

	private void setDioMsb(Integer dioMsb) {
		this.dioMsb = dioMsb;
	}

	/**
	 * Returns the DIO LSB, only if sample contains digital; null otherwise
	 * 
	 * @return
	 */
	public Integer getDioLsb() {
		return dioLsb;
	}

	private void setDioLsb(Integer dioLsb) {
		this.dioLsb = dioLsb;
	}
	
	/**
	 * Consider using getAnalog(pin) instead
	 * 
	 * @return
	 */
	public Integer getAnalog0() {
		return analog[0];
	}

	public void setAnalog0(Integer analog0) {
		analog[0] = analog0;
	}		
	
	public Integer getAnalog1() {
		return analog[1];
	}

	public void setAnalog1(Integer analog1) {
		analog[1] = analog1;
	}

	public Integer getAnalog2() {
		return analog[2];
	}

	public void setAnalog2(Integer analog2) {
		analog[2] = analog2;
	}

	public Integer getAnalog3() {
		return analog[3];
	}

	public void setAnalog3(Integer analog3) {
		analog[3] = analog3;
	}	
	
	/**
	 * Returns a 10 bit value of ADC line 0, if enabled.
	 * Returns null if ADC line 0 is not enabled.
	 * <p/>
	 * The range of Digi XBee series 2 ADC is 0 - 1.2V and although I couldn't find this in the spec 
	 * a few google searches seems to confirm.  When I connected 3.3V to just one of the ADC pins, it 
	 * displayed its displeasure by reporting all ADC pins at 1023.
	 * <p/>
	 * Analog pins seem to float around 512 when open circuit
	 * <p/>
	 * The reason this returns null is to prevent bugs in the event that you thought you were reading the 
	 * actual value when the pin is not enabled.
	 * 
	 * @return
	 */
	public Integer getAnalog(int pin) {
		if (this.isAnalogEnabled(pin)) {
			return analog[pin];
		}
		
		return null;
	}
		
//	public Integer getAnalog(int pin) {
//		// analog starts 19 bytes after MSB length, if no dio enabled
//		int startIndex = 18;
//
//		if (!isAnalogEnabled(pin)) {
//			// this pin is not turned on
//			return null;
//		}
//		
//		if (this.containsDigital()) {
//			// make room for digital i/o
//			startIndex+=2;
//		}
//
//		// start depends on how many pins before this pin are enabled
//		for (int i = 0; i < pin; i++) {
//			if (isAnalogEnabled(i)) {
//				startIndex+=2;
//			}
//		}
//
//		return (this.getProcessedPacketBytes()[startIndex] << 8) + this.getProcessedPacketBytes()[startIndex + 1];
//	}

	public Integer getSupplyVoltage() {
		return analog[SUPPLY_VOLTAGE_INDEX];
	}

	public void setSupplyVoltage(Integer supplyVoltage) {
		analog[SUPPLY_VOLTAGE_INDEX] = supplyVoltage;
	}
	
	public String toString() {
		StringBuilder builder = new StringBuilder();
		
		builder.append(super.toString());
		
		if (this.containsDigital()) {
			for (int i = 0; i <= 7; i++) {
				if (this.isDigitalEnabled(i)) {
					builder.append(",digital[" + i + "]=" + (this.isDigitalOn(i) ? "high" : "low"));
				}
			}
			
			for (int i = 10; i <= 12; i++) {
				if (this.isDigitalEnabled(i)) {
					builder.append(",digital[" + i + "]=" + (this.isDigitalOn(i) ? "high" : "low"));
				}
			}
		}
		
		if (this.containsAnalog()) {
			for (int i = 0; i <= 3; i++) {
				if (this.isAnalogEnabled(i)) {
					builder.append(",analog[" + i + "]=" + this.getAnalog(i));	
				}
			}
			
			if (this.isSupplyVoltageEnabled()) {
				builder.append(",supplyVoltage=" + this.getSupplyVoltage());
			}
		}
		
		return builder.toString();
	}
}