/* Copyright (C) 2014 Kristian Lauszus, TKJ Electronics. All rights reserved.

 This software may be distributed and modified under the terms of the GNU
 General Public License version 2 (GPL2) as published by the Free Software
 Foundation and appearing in the file GPL2.TXT included in the packaging of
 this file. Please note that GPL2 Section 2[b] requires that all works based
 on this software must also be made publicly available under the terms of
 the GPL2 ("Copyleft").

 Contact information
 -------------------

 Kristian Lauszus, TKJ Electronics
 Web      :  http://www.tkjelectronics.com
 e-mail   :  kristianl@tkjelectronics.com
 -------------------
 
 Sketch modified by Michael J Smorto (mjs513 on GITHUB) for
 use with FreeIMU-Update library and comparison to Madgwick
 filters.
 
 Same licens as original sketch by Kristian Lauszus, TKJ Electronics.
*/
#include <Wire.h>
#include <SPI.h>

//These are optional depending on your IMU configuration
#include <ADXL345.h>
#include <HMC58X3.h>
#include <LSM303.h>
#include <LPS.h> 
#include <L3G.h>
#include <ITG3200.h> //note LPS library must come before ITG lib
#include <bma180.h>
#include <MS561101BA.h> //Comment out for APM 2.5
#include <BMP085.h>
#include <I2Cdev.h>
#include <MPU60X0.h>
#include <AK8975.h>
#include <AK8963.h>
#include <SFE_LSM9DS0.h>
#include <BaroSensor.h>
//#include <AP_Baro_MS5611.h>  //Uncomment for APM2.5


//These are mandatory
#include <AP_Math_freeimu.h>
#include <Butter.h>    // Butterworth filter
#include <iCompass.h>
#include <MovingAvarageFilter.h>

//#define DEBUG
#include "DebugUtils.h"
#include "CommunicationUtils.h"
#include "DCM.h"
#include "FilteringScheme.h"
#include "RunningAverage.h"
#include "FreeIMU.h"

//Intel Edison, Arduino 101, Arduino Due, Arduino Zero: no eeprom 
#if defined(__SAMD21G18A__) || defined(__SAM3X8E__) || defined(__ARDUINO_ARC__) || defined(__SAMD21G18A__)
  #define HAS_EEPPROM 0
#else
  #include <EEPROM.h>
  #define HAS_EEPPROM 1
#endif

#include <Kalman.h> // Source: https://github.com/TKJElectronics/KalmanFilter
 
float output[6];
float val[13];

// Set the FreeIMU object and LSM303 Compass
FreeIMU my3IMU = FreeIMU();

//#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf
#define opt 1

Kalman kalmanX, kalmanY, kalmanZ; // Create the Kalman instances

/* IMU Data */
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
double magX, magY, magZ;
int16_t tempRaw;
float ypr[3]; // yaw pitch roll

double roll, pitch, yaw; // Roll and pitch are calculated using the accelerometer while yaw is calculated using the magnetometer
double kalAngleX, kalAngleY, kalAngleZ; // Calculated angle using a Kalman filter

uint32_t timer;

void setup() {
  delay(100); // Wait for sensors to get ready

  Serial.begin(38400);
  Wire.begin();
  
  my3IMU.init(true);
  
  delay(300); // Wait for sensors to stabilize

  /* Set Kalman and gyro starting angle */
  updateMPU6050();
  updateHMC5883L();
  updatePitchRoll();
  updateYaw();

  kalmanX.setAngle(roll); // First set roll starting angle
  kalmanY.setAngle(pitch); // Then pitch
  kalmanZ.setAngle(yaw); // And finally yaw

  timer = micros(); // Initialize the timer
}

