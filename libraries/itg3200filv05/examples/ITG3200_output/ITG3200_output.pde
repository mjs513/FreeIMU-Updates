// ITG-3200_output
// Copyright 2010-2011 Filipe Vieira & various contributors.
// http://code.google.com/p/itg-3200driver
// Simple example of library usage with almost every gyro and lib features being used.

#include <Wire.h>
#include <ITG3200.h>

ITG3200 gyro = ITG3200();
float xyz[3], temperature;

void setup(void) {
  Serial.begin(9600);
  Wire.begin();      // if experiencing gyro problems/crashes while reading XYZ values
                     // please read class constructor comments for further info.
  delay(1000);
  gyro.reset();
  // Use ITG3200_ADDR_AD0_HIGH or ITG3200_ADDR_AD0_LOW as the ITG3200 address 
  // depending on how AD0 is connected on your breakout board, check its schematics for details
  gyro.init(ITG3200_ADDR_AD0_LOW);
	
  Serial.print("zeroCalibrating...");
  gyro.zeroCalibrate(2500,2);
  Serial.println("done.");
		
  showall();
 
  Serial.println("Registers dump");
  Serial.println("==========================================================");
  dumpRegisters();
  Serial.println("==========================================================");
  
  delay(5000);
}

void loop(void) {
  while (gyro.isRawDataReady()) {
    gyro.readGyro(xyz);
    Serial.print("X:");
    Serial.print(xyz[0]);
    Serial.print("  Y:");
    Serial.print(xyz[1]);
    Serial.print("  Z:");
    Serial.println(xyz[2]);
  }
}
 
void showall(void) { 
  Serial.println("Current ITG3200 settings");
  Serial.println("==========================================================");
  Serial.print("device address                  = 0x");
  Serial.println(gyro.getDevAddr(), HEX);
  
  Serial.print("Sample rate divider (Hz)        = ");
  if (gyro.getFilterBW() == BW256_SR8)
    Serial.println(8000 / (gyro.getSampleRateDiv()+1), DEC);
  else
    Serial.println(1000 / (gyro.getSampleRateDiv()+1), DEC);
  
  Serial.print("full scale range                = ");
  if (gyro.getFSRange() == RANGE2000)
    Serial.println("+-2000 deg/sec");  
  else
    Serial.println("reserved");
    
  Serial.print("low pass filter BW              = ");
  switch (gyro.getFilterBW()) {
    case BW256_SR8:
        Serial.println("256Hz LowPassFilter BW/ 8Khz Sample Rate");
        break;
    case BW188_SR1:
        Serial.println("188Hz LowPassFilter BW/ 1Khz Sample Rate");
        break;
    case BW098_SR1:
        Serial.println("98Hz LowPassFilter BW/ 1Khz Sample Rate");
        break;
    case BW042_SR1:
        Serial.println("42Hz LowPassFilter BW/ 1Khz Sample Rate");
        break;
    case BW020_SR1:
        Serial.println("20Hz LowPassFilter BW/ 1Khz Sample Rate");
        break;
    case BW010_SR1:
        Serial.println("10Hz LowPassFilter BW/ 1Khz Sample Rate");
        break;
    case BW005_SR1:
        Serial.println("5Hz LowPassFilter BW/ 1Khz Sample Rate");
        break;        
  }
  
  Serial.print("Logic level for INT output pin  = ");
  if (gyro.isINTActiveOnLow())
    Serial.println("Active on Low");
  else
    Serial.println("Active on High");
  
  Serial.print("INT drive type                  = ");
  if (gyro.isINTOpenDrain())
    Serial.println("Open Drain");
  else
    Serial.println("Push-Pull");
    
  Serial.print("INT latch mode                  = ");  
  if (gyro.isLatchUntilCleared())
    Serial.println("Latch until interrupt is cleared");
  else
    Serial.println("50us pulse");
  
  Serial.print("INT latch clear mode            = ");  
  if (gyro.isAnyRegClrMode())
    Serial.println("Any register read");
  else
    Serial.println("Status register read only");

  Serial.print("ITGReady trigger status         = ");  
  if (gyro.isITGReadyOn())
    Serial.println("High/Set");
  else
    Serial.println("Low/Clear");

  Serial.print("RawDataReady trigger status     = ");  
  if (gyro.isRawDataReady())
    Serial.println("High/Set");
  else
    Serial.println("Low/Clear");
      
  Serial.print("Temperature (Celsius)           = "); 
  gyro.readTemp(&temperature);
  Serial.println(temperature); 
  
  Serial.print("Power mode                      = ");  
  gyro.setPowerMode(NORMAL);
  if (gyro.isLowPower() == STANDBY)
    Serial.println("Low power (sleep)");
  else
    Serial.println("Normal");

  Serial.print("Xgyro status                    = ");  
  if (gyro.isXgyroStandby() == NORMAL )
    Serial.println("Normal");
  else
    Serial.println("StandBy");
    
  Serial.print("Ygyro status                    = ");  
  if (gyro.isYgyroStandby() == NORMAL )
    Serial.println("Normal");
  else
    Serial.println("StandBy");

  Serial.print("Zgyro status                    = ");  
  if (gyro.isZgyroStandby() == NORMAL )
    Serial.println("Normal");
  else
    Serial.println("StandBy");

  Serial.print("Clock source                    = ");  
  switch (gyro.getClockSource()) {
    case INTERNALOSC:
        Serial.println("Internal oscillator");
        break;
    case PLL_XGYRO_REF:
        Serial.println("PLL with X Gyro reference");
        break;
    case PLL_YGYRO_REF:
        Serial.println("PLL with Y Gyro reference");
        break;
    case PLL_ZGYRO_REF:
        Serial.println("PLL with Z Gyro reference");
        break;
    case PLL_EXTERNAL32:
        Serial.println("PLL with external 32.768kHz reference");
        break;
    case PLL_EXTERNAL19:
        Serial.println("PLL with external 19.2MHz reference");
        break;        
  }
	
  Serial.print("X offset                        = ");  
  Serial.println(gyro.offsets[0]);
  Serial.print("Y offset                        = ");  
  Serial.println(gyro.offsets[1]);
  Serial.print("Z offset                        = ");  
  Serial.println(gyro.offsets[2]);
}

void dumpRegisters() {
  byte ValidRegisterAddr[]={0,21,22,23,26,27,28,29,30,31,32,33,34,57,56,62}; 
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
    gyro.readmem(ValidRegisterAddr[i], 1, &_b);
    Serial.print("b");
    print_bits(_b);
    Serial.print(",0x");
    Serial.print(_b,HEX);
    Serial.print(",");
    Serial.println(_b,DEC);  
  }
  Serial.println("---dump end---");
}

void print_bits(uint8_t val){
  for(int i=7; i>=0; i--) 
    Serial.print(val >> i & 1, BIN);
}
 void print_unit16(uint16_t val){
  for(int i=15; i>=0; i--) 
    Serial.print(val >> i & 1, BIN);
} 