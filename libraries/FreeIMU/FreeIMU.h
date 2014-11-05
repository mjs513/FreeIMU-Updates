/*
FreeIMU.h - A libre and easy to use orientation sensing library for Arduino
Copyright (C) 2011 Fabio Varesano <fabio at varesano dot net>

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

*/

#ifndef FreeIMU_h
#define FreeIMU_h

// Uncomment the appropriated version of FreeIMU you are using
//#define FREEIMU_v01
//#define FREEIMU_v02
//#define FREEIMU_v03
//#define FREEIMU_v035
//#define FREEIMU_v035_MS
//#define FREEIMU_v035_BMP
#define FREEIMU_v04

// 3rd party boards. Please consider donating or buying a FreeIMU board to support this library development.
//#define SEN_10121 //IMU Digital Combo Board - 6 Degrees of Freedom ITG3200/ADXL345 SEN-10121 http://www.sparkfun.com/products/10121
//#define SEN_10736 //9 Degrees of Freedom - Razor IMU SEN-10736 http://www.sparkfun.com/products/10736/
//#define SEN_10724 //9 Degrees of Freedom - Sensor Stick SEN-10724 http://www.sparkfun.com/products/10724
//#define SEN_10183 //9 Degrees of Freedom - Sensor Stick  SEN-10183 http://www.sparkfun.com/products/10183
//#define ARDUIMU_v3 //  DIYDrones ArduIMU+ V3 http://store.diydrones.com/ArduIMU_V3_p/kt-arduimu-30.htm or https://www.sparkfun.com/products/11055
//#define GEN_MPU6050 // Generic MPU6050 breakout board. Compatible with GY-521, SEN-11028 and other MPU6050 wich have the MPU6050 AD0 pin connected to GND.
//#define DFROBOT  //DFROBOT 10DOF SEN-1040 IMU
//#define MPU9250_5611  //MPU-9250 IMU with MS5611 Altimeter from eBay
//#define GEN_MPU9150
//#define GEN_MPU9250  // Use for Invensense MPU-9250 breakout board
//#define Altimu10  // Pololu AltIMU v10 - 10 DOF IMU - http://www.pololu.com/product/1269
//#define GY_88  //GY-88 Sensor Board from eBay
//#define GY_87  //GY-87 Sensor Board from eBay, NOTE: Pressusre sensor is BMP180 but BMP085 library should work
//#define Mario   // MPU-9150 plus Altitude/Pressure Sensor Breakout - MPL3115A2  https://www.sparkfun.com/products/11084
//#define APM_2_5  //  APM 2.5.2 (EBAY)

//#define DISABLE_MAGN // Uncomment this line to disable the magnetometer in the sensor fusion algorithm

//Magnetic declination angle for iCompass
#define MAG_DEC 4 //+4.0 degrees for Israel
#define MAG_DEC -13.1603  //degrees for Flushing, NY

//Number of samples to average in iCompass
#define WINDOW_SIZE 1 //Set to 1 to turn off the Running Average

// Set filter type: 1 = Madgwick Gradient Descent, 0 - Madgwick implementation of Mahoney DCM
// in Quaternion form, 3 = Madwick Original Paper AHRS
#define MARG 3

// proportional gain governs rate of convergence to accelerometer/magnetometer
// integral gain governs rate of convergence of gyroscope biases
// set up defines for various boards in my inventory, DFROBOT and Freeimu have
// temperature calibration curves. (3.31.14)

#if defined(DFROBOT) 
	#define twoKpDef  (2.0f * 0.5f)
	#define twoKiDef  (2.0f * 0.00002f)
	#define betaDef  0.1f
#elif defined(FREEIMU_v04)
	#define twoKpDef  (2.0f * 0.75f)	//works with and without mag enabled
	#define twoKiDef  (2.0f * 0.1625f)
	#define betaDef  0.085f
#elif defined(GEN_MPU6050)
	#define twoKpDef  (2.0f * 0.5f)
	#define twoKiDef  (2.0f * 0.25f)
	#define betaDef	  0.2f
