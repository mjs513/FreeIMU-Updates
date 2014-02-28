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
Below changes were made by Michael J Smorto
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
*/

#include <inttypes.h>
#include <stdint.h>
//#define DEBUG
#include "FreeIMU.h"
// #include "WireUtils.h"
#include "DebugUtils.h"

//#include "vector_math.h"

//initialize temperature calibrations, mjs, 10/24/13
//calibration for MPU-6050 at default scales of freeIMU
//need to change if you use different MPU-6050 or change scales.

//Temperature correction coefficients for temp limited gyro and accel
//Ax, Ay, Az, Gx, Gy, Gz, Mx, My, Mz
//First calibration about 4 degrees to 75 degrees C. break at -4300
//float c3[9] = {            0., 3.468376e-10,             0.,            0.,          0.,          0.,           0.,           0.,           0.};
//float c2[9] = {-1.043559e-06 , 9.286350e-06 , 3.079771e-06 ,-5.3689969e-08,-4.13619e-08,1.438296e-08,-4.952247e-09,-9.256070e-08,-6.978099e-08};
//float c1[9] = {-3.251887e-03 , 7.210134e-02 , -4.360955e-02, -3.457362e-04,1.298893e-03,1.618065e-04, 2.058041e-03,-6.310647e-04, 2.753109e-03};
//float c0[9] = {        4.6913,     -515.5508,       -222.72,     -0.779093,     6.23222,   0.06697  ,      8.33635,     -205.564,    12.241466};

//Ax, Ay, Az, Gx, Gy, Gz, Mx, My, Mz
//Reduced range of calibraion (-8500 to -3700 temp counts), break at -4000 still had some drift so
//changed break to -2000
//Mag cal not applied seems to make it worse
//float c3[9] = {            0.,            0.,             0.,            0.,          0.,          0.,           0.,           0.,           0.};
//float c2[9] = {-1.737912e-06 , 3.964816e-06 , 3.396598e-06 ,-8.600708e-08 , -5.563686e-08, 2.151368e-08, 3.720450e-08 , -1.383074e-07, -5.441653e-08 };
//float c1[9] = {-1.069253e-02 , 4.571826e-02 , -4.080359e-02, -7.143506e-04,  1.135774e-03, 2.350448e-04, 2.508297e-03, -1.136099e-03, 2.917745e-03};
//float c0[9] = {-15.51573     ,	     88.2226,	    -210.92,	 -1.496778,	   5.250218,	     0.5857, 9.0499511, -2.830876295, 11.8373645};

float c3[9] = {            0.,           0., -1.618180e-09,            0.,          0.,          0.,     0., 0.,  0.};
float c2[9] = {4.798083e-07 ,-7.104300e-08 , -1.899410e-05, -4.387634e-08, -1.779335e-08,  4.216745e-09, 0., 0., 0. };
float c1[9] = {1.801522e-02 ,-5.200081e-03 , -1.462879e-01, -5.878346e-04,  1.172002e-03, -6.897733e-05, 0., 0., 0. };
float c0[9] = {      -45.61 ,	     -45.18,       -305.58,  6.699801e+00,  8.341212e+00,	-2.171155e+01, 0., 0., 0. };


//float r_corr[9] = {0., 0., 0.,0.,0.,0.,0.,0.,0.};

FreeIMU::FreeIMU() {
  #if HAS_ADXL345()
    acc = ADXL345();
  #elif HAS_BMA180()
    acc = BMA180();
  #endif
  
  #if HAS_HMC5883L()
    magn = HMC58X3();
  #endif
  
  #if HAS_ITG3200()
    gyro = ITG3200();
  #elif HAS_MPU6050()
    accgyro = MPU60X0(); // I2C
  #elif HAS_MPU6000()
    accgyro = MPU60X0(); // SPI for Arduimu v3
  #endif
    
  #if HAS_MS5611()
    baro = MS561101BA();
  #endif
  
  // initialize quaternion
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
  lastUpdate = 0;
  now = 0;

  nsamples = 75;
  //original temp_break = -4300;
  temp_break = -1000;
  temp_corr_on = 0;
  nsamples = 75;
  instability_fix = 1;
  
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
  #else
  init(FIMU_ACCGYRO_ADDR, false);
  #endif
}

