/*
HMC58X3_basic.pde - Basic reading example for the HMC58X3 library
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

// Uncomment the following line if you are using the HMC5843

#include <Wire.h>
#include <HMC58X3.h>

HMC58X3 magn;

void setup(void) {
  Serial.begin(9600);
  Wire.begin();
  
  // no delay needed as we have already a delay(5) in HMC58X3::init()
  magn.init(true); // Dont set mode yet, we'll do that later on.
}

void loop() { 
  int ix,iy,iz;
  delay(10);
  // Get values, as ints and floats.
  magn.getRaw(&ix,&iy,&iz);
  
  Serial.print(ix);
  Serial.print(",");
  Serial.print(iy);
  Serial.print(",");
  Serial.print(iz);
  Serial.println(",");
}