#elif defined(GEN_MPU9150)
	#define twoKpDef  (2.0f * 0.75f)
	#define twoKiDef  (2.0f * 0.1f)	
	#define betaDef	  0.01f
#elif defined(Altimu10)
	//#define twoKpDef  (2.0f * 1.01f)
	//#define twoKiDef  (2.0f * 0.00002f)	
	#define twoKpDef  (2.0f * 2.75f)
	#define twoKiDef  (2.0f * 0.1625f)
	#define betaDef  2.0f
#elif defined(GEN_MPU9250) || defined(MPU9250_5611)
	#define twoKpDef  (2.0f * 1.75f) // was 0.95
	#define twoKiDef  (2.0f * 0.05f) // was 0.05	
	#define betaDef	  0.015f
#elif defined(APM_2_5)
	#define twoKpDef  (2.0f * 0.5f)
	#define twoKiDef  (2.0f * 0.25f)
	#define betaDef	  0.015f	//was 0.015
#else
	#define twoKpDef  (2.0f * 0.5f)
	#define twoKiDef  (2.0f * 0.1f)
	#define betaDef  0.1f
#endif 

//
// Other Options
//
  #define temp_break  -1000	  //original temp_break = -4300;
  #define senTemp_break  32
  #define temp_corr_on_default  0
  #define nsamples 75
  #define instability_fix 1

// ***********************************************
// *** No configuration needed below this line ***
// ***********************************************
#define FREEIMU_LIB_VERSION "DEV"

#define FREEIMU_DEVELOPER "Fabio Varesano - varesano.net"

#if F_CPU == 16000000L
  #define FREEIMU_FREQ "16 MHz"
#elif F_CPU == 8000000L
  #define FREEIMU_FREQ "8 MHz"
#elif F_CPU == 24000000L
  #define FREEIMU_FREQ "24 MHz"
#elif F_CPU == 48000000L
  #define FREEIMU_FREQ "48 MHz" 
#elif F_CPU == 72000000L
  #define FREEIMU_FREQ "48 MHz" 
#elif F_CPU == 84000000L
  #define FREEIMU_FREQ "84 MHz"   
#else
  #define FREEIMU_FREQ "Not Defined"  
#endif


// board IDs
#if defined(FREEIMU_v01)
  #define FREEIMU_ID "FreeIMU v0.1"
#elif defined(FREEIMU_v02)
  #define FREEIMU_ID "FreeIMU v0.2"
#elif defined(FREEIMU_v03)
  #define FREEIMU_ID "FreeIMU v0.3"
#elif defined(FREEIMU_v035)
  #define FREEIMU_ID "FreeIMU v0.3.5"
#elif defined(FREEIMU_v035_BMP)
  #define FREEIMU_ID "FreeIMU v0.3.5_BMP"
#elif defined(FREEIMU_v035_MS)
  #define FREEIMU_ID "FreeIMU v0.3.5_MS"
#elif defined(FREEIMU_v04)
  #define FREEIMU_ID "FreeIMU v0.4"
#elif defined(SEN_10121)
  #define FREEIMU_ID "SparkFun 10121"
#elif defined(SEN_10736)
  #define FREEIMU_ID "SparkFun 10736"
#elif defined(SEN_10724)
  #define FREEIMU_ID "SparkFun 10724"
#elif defined(SEN_10183)
  #define FREEIMU_ID "SparkFun 10183"
#elif defined(ARDUIMU_v3)
  #define FREEIMU_ID "DIY Drones ArduIMU+ V3"
#elif defined(DFROBOT)
  #define FREEIMU_ID "DFROBOT"
#elif defined(GEN_MPU6050)
  #define FREEIMU_ID "GEN MPU-6050"
#elif defined(GEN_MPU9150)
  #define FREEIMU_ID "GEN MPU-9150"  
#elif defined(MPU9250_5611)
  #define FREEIMU_ID "MPU9250_5611"
#elif defined(GEN_MPU9250)
  #define FREEIMU_ID "GEN MPU-9250"
