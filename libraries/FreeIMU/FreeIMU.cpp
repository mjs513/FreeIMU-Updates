/*
FreeIMU.cpp - A libre and easy to use orientation sensing library for Arduino
Copyright (C) 2011-2012 Fabio Varesano <fabio at varesano dot net>

Development of this code has been supported by the Department of Computer Science,
Universita' degli Studi di Torino, Italy within the Piemonte Project
http://www.piemonte.di.unito.it/


This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------------------------
Below changes were made by Michael J Smorto, above license still applies. 
All changes to this program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

10-27-2013   1. Modified incorporate temperature correction for gyro and accelerometer
			 2. Corrected __AVR__ to __AVR_ATmega128__
             3. Added call to get temperature from MPU-6050 in getRawValues
10-29-2013	Corrected temp coefficient array for temp
11-01-2013  Added new inverse sqrt code obtained from articles:
			http://pizer.wordpress.com/2008/10/12/fast-inverse-square-root/ and
			http://www.diydrones.com/forum/topics/madgwick-imu-ahrs-and-fast-inverse-square-root
11-05-13	1. Add code to reset MPU-6050 on start-up, implemented in the Arduino sketch as well as
			option to manually reset in processing sketch.
			2. Added LPF code line, currently set to MPU60X0_DLPF_BW_42
11-23-13	1. Changed twoKidef from 0.1 to 0.0f -
				12-12 => twoKiDef changed from 0f to 0.0025f based on article in DIYDrones
			2. Added bypass caps (0.1 and 0.01 ceramic caps on the power pins of the freeIMU
			2a. Added a 10 uF tantalum on the power bus
			3. Yaw drift stabilized very nicely
			4. Updated processing sketch to calculate heading and display based on the freescale 
			   application note and as implemented:
			   http://diydrones.com/profiles/blogs/tiltcompensated-heading
            5. update RESET function to zero out initial values
12-06-13	1. added LSM303D Polulo board instead of  calculating heading from FreeIMU
			2. added option to turn on/off temp correction, temp_corr_on
			3. added variable to set temperature break where calibration turns off, temp_break,
			note: temp_break in units directly from MPU-6050
01-02-14 to
  01-05-14  1. Recalibrated 6050 using Madgewick automated calibration rig - see paper
			2. Corrected temperature calibration coefficients to ensure that gyros are zeroed for whole
			   temp range when at rest - deleted necessary to run zerogyro routine.
			3. Acceleration coefficients adjusted so Ax = 0, Ay = 0 and Az = 1g when at rest
			4. Had to tune Ki to reduce drift - now set to 0.165
			5. Changed Kp to 1.0 - drift almost zero
01-08-14	1. Completed coding ZARA ZUPT for Zero velocity detemination - added code to the
			   processing sketch
01-09-14	1. Added a Reset Quaternion matrix only - option 2 in the sketch - to allow reset of matrix
			   after rapid movement.
11-02-2013  Todo list:
			2. add Yaw Drift correction - noticed that if the temp is above the temp cut-off
			   can get what I would consider unacceptable drift. Will also check temp calibration
			   above cut-off (high temp range)
			   
02-27-14	Fixed code issues with generic MPU6050 output for raw values.
03-02-14	1. Default to temp correction off.
			2. Updating code as temp correction only applicable to MPU-6050 or 9150 at this point.
			3. Clarified GenMPU6050 and DFROBOT defines
			4. Added code for BMP085 altimeter
03-04-14	Fixed issue with increased drift when magnetometer enabled by updating Fabio's code to the Madgwick
			code posted on his X-IO website.
03-09-14	1. With the magnetometer active Kp is better at 0.5 instead of 0.75. Will adjust as appropriate.
03-15-14	Tested code with magnetometer enabled for both Freeimu and DFROBOT. For the DFRobot which had the
			ADXL345 and ITG3200 Kp and Ki had to be adjusted to 0.15 and 0.000002 respectfully. This stabilized
			the drift but still more than the Freeimu. Code added to auto-adjust Kp and Ki depending on the board.
03-16-14	Added LPF to accelerometer and gyro
03-22-14	1. Added ArduPilot Gyro calibration code and modified AP_Math library calling the modifications
			AP_Math_freeimu to identify that changes were made
			2. Added initGyros option "g" to zero gyros from arduino code and processing sketches
			3. Changes resulted in allowing me to put back Kp and Ki to 0.75 and 0.1625 respectfully.
03-24-14	1. Added ArduPilot Filter library so I could implement a median filter on both the accelerometer
			and the gyroscope when using a ADXL345 and ITG3200. Using a 10 point median filter.
			2. Retuned Kp and Ki to 0.05 and 0.000002f, still have a bit of yaw drift. 
			3. Added a LPF filter per guidance at http://philstech.blogspot.com/2012/04/quadcopter-accelerometer-data-filtering.html.
03-25-14	Further tweaked filter factors and Kp/Ki for DFROBOT
				Median Filter = 7 point
				LPF factor = 0.6
03-31-14	Deleted all filtering and adjusted filters for DFROBOT and GENERAL MPU06050 (no mag) - had.
			little impact. Can not get the ADXL345/ITG3200 combo to stabilize as much as the MPU-6050.
04-03-14	Fixed cal curves - don't need gyro temp cal for ITG3200 since internal cal is used - seems to
			to work fine. Need to add back in filter for accelerometer when using ITG3200. 
04-09-14	1. Deleted Fabio's implementation of the AHRS filter and inserted the complete version from
			Seb Madgwick's X-io website. Seemed to fix the mag issue dramatically.
			2. Put median filter back in using a 5 point filter instead of 7
			3. Fixed zero gyro issue when using the ADXL345 chip
04-18-14
			Added 9150 library - 6050 library with getMotion9 function fully operational
			Fixed heading function so heading is aligned with the IMU axis alignment
			Put median filter on mag values
04-19-14
			Added support for GPS in Arduino and Processing Sketch using TinyGPS++ and AltSerialSoft libraries.
			TinyGPS++ : http://arduiniana.org/libraries/tinygpsplus/
			AltSoftwareSerial : https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
04-21-14	1. Added tilt compensated compass just using the HMC5883L or the 9150 on chip magnetomer
			2. Added GPS support using the TinyGPS++ and the AltSoftwareSerial libraries - modified processing
			GUI to support GPS.
04-23-14	Incorporated Butterworth filter from AP_Filter
04-28-14	Modified LS303D Pololu library for use with any magnetometer and accelerometer.
05-13-14
-------- Deleted 9150 library and added support for MPU-9150/9250 using Jeff Rowbergs AK8975 library for the magnetometer and maintained integrity of original 60X0 library
-------- Replaced Median Filter with Butterworh filters
-------- Added Filter library from ArduIMU (nice addition)
-------- Reduced size of serial sketch to fit on arduino uno for calibration
-------- Added azimuth code for heading when magnetometer and accelerometer available by extracting the code segment from the Pololu LSM303D library. Made a library called iCompass so it can be used elsewhere.
-------- In process of modifing code for use with Pololu AltIMU-10 v3.
---------------------------------------------------------------------------
05-17-14
-------- updated FreeIMU examples to work with current library
-------- Added support for the Pololu AltIMU-10 v3 IMU
--------------------------------------------------------------------------
05-24-14
-------- Corrected test for AltIMU-10 v3 and updated Kp/Ki
-------- Created a new Serial Example for when using a Arduino Due - AltSoftSerial does not work for Due.
-------- Minor correction to Processing sketch
--------------------------------------------------------------------------

*/

