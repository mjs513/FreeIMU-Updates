
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

int raw_values[11];
char str[512];
float val[9], q[4];

unsigned long tap_window_start, time_in_window;
bool tap_in_window = 0;


// values here are purely empirical
const int tap_threshold = 12000;
const long tap_duration = 19375;


// Set the default object
FreeIMU my3IMU = FreeIMU();


void setup() { 
  Serial.begin(115200);
  Wire.begin();
  
  delay(10);
  my3IMU.init(true); // the parameter enable or disable fast mode
  my3IMU.accgyro.setFullScaleAccelRange(1); // set accelerometer to 4g range
  delay(10);
  
  Serial.println("---\n---\n---\nTap detection starts!");
}


void loop() {
  my3IMU.getRawValues(raw_values);
  
  int a_x = raw_values[0];
  
  if(a_x > tap_threshold && !tap_in_window) {
    tap_window_start = micros();
    tap_in_window = true;
  }
  
  if(tap_in_window) {
    time_in_window = micros() - tap_window_start;
    
    if(a_x < tap_threshold && time_in_window < tap_duration) {
      sprintf(str, "TAP! time: %dus\n", time_in_window);
      Serial.print(str);
      tap_in_window = false;
    }
    if(time_in_window > tap_duration) { // time exceded
      tap_in_window = false;
    }
  }
  
}

