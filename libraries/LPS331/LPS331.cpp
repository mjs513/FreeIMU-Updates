#include <LPS331.h>
#include <Wire.h>
#include <Arduino.h>

// Defines ///////////////////////////////////////////////////////////

// The Arduino two-wire interface uses a 7-bit number for the address,
// and sets the last bit correctly based on reads and writes
#define LPS331AP_ADDRESS_SA0_LOW  0b1011100
#define LPS331AP_ADDRESS_SA0_HIGH 0b1011101

// Constructors //////////////////////////////////////////////////////

LPS331::LPS331(void)
{
  // Pololu board pulls SA0 high, so default assumption is that it is
  // high
  address = LPS331AP_ADDRESS_SA0_HIGH;
}

// Public Methods ////////////////////////////////////////////////////

// sets or detects slave address; returns bool indicating success
bool LPS331::init(byte sa0)
{
  switch(sa0)
  {
    case LPS331_SA0_LOW:
      address = LPS331AP_ADDRESS_SA0_LOW;
      return testWhoAmI();

    case LPS331_SA0_HIGH:
      address = LPS331AP_ADDRESS_SA0_HIGH;
      return testWhoAmI();

    default:
      return autoDetectAddress();
  }
}

// turns on sensor and enables continuous output
void LPS331::enableDefault(void)
{
  // active mode, 12.5 Hz output data rate
  writeReg(LPS331_CTRL_REG1, 0b11100000);
  delay(10);
  writeReg(LPS331_RES_CONF, 0x6A);
  delay(10);
}

// writes register
void LPS331::writeReg(byte reg, byte value)
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// reads register
byte LPS331::readReg(byte reg)
{
  byte value;

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission(false); // restart
  Wire.requestFrom(address, (byte)1);
  value = Wire.read();
  Wire.endTransmission();

  return value;
}

// reads pressure in millibars (mbar)/hectopascals (hPa)
float LPS331::readPressureMillibars(void)
{
  return (float)readPressureRaw() / 4096;
}

// reads pressure in inches of mercury (inHg)
float LPS331::readPressureInchesHg(void)
{
  return (float)readPressureRaw() / 138706.5;
}

// reads pressure and returns raw 24-bit sensor output
long LPS331::readPressureRaw(void)
{
  Wire.beginTransmission(address);
  // assert MSB to enable register address auto-increment
  Wire.write(LPS331_PRESS_OUT_XL | (1 << 7));
  Wire.endTransmission();
  Wire.requestFrom(address, (byte)3);

  while (Wire.available() < 3);

  uint8_t pxl = Wire.read();
  uint8_t pl = Wire.read();
  uint8_t ph = Wire.read();

  // combine bytes
  //  GCC performs an arithmetic right shift for signed negative
  //  numbers, but this code will not work if you port it to a
  //  compiler that does a logical right shift instead.
  return (int32_t)ph << 16 | (uint16_t)pl << 8 | pxl;
}

// reads temperature in degrees C
float LPS331::readTemperatureC(void)
{
  return 42.5 + (float)readTemperatureRaw() / 480.;
}

// reads temperature in degrees F
float LPS331::readTemperatureF(void)
{
  return 108.5 + (float)readTemperatureRaw() / 480 * 1.8;
}

// reads temperature and returns raw 16-bit sensor output
int16_t LPS331::readTemperatureRaw(void)
{
  Wire.beginTransmission(address);
  // assert MSB to enable register address auto-increment
  Wire.write(LPS331_TEMP_OUT_L | (1 << 7));
  Wire.endTransmission();
  Wire.requestFrom(address, (byte)2);

  while (Wire.available() < 2);

  uint8_t tl = Wire.read();
  uint8_t th = Wire.read();

  // combine bytes
  //  GCC performs an arithmetic right shift for signed negative
  //  numbers, but this code will not work if you port it to a
  //  compiler that does a logical right shift instead.
  return (int16_t)th << 8 | tl;
}

// converts pressure in mbar to altitude in meters, using 1976 US
// Standard Atmosphere model (note that this formula only applies to a
// height of 11 km, or about 36000 ft)
//  If altimeter setting (QNH, barometric pressure adjusted to sea
//  level) is given, this function returns an indicated altitude
//  compensated for actual regional pressure; otherwise, it returns
//  the pressure altitude above the standard pressure level of 1013.25
//  mbar or 29.9213 inHg
float LPS331::pressureToAltitudeMeters(float pressure_mbar, float altimeter_setting_mbar)
{
  //float height = LPS331::readTemperatureC();
  return (1 - pow(pressure_mbar / altimeter_setting_mbar, 0.190263)) * 44330.8;
  //return log10(1013.25/lps331ap_mbar)*8400*(1+lps331ap_deg/273.0);
}

// converts pressure in inHg to altitude in feet; see notes above
float LPS331::pressureToAltitudeFeet(float pressure_inHg, float altimeter_setting_inHg)
{
  return (1 - pow(pressure_inHg / altimeter_setting_inHg, 0.190263)) * 145442;
}

// Private Methods ///////////////////////////////////////////////////

bool LPS331::autoDetectAddress(void)
{
  // try each possible address and stop if reading WHO_AM_I returns the expected response
  address = LPS331AP_ADDRESS_SA0_LOW;
  if (testWhoAmI()) return true;
  address = LPS331AP_ADDRESS_SA0_HIGH;
  if (testWhoAmI()) return true;

  return false;
}

bool LPS331::testWhoAmI(void)
{
  return (readReg(LPS331_WHO_AM_I) == 0xBB);
}