#include "Arduino.h"
#include <inttypes.h>
#include <stdint.h>
//#define DEBUG
#include "FreeIMU.h"
#include "AHRS.h"

// #include "WireUtils.h"
#include "DebugUtils.h"
#include <Filter.h>             // Filter library
#include <Butter.h>

//#include "vector_math.h"

//initialize temperature calibrations, mjs, 10/24/13 - udapted 4-7-14
//comment out ifs and leave float 0's only if you don't temp cal boards
#if defined(FREEIMU_v04)
	//calibration for MPU-6050 at default scales of freeIMU
	//need to change if you use different MPU-6050 or change scales.
	//Mag cal not used - TBD
	//MPU-6050 Cal
	float c3[9] = {           0.,            0., -1.618180e-09,            0.,          0.,          0.,     0., 0.,  0.};
	float c2[9] = {4.798083e-07 ,-7.104300e-08 , -1.899410e-05, -4.387634e-08, -1.779335e-08,  4.216745e-09, 0., 0., 0. };
	float c1[9] = {1.801522e-02 ,-5.200081e-03 , -1.462879e-01, -5.878346e-04,  1.172002e-03, -6.897733e-05, 0., 0., 0. };
	float c0[9] = {      -45.61 ,	     -45.24,       -305.58,  6.699801e+00,  8.341212e+00,	-2.171155e+01, 0., 0., 0. };
#elif defined(DFROBOT)
	// DFROBOT
	float c3[9] = {          0.,             0.,           0.,          0.,            0.,           0., 0., 0., 0.};
	float c2[9] = {0.007829365 , -0.0009776705 ,   0.01271437,  -0.01214285,   0.00615132,  0.002638248, 0., 0., 0.};
	float c1[9] = {-0.330832467, -0.0945206152 ,  -0.59609025,  -2.14580824,  -4.68951187, -2.832623092, 0., 0., 0. };
	float c0[9] = {   +5.648888, +9.784001     ,     2.775708,  60.71009177, 109.32876618, 85.639595449, 0., 0., 0. };
#else
	float c3[9] = {0., 0., 0., 0., 0., 0., 0., 0., 0.};
	float c2[9] = {0., 0., 0., 0., 0., 0., 0., 0., 0.};
	float c1[9] = {0., 0., 0., 0., 0., 0., 0., 0., 0.};
	float c0[9] = {0., 0., 0., 0., 0., 0., 0., 0., 0.};
#endif

enum Mscale {
  MFS_14BITS = 0, // 0.6 mG per LSB
  MFS_16BITS = 1     // 0.15 mG per LSB
};


//used for BMP085
long Temperature = 0, Pressure = 0, Altitude = 0;

//Setup accelerometer filter
butter50hz2_0 mfilter_accx;
butter50hz2_0 mfilter_accy;
butter50hz2_0 mfilter_accz;		

#if HAS_MPU9150() || HAS_MPU9250()
	//Set up Butterworth Filter for 9150 mag - more noisy than HMC5883L
	butter50hz2_0 mfilter_mx;
	butter50hz2_0 mfilter_my;
	butter50hz2_0 mfilter_mz;
#endif

//Set-up constants for gyro calibration
uint8_t num_gyros = 1;
uint8_t INS_MAX_INSTANCES = 2;

FreeIMU::FreeIMU() {

  //pinMode(12,OUTPUT);
  
  #if HAS_ADXL345()
    acc = ADXL345();
  #elif HAS_BMA180()
    acc = BMA180();
  #endif
  
  #if HAS_HMC5883L()
    magn = HMC58X3();
	maghead = iCompass(MAG_DEC, WINDOW_SIZE, 500);
  #endif
  
  #if HAS_LSM303()
	compass = LSM303();
	maghead = iCompass(MAG_DEC, WINDOW_SIZE, 500);
  #endif
  
  #if HAS_ITG3200()
    gyro = ITG3200();
  #elif HAS_L3D20()
    gyro = L3G();
  #elif HAS_MPU6050()
    accgyro = MPU60X0(); // I2C
  #elif HAS_MPU6000()
    accgyro = MPU60X0(); // SPI for Arduimu v3
  #elif HAS_MPU9150()
    accgyro = MPU60X0();
	mag = AK8975();
	maghead = iCompass(MAG_DEC, WINDOW_SIZE, 500);
  #elif HAS_MPU9250()
    accgyro = MPU60X0();
	mag = AK8963();
	maghead = iCompass(MAG_DEC, WINDOW_SIZE, 500);  
  #endif
    
  #if HAS_MS5611()
    baro = MS561101BA();
  #elif HAS_BMP085()
    baro085 = BMP085();
  #elif HAS_LPS331()
    baro331 = LPS331();
  #endif
  
  #if HAS_MS5611() || HAS_BMP085() || HAS_LPS331()
    kPress.KalmanInit(0.0000005,0.01,1.0,0);
  #endif
  
  // initialize quaternion
  q0 = 1.0f;
  q1 = 0.0f;
  q2 = 0.0f;
  q3 = 0.0f;
  q3old = 0.0f;
  exInt = 0.0;
  eyInt = 0.0;
  ezInt = 0.0;
  twoKp = twoKpDef;
  twoKi = twoKiDef;
  integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;
  lastUpdate = 0;
  now = 0;

  nsamples = 75;
  temp_break = -1000;	  //original temp_break = -4300;
  senTemp_break = 32.;
  temp_corr_on = 1;
  nsamples = 75;
  instability_fix = 1;
  
  //initialize gyro offsets
  gyro_off_x = 0.;
  gyro_off_y = 0.;  
  gyro_off_z = 0.;
  
  #ifndef CALIBRATION_H
  // initialize scale factors to neutral values
	acc_scale_x = 1;
	acc_scale_y = 1;
	acc_scale_z = 1;
	magn_scale_x = 1;
	magn_scale_y = 1;
	magn_scale_z = 1;
  #else
	// get values from global variables of same name defined in calibration.h
	acc_off_x = ::acc_off_x;
	acc_off_y = ::acc_off_y;
	acc_off_z = ::acc_off_z;
	acc_scale_x = ::acc_scale_x;
	acc_scale_y = ::acc_scale_y;
	acc_scale_z = ::acc_scale_z;
	magn_off_x = ::magn_off_x;
	magn_off_y = ::magn_off_y;
	magn_off_z = ::magn_off_z;
	magn_scale_x = ::magn_scale_x;
	magn_scale_y = ::magn_scale_y;
	magn_scale_z = ::magn_scale_z;
  #endif
}

