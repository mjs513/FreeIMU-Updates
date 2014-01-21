/****************************************************************************
* ITG3200.h - ITG-3200/I2C library v0.5 for Arduino                         *
* Copyright 2010-2011 Filipe Vieira & various contributors                  *
* http://code.google.com/p/itg-3200driver                                   *
* This file is part of ITG-3200 Arduino library.                            *
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
* Tested on Arduino Mega with ITG-3200 Breakout                             *
* SCL     -> pin 21     (no pull up resistors)                              *
* SDA     -> pin 20     (no pull up resistors)                              *
* CLK & GND -> pin GND                                                    *
* INT       -> not connected  (but can be used)                             *
* VIO & VDD -> pin 3.3V                                                     *
*****************************************************************************/
#ifndef ITG3200_h
#define ITG3200_h

#include "Arduino.h"
#include <Wire.h>

#define ITG3200_ADDR_AD0_HIGH  0x69   //AD0=1 0x69 I2C address when AD0 is connected to HIGH (VCC) - default for sparkfun breakout
#define ITG3200_ADDR_AD0_LOW   0x68   //AD0=0 0x68 I2C address when AD0 is connected to LOW (GND)
// "The LSB bit of the 7 bit address is determined by the logic level on pin 9. 
// This allows two ITG-3200 devices to be connected to the same I2C bus.
// One device should have pin9 (or bit0) LOW and the other should be HIGH." source: ITG3200 datasheet
// Note that pin9 (AD0 - I2C Slave Address LSB) may not be available on some breakout boards so check 
// the schematics of your breakout board for the correct address to use.


#define GYROSTART_UP_DELAY  70    // 50ms from gyro startup + 20ms register r/w startup

/* ---- Registers ---- */
#define WHO_AM_I           0x00  // RW   SETUP: I2C address   
#define SMPLRT_DIV         0x15  // RW   SETUP: Sample Rate Divider       
#define DLPF_FS            0x16  // RW   SETUP: Digital Low Pass Filter/ Full Scale range
#define INT_CFG            0x17  // RW   Interrupt: Configuration
#define INT_STATUS         0x1A  // R	Interrupt: Status
#define TEMP_OUT           0x1B  // R	SENSOR: Temperature 2bytes
#define GYRO_XOUT          0x1D  // R	SENSOR: Gyro X 2bytes  
#define GYRO_YOUT          0x1F  // R	SENSOR: Gyro Y 2bytes
#define GYRO_ZOUT          0x21  // R	SENSOR: Gyro Z 2bytes
#define PWR_MGM            0x3E  // RW	Power Management

/* ---- bit maps ---- */
#define DLPFFS_FS_SEL             0x18  // 00011000
#define DLPFFS_DLPF_CFG           0x07  // 00000111
#define INTCFG_ACTL               0x80  // 10000000
#define INTCFG_OPEN               0x40  // 01000000
#define INTCFG_LATCH_INT_EN       0x20  // 00100000
#define INTCFG_INT_ANYRD_2CLEAR   0x10  // 00010000
#define INTCFG_ITG_RDY_EN         0x04  // 00000100
#define INTCFG_RAW_RDY_EN         0x01  // 00000001
#define INTSTATUS_ITG_RDY         0x04  // 00000100
#define INTSTATUS_RAW_DATA_RDY    0x01  // 00000001
#define PWRMGM_HRESET             0x80  // 10000000
#define PWRMGM_SLEEP              0x40  // 01000000
#define PWRMGM_STBY_XG            0x20  // 00100000
#define PWRMGM_STBY_YG            0x10  // 00010000
#define PWRMGM_STBY_ZG            0x08  // 00001000
#define PWRMGM_CLK_SEL            0x07  // 00000111