void FreeIMU::init(bool fastmode) {
  #if HAS_ITG3200()
  init(FIMU_ACC_ADDR, FIMU_ITG3200_DEF_ADDR, fastmode);
  #else
  init(FIMU_ACCGYRO_ADDR, fastmode);
  #endif
}

void FreeIMU::RESET() {
	accgyro.reset();
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
  #endif
  
  #if HAS_MPU6000()
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
    
  // zero gyro
   zeroGyro();
  
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
	//an accelerometer
	//raw_values[9] will be set to MPU-6050 temp, see zeroGyro to change raw_values dimension
    raw_values[6] = 0;
    raw_values[7] = 0;
    raw_values[8] = 0;
    raw_values[9] = 0;
	
  #if HAS_ITG3200()
    acc.readAccel(&raw_values[0], &raw_values[1], &raw_values[2]);
    gyro.readGyroRaw(&raw_values[3], &raw_values[4], &raw_values[5]);
  #else
    #ifdef __AVR_ATmega128__
     accgyro.getMotion6(&raw_values[0], &raw_values[1], &raw_values[2], &raw_values[3], &raw_values[4], &raw_values[5]);  	  
     rt = accgyro.getTemperature();	  
     raw_values[9] = rt;	 
	 #else
      int16_t ax, ay, az, gx, gy, gz, rt;
      accgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
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
  
  #if HAS_MS5611()
    int temp, press;
    
    //TODO: possible loss of precision
    temp = baro.rawTemperature(MS561101BA_OSR_4096);
    press = baro.rawPressure(MS561101BA_OSR_4096);
  # endif
}

