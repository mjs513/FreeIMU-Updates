#include <ADXL345.h>
#include <bma180.h>
#include <HMC58X3.h>
#include <ITG3200.h>
#include <MS561101BA.h>
#include <I2Cdev.h>
#include <MPU60X0.h>
#include <EEPROM.h>

//#define DEBUG
#include "DebugUtils.h"
#include "FreeIMU.h"
#include <Wire.h>
#include <SPI.h>

int raw_values[11];
char str[512];
float val[9];


// Set the default object
FreeIMU my3IMU = FreeIMU();

void setup() { 
  Serial.begin(115200);
  Wire.begin();
  
  delay(500);
  my3IMU.init(true); // the parameter enable or disable fast mode
  delay(500);
}

void loop() {
  my3IMU.getRawValues(raw_values);
  #if HAS_MS5611() 
    // with baro
    sprintf(str, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", raw_values[0], raw_values[1], raw_values[2], raw_values[3], raw_values[4], raw_values[5], raw_values[6], raw_values[7], raw_values[8], raw_values[9], raw_values[10]);
  #else  
    // without baro
    sprintf(str, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t", raw_values[0], raw_values[1], raw_values[2], raw_values[3], raw_values[4], raw_values[5], raw_values[6], raw_values[7], raw_values[8]);
  #endif
  Serial.print(str);
  Serial.print('\n');
  
  /*
  my3IMU.getValues(val);
  sprintf(str, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", int(val[0]), int(val[1]), int(val[2]), int(val[3]), int(val[4]), int(val[5]), int(val[6]), int(val[7]), int(val[8]));  
  Serial.print(str);
  Serial.print(10, BYTE);
  */
}

