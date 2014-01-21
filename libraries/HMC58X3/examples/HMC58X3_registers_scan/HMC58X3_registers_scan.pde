/*HMC5883LRegisterScanner.pde
**A sketch that attempts to read every register from a slave device
**Written by Wayne Truchsess  http://dsscircuits.com
*/

#include "Wire.h"
#define I2C 0x1E

byte x;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(1000);
  
  Wire.beginTransmission(I2C);   // start transmission to device 
  Wire.send(0); // send register address
  Wire.send(0x70); // send value to write
  Wire.endTransmission(); // end transmission
  delay(10);
  Wire.beginTransmission(I2C);   // start transmission to device 
  Wire.send(1); // send register address
  Wire.send(B11000000); // send value to write
  Wire.endTransmission(); // end transmission
  delay(10);
  Wire.beginTransmission(I2C);   // start transmission to device 
  Wire.send(2); // send register address
  Wire.send(0x00); // send value to write
  Wire.endTransmission(); // end transmission
  
  delay(100);
  baselineRegisters();
}

void loop() {
  int x,y,z;
  Wire.beginTransmission(I2C);
  Wire.send(3); // will start from DATA X MSB and fetch all the others
  Wire.endTransmission();
  
  Wire.beginTransmission(I2C);
  Wire.requestFrom(I2C, 6);
  if(6 == Wire.available()) {
    // read out the 3 values, 2 bytes each.
    x = (Wire.receive() << 8) + Wire.receive();
    y = (Wire.receive() << 8) + Wire.receive();
    z = (Wire.receive() << 8) + Wire.receive();
    // the HMC5843 will automatically wrap around on the next request
  }
  Wire.endTransmission();
  
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.println(z);
  
  delay(100);
  return;
}

void baselineRegisters() {
 for(int l = 0x00; l < 0x12; l++){
    Wire.beginTransmission(I2C);
    Wire.send(l);
    Wire.endTransmission();
    //delay(100);
    Wire.beginTransmission(I2C);
    Wire.requestFrom(I2C,1);
    x = Wire.receive();
    Serial.print("Register Address ");
    Serial.print(l,DEC);
    Serial.print(" = ");
    Serial.print(x,BIN);
    Serial.print(" = ");
    Serial.print(x,DEC);
    Serial.println("     ");
    Wire.endTransmission();
  }
}