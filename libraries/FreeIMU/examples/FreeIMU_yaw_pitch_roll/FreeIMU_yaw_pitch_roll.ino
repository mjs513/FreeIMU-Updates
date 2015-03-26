#include <AP_Math_freeimu.h>
#include <Butter.h>    // Butterworth filter
#include <iCompass.h>

/**
 * FreeIMU library serial communication protocol
*/

#include <ADXL345.h>
#include <HMC58X3.h>
#include <LSM303.h>
#include <ITG3200.h>
#include <bma180.h>
#include <MS561101BA.h>
#include <BMP085.h>
#include <I2Cdev.h>
#include <MPU60X0.h>
#include <AK8975.h>
#include <AK8963.h>
#include <L3G.h>
#include <LPS331.h> 
//#include <AP_Baro_MS5611.h>

#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>

//#define DEBUG
#include "DebugUtils.h"
#include "CommunicationUtils.h"
#include "FreeIMU.h"
#include "DCM.h"
#include "FilteringScheme.h"
#include "RunningAverage.h"
#include <MovingAvarageFilter.h>

int raw_values[11];
//char str[512];
float ypr[3]; // yaw pitch roll
float val[9];

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