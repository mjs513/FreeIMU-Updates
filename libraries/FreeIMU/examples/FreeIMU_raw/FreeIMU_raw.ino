#include <AP_Math_freeimu.h>
#include <Butter.h>    // Butterworth filter
#include <iCompass.h>

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
#include <AP_Baro_MS5611.h>

#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>

//#define DEBUG
#include "DebugUtils.h"
#include "CommunicationUtils.h"
#include "FreeIMU.h"
#include "DCM.h"
#include "FilteringScheme.h"
#include "RunningAverage.h"
#include <MovingAvarageFilter.h>

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
  // without baro
  sprintf(str, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t", raw_values[0], raw_values[1], raw_values[2], raw_values[3], raw_values[4], raw_values[5], raw_values[6], raw_values[7], raw_values[8]);
  Serial.print(str);
  #if HAS_PRESS()
     Serial.print(my3IMU.getBaroTemperature()); Serial.print(",");
     Serial.print(my3IMU.getBaroPressure()); Serial.print(",");
  #endif 
  
  Serial.print('\n');
  
}