#elif defined(Altimu10)
  #define FREEIMU_ID "AltIMU-v10"  
#elif defined(GY_88)
  #define FREEIMU_ID "GY-88 Sensor Board"  
#elif defined(GY_87)
  #define FREEIMU_ID "GY-87 Sensor Board" 
#elif defined(APM_2_5)
  #define FREEIMU_ID "APM 2.5.2 (EBAY)" 
#elif  defined(Mario)
  #define FREEIMU_ID "MPU-9150 plus MPL3115A2" 
#endif


#define HAS_ITG3200() (defined(DFROBOT) || defined(FREEIMU_v01) || defined(FREEIMU_v02) \
					  || defined(FREEIMU_v03) || defined(FREEIMU_v035) || defined(FREEIMU_v035_MS) \
					  || defined(FREEIMU_v035_BMP) || defined(SEN_10121) || defined(SEN_10736) \
					  || defined(SEN_10724) || defined(SEN_10183))
#define HAS_ADXL345() (defined(DFROBOT) || defined(FREEIMU_v01) || defined(FREEIMU_v02) \
					  || defined(FREEIMU_v03) || defined(SEN_10121) || defined(SEN_10736) \
					  || defined(SEN_10724) || defined(SEN_10183))
#define HAS_BMA180() (defined(FREEIMU_v035) || defined(FREEIMU_v035_MS) || defined(FREEIMU_v035_BMP))
#define HAS_MPU6050() (defined(GY_87) ||defined(GY_88) || defined(FREEIMU_v04) || defined(GEN_MPU6050))
#define HAS_MPU9150() (defined(GEN_MPU9150) )
#define HAS_MPU9250() (defined(MPU9250_5611) || defined(GEN_MPU9250)  || defined(Mario) || defined(MPU9250_5611)) 
#define HAS_HMC5883L() (defined(GY_87) ||defined(GY_88) || defined(DFROBOT) || defined(FREEIMU_v01) || defined(FREEIMU_v02) \
					   || defined(FREEIMU_v03) || defined(FREEIMU_v035) || defined(FREEIMU_v035_MS) \
					   || defined(FREEIMU_v035_BMP) || defined(FREEIMU_v04) || defined(SEN_10736) \
					   || defined(SEN_10724) || defined(SEN_10183) || defined(ARDUIMU_v3) \
					   || defined(APM_2_5))
#define HAS_MPU6000() (defined(ARDUIMU_v3) || defined(APM_2_5))
#define HAS_APM25()	(defined(APM_2_5))
#define HAS_ALTIMU10() (defined(Altimu10))
#define HAS_L3D20() (defined(Altimu10))
#define HAS_LSM303() (defined(Altimu10))

#define HAS_MS5611() (defined(MPU9250_5611) || defined(FREEIMU_v035_MS) || defined(FREEIMU_v04) \
					 || defined(APM_2_5))
#define HAS_BMP085() (defined(GY_88) || defined(GY_88) || defined(DFROBOT))
#define HAS_LPS331() (defined(Altimu10))
#define HAS_MPL3115A2() defined(Mario)
#define HAS_PRESS() (defined(Altimu10) || defined(MPU9250_5611) || defined(FREEIMU_v035_MS) \
					|| defined(FREEIMU_v04) || defined(FREEIMU_v035) || defined(FREEIMU_v035_MS) \
					|| defined(FREEIMU_v035_BMP) || defined(FREEIMU_v035_MS) || defined(FREEIMU_v04) \
					|| defined(GY_87) ||defined(GY_88) || defined(DFROBOT) || defined(APM_2_5) \
					|| defined(Mario) )
					
#define IS_6DOM() (defined(SEN_10121) || defined(GEN_MPU6050))
#define IS_9DOM() (defined(GY_87) ||defined(GY_88) || defined(Altimu10) || defined(GEN_MPU9250) || defined(MPU9250_5611) \
				   || defined(GEN_MPU9150) || defined(DFROBOT) || defined(FREEIMU_v01) || defined(FREEIMU_v02) \
				   || defined(FREEIMU_v03) || defined(FREEIMU_v035) || defined(FREEIMU_v035_MS) || defined(FREEIMU_v035_BMP) \
				   || defined(FREEIMU_v04) || defined(SEN_10736) || defined(SEN_10724) || defined(SEN_10183) \
				   || defined(ARDUIMU_v3)  || defined(APM_2_5) || defined(Mario) )
