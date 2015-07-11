#include <AP_Math_freeimu.h>
#include <Butter.h>    // Butterworth filter
#include <iCompass.h>
/**
 * FreeIMU library serial communication protocol to be used with
 * modified Adafruit bunnyrotate processing sketch
 * Kalman library extracted from picopter Githup
*/

#include <HMC58X3.h>
#include <BMP085.h>
#include <I2Cdev.h>
#include <MPU60X0.h>
#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>

//#define DEBUG
#include "DebugUtils.h"
#include "CommunicationUtils.h"
#include "FreeIMU.h"
#include "FilteringScheme.h"
#include "RunningAverage.h"
#include <MovingAvarageFilter.h>

#include <stlport.h>
#include <Eigen30.h>
#include <iostream>

#include <ExtendedKalman.h>

#include <struct_calibratedData.h>
#include <struct_euler.h>
#include <Quaternion.h>

s_calibratedData calibratedData;
s_euler orientation;
QuaternionClass quaternion;

//    KalmanClass kalmanPhi_, kalmanPsi_;
ExtendedKalmanClass EKF;

#define Has_LSM303 0
#define HAS_GPS 0

float const PI_F = 3.14159265F;
int16_t raw_values[11];
float ypr[3]; // yaw pitch roll
char str[128];
float val[11];
float val_array[18]; 
unsigned long lastUpdate, now; 			// sample period expressed in milliseconds
float dt;

// Set the FreeIMU object and LSM303 Compass
FreeIMU my3IMU = FreeIMU();


//The command from the PC
char cmd, tempCorr;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  //#if HAS_MPU6050()
  //    my3IMU.RESET();
  //#endif
	
  my3IMU.init(true);
  Serial.println("FreeIMU initialized");
  // LED
  pinMode(13, OUTPUT);
}

void loop() {

        my3IMU.getValues(val);
	now = micros();
        dt = ((now - lastUpdate) / 1000000.0);
     
        calibratedData.x = (val[0]);
        calibratedData.y = (val[1]);
        calibratedData.z = (val[2]); 
        calibratedData.p = (val[3]);
        calibratedData.q = (val[4]);
        calibratedData.r = (val[5]);
        calibratedData.magx = (val[6]);
        calibratedData.magy = (val[7]);
        calibratedData.magz = (val[8]);
        calibratedData.q = -calibratedData.q;
        //calibratedData.temp = (my3IMU.getBaroTemperature());
        //calibratedData.pressure = (my3IMU.getBaroPressure());
        //calibratedData.altitude = ((pow(((1013.5) / calibratedData.pressure), 1/5.257) - 1.0) * (calibratedData.temp + 273.15)) / 0.0065;
  
        //Serial.println(sampleFreq/100);
        
        //if(dt/100 < 0.06) {
	    quaternion = EKF.predict(&calibratedData, dt);
        //}
        quaternion = EKF.update(&calibratedData, dt);
        
        //getYawPitchRollDeg(&quaternion);
        getYawPitchRoll180(&quaternion);
        lastUpdate = now; 	
        //SerialPrintFloatArr(val_array,18);
} 

    //******************************************************/


char serial_busy_wait() {
  while(!Serial.available()) {
    ; // do nothing until ready
  }
  return Serial.read();
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  #if HAS_GPS
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
  #endif
}

void getYawPitchRollDeg(QuaternionClass* q) {
  //float q[4]; // quaternion
  //float val[11];
  float gx, gy, gz; // estimated gravity direction
  //getQ(q, val);
  
  gx = 2 * (q->x*q->z - q->w*q->y);
  gy = 2 * (q->w*q->x + q->y*q->z);
  gz = q->w*q->w - q->x*q->x - q->y*q->y + q->z*q->z;
  
  ypr[0] = (180/pi) * atan2(2 * q->x * q->y - 2 * q->w * q->z, 2 * q->w*q->w + 2 * q->x * q->x - 1);
  ypr[1] = -(180/pi) * atan(gx / sqrt(gy*gy + gz*gz));
  ypr[2] = (180/pi) * atan(gy / sqrt(gx*gx + gz*gz));

  //Serial.print(ypr[1]); Serial.print(","); Serial.print(ypr[2]);
  //Serial.print(","); Serial.print(ypr[0]); Serial.println('\n');
    Serial.print(F("Orientation: "));
    Serial.print(ypr[0]);
    Serial.print(F(" "));
    Serial.print(ypr[1]);
    Serial.print(F(" "));
    Serial.print(ypr[2]);
    Serial.println(F(""));  
}


void getYawPitchRoll180(QuaternionClass* q) {
	float gx, gy, gz;		// estimated gravity direction

	gx = 2 * (q->x*q->z - q->w*q->y);
	gy = 2 * (q->w*q->x + q->y*q->z);
	gz = q->w*q->w - q->x*q->x - q->y*q->y + q->z*q->z;

	//calculating yaw
	ypr[0] = (180./PI)*atan2(2 * q->x * q->y - 2 * q->w * q->z, 2 * q->w*q->w + 2 * q->x * q->x - 1);	
	//ypr[0] = val[9];
	if(ypr[0] > 180.) ypr[0] = ypr[0] - 360.;
	ypr[0] = ypr[0] * 0.0174532925;
	
	//calculating Pitch
	//Serial.print(gx); Serial.print("       "); Serial.print(gz); Serial.print("       ");
	if(gx > 0 && gz > 0) {
		ypr[1] = atan(gx / sqrt(gy*gy + gz*gz));
	} else if(gx > 0 && gz <= 0) {
		ypr[1] = PI - atan(gx / sqrt(gy*gy + gz*gz));
	} else if(gx < 0 && gz < 0) {
		ypr[1] = (-PI - atan(gx / sqrt(gy*gy + gz*gz)));
	} else  {
		ypr[1] =  atan(gx / sqrt(gy*gy + gz*gz));
	}
	
	//Calculating Roll1
        ypr[2] = atan(gy / sqrt(gx*gx + gz*gz));
        
        ypr[0]= ypr[0]*180./PI;
        ypr[1]= ypr[1]*180./PI;
        ypr[2]= ypr[2]*180./PI;
        
        //adjust pitch so its 0-360 deg
        ypr[1] = -ypr[1];
        if(ypr[1] < 0) ypr[1] = 360. + ypr[1];
   
    Serial.print(F("Orientation: "));
    Serial.print(ypr[0]);
    Serial.print(F(" "));
    Serial.print(ypr[1]);
    Serial.print(F(" "));
    Serial.print(ypr[2]);
    Serial.println(F(""));
}

