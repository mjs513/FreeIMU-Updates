#
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
#include <AP_Baro_MS5611.h>
#include "RunningAverage.h"
#include <iCompass.h>

#include <AP_Math_freeimu.h>
#include <Filter.h>    // Filter library
#include <Butter.h>    // Butterworth filter

#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>

//#define DEBUG
#include "DebugUtils.h"
#include "CommunicationUtils.h"
#include "FreeIMU.h"
#include "FilteringScheme.h"


float q[4], val[11];

// Set the FreeIMU object
FreeIMU my3IMU = FreeIMU();

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  delay(5);
  my3IMU.init();
  delay(5);
}


void loop() { 
  my3IMU.getQ(q,val);
  serialPrintFloatArr(q, 4);
  Serial.println("");
  delay(20);
}