#define HAS_AXIS_ALIGNED() (defined(Altimu10) || defined(GY_88) || defined(GEN_MPU6050) \
							|| defined(DFROBOT) || defined(FREEIMU_v01) || defined(FREEIMU_v02) \
							|| defined(FREEIMU_v03) || defined(FREEIMU_v035) || defined(FREEIMU_v035_MS) \
							|| defined(FREEIMU_v035_BMP) || defined(FREEIMU_v04) || defined(SEN_10121) \
							|| defined(SEN_10736) || defined(GY_87) )

#include <Wire.h>

#include "Arduino.h"
#include "calibration.h"
#include <MovingAvarageFilter.h>

#ifndef CALIBRATION_H
	#include <EEPROM.h>
#endif

#define FREEIMU_EEPROM_BASE 0x0A
#define FREEIMU_EEPROM_SIGNATURE 0x19

//#if FREEIMU_VER <= 3
#if HAS_ADXL345()
  #include <ADXL345.h>
  // default I2C 7-bit addresses of the sensors
  #define FIMU_ACC_ADDR ADXL345_ADDR_ALT_LOW // SDO connected to GND
  //#define FIMU_ADXL345_DEF_ADDR ADXL345_ADDR_ALT_HIGH // SDO connected to GND
#elif HAS_BMA180()
  #include <bma180.h>
  #define FIMU_ACC_ADDR BMA180_ADDRESS_SDO_LOW 
  //#define FIMU_ACC_ADDR BMA180_ADDRESS_SDO_HIGH
//#elif FREEIMU_VER == 7
#elif HAS_MPU6050()
  #include <Wire.h>
  #include "I2Cdev.h"
  #include "MPU60X0.h"
  //MPU Address Select 
  //Use following define if MPU60X0 address is set to 0x69
  //otherwise the default address is used = 0x68
  //uncomment following line and comment out successor line
  //#define FIMU_ACCGYRO_ADDR MPU60X0_ADDRESS_AD0_HIGH
  #define FIMU_ACCGYRO_ADDR MPU60X0_DEFAULT_ADDRESS
#elif HAS_MPU6000()
  #include <SPI.h>
  #include "I2Cdev.h"
  #include "MPU60X0.h"
  #define FIMU_ACCGYRO_ADDR MPU60X0_DEFAULT_SS_PIN
#elif HAS_MPU9150()
  #include <Wire.h>
  #include "I2Cdev.h"
  #include "MPU60X0.h"
  #include "AK8975.h"
  #include "iCompass.h"
  //MPU Address Select 
  //Use following define if MPU60X0 address is set to 0x69
  //otherwise the default address is used = 0x68
  //uncomment following line and comment out successor line
  //#define FIMU_ACCGYRO_ADDR MPU60X0_ADDRESS_AD0_HIGH
  #define FIMU_ACCGYRO_ADDR MPU60X0_DEFAULT_ADDRESS
#elif HAS_MPU9250()
  #include <Wire.h>
  #include "I2Cdev.h"
  #include "MPU60X0.h"
  #include "AK8963.h"
  #include "iCompass.h"
  //MPU Address Select 
  //Use following define if MPU60X0 address is set to 0x69
  //otherwise the default address is used = 0x68
  //uncomment following line and comment out successor line
  //#define FIMU_ACCGYRO_ADDR MPU60X0_ADDRESS_AD0_HIGH
  #define FIMU_ACCGYRO_ADDR MPU60X0_DEFAULT_ADDRESS
#elif HAS_ALTIMU10()
  #include <Wire.h>
#endif

#if HAS_BMP085()
  #include <BMP085.h>
