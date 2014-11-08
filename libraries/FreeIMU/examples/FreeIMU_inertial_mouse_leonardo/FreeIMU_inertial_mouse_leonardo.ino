/**
 * Example program for using the FreeIMU connected to an Arduino Leonardo.
 * The program reads sensor data from the FreeIMU, computes the yaw, pitch
 * and roll using the FreeIMU library sensor fusion and use them to move the
 * mouse cursor. The mouse is emulated by the Arduino Leonardo using the Mouse
 * library.
 * 
 * @author Fabio Varesano - fvaresano@yahoo.it
*/

#include <ADXL345.h>
#include <bma180.h>
#include <HMC58X3.h>
#include <LSM303.h>
#include <ITG3200.h>
#include <MS561101BA.h>
#include <I2Cdev.h>
#include <MPU60X0.h>
#include <EEPROM.h>
#include <L3G.h>
#include <LPS331.h> 
#include <iCompass.h>

#include <AP_Math_freeimu.h>
#include <Filter.h>    // Filter library
#include <Butter.h>    // Butterworth filter
#include <RunningAverage.h>
#include <MovingAvarageFilter.h>

//#define DEBUG
#include "DebugUtils.h"
#include "FreeIMU.h"
#include <Wire.h>
#include <SPI.h>

#include <math.h>

float ypr[3]; // yaw pitch roll

// Set the FreeIMU object
FreeIMU my3IMU = FreeIMU();


void setup() {
  Mouse.begin();
  //Serial.begin(115200);
  Wire.begin();
  
  my3IMU.init(true);
}


void loop() {
  
  my3IMU.getYawPitchRoll(ypr);
  
  // scale angles to mouse movements. You can replace 10 with whatever feels adeguate for you.
  // biggere values mean faster movements
  int x = map(ypr[1], -90, 90, -10, 10);
  int y = map(ypr[2], -90, 90, -10, 10);
  
  // move mouse
  Mouse.move(-x, y, 0);
}


