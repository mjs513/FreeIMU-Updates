/**
 * FreeIMU library serial communication protocol
*/
//These are optional depending on your IMU configuration

#include <ADXL345.h>
#include <HMC58X3.h>
#include <LSM303.h>
#include <ITG3200.h>
#include <bma180.h>
#include <MS561101BA.h> //Comment out for APM 2.5
#include <BMP085.h>
#include <I2Cdev.h>
#include <MPU60X0.h>
#include <AK8975.h>
#include <AK8963.h>
#include <L3G.h>
#include <LPS331.h> 
#include <SFE_LSM9DS0.h>
#include <BaroSensor.h>
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

float output[6];
float val[12];
float ypr[3];
float q[4];

// Set the default object
FreeIMU my3IMU = FreeIMU();

void setup() {
  Serial.begin(115200);
  Wire.begin();

  my3IMU.init(true); // the parameter enable or disable fast mode
  delay(500);
  
}

void loop() {
    my3IMU.getQ(q, val);   // just calling this will get you q, calibrated sensor values, heading
                           // and altitude.
    my3IMU.dcm.getEulerDeg(ypr);  // obviously gets you the Euler Angles.
    for(int i = 0; i < 3; i++) {
      Serial.print(ypr[i]); Serial.print(", ");
    }
    Serial.println();
}
