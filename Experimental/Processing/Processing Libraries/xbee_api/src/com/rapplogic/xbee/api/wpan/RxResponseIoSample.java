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

import java.io.IOException;

import org.apache.log4j.Logger;

import com.rapplogic.xbee.api.NoRequestResponse;
import com.rapplogic.xbee.util.ByteUtils;
import com.rapplogic.xbee.util.IIntArrayInputStream;

/**
 * Series 1 XBee. Represents an I/O sample
 * <p/>
 * See http://code.google.com/p/xbee-api/wiki/XBeePins for information on configuring
 * the XBee for digital/analog inputs.
 * <p/>
 * API ID: 64-bit 0x82, 16-bit: 0x83
 * <p/>
 * @author andrew
 *
 */
public class RxResponseIoSample extends RxBaseResponse implements NoRequestResponse {
	
	private final static Logger log = Logger.getLogger(RxResponseIoSample.class);
	
	public final static int ADC_CHANNEL1 = 0x7e; //01111110
	public final static int DIO_CHANNEL1 = 0x1; //00000001
	
	private IoSample[] samples;
	
	private int channelIndicator1;
	private int channelIndicator2;

	public RxResponseIoSample() {
		
	}

	public void parse(IIntArrayInputStream ps) throws IOException {
		
		// first byte is # of samples
		int sampleSize = ps.read("# I/O Samples");
		
		// create i/o samples array
		this.setSamples(new IoSample[sampleSize]);
		
		// channel indicator 1
		this.setChannelIndicator1(ps.read("Channel Indicator 1"));
		
		log.debug("channel indicator 1 is " + ByteUtils.formatByte(this.getChannelIndicator1()));
		
		// channel indicator 2 (dio)
		this.setChannelIndicator2(ps.read("Channel Indicator 2"));
		
		log.debug("channel indicator 2 is " + ByteUtils.formatByte(this.getChannelIndicator2()));	
		
		// collect each sample
		for (int i = 0; i < this.getSamples().length; i++) {
			
			log.debug("parsing sample " + (i + 1));
			
			IoSample sample = parseIoSample(ps);
			
			// attach sample to parent
			this.getSamples()[i] = sample;
		}		
	}
			
	private IoSample parseIoSample(IIntArrayInputStream ps) throws IOException {

		IoSample sample = new IoSample(this);
		
		// DIO 8 occupies the first bit of the adcHeader
		if (this.containsDigital()) {
			// at least one DIO line is active
			// next two bytes are DIO
			
			log.debug("Digital I/O was received");
			
			sample.setDioMsb(ps.read("DIO MSB"));
			sample.setDioLsb(ps.read("DIO LSB"));
		}
		
		// ADC is active if any of bits 2-7 are on
		if (this.containsAnalog()) {
			// adc is active
			// adc is 10 bits
			
			log.debug("Analog input was received");
			
			// 10-bit values are read two bytes per sample
			
			int analog = 0;
			
			// Analog inputs A0-A5 are bits 2-7 of the adcHeader
			
			if (this.isA0Enabled()) {
				sample.setAnalog0(ByteUtils.parse10BitAnalog(ps, analog));
				analog++;				
			}

			if (this.isA1Enabled()) {
				sample.setAnalog1(ByteUtils.parse10BitAnalog(ps, analog));
				analog++;
			}

			if (this.isA2Enabled()) {
				sample.setAnalog2(ByteUtils.parse10BitAnalog(ps, analog));
				analog++;
			}

			if (this.isA3Enabled()) {
				sample.setAnalog3(ByteUtils.parse10BitAnalog(ps, analog));
				analog++;
			}

			if (this.isA4Enabled()) {
				sample.setAnalog4(ByteUtils.parse10BitAnalog(ps, analog));
				analog++;
			}
			
			if (this.isA5Enabled()) {
				sample.setAnalog5(ByteUtils.parse10BitAnalog(ps, analog));
				analog++;
			}
			
			log.debug("There are " + analog + " analog inputs turned on");
		}
		
		return sample;
	}
	
