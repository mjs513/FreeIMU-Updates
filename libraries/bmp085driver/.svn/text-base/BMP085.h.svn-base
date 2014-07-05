/****************************************************************************
* BMP085.h - BMP085/I2C (Digital Pressure Sensor) library for Arduino       *
* Copyright 2010-2012 Filipe Vieira & various contributors                  *
*                                                                           *
* This file is part of BMP085 Arduino library.                              *
*                                                                           *
* This library is free software: you can redistribute it and/or modify      *
* it under the terms of the GNU Lesser General Public License as published  *
* by the Free Software Foundation, either version 3 of the License, or      *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU Lesser General Public License for more details.                       *
*                                                                           *
* You should have received a copy of the GNU Lesser General Public License  *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
****************************************************************************/
/****************************************************************************
* Tested on Arduino Mega with BMP085 Breakout                               *
* SDA   -> pin 20   (no pull up resistors)                                  *
* SCL   -> pin 21   (no pull up resistors)                                  *
* XCLR  -> not connected                                                    *
* EOC   -> not connected                                                    *
* GND   -> pin GND                                                          *
* VCC   -> pin 3.3V                                                         *
* NOTE: SCL and SDA needs pull-up resistors for each I2C bus.               *
*  2.2kOhm..10kOhm, typ. 4.7kOhm                                            *
*****************************************************************************/
#ifndef BMP085_h
#define BMP085_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define BMP085_ADDR                 0x77     //0x77 default I2C address
#define BUFFER_SIZE                 3

#define AUTO_UPDATE_TEMPERATURE     true    //default is true
        // when true, temperature is measured everytime pressure is measured (Auto).
        // when false, user chooses when to measure temperature (just call calcTrueTemperature()).
        // used for dynamic measurement to increase sample rate (see BMP085 modes below).
       
/* ---- Registers ---- */
#define CAL_AC1           0xAA  // R   Calibration data (16 bits)
#define CAL_AC2           0xAC  // R   Calibration data (16 bits)
#define CAL_AC3           0xAE  // R   Calibration data (16 bits)    
#define CAL_AC4           0xB0  // R   Calibration data (16 bits)
#define CAL_AC5           0xB2  // R   Calibration data (16 bits)
#define CAL_AC6           0xB4  // R   Calibration data (16 bits)
#define CAL_B1            0xB6  // R   Calibration data (16 bits)
#define CAL_B2            0xB8  // R   Calibration data (16 bits)
#define CAL_MB            0xBA  // R   Calibration data (16 bits)
#define CAL_MC            0xBC  // R   Calibration data (16 bits)
#define CAL_MD            0xBE  // R   Calibration data (16 bits)
#define CONTROL           0xF4  // W   Control register 
#define CONTROL_OUTPUT    0xF6  // R   Output registers 0xF6=MSB, 0xF7=LSB, 0xF8=XLSB

// unused registers
#define SOFTRESET         0xE0
#define VERSION           0xD1  // ML_VERSION  pos=0 len=4 msk=0F  AL_VERSION pos=4 len=4 msk=f0
#define CHIPID            0xD0  // pos=0 mask=FF len=8
                                // BMP085_CHIP_ID=0x55

/************************************/
/*    REGISTERS PARAMETERS          */
/************************************/
// BMP085 Modes
#define MODE_ULTRA_LOW_POWER    0 //oversampling=0, internalsamples=1, maxconvtimepressure=4.5ms, avgcurrent=3uA, RMSnoise_hPA=0.06, RMSnoise_m=0.5
#define MODE_STANDARD           1 //oversampling=1, internalsamples=2, maxconvtimepressure=7.5ms, avgcurrent=5uA, RMSnoise_hPA=0.05, RMSnoise_m=0.4
#define MODE_HIGHRES            2 //oversampling=2, internalsamples=4, maxconvtimepressure=13.5ms, avgcurrent=7uA, RMSnoise_hPA=0.04, RMSnoise_m=0.3
#define MODE_ULTRA_HIGHRES      3 //oversampling=3, internalsamples=8, maxconvtimepressure=25.5ms, avgcurrent=12uA, RMSnoise_hPA=0.03, RMSnoise_m=0.25
                  // "Sampling rate can be increased to 128 samples per second (standard mode) for
                  // dynamic measurement.In this case it is sufficient to measure temperature only 
                  // once per second and to use this value for all pressure measurements during period."
                  // (from BMP085 datasheet Rev1.2 page 10).
                  // To use dynamic measurement set AUTO_UPDATE_TEMPERATURE to false and
                  // call calcTrueTemperature() from your code. 
// Control register
#define READ_TEMPERATURE        0x2E 
#define READ_PRESSURE           0x34 
//Other
#define MSLP                    101325          // Mean Sea Level Pressure = 1013.25 hPA (1hPa = 100Pa = 1mbar)



class BMP085 {
public:  
  BMP085();
  
  // BMP initialization
  void init();                                              // sets current elevation above ground level to 0 meters
  void init(byte _BMPMode, int32_t _initVal, bool _centimeters);   // sets a reference datum
                                                            // if _centimeters=false _initVal is Pa
  // Who Am I
  byte getDevAddr();
  
  // BMP mode  
  byte getMode();        
  void setMode(byte _BMPMode);                   // BMP085 mode 
  // initialization
  void setLocalPressure(int32_t _Pa);            // set known barometric pressure as reference Ex. QNH
  void setLocalAbsAlt(int32_t _centimeters);     // set known altitude as reference
  void setAltOffset(int32_t _centimeters);       // altitude offset
  void sethPaOffset(int32_t _Pa);                // pressure offset
  void zeroCal(int32_t _Pa, int32_t _centimeters);// zero Calibrate output to a specific Pa/altitude 
  // BMP Sensors
  void getPressure(int32_t *_Pa);                // pressure in Pa + offset  
  void getAltitude(int32_t *_centimeters);       // altitude in centimeters + offset  
  void getTemperature(int32_t *_Temperature);    // temperature in Cº   
  void calcTrueTemperature();                    // calc temperature data b5 (only needed if AUTO_UPDATE_TEMPERATURE is false)  
  void calcTruePressure(long *_TruePressure);    // calc Pressure in Pa     
  // dummy stuff
   void dumpCalData();                           // debug only

  void writemem(uint8_t _addr, uint8_t _val);
  void readmem(uint8_t _addr, uint8_t _nbytes, uint8_t __buff[]);
  
  private:
  
  int ac1,ac2,ac3,b1,b2,mb,mc,md;               // cal data  
  unsigned int ac4,ac5,ac6;                     // cal data
  long b5, oldEMA;                                      // temperature data
  
  uint8_t _dev_address;
  byte _buff[BUFFER_SIZE];                      // buffer  MSB LSB XLSB
  int _oss;                                     // OverSamplingSetting
  int _pressure_waittime[4];                    // Max. Conversion Time Pressure is ms for each mode
  
  int32_t _cm_Offset, _Pa_Offset;
  int32_t _param_datum, _param_centimeters;

  void getCalData();        
  

};

#endif