#elif HAS_MS5611()
  #if HAS_APM25()
	#include <AP_Baro_MS5611.h>
  #else
	#include <MS561101BA.h>
	#define FIMU_BARO_ADDR MS561101BA_ADDR_CSB_LOW
	//#define FIMU_BARO_ADDR MS561101BA_ADDR_CSB_HIGH
  #endif
#elif HAS_LPS331()
  #include <LPS331.h>
#elif HAS_MPL3115A2()
  #include <MPL3115A2.h>
#endif

#if HAS_PRESS()
  #include <FilteringScheme.h>
  #include <AltitudeComplementary.h>
#endif

#if HAS_ITG3200()
  #include <ITG3200.h>
  //Added next line for Sen1040
  #define FIMU_ITG3200_DEF_ADDR ITG3200_ADDR_AD0_LOW 
#elif HAS_L3D20()
  #include <L3G.h>
#endif

#if HAS_HMC5883L()
  #include <HMC58X3.h>
  #include "iCompass.h"
#endif

#if HAS_LSM303()
  #include <LSM303.h>
  #include "iCompass.h"
#endif

#define FIMU_BMA180_DEF_ADDR BMA180_ADDRESS_SDO_LOW
#define FIMU_ITG3200_DEF_ADDR ITG3200_ADDR_AD0_LOW // AD0 connected to GND
// HMC5843 address is fixed so don't bother to define it

#ifndef cbi
    #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

class FreeIMU
{
  public:
    FreeIMU();
	void init();
    /*void init();
	//void init0(bool fastmode);
    //void init(bool fastmode); */
	void RESET();
	void RESET_Q();
	
    #if HAS_ITG3200()
		void init(bool fastmode);
		void init(int acc_addr, int gyro_addr, bool fastmode);
	#elif HAS_ALTIMU10()
		void init(bool fastmode);
		void init0(bool fastmode);
	#else
		void init(bool fastmode);
		void init(int accgyro_addr, bool fastmode);
    #endif
	
    #ifndef CALIBRATION_H
		void calLoad();
    #endif
	
    void zeroGyro();
	void initGyros();
    void getRawValues(int * raw_values);
    void getValues(float * values);
    void getQ(float * q, float * val);
    void getEuler(float * angles);
    void getYawPitchRoll(float * ypr);
    void getEulerRad(float * angles);
    void getYawPitchRollRad(float * ypr);
	void getYawPitchRollRadAHRS(float * ypr, float * q);
	float invSqrt(float x);
	void setTempCalib(int opt_temp_cal);
	void setSeaPress(float sea_press_inp);
	float calcMagHeading(float q0, float q1, float q2, float q3, float bx, float by, float bz);
	void getQ_simple(float* q);
	void MotionDetect(float * val);
	
    #if HAS_MS5611()
      float getBaroAlt();
      float getBaroAlt(float sea_press);
	  float getBaroTemperature();
	  float getBaroPressure();
    #elif HAS_BMP085()
      float getBaroAlt();
      float getBaroAlt(float sea_press);
	  float getBaroTemperature();
	  float getBaroPressure();
	#elif HAS_LPS331()
      float getBaroAlt();
      float getBaroAlt(float sea_press);
	  float getBaroTemperature();
	  float getBaroPressure();
	#elif HAS_MPL3115A2()
      float getBaroAlt();
      float getBaroAlt(float sea_press);
	  float getBaroTemperature();
	  float getBaroPressure();	  
    #endif	
    
	#if HAS_PRESS()
      //float getEstAltitude();
	  float getEstAltitude(float * q, float * val, float dt2);
    #endif
        
    // we make them public so that users can interact directly with device classes
    #if HAS_ADXL345()
      ADXL345 acc;
    #elif HAS_BMA180()
      BMA180 acc;
    #endif
    
    #if HAS_HMC5883L()
      HMC58X3 magn;
	  iCompass maghead;	
    #endif
    