void FreeIMU::init() {
  #if HAS_ITG3200()
	init(FIMU_ACC_ADDR, FIMU_ITG3200_DEF_ADDR, false);
  #elif HAS_ALTIMU10()
    init0(false);
  #else
	init(FIMU_ACCGYRO_ADDR, false);
  #endif
}

void FreeIMU::init(bool fastmode) {
  #if HAS_ITG3200()
	init(FIMU_ACC_ADDR, FIMU_ITG3200_DEF_ADDR, fastmode);
  #elif HAS_ALTIMU10()
    init0(fastmode);
  #else
	init(FIMU_ACCGYRO_ADDR, fastmode);
  #endif
}

void FreeIMU::RESET() {
	#if HAS_MPU6050()
		accgyro.reset();
	#endif
	
	delay(50);
	//reset matrix
	q0 = 1.0f;
    q1 = 0.0f;
    q2 = 0.0f;
    q3 = 0.0f;
    exInt = 0.0;
    eyInt = 0.0;
    ezInt = 0.0;
    twoKp = twoKpDef;
    twoKi = twoKiDef;
    integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;
    //lastUpdate = 0;
    //now = 0;
}

void FreeIMU::RESET_Q() {
	//reset matrix
	q0 = 1.0f;
    q1 = 0.0f;
    q2 = 0.0f;
    q3 = 0.0f;
    exInt = 0.0;
    eyInt = 0.0;
    ezInt = 0.0;
    twoKp = twoKpDef;
    twoKi = twoKiDef;
    integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;
    //lastUpdate = 0;
    //now = 0;
}