	public IoSample[] getSamples() {
		return samples;
	}

	public void setSamples(IoSample[] samples) {
		this.samples = samples;
	}
	
	public boolean isDigitalEnabled(int pin) {
		if (pin >= 0 && pin <= 7) { 
			return ByteUtils.getBit(channelIndicator2, pin + 1);
		} else if (pin == 8) {
			return ByteUtils.getBit(channelIndicator1, 1);
		} else {
			throw new IllegalArgumentException("Unsupported pin: " + pin);
		}
	}

	public boolean isD0Enabled() {
		return ByteUtils.getBit(channelIndicator2, 1);
	}

	public boolean isD1Enabled() {
		return ByteUtils.getBit(channelIndicator2, 2);
	}

	public boolean isD2Enabled() {
		return ByteUtils.getBit(channelIndicator2, 3);
	}

	public boolean isD3Enabled() {
		return ByteUtils.getBit(channelIndicator2, 4);
	}
	
	public boolean isD4Enabled() {
		return ByteUtils.getBit(channelIndicator2, 5);
	}
	
	public boolean isD5Enabled() {
		return ByteUtils.getBit(channelIndicator2, 6);
	}
	
	public boolean isD6Enabled() {
		return ByteUtils.getBit(channelIndicator2, 7);
	}
	
	public boolean isD7Enabled() {
		return ByteUtils.getBit(channelIndicator2, 8);
	}	

	public boolean isD8Enabled() {
		return ByteUtils.getBit(channelIndicator1, 1);
	}	
	
	public boolean isAnalogEnabled(int pin) {
		if (pin >= 0 && pin <= 5) {
			return  ByteUtils.getBit(channelIndicator1, pin + 2);
		} else {
			throw new IllegalArgumentException("Unsupported pin: " + pin);
		}
	}
		
	public boolean isA0Enabled() {
		return ByteUtils.getBit(channelIndicator1, 2);
	}

	public boolean isA1Enabled() {
		return ByteUtils.getBit(channelIndicator1, 3);
	}
	
	public boolean isA2Enabled() {
		return ByteUtils.getBit(channelIndicator1, 4);
	}
	
	public boolean isA3Enabled() {
		return ByteUtils.getBit(channelIndicator1, 5);
	}
	
	public boolean isA4Enabled() {
		return ByteUtils.getBit(channelIndicator1, 6);
	}
	
	public boolean isA5Enabled() {
		return ByteUtils.getBit(channelIndicator1, 7);
	}
	
	public int getChannelIndicator1() {
		return channelIndicator1;
	}

	public void setChannelIndicator1(int channelIndicator1) {
		this.channelIndicator1 = channelIndicator1;
	}

	public int getChannelIndicator2() {
		return channelIndicator2;
	}

	public void setChannelIndicator2(int channelIndicator2) {
		this.channelIndicator2 = channelIndicator2;
	}

	/**
	 * Return true if this packet contains at least one analog sample
	 */
	public boolean containsAnalog() {
		// ADC is active if > 0 after channel mask is applied
		return (this.channelIndicator1 & ADC_CHANNEL1) > 0;
	}
	
	/**
	 * Returns true if this packet contains at least one digital sample
	 * 
	 * @return
	 */
	public boolean containsDigital() {
		// DIO 8 occupies the first bit of the adcHeader
		return (this.channelIndicator1 & DIO_CHANNEL1) > 0 || this.channelIndicator2 > 0;
	}
	
	public String toString() {
			
		StringBuilder sb = new StringBuilder();
		
		sb.append(super.toString());
		
		sb.append(",#samples=" + this.samples.length);
				
		for (int i = 0; i < samples.length; i++) {
			sb.append(",Sample#" + (i + 1) + ":" + samples[i].toString() + "]");
		}
		
		return sb.toString();
	}
}