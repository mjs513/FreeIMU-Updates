#/**
 * FreeIMU library serial communication protocol
*/
//These are optional depending on your IMU configuration


#include <SFE_LSM9DS0.h>
//#include <AP_Baro_MS5611.h>  //Uncomment for APM2.5
#include <BaroSensor.h>

//These are mandatory
#include <AP_Math_freeimu.h>
#include <Butter.h>    // Butterworth filter
#include <iCompass.h>
#include <MovingAvarageFilter.h>

#include <Wire.h>
#include <SPI.h>

#if defined(__AVR__)
  #include <EEPROM.h>
#endif

//#define DEBUG
#include "DebugUtils.h"
#include "CommunicationUtils.h"
#include "FreeIMU.h"
#include "DCM.h"
#include "FilteringScheme.h"
#include "RunningAverage.h"

float euler[3];

// Set the FreeIMU object
FreeIMU my3IMU = FreeIMU();

void setup() {
  Serial.begin(57600);
  Wire.begin();
  
  my3IMU.init(true);
  delay(500);
  my3IMU.initGyros();
}


void loop() { 
  //my3IMU.getQ(q,val);
  my3IMU.getEuler360deg(euler);


  Serial.print(F("Orientation: "));
  Serial.print(euler[0]);
  Serial.print(F(" "));
  Serial.print(euler[1]);
  Serial.print(F(" "));
  Serial.print(euler[2]);
  Serial.println(F(""));
  
  delay(10);
}
