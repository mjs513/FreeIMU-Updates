#include "Wire.h"
#include "ADXL345.h"

ADXL345 accel;

void setup(void){
  int x, y, z, i;
  float xyz[3];

  Serial.begin(9600);
  accel.init(ADXL345_ADDR_ALT_LOW);
  accel.powerOn();
  Serial.println("Arduino driver for ADXL345 3-axes accelerometer");
  Serial.println("Version 2.0");
  Serial.println("");
  Serial.println("This program is free software; you can redistribute it and/or modify");
  Serial.println("it under the terms of the GNU License V2.");
  Serial.println("This program is distributed in the hope that it will be useful,");
  Serial.println("but WITHOUT ANY WARRANTY; without even the implied warranty of");
  Serial.println("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the ");
  Serial.println("GNU General Public License, version 2 for more details");

  Serial.println("");
  Serial.println("Original development: Kevin Stevenard");
  Serial.println("Modified by Justin Shaw May 2010");
  Serial.println("");
  Serial.println("");
  
  accel.get_Gxyz(xyz);
  Serial.print("XYZ Gs: ");
  for(i = 0; i<3; i++){
    Serial.print(xyz[i], DEC);
    Serial.print(" ");
  }
  Serial.println("");

  accel.setTapThreshold(1);
  Serial.print("getTapThreshold(): ");
  Serial.println(accel.getTapThreshold(), DEC);
  
  accel.setAxisOffset(2, 3, 4);
  Serial.print("getAxisOffset(&x, &y, &z): ");
  accel.getAxisOffset(&x, &y, &z);
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(z);
  Serial.println("");

  accel.setTapDuration(5);
  Serial.print("getTapDuration(): ");
  Serial.println(accel.getTapDuration(), DEC);

  accel.setDoubleTapLatency(6);
  Serial.print("getDoubleTapLatency(): ");
  Serial.println(accel.getDoubleTapLatency(), DEC);

  accel.setDoubleTapWindow(7);
  Serial.print("getDoubleTapWindow() ");
  Serial.println(accel.getDoubleTapWindow());

  accel.setActivityThreshold(8);
  Serial.print("getActivityThreshold() ");
  Serial.println(accel.getActivityThreshold(), DEC);

  accel.setInactivityThreshold(9);
  Serial.print("getInactivityThreshold() ");
  Serial.println(accel.getInactivityThreshold(), DEC);

  accel.setTimeInactivity(10);
  Serial.print("getTimeInactivity(): ");
  Serial.println(accel.getTimeInactivity());

  accel.setFreeFallThreshold(11);
  Serial.print("getFreeFallThreshold(): ");
  Serial.println(accel.getFreeFallThreshold());

  accel.setFreeFallDuration(12);
  Serial.print("getFreeFallDuration(): ");
  Serial.println(accel.getFreeFallDuration(), DEC);

  Serial.print("isActivityXEnabled(): ");
  Serial.println(accel.isActivityXEnabled(), DEC);

  Serial.print("isActivityYEnabled(): ");
  Serial.println(accel.isActivityYEnabled(), DEC);

  Serial.print("isActivityZEnabled(): ");
  Serial.println(accel.isActivityZEnabled(), DEC);

  Serial.print("isInactivityXEnabled(): ");
  Serial.println(accel.isInactivityXEnabled(), DEC);

  Serial.print("isInactivityYEnabled(): ");
  Serial.println(accel.isInactivityYEnabled(), DEC);

  Serial.print("isInactivityZEnabled(): ");
  Serial.println(accel.isInactivityZEnabled(), DEC);

  Serial.print("isActivityAc(): ");
  Serial.println(accel.isInactivityAc(), DEC);

  accel.setActivityAc(true);
  accel.setInactivityAc(true);

  accel.setSuppressBit(true);
  Serial.print("getSuppressBit(); true? ");
  Serial.println(accel.getSuppressBit());

  accel.setSuppressBit(false);
  Serial.print("getSuppressBit(); false? ");
  Serial.println(accel.getSuppressBit());
  
  accel.setTapDetectionOnX(true);
  Serial.print("isTapDetectionOnX(); true? ");
  Serial.println(accel.isTapDetectionOnX(), DEC);
  accel.setTapDetectionOnX(false);
  Serial.print("isTapDetectionOnX(); false? ");
  Serial.println(accel.isTapDetectionOnX(), DEC);

  accel.setTapDetectionOnY(true);
  Serial.print("isTapDetectionOnY(); true? ");
  Serial.println(accel.isTapDetectionOnY(), DEC);
  accel.setTapDetectionOnY(false);
  Serial.print("isTapDetectionOnY(); false? ");
  Serial.println(accel.isTapDetectionOnY(), DEC);

  accel.setTapDetectionOnZ(true);
  Serial.print("isTapDetectionOnZ(); true? ");
  Serial.println(accel.isTapDetectionOnZ(), DEC);
  accel.setTapDetectionOnZ(false);
  Serial.print("isTapDetectionOnZ(); false? ");
  Serial.println(accel.isTapDetectionOnZ(), DEC);

  accel.setActivityX(true);
  accel.setActivityY(true);
  accel.setActivityZ(true);

  accel.setInactivityX(false);
  accel.setInactivityY(false);
  accel.setInactivityZ(false);

  Serial.print("isActivitySourceOnX(): ");
  Serial.println(accel.isActivitySourceOnX(), DEC);

  Serial.print("accel.isActivitySourceOnY(): ");
  Serial.println(accel.isActivitySourceOnY(), DEC);

  Serial.print("accel.isActivitySourceOnZ(): ");
  Serial.println(accel.isActivitySourceOnZ(), DEC);

  Serial.print("accel.isTapSourceOnX(): ");
  Serial.println(accel.isTapSourceOnX(), DEC);

  Serial.print("accel.isTapSourceOnY(): ");
  Serial.println(accel.isTapSourceOnY(), DEC);

  Serial.print("accel.isTapSourceOnZ(): ");
  Serial.println(accel.isTapSourceOnZ(), DEC);

  Serial.print("accel.isAsleep(): ");
  Serial.println(accel.isAsleep(), DEC);

  Serial.print("accel.isLowPower(): ");
  Serial.println(accel.isLowPower(), DEC);

  accel.setLowPower(false);
  
  accel.setRate(3.14159);
  Serial.print("getRate(): 3.14159?");
  Serial.println(accel.getRate());

  Serial.print("getInterruptSource(): ");
  Serial.println(accel.getInterruptSource(), DEC);

  Serial.print("getInterruptSource(1): ");
  Serial.println(accel.getInterruptSource(1), DEC);
  
  Serial.print("getInterruptMapping(1): ");
  Serial.println(accel.getInterruptMapping(1), DEC);

  accel.setInterruptMapping(1, true);
  Serial.print("isInterruptEnabled(1): ");
  Serial.println(accel.isInterruptEnabled(1));
  
  accel.setInterrupt(1, true);

  accel.setSelfTestBit(false);
  Serial.print("getSelfTestBit(): ");
  Serial.println(accel.getSelfTestBit(), DEC);

  accel.printAllRegister();
}
void loop(void){
}
