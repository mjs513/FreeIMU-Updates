/**
 * Streams calibrated sensors readings.
 *
 * @author Fabio Varesano - fvaresano at yahoo dot it
*/ 

/*
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

#include <AP_Math_freeimu.h>
#include <Butter.h>    // Butterworth filter
#include <iCompass.h>

#include <ADXL345.h>
#include <HMC58X3.h>
#include <LSM303.h>
#include <ITG3200.h>
#include <bma180.h>
#include <MS561101BA.h>
#include <BMP085.h>
#include <I2Cdev.h>
#include <MPU60X0.h>
#include <AK8975.h>
#include <AK8963.h>
#include <L3G.h>
#include <LPS331.h> 
#include <AP_Baro_MS5611.h>

#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>

//#define DEBUG
#include "DebugUtils.h"
#include "CommunicationUtils.h"
#include "FreeIMU.h"
#include "DCM.h"
#include "FilteringScheme.h"
#include "RunningAverage.h"
#include <MovingAvarageFilter.h>

float val[11];


// Set the default object
FreeIMU my3IMU = FreeIMU();

void setup() { 
  Serial.begin(115200);
  Wire.begin();
  
  delay(500);
  my3IMU.init(true); // the parameter enable or disable fast mode
  delay(500);
}

void loop() {
  my3IMU.getValues(val);
  for(int i=0; i<9; i++) {
    Serial.print(val[i], 4);
    Serial.print('\t');
  }
  #if HAS_PRESS()
    // with baro - pressure temp
    Serial.print(my3IMU.getBaroTemperature()); Serial.print(",");
    Serial.print('\t');
    Serial.print(my3IMU.getBaroPressure()); Serial.print(",");
  #endif
  Serial.print('\n');
  
  /*
  my3IMU.getValues(val);
  sprintf(str, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", int(val[0]), int(val[1]), int(val[2]), int(val[3]), int(val[4]), int(val[5]), int(val[6]), int(val[7]), int(val[8]));  
  Serial.print(str);
  Serial.print(10, BYTE);
  */
}