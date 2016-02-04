/*
 * Copyright (c) 2015 Intel Corporation.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/*
 * This sketch example demonstrates how the BMI160 accelerometer on the
 * Intel(R) Curie(TM) module can be used to detect tap events
 */

#include "CurieImu.h"

void setup() {
    Serial.begin(9600);

    // Initialise the IMU
    CurieImu.initialize();
    CurieImu.attachInterrupt(eventCallback);

    // Increase Accelerometer range to allow detection of stronger taps (< 4g)
    CurieImu.setFullScaleAccelRange(BMI160_ACCEL_RANGE_4G);

    // Reduce threshold to allow detection of weaker taps (>= 750mg)
    CurieImu.setTapDetectionThreshold(6); // (6 x 125mg)

    // Set the time window for 2 taps to be registered as a double-tap (<= 250 milliseconds)
    CurieImu.setDoubleTapDetectionDuration(BMI160_DOUBLE_TAP_DURATION_250MS);

    // Enable Double-Tap detection
    CurieImu.setIntDoubleTapEnabled(true);

    // Enable Interrupts Notifications
    CurieImu.setIntEnabled(true);

    Serial.println("IMU initialisation complete, waiting for events...");
}

void loop() {
  // nothing happens in the loop because all the action happens
  // in the callback function. 
}

static void eventCallback()
{
    if (CurieImu.getIntDoubleTapStatus()) {
     if (CurieImu.getXNegTapDetected())
        Serial.println("Double Tap detected on negative X-axis");
     if (CurieImu.getXPosTapDetected())
        Serial.println("Double Tap detected on positive X-axis");
     if (CurieImu.getYNegTapDetected())
        Serial.println("Double Tap detected on negative Y-axis");
     if (CurieImu.getYPosTapDetected())
        Serial.println("Double Tap detected on positive Y-axis");
     if (CurieImu.getZNegTapDetected())
        Serial.println("Double Tap detected on negative Z-axis");
     if (CurieImu.getZPosTapDetected())
        Serial.println("Double Tap detected on positive Z-axis");
  }
}
