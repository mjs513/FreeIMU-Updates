#include <Wire.h>
#include <bma180.h>
#define DEBUGOFF

BMA180 bma180 = BMA180(BMA180_ADDRESS_SDO_LOW);


volatile byte newData=0;
volatile unsigned int countISR=0;
unsigned int served=0;

void BMAISR(void)
{
   countISR++;
   //bma180.readAccel(); 
   newData=1;
}

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  bma180.SoftReset();
  bma180.enableWrite();
  int sversion;
  int id;
  bma180.getIDs(&id,&sversion);
  Serial.print("Id = ");
  Serial.print(id,DEC);
  Serial.print(" v.");
  Serial.println(sversion,HEX);
  bma180.SetFilter(bma180.F10HZ);
  bma180.setGSensitivty(bma180.G15);
  attachInterrupt(0, BMAISR, RISING);
  bma180.SetSMPSkip();
  bma180.SetISRMode();
  bma180.disableWrite();
  delay(2000);
}

void loop()
{
  if (newData)
  {
      newData=0;
      bma180.readAccel();
      Serial.print("[");
      Serial.print(bma180.x,DEC);
      Serial.print(" ");
      Serial.print(bma180.y,DEC);
      Serial.print(" ");
      Serial.print(bma180.z,DEC);
      Serial.println("]");
  }
 #ifdef DEBUGON
  if (millis() > tAlive)
  {
     bma180.readAccel();
      Serial.print("ISR's=");
      Serial.print(countISR);
      Serial.print(" served=");
      Serial.println(served);
      Serial.print("t=");
      Serial.print(bma180.temp);
      Serial.print("[");
      Serial.print(bma180.x,DEC);
      Serial.print(" ");
      Serial.print(bma180.y,DEC);
      Serial.print(" ");
      Serial.print(bma180.z,DEC);
      Serial.println("]");

     tAlive=millis()+5000;
     Serial.print("filter reg=");
     Serial.println(bma180.getRegValue(0x20),BIN);
     Serial.print("status_reg1=");
     Serial.println(bma180.getRegValue(0x09),BIN);
     Serial.print("status_reg2=");
     Serial.println(bma180.getRegValue(0x0A),BIN);
     Serial.print("status_reg3=");
     Serial.println(bma180.getRegValue(0x0B),BIN);
     Serial.print("status_reg4=");
     Serial.println(bma180.getRegValue(0x0C),BIN);
     Serial.print("ctrl_reg0=");
     Serial.println(bma180.getRegValue(0x0d),BIN);
     Serial.print("ctrl_reg1=");
     Serial.println(bma180.getRegValue(0x0e),BIN);
     Serial.print("ctrl_reg2=");
     Serial.println(bma180.getRegValue(0x0f),BIN);
     Serial.print("ctrl_reg3=");
     Serial.println(bma180.getRegValue(0x21),BIN);
     Serial.print("ctrl_reg4=");
     Serial.println(bma180.getRegValue(0x22),BIN);
 } 
 #endif
}



