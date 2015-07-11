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

package com.rapplogic.xbee.api.wpan;

import com.rapplogic.xbee.util.ByteUtils;

/**
 * Series 1 XBee.  Represents an I/O Sample, sent from a remote radio.
 * Each I/O packet (RxResponseIoSample) may contain one for more IoSample instances.
 * <p/>
 * This class is accessed from the getSamples() method of RxResponseIoSample, which
 * returns an array of IoSample objects.
 * <p/>
 * Provides access to XBee's 8 Digital (0-7) and 6 Analog (0-5) IO pins
 * <p/>
 * @author andrew
 *
 */
public class IoSample {
	
	private RxResponseIoSample parent;
	
	private Integer dioMsb;
	private Integer dioLsb;
	private Integer[] analog = new Integer[6];

	public IoSample(RxResponseIoSample parent) {
		this.parent = parent;
	}

	public void setDioMsb(Integer dioMsb) {
		this.dioMsb = dioMsb;
	}

	public void setDioLsb(Integer dioLsb) {
		this.dioLsb = dioLsb;
	}
	
	public Integer getDioMsb() {
		return dioMsb;
	}

	public Integer getDioLsb() {
		return dioLsb;
	}
	
	/**
	 * Returns the 10-bit analog value of the specified pin.
	 * Returns null if pin is not configured for Analog input.
	 * 
	 * @return
	 */
	public Integer getAnalog(int pin) {
		if (parent.isAnalogEnabled(pin)) {
			return analog[pin];	
		}
		
		return null;
	}
	
	public Integer getAnalog0() {
		return this.getAnalog(0);
	}

	public void setAnalog0(Integer analog0) {
		analog[0] = analog0;
	}

	/**
	 * Returns the 10-bit analog value of pin 19 (D1), when this pin configured for Analog Input (D1=2)
	 * Returns null if pin 19 is not configured for Analog input.
	 * 
	 * @return
	 */
	public Integer getAnalog1() {
		return this.getAnalog(1);
	}

	public void setAnalog1(Integer analog1) {
		analog[1] = analog1;
	}

	/**
	 * Returns the 10-bit analog value of pin 18 (D2), when this pin configured for Analog Input (D2=2)
	 * Returns null if pin 18 is not configured for Analog input.
	 * 
	 * @return
	 */
	public Integer getAnalog2() {
		return this.getAnalog(2);
	}

	public void setAnalog2(Integer analog2) {
		analog[2] = analog2;
	}

	/**
	 * Returns the 10-bit analog value of pin 17 (D3), when this pin configured for Analog Input (D3=2)
	 * Returns null if pin 17 is not configured for Analog input.
	 * 
	 * @return
	 */
	public Integer getAnalog3() {
		return this.getAnalog(3);
	}

	public void setAnalog3(Integer analog3) {
		analog[3] = analog3;
	}

	/**
	 * Returns the 10-bit analog value of pin 11 (D4), when this pin configured for Analog Input (D4=2)
	 * Returns null if pin 11 is not configured for Analog input.
	 * 
	 * @return
	 */
	public Integer getAnalog4() {
		return this.getAnalog(4);
	}

	public void setAnalog4(Integer analog4) {
		analog[4] = analog4;
	}

	/**
	 * Returns the 10-bit analog value of pin 15 (D5), when this pin configured for Analog Input (D5=2)
	 * Returns null if pin 15 is not configured for Analog input.
	 * 
	 * @return
	 */
	public Integer getAnalog5() {
		return this.getAnalog(5);
	}

	public void setAnalog5(Integer analog5) {
		analog[5] = analog5;
	}

	/**
	 * Returns the digital value of the specified pin.
	 * Returns null if pin is not configured for Digital input 
	 * 
	 * @return
	 */
	public Boolean isDigitalOn(int pin) {
		
		if (!parent.isDigitalEnabled(pin)) {
			return null;
		}
		
		if (pin >= 0 && pin <= 7) {
			return ByteUtils.getBit(dioLsb, pin + 1);
		} else {
			// pin 8
			return ByteUtils.getBit(dioMsb, 1);		
		}
	}

	/**
	 * Returns the digital value of pin 20 (D0) when this pin is configured for Digital input (D0=3)
	 * Returns null if pin 20 is not configured for Digital input 
	 * 
	 * @return
	 */
	public Boolean isD0On() {
		return this.isDigitalOn(0);
	}

	/**
	 * Returns the digital value of pin 19 (D1) when this pin is configured for Digital input (D1=3)
	 * Returns null if pin 19 is not configured for Digital input 
	 * 
	 * @return
	 */
	public Boolean isD1On() {
		return this.isDigitalOn(1);
	}

	/**
	 * Returns the digital value of pin 18 (D2) when this pin is configured for Digital input (D2=3)
	 * Returns null if pin 18 is not configured for Digital input 
	 * 
	 * @return
	 */
	public Boolean isD2On() {
		return this.isDigitalOn(2);
	}	

	/**
	 * Returns the digital value of pin 17 (D3) when this pin is configured for Digital input (D3=3)
	 * Returns null if pin 17 is not configured for Digital input 
	 * 
	 * @return
	 */
	public Boolean isD3On() {
		return this.isDigitalOn(3);	}
	
	/**
	 * Returns the digital value of pin 11 (D4) when this pin is configured for Digital input (D4=3)
	 * Returns null if pin 11 is not configured for Digital input 
	 * 
	 * @return
	 */
	public Boolean isD4On() {
		return this.isDigitalOn(4);
	}
	
	/**
	 * Returns the digital value of pin 15 (D5) when this pin is configured for Digital input (D5=3)
	 * Returns null if pin 15 is not configured for Digital input 
	 * 
	 * @return
	 */
	public Boolean isD5On() {
		return this.isDigitalOn(5);
	}

	/**
	 * Returns the digital value of pin 16 (D6) when this pin is configured for Digital input (D6=3)
	 * Returns null if pin 16 is not configured for Digital input 
	 * 
	 * @return
	 */
	public Boolean isD6On() {
		return this.isDigitalOn(6);
	}
	
	/**
	 * Returns the digital value of pin 12 (D7) when this pin is configured for Digital input (D7=3)
	 * Returns null if pin 12 is not configured for Digital input 
	 * 
	 * @return
	 */	
	public Boolean isD7On() {
		return this.isDigitalOn(7);
	}

	/**
	 * Returns the digital value of pin 9 (D8) when this pin is configured for Digital input (D8=3)
	 * Returns null if pin 9 is not configured for Digital input 
	 * 
	 * @return
	 */	
	public Boolean isD8On() {
		return this.isDigitalOn(8);
	}
	
	
	public String toString() {
		StringBuilder builder = new StringBuilder();
		// TODO only prefix with comma if not first entry written.  Use reflection
			
		if (parent.containsDigital()) {
			for (int i = 0; i <= 8; i++) {
				if (parent.isDigitalEnabled(i)) {
					builder.append(",digital[" + i + "]=" + (this.isDigitalOn(i) ? "high" : "low"));
				}
			}
		}
		
		if (parent.containsAnalog()) {
			for (int i = 0; i <= 5; i++) {
				if (parent.isAnalogEnabled(i)) {
					builder.append(",analog[" + i + "]=" + this.getAnalog(i));	
				}
			}
		}

		return builder.toString();
	}
}