/**
 * Initialize the FreeIMU I2C bus, sensors and performs gyro offsets calibration
*/
#if HAS_ITG3200()
	void FreeIMU::init(int acc_addr, int gyro_addr, bool fastmode) {
#elif HAS_ALTIMU10()
	void FreeIMU::init0(bool fastmode) {
#else
	void FreeIMU::init(int accgyro_addr, bool fastmode) {
#endif
  delay(5);

    // disable internal pullups of the ATMEGA which Wire enable by default
  #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
    // deactivate internal pull-ups for twi
    // as per note from atmega8 manual pg167
    cbi(PORTC, 4);
    cbi(PORTC, 5);
  #elif defined(__AVR_ATmega128__)
    // deactivate internal pull-ups for twi
    // as per note from atmega128 manual pg204
    cbi(PORTD, 0);
    cbi(PORTD, 1);
  #endif

  #if defined(__AVR_ATmega128) // only valid on AVR, not on 32bit platforms (eg: Arduino 2, Teensy 3.0)
    if(fastmode) { // switch to 400KHz I2C - eheheh
      TWBR = ((F_CPU / 400000L) - 16) / 2; // see twi_init in Wire/utility/twi.c
    }
  #elif defined(__arm__)
    if(fastmode) {
      #if defined(CORE_TEENSY) && F_BUS == 48000000
        I2C0_F = 0x1A;  // Teensy 3.0 at 48 or 96 MHz
        I2C0_FLT = 2;
      #elif defined(CORE_TEENSY) && F_BUS == 24000000
        I2C0_F = 0x45;  // Teensy 3.0 at 24 MHz
        I2C0_FLT = 1;
      #endif
    }
  #endif
  
  #if HAS_ADXL345()
    // init ADXL345
    acc.init(acc_addr);
	acc.set_bw(ADXL345_BW_100);
  #elif HAS_BMA180()
    // init BMA180
    acc.setAddress(acc_addr);
    acc.SoftReset();
    acc.enableWrite();
    acc.SetFilter(acc.F10HZ);
    acc.setGSensitivty(acc.G15);
    acc.SetSMPSkip();
    acc.SetISRMode();
    acc.disableWrite();
  #endif

  #if HAS_ITG3200()
	// init ITG3200
	gyro.init(gyro_addr);
	delay(1000);
	gyro.setFilterBW(BW020_SR1);
	delay(1000);
	// calibrate the ITG3200
	gyro.zeroCalibrate(128,5);
  #endif
  
  
  #if HAS_MPU6050()
	accgyro = MPU60X0(false, accgyro_addr);
	accgyro.initialize();
	accgyro.setDLPFMode(MPU60X0_DLPF_BW_20); 
	accgyro.setI2CMasterModeEnabled(0);
	accgyro.setI2CBypassEnabled(1);
	accgyro.setFullScaleGyroRange(MPU60X0_GYRO_FS_2000);
	delay(5);
	delay(30);
  #elif HAS_MPU9150()
	//initialize accelerometer and gyroscope
	accgyro = MPU60X0(false, accgyro_addr);
	#if HAS_MPU9250()
		accgyro.initialize9250();
	#else
		accgyro.initialize();
	#endif
	accgyro.setDLPFMode(MPU60X0_DLPF_BW_20); 
	accgyro.setI2CMasterModeEnabled(0);
	accgyro.setI2CBypassEnabled(1);
	accgyro.setFullScaleGyroRange(MPU60X0_GYRO_FS_2000);
	delay(100);
	//initialize magnetometer
	mag = AK8975(false, AK8975_DEFAULT_ADDRESS);
	mag.initialize();
  #elif HAS_MPU9250()
	//initialize accelerometer and gyroscope
	accgyro = MPU60X0(false, accgyro_addr);
	accgyro.initialize9250();
	accgyro.setDLPFMode(MPU60X0_DLPF_BW_20); 
	accgyro.setI2CMasterModeEnabled(0);
	accgyro.setI2CBypassEnabled(1);
	accgyro.setFullScaleGyroRange(MPU60X0_GYRO_FS_2000);
	delay(100);
	//initialize magnetometer
	mag = AK8963(false, AK8963_DEFAULT_ADDRESS);
	mag.initialize();  
	mag.setModeRes(AK8963_MODE_CONT2, MFS_16BITS);
  #elif HAS_MPU6000()
	accgyro = MPU60X0(true, accgyro_addr);
	accgyro.initialize();
	accgyro.setFullScaleGyroRange(MPU60X0_GYRO_FS_2000);
	delay(5);
  #endif 
  
  #if HAS_HMC5883L()
	// init HMC5843
	magn.init(false); // Don't set mode yet, we'll do that later on.
	// Calibrate HMC using self test, not recommended to change the gain after calibration.
	magn.calibrate(1); // Use gain 1=default, valid 0-7, 7 not recommended.
	// Single mode conversion was used in calibration, now set continuous mode
	magn.setMode(0);
	delay(10);
	magn.setDOR(B110);
  #endif
  
  
  #if HAS_MS5611()
    baro.init(FIMU_BARO_ADDR);
  #endif

  #if HAS_BMP085()
	// 19.8 meters for my location, true = using meter units
    // this initialization is useful if current altitude is known,
    // pressure will be calculated based on TruePressure and known altitude.	
	baro085.init(3, 1981.6469, true);  
  #endif
  
  //ALTIMU SENSOR INIT
  #if HAS_L3D20()
	gyro.init();
	gyro.enableDefault();
  #endif
  
  #if HAS_LSM303()
	compass.init();
    compass.enableDefault();
  #endif
  
  #if HAS_LPS331()
    baro331.init();
	baro331.enableDefault();
  #endif
  
  // zero gyro
  //zeroGyro();
  //if(temp_corr_on == 0) {
  //digitalWrite(12,HIGH);  
  initGyros(); //}
  //digitalWrite(12,LOW);
	
  #ifndef CALIBRATION_H
	// load calibration from eeprom
	calLoad();
  #endif
}

#ifndef CALIBRATION_H

static uint8_t location; // assuming ordered reads

void eeprom_read_var(uint8_t size, byte * var) {
  for(uint8_t i = 0; i<size; i++) {
    var[i] = EEPROM.read(location + i);
  }
  location += size;
}

void FreeIMU::calLoad() {
  if(EEPROM.read(FREEIMU_EEPROM_BASE) == FREEIMU_EEPROM_SIGNATURE) { // check if signature is ok so we have good data
    location = FREEIMU_EEPROM_BASE + 1; // reset location
    
    eeprom_read_var(sizeof(acc_off_x), (byte *) &acc_off_x);
    eeprom_read_var(sizeof(acc_off_y), (byte *) &acc_off_y);
    eeprom_read_var(sizeof(acc_off_z), (byte *) &acc_off_z);
    
    eeprom_read_var(sizeof(magn_off_x), (byte *) &magn_off_x);
    eeprom_read_var(sizeof(magn_off_y), (byte *) &magn_off_y);
    eeprom_read_var(sizeof(magn_off_z), (byte *) &magn_off_z);
    
    eeprom_read_var(sizeof(acc_scale_x), (byte *) &acc_scale_x);
    eeprom_read_var(sizeof(acc_scale_y), (byte *) &acc_scale_y);
    eeprom_read_var(sizeof(acc_scale_z), (byte *) &acc_scale_z);
    
    eeprom_read_var(sizeof(magn_scale_x), (byte *) &magn_scale_x);
    eeprom_read_var(sizeof(magn_scale_y), (byte *) &magn_scale_y);
    eeprom_read_var(sizeof(magn_scale_z), (byte *) &magn_scale_z);
  }
  else { // neutral values
    acc_off_x = 0;
    acc_off_y = 0;
    acc_off_z = 0;
    acc_scale_x = 1;
    acc_scale_y = 1;
    acc_scale_z = 1;

    magn_off_x = 0;
    magn_off_y = 0;
    magn_off_z = 0;
    magn_scale_x = 1;
    magn_scale_y = 1;
    magn_scale_z = 1;
  }
}
#endif


/**
 * Populates raw_values with the raw_values from the sensors
*/
void FreeIMU::getRawValues(int * raw_values) {
    //Set raw values for Magnetometer, Press, Temp to 0 in case you are only using
	//an accelerometer and gyro
	//raw_values[9] will be set to MPU-6050 temp, see zeroGyro to change raw_values dimension
    raw_values[6] = 0;
    raw_values[7] = 0;
    raw_values[8] = 0;
    raw_values[9] = 0;
	
  #if HAS_ITG3200()
    acc.readAccel(&raw_values[0], &raw_values[1], &raw_values[2]);
    gyro.readGyroRaw(&raw_values[3], &raw_values[4], &raw_values[5]);
	gyro.readTemp(&senTemp);
	raw_values[9] = senTemp*100;
  #elif HAS_MPU6050() || HAS_MPU6000() || HAS_MPU9150() || HAS_MPU9250()
    #ifdef __AVR__
     accgyro.getMotion6(&raw_values[0], &raw_values[1], &raw_values[2], &raw_values[3], &raw_values[4], &raw_values[5]);  	  
 	  #if HAS_MPU9150() || HAS_MPU9250()
		mag.getHeading(&raw_values[6], &raw_values[7], &raw_values[8]);			
		delay(10);
		#endif
	 rt = accgyro.getTemperature();	  
     raw_values[9] = rt;
	 #else
      int16_t ax, ay, az, gx, gy, gz, mx, my, mz, rt;
      accgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
	  #if HAS_MPU9150() || HAS_MPU9250() 
		mag.getHeading(&mx, &my, &mz);
		raw_values[6] = mx;
		raw_values[7] = my;
		raw_values[8] = mz;				
		delay(10);
		#endif
      raw_values[0] = ax;
      raw_values[1] = ay;
      raw_values[2] = az;
      raw_values[3] = gx;
      raw_values[4] = gy;
      raw_values[5] = gz;
      rt = accgyro.getTemperature();	  
      raw_values[9] = rt; 
    #endif
  #endif 

  #if HAS_HMC5883L()
    magn.getValues(&raw_values[6], &raw_values[7], &raw_values[8]);
  #endif  
  
 #if HAS_L3D20()
	  gyro.read();
      raw_values[3] = gyro.g.x;
      raw_values[4] = gyro.g.y;
      raw_values[5] = gyro.g.z;	
  #endif

  #if HAS_LSM303()
	compass.read();
    raw_values[0] = compass.a.x;
    raw_values[1] = compass.a.y;
    raw_values[2] = compass.a.z;
    raw_values[6] = compass.m.x;
    raw_values[7] = compass.m.y;
    raw_values[8] = compass.m.z;
  #endif	
  
}


/**
 * Populates values with calibrated readings from the sensors
*/
void FreeIMU::getValues(float * values) { 

  float acgyro_corr[9] = {0.,0.,0.,0.,0.,0.,0.,0.,0.};
  uint8_t i;

  #if HAS_ITG3200()
    int accval[3];
    acc.readAccel(&accval[0], &accval[1], &accval[2]);
	accval[0] = mfilter_accx.filter((float) accval[0]);
    accval[1] = mfilter_accy.filter((float) accval[1]);
    accval[2] = mfilter_accz.filter((float) accval[2]);
	
    gyro.readGyro(&values[3]);	
	gyro.readTemp(&senTemp);
	if(temp_corr_on == 1) {
		if(senTemp < senTemp_break) {
			for(i = 0; i < 9; i++) { 
				acgyro_corr[i] = c3[i]*(senTemp*senTemp*senTemp) + c2[i]*(senTemp*senTemp) + c1[i]*senTemp + c0[i];
			}		
		} 
	} else {
		for(i = 0; i < 9; i++) { 
			acgyro_corr[i] = 0.0f;
		}
	}

	values[0] = (float) accval[0] - acgyro_corr[0];
	values[1] = (float) accval[1] - acgyro_corr[1];
	values[2] = (float) accval[2] - acgyro_corr[2];
		
	values[3] = values[3] - gyro_off_x;
	values[4] = values[4] - gyro_off_y;
	values[5] = values[5] - gyro_off_z;

  #elif HAS_ALTIMU10()
	gyro.read();
	compass.read();
	values[0] = (float) compass.a.x;
	values[1] = (float) compass.a.y;
	values[2] = (float) compass.a.z;
	//values[0] = mfilter_accx.filter((float) compass.a.x);
	//values[1] = mfilter_accy.filter((float) compass.a.y);
	//values[2] = mfilter_accz.filter((float) compass.a.z);	
	values[3] = (float) gyro.g.x;
	values[4] = (float) gyro.g.y;
	values[5] = (float) gyro.g.z;
    values[6] = (float) compass.m.x;
    values[7] = (float) compass.m.y; 
    values[8] = (float) compass.m.z;
	
	values[3] = (values[3] - gyro_off_x) / 70.0f;  //Sensitivity set at 70 for +/-2000 deg/sec
	values[4] = (values[4] - gyro_off_y) / 70.0f;
	values[5] = (values[5] - gyro_off_z) / 70.0f;
	
  #else  // MPU6050
    int16_t accgyroval[9];
	#if HAS_MPU9150() || HAS_MPU9250()
		accgyro.getMotion6(&accgyroval[0], &accgyroval[1], &accgyroval[2], 
						   &accgyroval[3], &accgyroval[4], &accgyroval[5]);	   
		// read raw heading measurements from device
		mag.getHeading(&accgyroval[6], &accgyroval[7], &accgyroval[8]);	
		delay(10);
		
		accgyroval[0] = mfilter_accx.filter((float) accgyroval[0]);
		accgyroval[1] = mfilter_accy.filter((float) accgyroval[1]);
		accgyroval[2] = mfilter_accz.filter((float) accgyroval[2]);
		
		values[6] = mfilter_mx.filter((float) accgyroval[6]);
		values[7] = mfilter_my.filter((float) accgyroval[7]);
		values[8] = mfilter_mz.filter((float) accgyroval[8]); 

	#else
		accgyro.getMotion6(&accgyroval[0], &accgyroval[1], &accgyroval[2], 
						   &accgyroval[3], &accgyroval[4], &accgyroval[5]);
	#endif
	
	DTemp = accgyro.getTemperature();

	if(temp_corr_on == 1){
		if(DTemp < temp_break){    
			for( i = 0; i < 9; i++) { 
				acgyro_corr[i] = c3[i]*(DTemp*DTemp*DTemp) + c2[i]*(DTemp*DTemp) + c1[i]*DTemp + c0[i];
			}
		} 
	} else {
		for( i = 0; i < 9; i++) { 
			acgyro_corr[i] = 0.0f;
	  }
	}
	
    // remove offsets from the gyroscope
	if(temp_corr_on == 1){
		//accgyroval[3] = accgyroval[3] - acgyro_corr[3];
		//accgyroval[4] = accgyroval[4] - acgyro_corr[4];
		//accgyroval[5] = accgyroval[5] - acgyro_corr[5];
		
		values[3] = (float) accgyroval[3] - acgyro_corr[3];
		values[4] = (float) accgyroval[4] - acgyro_corr[4];
		values[5] = (float) accgyroval[5] - acgyro_corr[5];
		}
	  else {
		values[3] = (float) accgyroval[3] - gyro_off_x;
		values[4] = (float) accgyroval[4] - gyro_off_y;
		values[5] = (float) accgyroval[5] - gyro_off_z;
	  }
    for( i = 0; i<6; i++) {
      if( i < 3 ) {
        values[i] = (float) accgyroval[i] - acgyro_corr[i];
      }
      else {
        //values[i] = ((float) accgyroval[i] - acgyro_corr[i])/ 16.4f; // NOTE: this depends on the sensitivity chosen
		values[i] = values[i] / 16.4f; // NOTE: this depends on the sensitivity chosen, this is for 6050
	  }
    }	
  #endif

  
  #warning Accelerometer calibration active: have you calibrated your device?
  // remove offsets and scale accelerometer (calibration)
  values[0] = (values[0] - acc_off_x) / acc_scale_x;
  values[1] = (values[1] - acc_off_y) / acc_scale_y;
  values[2] = (values[2] - acc_off_z) / acc_scale_z;
  
  #if HAS_HMC5883L()
    magn.getValues(&values[6]);
  #endif
  
  #if HAS_HMC5883L() || HAS_MPU9150() || HAS_MPU9250() || HAS_LSM303()
    // calibration
	if(temp_corr_on == 1) {
		values[6] = (values[6] - acgyro_corr[6] - magn_off_x) / magn_scale_x;
		values[7] = (values[7] - acgyro_corr[7] - magn_off_y) / magn_scale_y;
		values[8] = (values[8] - acgyro_corr[8] - magn_off_z) / magn_scale_z;
	}	
	else {
		#warning Magnetometer calibration active: have you calibrated your device?
		values[6] = (values[6] - magn_off_x) / magn_scale_x;
		values[7] = (values[7] - magn_off_y) / magn_scale_y;
		values[8] = (values[8] - magn_off_z) / magn_scale_z;	
	}
  #endif
}


/**
 * Computes gyro offsets
*/
void FreeIMU::zeroGyro() {
  const int totSamples = nsamples;
  int raw[11];
  float values[10];
  float tmpOffsets[] = {0,0,0};
  
  for (int i = 0; i < totSamples; i++){
	#if HAS_ITG3200()
		gyro.readGyro(&values[3]);
		tmpOffsets[0] += values[3];
		tmpOffsets[1] += values[4];
		tmpOffsets[2] += values[5];		
	#else
		getRawValues(raw);
		tmpOffsets[0] += raw[3];
		tmpOffsets[1] += raw[4];
		tmpOffsets[2] += raw[5]; 
	#endif
  }
  
  gyro_off_x = tmpOffsets[0] / totSamples;
  gyro_off_y = tmpOffsets[1] / totSamples;
  gyro_off_z = tmpOffsets[2] / totSamples;

  delay(5);
}

void FreeIMU::initGyros() {
	//Code modified from Ardupilot library
	//
    Vector3f last_average[INS_MAX_INSTANCES], best_avg[INS_MAX_INSTANCES], gyro_offset[INS_MAX_INSTANCES];
    float best_diff[INS_MAX_INSTANCES];
    bool converged[INS_MAX_INSTANCES];
	
	////digitalWrite(12,HIGH);
	
    // remove existing gyro offsets
    for (uint8_t k=0; k<num_gyros; k++) {
        gyro_offset[k] = Vector3f(0,0,0);
        best_diff[k] = 0;
        last_average[k].zero();
        converged[k] = false;
    }
    
	// the strategy is to average 50 points over 0.5 seconds, then do it
    // again and see if the 2nd average is within a small margin of
    // the first

    uint8_t num_converged = 0;	
	
    // we try to get a good calibration estimate for up to 10 seconds
    // if the gyros are stable, we should get it in 1 second
	for (int16_t j = 0; j <= 10 && num_converged < num_gyros; j++) {
		Vector3f gyro_sum[INS_MAX_INSTANCES], gyro_avg[INS_MAX_INSTANCES], gyro_diff[INS_MAX_INSTANCES];
		float diff_norm[INS_MAX_INSTANCES];
		
		//For FreeIMU and most boards we are using only one gyro
		//if you have more change code to match Arduimu
		zeroGyro();
		gyro_avg[0] = Vector3f(gyro_off_x, gyro_off_y,gyro_off_z) ;
		
		for (uint8_t k=0; k<num_gyros; k++) {
            gyro_diff[k] = last_average[k] - gyro_avg[k];
            diff_norm[k] = gyro_diff[k].length();
        }
		
		for (uint8_t k=0; k<num_gyros; k++) {
            if (converged[k]) continue;
            if (j == 0) {
                best_diff[k] = diff_norm[k];
                best_avg[k] = gyro_avg[k];
            } else if (gyro_diff[k].length() < ToRad(0.1f)) {
                // we want the average to be within 0.1 bit, which is 0.04 degrees/s
                last_average[k] = (gyro_avg[k] * 0.5f) + (last_average[k] * 0.5f);
                gyro_offset[k] = last_average[k];            
                converged[k] = true;
                num_converged++;
            } else if (diff_norm[k] < best_diff[k]) {
                best_diff[k] = diff_norm[k];
                best_avg[k] = (gyro_avg[k] * 0.5f) + (last_average[k] * 0.5f);
            }
            last_average[k] = gyro_avg[k];
        }
    }
	
	delay(5);
	
	if (num_converged == num_gyros) {
        // all OK
		gyro_off_x = gyro_offset[0].x;
		gyro_off_y = gyro_offset[0].y;
		gyro_off_z = gyro_offset[0].z;
		////digitalWrite(12,LOW);
		return;
	}

    // we've kept the user waiting long enough - use the best pair we
    // found so far
    for (uint8_t k=0; k<num_gyros; k++) {
        if (!converged[k]) {
            gyro_offset[k] = best_avg[k];
        }
    }
	
	gyro_off_x = gyro_offset[0].x;
	gyro_off_y = gyro_offset[0].y;
	gyro_off_z = gyro_offset[0].z;
	
	//digitalWrite(12,LOW);

}


/**
 * Populates array q with a quaternion representing the IMU orientation with respect to the Earth
 * 
 * @param q the quaternion to populate
*/
void FreeIMU::getQ(float * q, float * val) {
  //float val[10];
  getValues(val);
  
  //DEBUG_PRINT(val[3] * M_PI/180);
  //DEBUG_PRINT(val[4] * M_PI/180);
  //DEBUG_PRINT(val[5] * M_PI/180);
  //DEBUG_PRINT(val[0]);
  //DEBUG_PRINT(val[1]);
  //DEBUG_PRINT(val[2]);
  //DEBUG_PRINT(val[6]);
  //DEBUG_PRINT(val[7]);
  //DEBUG_PRINT(val[8]);
  
  now = micros();
  sampleFreq = 1.0 / ((now - lastUpdate) / 1000000.0);
  lastUpdate = now;
  
  // gyro values are expressed in deg/sec, the * M_PI/180 will convert it to radians/sec
  #if IS_9DOM() && not defined(DISABLE_MAGN)
    #if HAS_AXIS_ALIGNED()
      AHRSupdate(val[3] * M_PI/180, val[4] * M_PI/180, val[5] * M_PI/180, val[0], val[1], val[2], val[6], val[7], val[8]);
	  val[9] = maghead.iheading(0, 1, 0, val[0], val[1], val[2], val[7], val[6], val[8]);
	#elif defined(SEN_10724)
      AHRSupdate(val[3] * M_PI/180, val[4] * M_PI/180, val[5] * M_PI/180, val[0], val[1], val[2], val[7], -val[6], val[8]);
      val[9] = calcMagHeading( q0,  q1,  q2,  q3, val[7], -val[6], val[8]);
	#elif defined(ARDUIMU_v3)
      AHRSupdate(val[3] * M_PI/180, val[4] * M_PI/180, val[5] * M_PI/180, val[0], val[1], val[2], -val[6], -val[7], val[8]);
      val[9] = calcMagHeading( q0,  q1,  q2,  q3, -val[6], -val[7], val[8]);    
	#elif defined(GEN_MPU9150) || defined(MPU9250_5611) || defined(GEN_MPU9250)
      AHRSupdate(val[3] * M_PI/180, val[4] * M_PI/180, val[5] * M_PI/180, val[0], val[1], val[2], val[7], val[6], val[8]);
      //val[9] = calcMagHeading( q0,  q1,  q2, q3, val[7], val[6], val[8]); 
	  val[9] = maghead.iheading(1, 0, 0, val[0], val[1], val[2], val[7], val[6], val[8]);
	#elif defined(Altimu10)
      AHRSupdate(val[3] * M_PI/180, val[4] * M_PI/180, val[5] * M_PI/180, val[0], val[1], -val[2], val[6], val[7], -val[8]);
	  //val[9] = maghead.iheading(0, 1, 0, val[0], val[1], -val[2], val[7], val[6], -val[8]);
	  val[9] = compass.heading();
	#endif
  #else
    AHRSupdateIMU(val[3] * M_PI/180, val[4] * M_PI/180, val[5] * M_PI/180, val[0], val[1], val[2]);
	val[9] = -9999.0f;
  #endif
  
  q[0] = q0;
  q[1] = q1;
  q[2] = q2;
  q[3] = q3;
  
}

const float def_sea_press = 1013.25;

#if HAS_MS5611()
	/**
	* Returns an altitude estimate from barometer readings only using sea_press as current sea level pressure
	*/
	float FreeIMU::getBaroAlt(float sea_press) {
		float temp = baro.getTemperature(MS561101BA_OSR_4096);
		float press = baro.getPressure(MS561101BA_OSR_4096);
        float new_press = kPress.measureRSSI(press);
		return ((pow((sea_press / new_press), 1/5.257) - 1.0) * (temp + 273.15)) / 0.0065;
	}

	// Returns temperature from MS5611 - added by MJS
	float FreeIMU::getBaroTemperature() {
		float temp1 = baro.getTemperature(MS561101BA_OSR_4096);
		return(temp1);
	}

	float FreeIMU::getBaroPressure() {
		float temp2 = baro.getPressure(MS561101BA_OSR_4096);
		return(temp2);
	}

	/**
	* Returns an altitude estimate from baromether readings only using a default sea level pressure
	*/
	float FreeIMU::getBaroAlt() {
		return getBaroAlt(def_sea_press);
	}

#endif

#if HAS_BMP085()

	// Returns temperature from BMP085 - added by MJS
	float FreeIMU::getBaroTemperature() {
		baro085.getTemperature(&Temperature);
		float temp1 = Temperature * 0.1;		
		return(temp1);
	}

	float FreeIMU::getBaroPressure() {
		baro085.getPressure(&Pressure);
        float new_press = kPress.measureRSSI(Pressure * 0.01);
		return(new_press);
	}

	/**
	* Returns an altitude estimate from baromether readings only using a default sea level pressure
	*/
	float FreeIMU::getBaroAlt() {
		baro085.getAltitude(&Altitude);
		return Altitude * 0.01;
	}
	
#endif

#if HAS_LPS331()

	// Returns temperature from BMP085 - added by MJS
	float FreeIMU::getBaroTemperature() {
		float temp1 = baro331.readTemperatureC();		
		return(temp1);
	}

	float FreeIMU::getBaroPressure() {
        float new_press = kPress.measureRSSI(baro331.readPressureMillibars());
		return(new_press);
	}

	/**
	* Returns an altitude estimate from baromether readings only using a default sea level pressure
	*/
	float FreeIMU::getBaroAlt() {        
        float new_press = kPress.measureRSSI(baro331.readPressureMillibars());
		float temp3 = baro331.pressureToAltitudeMeters(new_press);
		return(temp3);
	}
	
#endif

/**
 * Returns the estimated altitude from fusing barometer and accelerometer
 * in a complementary filter.
*/
float FreeIMU::getEstAltitude() {
  float q1[4]; // quaternion
  float q2[4]; // quaternion
  float val[10];
  float dyn_acc[4];
  float dyn_acc_temp[4];
  float dyn_acc_earth[4];

  getQ(q1, val);

  dyn_acc[0] = 0;
  dyn_acc[1] = val[0];
  dyn_acc[2] = val[1];
  dyn_acc[3] = val[2];
  
  gravityCompensateAcc(dyn_acc, q1);
  dyn_acc[0] = 0;
  
  Qmultiply(dyn_acc_temp, q1, dyn_acc);

  q2[1] = -q1[1]; q2[2] = -q1[2]; q2[3] = -q1[3]; q2[0] = q1[0]; //Conjugating
  Qmultiply(dyn_acc_earth, dyn_acc_temp, q2);
  
  float alt = getBaroAlt();
  return altComp.update(dyn_acc_earth[3], alt, (1./(sampleFreq*4)));
}

/**
 * Returns the Euler angles in radians defined in the Aerospace sequence.
 * See Sebastian O.H. Madwick report "An efficient orientation filter for 
 * inertial and intertial/magnetic sensor arrays" Chapter 2 Quaternion representation
 * 
 * @param angles three floats array which will be populated by the Euler angles in radians
*/
void FreeIMU::getEulerRad(float * angles) {
  float q[4]; // quaternion
  float val[10];
  
  getQ(q, val);
  angles[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1); // psi
  angles[1] = -asin(2 * q[1] * q[3] + 2 * q[0] * q[2]); // theta
  angles[2] = atan2(2 * q[2] * q[3] - 2 * q[0] * q[1], 2 * q[0] * q[0] + 2 * q[3] * q[3] - 1); // phi
}


/**
 * Returns the Euler angles in degrees defined with the Aerospace sequence.
 * See Sebastian O.H. Madwick report "An efficient orientation filter for 
 * inertial and intertial/magnetic sensor arrays" Chapter 2 Quaternion representation
 * 
 * @param angles three floats array which will be populated by the Euler angles in degrees
*/
void FreeIMU::getEuler(float * angles) {
  getEulerRad(angles);
  arr3_rad_to_deg(angles);
}


/**
 * Returns the yaw pitch and roll angles, respectively defined as the angles in radians between
 * the Earth North and the IMU X axis (yaw), the Earth ground plane and the IMU X axis (pitch)
 * and the Earth ground plane and the IMU Y axis.
 * 
 * @note This is not an Euler representation: the rotations aren't consecutive rotations but only
 * angles from Earth and the IMU. For Euler representation Yaw, Pitch and Roll see FreeIMU::getEuler
 * 
 * @param ypr three floats array which will be populated by Yaw, Pitch and Roll angles in radians
*/
void FreeIMU::getYawPitchRollRad(float * ypr) {
  float q[4]; // quaternion
  float val[10];
  float gx, gy, gz; // estimated gravity direction
  getQ(q, val);
  
  gx = 2 * (q[1]*q[3] - q[0]*q[2]);
  gy = 2 * (q[0]*q[1] + q[2]*q[3]);
  gz = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];
  
  ypr[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1);
  ypr[1] = atan(gx / sqrt(gy*gy + gz*gz));
  ypr[2] = atan(gy / sqrt(gx*gx + gz*gz));
}

/**
 * Returns the yaw pitch and roll angles, respectively defined as the angles in degrees between
 * the Earth North and the IMU X axis (yaw), the Earth ground plane and the IMU X axis (pitch)
 * and the Earth ground plane and the IMU Y axis.
 * 
 * @note This is not an Euler representation: the rotations aren't consecutive rotations but only
 * angles from Earth and the IMU. For Euler representation Yaw, Pitch and Roll see FreeIMU::getEuler
 * 
 * @param ypr three floats array which will be populated by Yaw, Pitch and Roll angles in degrees
*/
void FreeIMU::getYawPitchRoll(float * ypr) {
  getYawPitchRollRad(ypr);
  arr3_rad_to_deg(ypr);
}

/**
 * Get heading from magnetometer if LSM303 not available
 * code extracted from rob42/FreeIMU-20121106_1323 Github library
 * (https://github.com/rob42/FreeIMU-20121106_1323.git)
 * which is based on 
 *
**/
float FreeIMU::calcMagHeading(float q0, float q1, float q2, float q3, float bx, float by, float bz){
  float Head_X, Head_Y;
  float cos_roll, sin_roll, cos_pitch, sin_pitch;
  float gx, gy, gz; // estimated gravity direction
  float ypr[3];
  
  gx = 2 * (q1*q3 - q0*q2);
  gy = 2 * (q0*q1 + q2*q3);
  gz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
  
  ypr[0] = atan2(2 * q1 * q2 - 2 * q0 * q3, 2 * q0*q0 + 2 * q1 * q1 - 1);
  ypr[1] = atan(gx / sqrt(gy*gy + gz*gz));
  ypr[2] = atan(gy / sqrt(gx*gx + gz*gz)); 

  cos_roll = cos(-ypr[2]);
  sin_roll = sin(-ypr[2]);
  cos_pitch = cos(ypr[1]);
  sin_pitch = sin(ypr[1]);
  
  //Example calc
  //Xh = bx * cos(theta) + by * sin(phi) * sin(theta) + bz * cos(phi) * sin(theta)
  //Yh = by * cos(phi) - bz * sin(phi)
  //return wrap((atan2(-Yh, Xh) + variation))
    
  // Tilt compensated Magnetic field X component:
  Head_X = bx*cos_pitch + by*sin_roll*sin_pitch + bz*cos_roll*sin_pitch;
  // Tilt compensated Magnetic field Y component:
  Head_Y = by*cos_roll - bz*sin_roll;
  // Magnetic Heading
  return (atan2(-Head_Y,Head_X)*180./M_PI);
 
}


/**
 * Sets thermal calibration on (1) or off (0) for the accelerometer and gyro calibration from the
 * main sketch
*/
void FreeIMU::setTempCalib(int opt_temp_cal) {
   temp_corr_on = opt_temp_cal;
   if(temp_corr_on == 1) {
		gyro_off_x = 0.0;
		gyro_off_y = 0.0;
		gyro_off_z = 0.0;
	}
	if(temp_corr_on == 0) {
		//digitalWrite(12,HIGH);
		initGyros();
		//digitalWrite(12,LOW);
	}
}

/**                           END OF FREEIMU                           **/
/************************************************************************/
/**                           HELP FUNCTIONS                           **/

/**
 * Compensates the accelerometer readings in the 3D vector acc expressed in the sensor frame for gravity
 * @param acc the accelerometer readings to compensate for gravity
 * @param q the quaternion orientation of the sensor board with respect to the world
*/
void gravityCompensateAcc(float * acc, float * q) {
  float g[3];
  
  // get expected direction of gravity in the sensor frame
  g[0] = 2 * (q[1] * q[3] - q[0] * q[2]);
  g[1] = 2 * (q[0] * q[1] + q[2] * q[3]);
  g[2] = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
  
  // compensate accelerometer readings with the expected direction of gravity
  acc[0] = acc[0] - g[0];
  acc[1] = acc[1] - g[1];
  acc[2] = acc[2] - g[2];
}

/**
 * Sets the Quaternion to be equal to the product of quaternions {@code q1} and {@code q2}.
 * 
 * @param q1
 *          the first Quaternion
 * @param q2
 *          the second Quaternion
 */
void Qmultiply(float *  q, float *  q1, float * q2) {
    q[0] = q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3];
    q[1] = q1[0] * q2[1] + q2[0] * q1[1] + q1[2] * q2[3] - q1[3] * q2[2];
    q[2] = q1[0] * q2[2] + q2[0] * q1[2] - q1[1] * q2[3] + q1[3] * q2[1];
    q[3] = q1[0] * q2[3] + q2[0] * q1[3] + q1[1] * q2[2] - q1[2] * q2[1];
}

