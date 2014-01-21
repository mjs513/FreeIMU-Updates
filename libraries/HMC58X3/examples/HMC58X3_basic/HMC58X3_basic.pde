/*
HMC5843_basic.pde - Basic reading example for the HMC5843 library
Copyright (C) 2011 Fabio Varesano <fabio at varesano dot net>


This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <Wire.h>
//#include <DebugUtils.h>
#include <HMC58X3.h>

HMC58X3 magn;

void setup(void) {
  Serial.begin(9600);
  Wire.begin();
  
  // no delay needed as we have already a delay(5) in HMC5843::init()
  magn.init(false); // Dont set mode yet, we'll do that later on.
  // Calibrate HMC using self test, not recommended to change the gain after calibration.
  magn.calibrate(1, 32); // Use gain 1=default, valid 0-7, 7 not recommended.
  // Single mode conversion was used in calibration, now set continuous mode
  magn.setMode(0);
}

void loop() { 
  int ix,iy,iz;
  float fx,fy,fz;
  delay(10);
  // Get values, as ints and floats.
  magn.getValues(&ix,&iy,&iz);
  magn.getValues(&fx,&fy,&fz);
  // also available HMC5843::getValues(float *xyz) you can pass to it an array of 3 floats
  
  // Print int values
  Serial.print("Ints x:");
  Serial.print(ix);
  Serial.print(",");
  Serial.print(iy);
  Serial.print(",");
  Serial.print(iz);
  Serial.print(",");
  
  // Print float values
  Serial.print(" Floats x:");
  Serial.print(fx);
  Serial.print(" y:");
  Serial.print(fy);
  Serial.print(" z:");
  Serial.print(fz);
  
  // a simple heading, assuming it's close to horizontal. See:
  // M.J. Caruso. Applications of magnetic sensors for low cost compass systems. 
  // In Position Location and Navigation Symposium, IEEE, 2000. Available from:
  // http://hnc.ru/lib/a%26c%20%28automatic%20%26%20controls%29/sensors/DataSheet/Magnit/Honeywell/lowcost.pdf

  Serial.print(" Heading: ");
  float heading = atan2(fy, fx);
  if(heading < 0) {
    heading += 2 * M_PI;
  }
  Serial.println(heading * 180/M_PI); 
  // x and y axis are swapped above with respect to the above paper as our Z axis points to the sky while in the paper it points to the bottom
}