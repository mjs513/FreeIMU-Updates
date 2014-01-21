// BMP085_test1
// by Filipe Vieira
// Simple test of BMP085 output using default settings.
// This example requires AUTO_UPDATE_TEMPERATURE to be true in bmp085.h otherwise temperature will not update.
// IMPORTANT!! To get correct values you MUST CHANGE init() parameters, in 
// this example I've set 250m based on GPS data for my location.

#include <Wire.h>
#include <BMP085.h>

BMP085 dps = BMP085();      // Digital Pressure Sensor 

long Temperature = 0, Pressure = 0, Altitude = 0;

void setup(void) {
  Serial.begin(9600);
  Wire.begin();
  delay(1000);
  
  // uncomment for different initialization settings
  //dps.init();     // QFE (Field Elevation above ground level) is set to 0 meters.
                  // same as init(MODE_STANDARD, 0, true);
  
  //dps.init(MODE_STANDARD, 101850, false);  // 101850Pa = 1018.50hPa, false = using Pa units
                  // this initialization is useful for normalizing pressure to specific datum.
                  // OR setting current local hPa information from a weather station/local airport (QNH).
  
  dps.init(MODE_ULTRA_HIGHRES, 25000, true);  // 250 meters, true = using meter units
                  // this initialization is useful if current altitude is known,
                  // pressure will be calculated based on TruePressure and known altitude.

  // note: use zeroCal only after initialization.
  // dps.zeroCal(101800, 0);    // set zero point
}            

void loop(void) { 
  dps.getPressure(&Pressure);
  dps.getAltitude(&Altitude);
  
  Serial.print("  Alt(cm):");
  Serial.print(Altitude);
  Serial.print("  Pressure(Pa):");
  Serial.println(Pressure);
}