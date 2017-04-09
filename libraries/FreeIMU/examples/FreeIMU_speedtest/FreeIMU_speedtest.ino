/**
 * FreeIMU library serial communication protocol
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

int16_t raw_values[11];
char str[512];
float val[13], q[4];

unsigned long start, stop;

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
  Serial.println("Testing raw reading speed (average on 1024 samples):");
  start = micros();
  for(int i=0; i<1024; i++) {
    my3IMU.getRawValues(raw_values);
  }
  stop = micros();
  Serial.print("--> result: ");
  Serial.print((stop - start) / 1024);
  Serial.print(" microseconds .... ");
  Serial.print(((stop - start) / 1024) / 1000);
  Serial.println(" milliseconds");
  
  
  Serial.println("Testing calibrated reading speed (average on 1024 samples):");
  start = micros();
  for(int i=0; i<1024; i++) {
    my3IMU.getValues(val);
  }
  stop = micros();
  Serial.print("--> result: ");
  Serial.print((stop - start) / 1024);
  Serial.print(" microseconds .... ");
  Serial.print(((stop - start) / 1024) / 1000);
  Serial.println(" milliseconds");
  
  
  Serial.println("Testing sensor fusion speed (average on 1024 samples):");
  start = micros();
  for(int i=0; i<1024; i++) {
    my3IMU.getQ(q,val);
  }
  stop = micros();
  Serial.print("--> result: ");
  Serial.print((stop - start) / 1024);
  Serial.print(" microseconds .... ");
  Serial.print(((stop - start) / 1024) / 1000);
  Serial.println(" milliseconds");
  
  
  Serial.println("Looping again..");
  Serial.println("----");
}