    #if HAS_ITG3200()
      ITG3200 gyro;
    #elif HAS_MPU6050()
      MPU60X0 accgyro; 
    #elif HAS_MPU6000()
      MPU60X0 accgyro;
	#elif HAS_MPU9150()
	  MPU60X0 accgyro;
	  AK8975 mag;
	  iCompass maghead;	  
	#elif HAS_MPU9250()
	  MPU60X0 accgyro;
	  AK8963 mag;
	  iCompass maghead;	 	
    #endif

	#if HAS_L3D20()
	  L3G gyro;
	#endif
	
	#if HAS_LSM303()
	  LSM303 compass;  // accelerometer, magnetometer and heading - same as iCompass
	  iCompass maghead;
	#endif
      
    #if HAS_MS5611()
		#if HAS_APM25()
			AP_Baro_MS5611 baro;
		#else
			MS561101BA baro;
		#endif
    #elif HAS_BMP085()
      BMP085 baro085;
	#elif HAS_LPS331()
	  LPS331 baro331;
	#elif HAS_MPL3115A2()
	  MPL3115A2 baro3115;	  
    #endif
    
    #if HAS_PRESS()
      KalmanFilter kPress; // Altitude Kalman Filter.
      AltComp altComp; // Altitude Complementary Filter.
    #endif
     
	//Global Variables
	 
    int* raw_acc, raw_gyro, raw_magn;
    // calibration parameters
    int16_t gyro_off_x, gyro_off_y, gyro_off_z;
    int16_t acc_off_x, acc_off_y, acc_off_z, magn_off_x, magn_off_y, magn_off_z;
    float acc_scale_x, acc_scale_y, acc_scale_z, magn_scale_x, magn_scale_y, magn_scale_z;
	float val[12];
	//int8_t nsamples, temp_break, instability_fix, senTemp_break;
	int16_t DTemp, temp_corr_on; 
	float rt, senTemp;
	float sampleFreq; // half the sample period expressed in seconds
	byte deviceType;
	
	#define gyroMeasError 3.14159265358979 * (.50f / 180.0f) 	// gyroscope measurement error in rad/s (shown as 5 deg/s)
	#define gyroMeasDrift 3.14159265358979 * (0.02f / 180.0f) 	// gyroscope measurement error in rad/s/s (shown as 0.2f deg/s/s)
	#define beta1 sqrt(3.0f / 4.0f) * gyroMeasError 			// compute beta
	#define zeta sqrt(3.0f / 4.0f) * gyroMeasDrift 				// compute zeta

  private:
    //void AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
    //void AHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
    
	bool  bSPI;
	float bx, by, bz;
    float iq0, iq1, iq2, iq3;
    float exInt, eyInt, ezInt;  			// scaled integral error
    volatile float twoKp;      				// 2 * proportional gain (Kp)
    volatile float twoKi;      				// 2 * integral gain (Ki)
    volatile float q0, q1, q2, q3, q3old; 	// quaternion of sensor frame relative to auxiliary frame
    volatile float integralFBx,  integralFBy, integralFBz;
    unsigned long lastUpdate, now; 			// sample period expressed in milliseconds
	unsigned long lastUpdate1 = 0;
	unsigned long now1;
	
	//Madgwick AHRS Gradient Descent 
    volatile float beta;				// algorithm gain

	//Following lines defines Madgwicks Grad Descent Algorithm from his original paper
	// Global system variables
	float SEq_1 = 1, SEq_2 = 0, SEq_3 = 0, SEq_4 = 0; 	// estimated orientation quaternion elements with initial conditions
	float b_x = 1, b_z = 0; 				// reference direction of flux in earth frame
	float w_bx = 0, w_by = 0, w_bz = 0; // estimate gyroscope biases error
	
	#if(MARG == 0)
		void AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
		void AHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
  	#elif(MARG == 1)
		void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
		void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
	#else
		void MARGUpdateFilter(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
		void MARGUpdateFilterIMU(float gx, float gy, float gz, float ax, float ay, float az);
	#endif
};

float invSqrt(float number);
void arr3_rad_to_deg(float * arr);
void Qmultiply(float *  q, float *  q1, float * q2);
void gravityCompensateAcc(float * acc, float * q);

#endif // FreeIMU_h