/**
 * Populates values with calibrated readings from the sensors
*/
void FreeIMU::getValues(float * values) { 

  float acgyro_corr[9] = {0.,0.,0.,0.,0.,0.,0.,0.,0.};
  int16_t DTemp;
  
  if(temp_corr_on == 1) {
	DTemp = accgyro.getTemperature();
	if(DTemp < temp_break){    
		for(int i = 0; i < 9; i++) { 
			acgyro_corr[i] = c3[i]*(DTemp*DTemp*DTemp) + c2[i]*(DTemp*DTemp) + c1[i]*DTemp + c0[i];
		}
	}
	else {
		float acgyro_corr[9] = {0.,0.,0.,0.,0.,0.,0.,0.,0.};
	}
  }
  
  #if HAS_ITG3200()
    int accval[3];
    acc.readAccel(&accval[0], &accval[1], &accval[2]);
    values[0] = (float) accval[0] - acgyro_corr[0];
    values[1] = (float) accval[1] - acgyro_corr[1];
    values[2] = (float) accval[2] - acgyro_corr[2];
    gyro.readGyro(&values[3]);
    values[3] = values[3] - acgyro_corr[3];
    values[4] = values[4] - acgyro_corr[4];
    values[5] = values[5] - acgyro_corr[5];	
  #else // MPU6050
    int16_t accgyroval[6];
	
    accgyro.getMotion6(&accgyroval[0], &accgyroval[1], &accgyroval[2], &accgyroval[3], &accgyroval[4], &accgyroval[5]);

    // remove offsets from the gyroscope
	if(temp_corr_on == 1){
		accgyroval[3] = accgyroval[3] - acgyro_corr[3];
		accgyroval[4] = accgyroval[4] - acgyro_corr[4];
		accgyroval[5] = accgyroval[5] - acgyro_corr[5];
		}
	  else {
		accgyroval[3] = accgyroval[3] - gyro_off_x;
		accgyroval[4] = accgyroval[4] - gyro_off_y;
		accgyroval[5] = accgyroval[5] - gyro_off_z;
	  }
	
    for(int i = 0; i<6; i++) {
      if(i < 3) {
        values[i] = (float) accgyroval[i] - acgyro_corr[i];
      }
      else {
        //values[i] = ((float) accgyroval[i] - acgyro_corr[i])/ 16.4f; // NOTE: this depends on the sensitivity chosen
        values[i] = ((float) accgyroval[i] )/ 16.4f; // NOTE: this depends on the sensitivity chosen
     
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
    // calibration
	if(DTemp < temp_break) {
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
  int raw[10];
  float tmpOffsets[] = {0,0,0};
  
  for (int i = 0; i < totSamples; i++){
    getRawValues(raw);
    tmpOffsets[0] += raw[3];
    tmpOffsets[1] += raw[4];
    tmpOffsets[2] += raw[5];
  }
  
  gyro_off_x = tmpOffsets[0] / totSamples;
  gyro_off_y = tmpOffsets[1] / totSamples;
  gyro_off_z = tmpOffsets[2] / totSamples;
}


/**
 * Quaternion implementation of the 'DCM filter' [Mayhony et al].  Incorporates the magnetic distortion
 * compensation algorithms from Sebastian Madgwick's filter which eliminates the need for a reference
 * direction of flux (bx bz) to be predefined and limits the effect of magnetic distortions to yaw
 * axis only.
 * 
 * @see: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
*/
#if IS_9DOM()
void  FreeIMU::AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {
#elif IS_6DOM()
void  FreeIMU::AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az) {
#endif
  float recipNorm;
  float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
  float halfex = 0.0f, halfey = 0.0f, halfez = 0.0f;
  float qa, qb, qc;

  // Auxiliary variables to avoid repeated arithmetic
  q0q0 = q0 * q0;
  q0q1 = q0 * q1;
  q0q2 = q0 * q2;
  q0q3 = q0 * q3;
  q1q1 = q1 * q1;
  q1q2 = q1 * q2;
  q1q3 = q1 * q3;
  q2q2 = q2 * q2;
  q2q3 = q2 * q3;
  q3q3 = q3 * q3;
  
  #if IS_9DOM() && not defined(DISABLE_MAGN)
  // Use magnetometer measurement only when valid (avoids NaN in magnetometer normalisation)
  if((mx != 0.0f) && (my != 0.0f) && (mz != 0.0f)) {
    float hx, hy, bx, bz;
    float halfwx, halfwy, halfwz;
    
    // Normalise magnetometer measurement
    recipNorm = invSqrt(mx * mx + my * my + mz * mz);
    mx *= recipNorm;
    my *= recipNorm;
    mz *= recipNorm;
    
    // Reference direction of Earth's magnetic field
    hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
    hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
    bx = sqrt(hx * hx + hy * hy);
    bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));
    
    // Estimated direction of magnetic field
    halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
    halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
    halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);
    
    // Error is sum of cross product between estimated direction and measured direction of field vectors
    halfex = (my * halfwz - mz * halfwy);
    halfey = (mz * halfwx - mx * halfwz);
    halfez = (mx * halfwy - my * halfwx);
  }
  #endif

  // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
  if((ax != 0.0f) && (ay != 0.0f) && (az != 0.0f)) {
    float halfvx, halfvy, halfvz;
    
    // Normalise accelerometer measurement
    recipNorm = invSqrt(ax * ax + ay * ay + az * az);
    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;
    
    // Estimated direction of gravity
    halfvx = q1q3 - q0q2;
    halfvy = q0q1 + q2q3;
    halfvz = q0q0 - 0.5f + q3q3;
  
    // Error is sum of cross product between estimated direction and measured direction of field vectors
    halfex += (ay * halfvz - az * halfvy);
    halfey += (az * halfvx - ax * halfvz);
    halfez += (ax * halfvy - ay * halfvx);
  }

  // Apply feedback only when valid data has been gathered from the accelerometer or magnetometer
  if(halfex != 0.0f && halfey != 0.0f && halfez != 0.0f) {
    // Compute and apply integral feedback if enabled
    if(twoKi > 0.0f) {
      integralFBx += twoKi * halfex * (1.0f / sampleFreq);  // integral error scaled by Ki
      integralFBy += twoKi * halfey * (1.0f / sampleFreq);
      integralFBz += twoKi * halfez * (1.0f / sampleFreq);
      gx += integralFBx;  // apply integral feedback
      gy += integralFBy;
      gz += integralFBz;
    }
    else {
      integralFBx = 0.0f; // prevent integral windup
      integralFBy = 0.0f;
      integralFBz = 0.0f;
    }

    // Apply proportional feedback
    gx += twoKp * halfex;
    gy += twoKp * halfey;
    gz += twoKp * halfez;
  }
  
  // Integrate rate of change of quaternion
  gx *= (0.5f * (1.0f / sampleFreq));   // pre-multiply common factors
  gy *= (0.5f * (1.0f / sampleFreq));
  gz *= (0.5f * (1.0f / sampleFreq));
  qa = q0;
  qb = q1;
  qc = q2;
  q0 += (-qb * gx - qc * gy - q3 * gz);
  q1 += (qa * gx + qc * gz - q3 * gy);
  q2 += (qa * gy - qb * gz + q3 * gx);
  q3 += (qa * gz + qb * gy - qc * gx);
  
  // Normalise quaternion
  recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
  q0 *= recipNorm;
  q1 *= recipNorm;
  q2 *= recipNorm;
  q3 *= recipNorm;
}


