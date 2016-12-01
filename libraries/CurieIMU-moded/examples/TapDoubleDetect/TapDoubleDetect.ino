/*
   Copyright (c) 2016 Intel Corporation.  All rights reserved.
   See the bottom of this file for license terms.
*/

/*
   This sketch example demonstrates how the BMI160 accelerometer on the
   Intel(R) Curie(TM) module can be used to detect tap events
*/

#include "CurieIMU.h"

void setup() {
  Serial.begin(9600); // initialize Serial communication
  while(!Serial) ;    // wait for serial port to connect.
  // Initialise the IMU
  CurieIMU.begin();
  CurieIMU.attachInterrupt(eventCallback);

  // Increase Accelerometer range to allow detection of stronger taps (< 4g)
  CurieIMU.setAccelerometerRange(4);

  // Reduce threshold to allow detection of weaker taps (>= 750mg)
  CurieIMU.setDetectionThreshold(CURIE_IMU_DOUBLE_TAP, 750); // (750mg)

  // Set the quite time window for 2 taps to be registered as a double-tap (Gap time between taps <= 1000 milliseconds)
  CurieIMU.setDetectionDuration(CURIE_IMU_DOUBLE_TAP, 1000);

  // Enable Double-Tap detection
  CurieIMU.interrupts(CURIE_IMU_DOUBLE_TAP);

  Serial.println("IMU initialization complete, waiting for events...");
}

void loop() {
  // nothing happens in the loop because all the action happens
  // in the callback function.
}

static void eventCallback()
{
  if (CurieIMU.getInterruptStatus(CURIE_IMU_DOUBLE_TAP)) {
    if (CurieIMU.tapDetected(X_AXIS, NEGATIVE))
      Serial.println("Double Tap detected on negative X-axis");
    if (CurieIMU.tapDetected(X_AXIS, POSITIVE))
      Serial.println("Double Tap detected on positive X-axis");
    if (CurieIMU.tapDetected(Y_AXIS, NEGATIVE))
      Serial.println("Double Tap detected on negative Y-axis");
    if (CurieIMU.tapDetected(Y_AXIS, POSITIVE))
      Serial.println("Double Tap detected on positive Y-axis");
    if (CurieIMU.tapDetected(Z_AXIS, NEGATIVE))
      Serial.println("Double Tap detected on negative Z-axis");
    if (CurieIMU.tapDetected(Z_AXIS, POSITIVE))
      Serial.println("Double Tap detected on positive Z-axis");
  }
}

/*
   Copyright (c) 2016 Intel Corporation.  All rights reserved.

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
