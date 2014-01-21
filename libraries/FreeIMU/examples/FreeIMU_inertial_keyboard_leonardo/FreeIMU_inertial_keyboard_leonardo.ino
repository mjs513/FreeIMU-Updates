/**
 * Example program for using the FreeIMU connected to an Arduino Leonardo.
 * The program reads sensor data from the FreeIMU, computes the yaw, pitch
 * and roll using the FreeIMU library sensor fusion and use them to move the
 * mouse cursor. The Keyboard is emulated by the Arduino Leonardo using the Keyboard
 * library.
 * 
 * @author Fabio Varesano - fvaresano@yahoo.it
*/

#include <ADXL345.h>
#include <HMC58X3.h>
#include <ITG3200.h>
#include <bma180.h>
#include <MS561101BA.h>
#include <I2Cdev.h>
#include <MPU6050.h>

//#define DEBUG
#include "DebugUtils.h"

#include "FreeIMU.h"
#include "CommunicationUtils.h"
#include <Wire.h>

#include <math.h>

float ypr[3]; // yaw pitch roll

// Set the FreeIMU object
FreeIMU my3IMU = FreeIMU();

const int angleThreshold = 10;

void setup() {
  Keyboard.begin();
  //Serial.begin(115200);
  Wire.begin();
  
  my3IMU.init(true);
}


void loop() {
  
  my3IMU.getYawPitchRoll(ypr);
  
  // scale angles to mouse movements. You can replace 10 with whatever feels adeguate for you.
  // biggere values mean faster movements
//   int x = map(ypr[1], -90, 90, -10, 10);
//   int y = map(ypr[2], -90, 90, -10, 10);
  
  
  if(ypr[1] < -angleThreshold) {
    Keyboard.press( KEY_LEFT_ARROW );
  }
  else {
    Keyboard.release( KEY_LEFT_ARROW );
  }
  
  if(ypr[1] > angleThreshold) {
    Keyboard.press( KEY_RIGHT_ARROW );
  }
  else {
    Keyboard.release( KEY_RIGHT_ARROW );
  }
  
  if(ypr[2] < -angleThreshold) {
    Keyboard.press( KEY_UP_ARROW );
  }
  else {
    Keyboard.release( KEY_UP_ARROW );
  }
  
  if(ypr[2] > angleThreshold) {
    Keyboard.press( KEY_DOWN_ARROW );
  }
  else {
    Keyboard.release( KEY_UP_ARROW );
  }
  
}



