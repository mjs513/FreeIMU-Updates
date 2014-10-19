/*
 MPL3115A2 Barometric Pressure Sensor Library Example Code
 By: Nathan Seidle
 SparkFun Electronics
 Date: September 24th, 2013
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This example coverts pressure from Pascals to inches of mercury, altimeter setting adjusted.
 This type of pressure reading is used in the USA on Wunderground. I find it SUPER weird, but it
 is needed for home weather stations and aircraft.
 
 You must set the correct altitude (station_elevation_m) below. Find this altitude using
 a GPS unit such as your cell phone. Be sure to put the elevation into meters (1meter = 0.3048ft).
 
 Hardware Connections (Breakoutboard to Arduino):
 -VCC = 3.3V
 -SDA = A4 (use inline 10k resistor if your board is 5V)
 -SCL = A5 (use inline 10k resistor if your board is 5V)
 -INT pins can be left unconnected for this demo
 
 During testing, GPS with 9 satellites reported 5393ft, sensor reported 5360ft (delta of 33ft). Very close!
 
*/

#include <Wire.h>
#include "MPL3115A2.h"

//Create an instance of the object
MPL3115A2 myPressure;

void setup()
{
  Wire.begin();        // Join i2c bus
  Serial.begin(9600);  // Start serial for output

  myPressure.begin(); // Get sensor online

  //Configure the sensor
  //myPressure.setModeAltimeter(); // Measure altitude above sea level in meters
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  
  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 
}

void loop()
{
  float pressure = myPressure.readPressure();
  Serial.print("Pressure(Pa):");
  Serial.print(pressure, 2);

  float temperature = myPressure.readTempF();
  Serial.print(" Temp(f):");
  Serial.print(temperature, 2);

  //References: 
  //Definition of "altimeter setting": http://www.crh.noaa.gov/bou/awebphp/definitions_pressure.php
  //Altimeter setting: http://www.srh.noaa.gov/epz/?n=wxcalc_altimetersetting
  //Altimeter setting: http://www.srh.noaa.gov/images/epz/wxcalc/altimeterSetting.pdf
  //Verified against Boulder, CO readings: http://www.crh.noaa.gov/bou/include/webpres.php?product=webpres.txt
  
  //const int station_elevation_ft = 5374; //Must be obtained with a GPS unit
  //float station_elevation_m = station_elevation_ft * 0.3048; //I'm going to hard code this
  const int station_elevation_m = 1638; //Accurate for the roof on my house
  //1 pascal = 0.01 millibars
  pressure /= 100; //pressure is now in millibars

  float part1 = pressure - 0.3; //Part 1 of formula
  
  const float part2 = 8.42288 / 100000.0;
  float part3 = pow((pressure - 0.3), 0.190284);
  float part4 = (float)station_elevation_m / part3;
  float part5 = (1.0 + (part2 * part4));
  float part6 = pow(part5, (1.0/0.190284));
  float altimeter_setting_pressure_mb = part1 * part6; //Output is now in adjusted millibars
  float baroin = altimeter_setting_pressure_mb * 0.02953;

  Serial.print(" Altimeter setting InHg:");
  Serial.print(baroin, 2);

  Serial.println();
}
