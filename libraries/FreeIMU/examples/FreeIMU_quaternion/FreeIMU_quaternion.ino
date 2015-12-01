#/**
 * FreeIMU library serial communication protocol
*/
//These are optional depending on your IMU configuration

#include <ADXL345.h>
#include <HMC58X3.h>
#include <LSM303.h>
#include <LPS.h> 
#include <ITG3200.h> //note LPS library must come before ITG lib
#include <bma180.h>
#include <MS561101BA.h> //Comment out for APM 2.5
#include <BMP085.h>
#include <I2Cdev.h>
#include <MPU60X0.h>
#include <AK8975.h>
#include <AK8963.h>
#include <L3G.h>
#include <SFE_LSM9DS0.h>
#include <BaroSensor.h> // MS5637-1 pressure sensor
//#include <AP_Baro_MS5611.h>  //Uncomment for APM2.5


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

float q[4], val[11];

// Set the FreeIMU object
FreeIMU my3IMU = FreeIMU();

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  delay(500);
  my3IMU.init();
  delay(500);
}


void loop() { 
  my3IMU.getQ(q,val);
  serialPrintFloatArr(q, 4);
  Serial.println("");
  delay(20);
}
