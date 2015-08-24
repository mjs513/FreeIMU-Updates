/*
 * This is a sketch for debugging any calibration issues with the MS5637 sensor.
 *
 * To use, Upload the sketch to the Arduino then open the Serial Monitor and set
 * the baud rate to 9600. A short list of calibration values and sample readings
 * will be displayed.
 */

#include <Wire.h>
#include <BaroSensor.h>

void setup()
{
  Serial.begin(9600);
  Serial.println("Initialising sensor...");
  BaroSensor.begin();
  Serial.println("Outputting debug values:");
  BaroSensor.dumpDebugOutput();
  Serial.println("Done.");
}

void loop()
{
}
