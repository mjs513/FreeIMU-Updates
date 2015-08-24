#include <Wire.h>
#include <BaroSensor.h>

void setup()
{
  Serial.begin(9600);
  BaroSensor.begin();
}

void loop()
{
  if(!BaroSensor.isOK()) {
    Serial.print("Sensor not Found/OK. Error: "); 
    Serial.println(BaroSensor.getError());
    BaroSensor.begin(); // Try to reinitialise the sensor if we can
  }
  else {
    Serial.print("Temperature: "); 
    Serial.println(BaroSensor.getTemperature());
    Serial.print("Pressure:    ");
    Serial.println(BaroSensor.getPressure());
  }
  delay(1000);
}
