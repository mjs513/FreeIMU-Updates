#include <Wire.h>
#include <bma180.h>

BMA180 bma180 = BMA180(BMA180_ADDRESS_SDO_LOW);

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  bma180.SoftReset();
  bma180.enableWrite();
  bma180.SetFilter(bma180.F10HZ);
  bma180.setGSensitivty(bma180.G15);
  bma180.SetSMPSkip();
  bma180.SetISRMode();
  bma180.disableWrite();
  delay(100);
}

void loop()
{
  bma180.readAccel();
  Serial.print(bma180.x,DEC);
  Serial.print(",");
  Serial.print(bma180.y,DEC);
  Serial.print(",");
  Serial.println(bma180.z,DEC);
  delay(20);
}