/************************************/
/*    REGISTERS PARAMETERS    */
/************************************/
// Sample Rate Divider
#define NOSRDIVIDER         0 // default    FsampleHz=SampleRateHz/(divider+1)
// Gyro Full Scale Range
#define RANGE2000           3   // default
// Digital Low Pass Filter BandWidth and SampleRate
#define BW256_SR8           0   // default    256Khz BW and 8Khz SR
#define BW188_SR1           1
#define BW098_SR1           2
#define BW042_SR1           3
#define BW020_SR1           4
#define BW010_SR1           5
#define BW005_SR1           6
// Interrupt Active logic lvl
#define ACTIVE_ONHIGH       0 // default
#define ACTIVE_ONLOW        1
// Interrupt drive type
#define PUSH_PULL           0 // default
#define OPEN_DRAIN          1
// Interrupt Latch mode
#define PULSE_50US          0 // default
#define UNTIL_INT_CLEARED   1
// Interrupt Latch clear method
#define READ_STATUSREG      0 // default
#define READ_ANYREG         1
// Power management
#define NORMAL              0 // default
#define STANDBY             1
// Clock Source - user parameters
#define INTERNALOSC         0   // default
#define PLL_XGYRO_REF       1
#define PLL_YGYRO_REF       2
#define PLL_ZGYRO_REF       3
#define PLL_EXTERNAL32      4   // 32.768 kHz
#define PLL_EXTERNAL19      5   // 19.2 Mhz

class ITG3200 {

public:
  float gains[3]; 
  int offsets[3];
  float polarities[3];

  ITG3200();
  
  // Gyro initialization
  void init(unsigned int address);
  void init(unsigned int address, byte _SRateDiv, byte _Range, byte _filterBW, byte _ClockSrc, bool _ITGReady, bool _INTRawDataReady);      
    
  // Who Am I
  byte getDevAddr();
  void setDevAddr(unsigned int _addr);
  // Sample Rate Divider
  byte getSampleRateDiv();          
  void setSampleRateDiv(byte _SampleRate);
  // Digital Low Pass Filter BandWidth and SampleRate 
  byte getFSRange();
  void setFSRange(byte _Range); // RANGE2000
  byte getFilterBW(); 
  void setFilterBW(byte _BW); // see register parameters above
  // Interrupt Configuration
  bool isINTActiveOnLow();
  void setINTLogiclvl(bool _State); //ACTIVE_ONHIGH, ACTIVE_ONLOW
  // Interrupt drive type
  bool isINTOpenDrain();
  void setINTDriveType(bool _State); //OPEN_DRAIN, PUSH_PULL
  // Interrupt Latch mode
  bool isLatchUntilCleared();
  void setLatchMode(bool _State); //UNTIL_INT_CLEARED, PULSE_50US
  // Interrupt Latch clear method
  bool isAnyRegClrMode();
  void setLatchClearMode(bool _State); //READ_ANYREG, READ_STATUSREG
  // INT pin triggers
  bool isITGReadyOn();          
  void setITGReady(bool _State);
  bool isRawDataReadyOn();
  void setRawDataReady(bool _State);      
  // Trigger Status
  bool isITGReady();
  bool isRawDataReady();
  // Gyro Sensors
  void readTemp(float *_Temp);
  void readGyroRaw(int  *_GyroXYZ);
  void readGyroRaw(int *_GyroX, int *_GyroY, int *_GyroZ);
  void setRevPolarity(bool _Xpol, bool _Ypol, bool _Zpol);	// true = Reversed  false = default
  void setGains(float _Xgain, float _Ygain, float _Zgain);
  void setOffsets(int _Xoffset, int _Yoffset, int _Zoffset);
  void zeroCalibrate(unsigned int totSamples, unsigned int sampleDelayMS);	// assuming gyroscope is stationary (updates XYZ offsets for zero)
  void readGyroRawCal(int *_GyroX, int *_GyroY, int *_GyroZ);
  void readGyroRawCal(int *_GyroXYZ);
  void readGyro(float *_GyroXYZ); // includes gain and offset
  void readGyro(float *_GyroX, float *_GyroY, float *_GyroZ); // includes gain and offset    
  // Power management
  void reset(); // after reset all registers have default values
  bool isLowPower();
  void setPowerMode(bool _State); // NORMAL, STANDBY
  bool isXgyroStandby();            
  bool isYgyroStandby();
  bool isZgyroStandby();
  void setXgyroStandby(bool _Status); // NORMAL, STANDBY
  void setYgyroStandby(bool _Status);
  void setZgyroStandby(bool _Status);
  byte getClockSource();
  void setClockSource(byte _CLKsource); // see register parameters above
  
  void writemem(uint8_t _addr, uint8_t _val);
  void readmem(uint8_t _addr, uint8_t _nbytes, uint8_t __buff[]);
  
private:

  uint8_t _dev_address;
  uint8_t _buff[6];      
};
#endif