#include "AP_Math_freeimu.h"
#include "Butter.h"    // Butterworth filter
#include "iCompass.h"

//#define DEBUG

#include "FreeIMU.h"
#include "DCM.h"
#include "DebugUtils.h"
#include "CommunicationUtils.h"
#include "FilteringScheme.h"
#include "RunningAverage.h"
#include "MovingAvarageFilter.h"

/**
 * FreeIMU library serial communication protocol
*/
#include <HMC58X3.h>
#include <BMP085.h>
#include <I2Cdev.h>
#include <MPU60X0.h>

#include <Wire.h>
#include <SPI.h>

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
