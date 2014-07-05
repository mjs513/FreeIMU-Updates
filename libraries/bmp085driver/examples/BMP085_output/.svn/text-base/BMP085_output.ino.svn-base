// BMP085_output
// by Filipe Vieira
// Simple example of library usage with almost every BMP085 and lib features being used.

#include <Wire.h>
#include <BMP085.h>

BMP085 dps = BMP085();
long Temperature = 0, Pressure = 0, Altitude = 0;

void setup(void) {
  Serial.begin(9600);
  Wire.begin();
  delay(1000);
	
  dps.init();   
  
  showall();
  
  Serial.println("Registers dump");
  Serial.println("==========================================================");
  dumpRegisters();
  Serial.println("Calibration data");
  Serial.println("==========================================================");
  dps.dumpCalData();
  
  delay(5000);
}
 

void loop(void) {
  dps.getTemperature(&Temperature); 
  dps.getPressure(&Pressure);
  dps.getAltitude(&Altitude);
  
  Serial.print("Temp(C):");
  Serial.print(Temperature);
  Serial.print("  Alt(cm):");
  Serial.print(Altitude);
  Serial.print("  Pressure(Pa):");
  Serial.println(Pressure);
}

void showall(void) { 
  Serial.println("Current BMP085 settings");
  Serial.println("==========================================================");
  Serial.print("device address                  = 0x");
  Serial.println(dps.getDevAddr(), HEX);
  Serial.print("Mode                            = ");
  switch (dps.getMode()) {
    case MODE_ULTRA_LOW_POWER: 
      Serial.println("MODE_ULTRA_LOW_POWER");
      break;
    case MODE_STANDARD: 
      Serial.println("MODE_STANDARD");
      break;    
    case MODE_HIGHRES: 
      Serial.println("MODE_HIGHRES");
      break;    
    case MODE_ULTRA_HIGHRES:     
      Serial.println("MODE_ULTRA_HIGHRES");
      break; 
  }  
}

void dumpRegisters() {
  byte ValidRegisterAddr[]={0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,0xF6,0xF7,0xF8,0xF9}; 
  byte _b, i, totregisters = sizeof(ValidRegisterAddr);
  Serial.println("---dump start---");
  Serial.println("Register address|Register data");
  Serial.println("Reg.address(hex,dec) Reg.data(bin,hex,dec)");
  for (i=0;i<totregisters;i++){    
    Serial.print("0x");
    Serial.print(ValidRegisterAddr[i], HEX);
    Serial.print(",");
    Serial.print(ValidRegisterAddr[i], DEC);
    Serial.print(",");
    dps.readmem(ValidRegisterAddr[i], 1, &_b);
    Serial.print("b");
    print_bits(_b);
    Serial.print(",0x");
    Serial.print(_b,HEX);
    Serial.print(",");
    Serial.println(_b,DEC);  
  }
  Serial.println("---dump end---");
}

void print_bits(byte val){
  int i;
  for(i=7; i>=0; i--) 
    Serial.print(val >> i & 1, BIN);
}
/* void print_unit16(uint16_t val){
  int i;
  for(i=15; i>=0; i--) 
    Serial.print(val >> i & 1, BIN);
} 
*/