/**
 * Converts a 3 elements array arr of angles expressed in radians into degrees
*/
void arr3_rad_to_deg(float * arr) {
  arr[0] *= 180/M_PI;
  arr[1] *= 180/M_PI;
  arr[2] *= 180/M_PI;
}

/* Madgwick IMU/AHRS and Fast Inverse Square Root
http://www.diydrones.com/forum/topics/madgwick-imu-ahrs-and-fast-inverse-square-root

"After experimenting with real sensors I moved to artificial ACC input data and set up a 
test bed for Madgwick's algorithm (MadgwickTests on GitHub). I've figured out that in 
Madgwick's algorithm the fast inverse square root leads to huge instabilities when noisy 
measurements are applied.

Posted by Tobias Simon on November 2, 2012 
*/

//int instability_fix = 1;

float FreeIMU::invSqrt(float x) {
        if (instability_fix == 0)
        {
             union {
               float f;
               int32_t i;
               } y;

             y.f = x;
             y.i = 0x5f375a86 - (y.i >> 1);
             y.f = y.f * ( 1.5f - ( x * 0.5f * y.f * y.f ) );
             return y.f;
        }
        else if (instability_fix == 1)
        {
                /* close-to-optimal  method with low cost from
				http://pizer.wordpress.com/2008/10/12/fast-inverse-square-root */
                uint32_t i = 0x5F1F1412 - (*(uint32_t*)&x >> 1);
                float tmp = *(float*)&i;
                return tmp * (1.69000231f - 0.714158168f * x * tmp * tmp);
        }
        else
        {
                /* optimal but expensive method: */
                return 1.0f / sqrt(x);
        }
}


/**
 * Fast inverse square root implementation. Compatible both for 32 and 8 bit microcontrollers.
 * @see http://en.wikipedia.org/wiki/Fast_inverse_square_root
*/
/* original code from FreeIMU Library
float invSqrt(float number) {
  union {
    float f;
    int32_t i;
  } y;

  y.f = number;
  y.i = 0x5f375a86 - (y.i >> 1);
  y.f = y.f * ( 1.5f - ( number * 0.5f * y.f * y.f ) );
  return y.f;
}
*/
/* Old 8bit version. Kept it here only for testing/debugging of the new code above.
float invSqrt(float number) {
  volatile long i;
  volatile float x, y;
  volatile const float f = 1.5F;

  x = number * 0.5F;
  y = number;
  i = * ( long * ) &y;
  i = 0x5f375a86 - ( i >> 1 );
  y = * ( float * ) &i;
  y = y * ( f - ( x * y * y ) );
  return y;
}
*/