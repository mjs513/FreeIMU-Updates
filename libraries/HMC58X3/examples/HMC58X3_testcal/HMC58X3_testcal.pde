/*
HMC58X3_testcal.pde - Testing calibration
Outputs raw data and offsetted data after calibration

Copyright (C) 2011 Fabio Varesano <fvaresano@yahoo.it>

Development of this code has been supported by the Department of Computer Science,
Universita' degli Studi di Torino, Italy within the Piemonte Project
http://www.piemonte.di.unito.it/


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
#include <HMC58X3.h>

int x, y, z;
HMC58X3 magn;

void setup(void) {
  Serial.begin(9600);
  Wire.begin();
  
  // no delay needed as we have already a delay(5) in HMC58X3::init()
  magn.init(true); // Dont set mode yet, we'll do that later on.
  magn.calibrate(1, 64);
  magn.setMode(0);
}

void loop() {
  delay(10);
  // Get values, as ints and floats.
  magn.getValues(&x,&y,&z);
  
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.print(z);
  Serial.println(",");
}



