/**
 * FreeIMU library serial communication protocol
*/

#include <Wire.h>
#include <SPI.h>

//These are optional depending on your IMU configuration
#include "CurieImu.h"
#include <HMC58X3.h>
#include <MS561101BA.h> //Comment out for APM 2.5

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

int raw_values[11];
//char str[512];
float ypr[3]; // yaw pitch roll
float val[13];

// Set the FreeIMU object
FreeIMU my3IMU = FreeIMU();

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  delay(5);
  my3IMU.init(); // the parameter enable or disable fast mode
  delay(5);
}

void loop() { 
  
  my3IMU.getYawPitchRoll(ypr);
  Serial.print("Yaw: ");
  Serial.print(ypr[0]);
  Serial.print(" Pitch: ");
  Serial.print(ypr[1]);
  Serial.print(" Roll: ");
  Serial.print(ypr[2]);
  Serial.println("");
  
  delay(10);
}