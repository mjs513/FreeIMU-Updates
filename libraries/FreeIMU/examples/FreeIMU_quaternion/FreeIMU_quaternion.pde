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
#include "CommunicationUtils.h"
#include "FreeIMU.h"
#include <Wire.h>
#include <SPI.h>


float q[4];

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
  my3IMU.getQ(q);
  serialPrintFloatArr(q, 4);
  Serial.println("");
  delay(20);
}

