/*
   Copyright (c) 2015 Intel Corporation.  All rights reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

/*
   This sketch example demonstrates how the BMI160 accelerometer on the
   Intel(R) Curie(TM) module can be used as a Step Counter (pedometer)
*/

#include "CurieImu.h"

/* To get an interrupt notification for every step detected,
    set stepEventsEnabeled to true. Otherwise, the main loop will
    poll for the current step count.

   By design, the step counter does not immediately update on every step detected.
   Please refer to Section 2.7 of the BMI160 IMU SensorData Sheet
   for more information on this feature.
*/
const int ledPin = 13;

boolean stepEventsEnabeled = true;   // whether you're polling or using events
long lastStepCount = 0;              // step count on previous polling check
boolean blinkState = false;          // state of the LED

void setup() {
  Serial.begin(9600);
  // pinMode(13, OUTPUT);
  // intialize the sensor:
  CurieImu.initialize();
  // turn on step detection mode:
  CurieImu.setStepDetectionMode(BMI160_STEP_MODE_NORMAL);
  // enable step counting:
  CurieImu.setStepCountEnabled(true);

  if (stepEventsEnabeled) {
    // attach the eventCallback function as the
    // step event handler:
    CurieImu.attachInterrupt(eventCallback);
    CurieImu.setIntStepEnabled(true);        // turn on step detection
    CurieImu.setIntEnabled(true);            // enable interrupts

    Serial.println("IMU initialisation complete, waiting for events...");
  }
}

void loop() {
  /* Instead of using step detection event notifications,
     we can check the step count periodically */
  if (!stepEventsEnabeled) {
    updateStepCount();
  }
  digitalWrite(13, blinkState);
  blinkState = !blinkState;
  delay(1000);
}

static void updateStepCount() {
  // get the step count:
  int stepCount = CurieImu.getStepCount();
  
  // if the step count has changed, print it:
  if (stepCount != lastStepCount) {
    Serial.print("Step count: "); 
    Serial.println(stepCount);
    // save the current count for comparison next check:
    lastStepCount = stepCount;
  }
}

static void eventCallback(void) {
  if (CurieImu.getIntStepStatus())
    updateStepCount();
}