/**
 * Populates array q with a quaternion representing the IMU orientation with respect to the Earth
 * 
 * @param q the quaternion to populate
*/
void FreeIMU::getQ(float * q) {
  float val[9];
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
  #if IS_9DOM()
    #if HAS_AXIS_ALIGNED()
      AHRSupdate(val[3] * M_PI/180, val[4] * M_PI/180, val[5] * M_PI/180, val[0], val[1], val[2], val[6], val[7], val[8]);
    #elif defined(SEN_10724)
      AHRSupdate(val[3] * M_PI/180, val[4] * M_PI/180, val[5] * M_PI/180, val[0], val[1], val[2], val[7], -val[6], val[8]);
    #elif defined(ARDUIMU_v3)
      AHRSupdate(val[3] * M_PI/180, val[4] * M_PI/180, val[5] * M_PI/180, val[0], val[1], val[2], -val[6], -val[7], val[8]);
    #endif
  #else
    AHRSupdate(val[3] * M_PI/180, val[4] * M_PI/180, val[5] * M_PI/180, val[0], val[1], val[2]);
  #endif
  
  q[0] = q0;
  q[1] = q1;
  q[2] = q2;
  q[3] = q3;
  
}
#if HAS_MS5611()
const float def_sea_press = 1013.25;

/**
 * Returns an altitude estimate from baromether readings only using sea_press as current sea level pressure
*/
float FreeIMU::getBaroAlt(float sea_press) {
  float temp = baro.getTemperature(MS561101BA_OSR_4096);
  float press = baro.getPressure(MS561101BA_OSR_4096);
  return ((pow((sea_press / press), 1/5.257) - 1.0) * (temp + 273.15)) / 0.0065;
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

/**
 * Compensates the accelerometer readings in the 3D vector acc expressed in the sensor frame for gravity
 * @param acc the accelerometer readings to compensate for gravity
 * @param q the quaternion orientation of the sensor board with respect to the world
*/
void FreeIMU::gravityCompensateAcc(float * acc, float * q) {
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
 * Returns the Euler angles in radians defined in the Aerospace sequence.
 * See Sebastian O.H. Madwick report "An efficient orientation filter for 
 * inertial and intertial/magnetic sensor arrays" Chapter 2 Quaternion representation
 * 
 * @param angles three floats array which will be populated by the Euler angles in radians
*/
void FreeIMU::getEulerRad(float * angles) {
  float q[4]; // quaternion
  getQ(q);
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
  float gx, gy, gz; // estimated gravity direction
  getQ(q);
  
  
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
 * Sets thermal calibration on (1) or off (0) for the accelerometer and gyro calibration from the
 * main sketch
*/
void FreeIMU::setTempCalib(int opt_temp_cal) {
   temp_corr_on = opt_temp_cal;
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

