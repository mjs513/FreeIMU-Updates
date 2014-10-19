/*
 MPL3115A2 Barometric Pressure Sensor - Calibration Code
 by https://github.com/mariocannistra
 
 i've put together this code to obtain more precise values 
 from the sensor   --> when using it for altitude measurements <--
 and to get more precise barometer indications

 How I glued things together:
 - slightly customize the code by http://www.henrylahr.com/?p=99
 - used the offset routines from Michael Lange on mbed.org
 - completed with elevation_offset calculation by digitalmisery comment at https://www.sparkfun.com/products/11084
 - then packaged the calibration function within the original 
   library code by Nathan Seidle - SparkFun Electronics

 About the following line, hope to meet Nathan someday for the beer :-) If I will meet all of them.. then also food :-)
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license). 
*/

#include <Wire.h>
#include "MPL3115A2.h"

//Create an instance of the object
MPL3115A2 baro3115;

#define ALTBASIS 332.0  // start altitude to calculate mean sea level pressure in meters
					    // got this averaging readouts from 3 different GPS

void setup()
{
	// give me time to open serial monitor:
	delay(3000);

	Wire.begin();        // Join i2c bus
	Serial.begin(57600);  // Start serial for output

	baro3115.begin(); // Get sensor online

	//Configure the sensor
	// Set the # of samples from 1 to 128. See datasheet.
	// Integer values between 0 < n < 7 give oversample ratios 2^n and 
	// sampling intervals of 0=6 ms , 1=10, 2=18, 3=34, 4=66, 5=130, 6=258, and 7=512
	baro3115.setOversampleRate(7); // Set Oversample to the recommended 128 --> 512ms
	baro3115.enableEventFlags(); // Enable all three pressure and temp event flags 

	Serial.println("BEFORE calibration...");
	// show sample data readouts before starting the calibration:
	outData();

	// set to zero all the sensor offsets and input values:
	baro3115.setOffsetTemperature(0);
	baro3115.setOffsetPressure(0);
	baro3115.setOffsetAltitude(0);
	baro3115.setBarometricInput(0.0);

	// let's have a look, just in case:
	Serial.println("OFFSETS:");
	Serial.print("  pressure: ");
	Serial.println(baro3115.offsetPressure() ,2);
	Serial.print("  altitude: ");
	Serial.println((float)baro3115.offsetAltitude() ,2);
	Serial.print("  temperature(C): ");
	Serial.println(baro3115.offsetTemperature() ,2);

	// now perform the calibration

	//calculate pressure for current ALTBASIS altitude by averaging a few readings
	Serial.println("Pressure calibration...");
	
	// this function calculates a new sea level pressure ref value
	// it will NOT change the sensor registers
	// see below setBarometricInput() where that value is actually set
	// in the registers. the sensor will start using it just after.
	baro3115.runCalibration(ALTBASIS);


	Serial.print("calculated sea level pressure: "); 
	Serial.print(baro3115.calculated_sea_level_press ,2); 
	Serial.println(" Pa");

	Serial.print("calculated elevation_offset: "); 
	Serial.print(baro3115.elevation_offset ,2); 
	Serial.println(" Pa");

	// i originally had 35 hPa error in pressure readouts
	// once i added the elevation_offset calculation, the error
	// decreased and is now within 4 hPa (ie: 1015.18 instead of official 1011.5)
	// (official = pro measurement at same elevation at 500 m from my laptop)

	// This configuration option calibrates the sensor according to
	// the sea level pressure for the measurement location (2 Pa per LSB)
	// The default value for "Barometric input for Altitude calculation" is 101,326 Pa
	
	baro3115.setBarometricInput( baro3115.calculated_sea_level_press );

	// calibration performed

	// add temperature offset for my tested sensor
	// seems the temperature probe is within the ADC and should not be used
	// to measure the environment. Will try adding the offset i got by comparison
	// with another thermometer
	// you can enable this if you need it:
	// baro3115.setOffsetTemperature((char) (0.65 / 0.0625) );
}

void outData()
{
	baro3115.setModeBarometer(); // Measure pressure in Pascals
	delay(520);
	// since we are using the calibration then we also have to add the
	// calculated elevation related pressure offset:
	float pressure = baro3115.readPressure() + baro3115.elevation_offset;
	Serial.print("Pressure(Pa): ");
	Serial.print(pressure, 2);

	// output is in Pa
	// 1 kPa = 10 hPa = 1000 Pa
	// 1 hPa = 100 Pascals = 1 mb
	pressure = (pressure / 100) ;		//  ... / 1000 * 10 ;
	Serial.print(" Pressure(hPa): ");
	Serial.print(pressure, 2);

	float temperature = baro3115.readTemp();
	Serial.print(" Temperature(C): ");
	Serial.print(temperature, 2);

	baro3115.setModeAltimeter(); // Measure altitude above sea level in meters
	delay(520);
	float altitude = baro3115.readAltitude();
	Serial.print(" Altitude above sea(m): ");
	Serial.println(altitude, 2);
}

void loop()
{
	outData();
}

// sample output:
//BEFORE calibration...
//Pressure(Pa): 1327.00 Pressure(hPa): 13.27 Temperature(C): 21.25 Altitude above sea(m): 332.25
//OFFSETS:
//  pressure: 0.00
//  altitude: 0.00
//  temperature(C): 0.00
//Pressure calibration...
//calculated sea level pressure: 101552.34 Pa
//calculated elevation_offset: 3925.32 Pa
//Pressure(Pa): 101547.56 Pressure(hPa): 1015.48 Temperature(C): 21.25 Altitude above sea(m): 331.94
//Pressure(Pa): 101543.31 Pressure(hPa): 1015.43 Temperature(C): 21.25 Altitude above sea(m): 331.62
//Pressure(Pa): 101544.31 Pressure(hPa): 1015.44 Temperature(C): 21.25 Altitude above sea(m): 331.94
//Pressure(Pa): 101546.06 Pressure(hPa): 1015.46 Temperature(C): 21.25 Altitude above sea(m): 331.75
//Pressure(Pa): 101543.56 Pressure(hPa): 1015.44 Temperature(C): 21.25 Altitude above sea(m): 331.94
