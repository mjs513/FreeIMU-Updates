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
#include <MovingAvarageFilter.h>
#include "RunningAverage.h"
#include <Wire.h>
#include <SPI.h>

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