void loop() {

  /* Update all the IMU values */
  updateMPU6050();
  updateHMC5883L();

  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();

  /* Roll and pitch estimation */
  updatePitchRoll();
  double gyroXrate = gyroX ; // Convert to deg/s
  double gyroYrate = gyroY ; // Convert to deg/s

  #ifdef RESTRICT_PITCH
    // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
    if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
      kalmanX.setAngle(roll);
      kalAngleX = roll;
    } else
      kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter

    if (abs(kalAngleX) > 90)
      gyroYrate = -gyroYrate; // Invert rate, so it fits the restricted accelerometer reading
      kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);
  #else
    // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
    if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) {
      kalmanY.setAngle(pitch);
      kalAngleY = pitch;
     } else
      kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter

    if (abs(kalAngleY) > 90)
      gyroXrate = -gyroXrate; // Invert rate, so it fits the restricted accelerometer reading
      kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
  #endif

  /* Yaw estimation */
  updateYaw();
  double gyroZrate = gyroZ ; // Convert to deg/s
  // This fixes the transition problem when the yaw angle jumps between -180 and 180 degrees
  if ((yaw < -90 && kalAngleZ > 90) || (yaw > 90 && kalAngleZ < -90)) {
    kalmanZ.setAngle(yaw);
    kalAngleZ = yaw;
   } else
    kalAngleZ = kalmanZ.getAngle(yaw, gyroZrate, dt); // Calculate the angle using a Kalman filter
 
  /* Print Data */
  #if 1
    my3IMU.getYawPitchRoll180(ypr);
    output[0] = kalAngleZ;
    output[1] = ypr[0] * RAD_TO_DEG;
    output[2] = -kalAngleY;
    output[3] = ypr[1] * RAD_TO_DEG;    
    output[4] = kalAngleX;
    output[5] = ypr[2] * RAD_TO_DEG;   
    if(opt == 1) {
      serialPrintFloatArr(output, 6);
    } else {
      Serial.print(kalAngleZ); Serial.print(",");
      Serial.print(ypr[0] * RAD_TO_DEG); Serial.print(",");
      Serial.print(-kalAngleY); Serial.print(",");
      Serial.print(ypr[1] * RAD_TO_DEG); Serial.print(",");
      Serial.print(kalAngleX); Serial.print(",");
      Serial.print(ypr[2] * RAD_TO_DEG);
      //Serial.println();
    }
    delay(150);
  #endif

  #if 0 // Set to 1 to print the IMU data
    Serial.print(accX ); Serial.print("\t"); // Converted into g's
    Serial.print(accY ); Serial.print("\t");
    Serial.print(accZ); Serial.print("\t");

    Serial.print(gyroXrate); Serial.print("\t"); // Converted into degress per second
    Serial.print(gyroYrate); Serial.print("\t");
    Serial.print(gyroZrate); Serial.print("\t");

    Serial.print(magX); Serial.print("\t"); // After gain and offset compensation
    Serial.print(magY); Serial.print("\t");
    Serial.print(magZ); Serial.print("\t");
  #endif

  #if 0 // Set to 1 to print the temperature
    Serial.print("\t");

    double temperature = (double)tempRaw / 340.0 + 36.53;
    Serial.print(temperature); Serial.print("\t");
  #endif

  Serial.println();

  delay(10);
  
}

void updateMPU6050() {
    //float val[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    my3IMU.getValues(val);
    gyroX = (double)(val[3]);
    gyroY = (double)(val[4]);
    gyroZ = (double)(val[5]);
    accX = (double)(val[0]);
    accY = (double)(val[1]);
    accZ = (double)(val[2]);
    magX = (double)(val[6]);
    magY = (double)(val[7]);
    magZ = (double)(val[8]);
}

void updateHMC5883L() {
}

void updatePitchRoll() {
  // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
  // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
  // It is then converted from radians to degrees
  #ifdef RESTRICT_PITCH // Eq. 25 and 26
    roll = atan2(accY, accZ) * RAD_TO_DEG;
    pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
  #else // Eq. 28 and 29
    roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
    pitch = atan2(-accX, accZ) * RAD_TO_DEG;
  #endif
}

void updateYaw() { // See: http://www.freescale.com/files/sensors/doc/app_note/AN4248.pdf
  //magX *= -1; // Invert axis - this it done here, as it should be done after the calibration
  //magZ *= -1;

  double rollAngle = kalAngleX * DEG_TO_RAD;
  double pitchAngle = kalAngleY * DEG_TO_RAD;

  double Bfy = magZ * sin(rollAngle) - magY * cos(rollAngle);
  double Bfx = magX * cos(pitchAngle) + magY * sin(pitchAngle) * sin(rollAngle) + magZ * sin(pitchAngle) * cos(rollAngle);
  yaw = atan2(-Bfy, Bfx) * RAD_TO_DEG;

  //yaw *= -1;
}



