/* BaroSensor Arduino library, for Freetronics BARO module (MS5637-02BA03)
 * http://www.freetronics.com/baro
 *
 * Copyright (C)2014 Freetronics Pty Ltd. Licensed under GNU GPLv3 as described in the LICENSE file.
 *
 * Written by Angus Gratton (angus at freetronics dot com)
 */
#include "BaroSensor.h"

/* i2c address of module */
#define BARO_ADDR 0x76

/* delay to wait for sampling to complete, on each OSR level */
const uint8_t SamplingDelayMs[6] PROGMEM = {
  2,
  4,
  6,
  10,
  18,
  34
};

/* module commands */
#define CMD_RESET 0x1E
#define CMD_PROM_READ(offs) (0xA0+(offs<<1)) /* Offset 0-7 */
#define CMD_START_D1(oversample_level) (0x40 + 2*(int)oversample_level)
#define CMD_START_D2(oversample_level) (0x50 + 2*(int)oversample_level)
#define CMD_READ_ADC 0x00

//****BaroSensorClass BaroSensor;

// Temporary hack due to bug in Arduino 1.5.0-1.5.6 on ARM (see below)
inline static int8_t _endTransmission(bool stop = true)
{
  int8_t res = Wire.endTransmission(stop);
#ifdef __AVR__
  return res;
#else
  // ARM Arduino <= 1.5.6 doesn't return an error code from endTransmission(),
  // instead number of bytes is returned (regardless of error status or not.)
  // This is corrected in https://github.com/arduino/Arduino/pull/1994 but not yet released.
  return 0;
#endif
}

void BaroSensorClass::begin()
{
  Wire.begin();
  Wire.beginTransmission(BARO_ADDR);
  Wire.write(CMD_RESET);
  err = _endTransmission();
  if(err) return;

  uint16_t prom[7];
  for(int i = 0; i < 7; i++) {
    Wire.beginTransmission(BARO_ADDR);
    Wire.write(CMD_PROM_READ(i));
    err = _endTransmission(false);
    if(err)
      return;
    int req = Wire.requestFrom(BARO_ADDR, 2);
    if(req != 2) {
      err = ERR_BAD_READLEN;
      return;
    }
    prom[i] = ((uint16_t)Wire.read()) << 8;
    prom[i] |= Wire.read();
  }

  // TODO verify CRC4 in top 4 bits of prom[0] (follows AN520 but not directly...)

  c1 = prom[1];
  c2 = prom[2];
  c3 = prom[3];
  c4 = prom[4];
  c5 = prom[5];
  c6 = prom[6];
  initialised = true;
}

float BaroSensorClass::getTemperature(TempUnit scale, BaroOversampleLevel level)
{
  float result;
  if(getTempAndPressure(&result, NULL, scale, level))
    return result;
  else
    return NAN;
}

float BaroSensorClass::getPressure(BaroOversampleLevel level)
{
  float result;
  if(getTempAndPressure(NULL, &result, CELSIUS, level))
    return result;
  else
    return NAN;
}

bool BaroSensorClass::getTempAndPressure(float *temperature, float *pressure, TempUnit tempScale, BaroOversampleLevel level)
{
  if(err || !initialised)
    return false;

  int32_t d2 = takeReading(CMD_START_D2(level), level);
  if(d2 == 0)
    return false;
  int64_t dt = d2 - c5 * (1L<<8);

  int32_t temp = 2000 + (dt * c6) / (1L<<23);

  /* Second order temperature compensation */
  int64_t t2;
  if(temp >= 2000) {
    /* High temperature */
    t2 = 5 * (dt * dt) / (1LL<<38);
  } else {
      /* Low temperature */
    t2 = 3 * (dt * dt) / (1LL<<33);
  }

  if(temperature != NULL) {
    *temperature = (float)(temp - t2) / 100;
    if(tempScale == FAHRENHEIT)
      *temperature = *temperature * 9 / 5 + 32;
  }

  if(pressure != NULL) {
    int32_t d1 = takeReading(CMD_START_D1(level), level);
    if(d1 == 0)
      return false;

    int64_t off = c2 * (1LL<<17) + (c4 * dt) / (1LL<<6);
    int64_t sens = c1 * (1LL<<16) + (c3 * dt) / (1LL<<7);

    /* Second order temperature compensation for pressure */
    if(temp < 2000) {
      /* Low temperature */
      int32_t tx = temp-2000;
      tx *= tx;
      int32_t off2 = 61 * tx / (1<<4);
      int32_t sens2 = 29 * tx / (1<<4);
      if(temp < -1500) {
        /* Very low temperature */
        tx = temp+1500;
        tx *= tx;
        off2 += 17 * tx;
        sens2 += 9 * tx;
      }
      off -= off2;
      sens -= sens2;
    }

    int32_t p = ((int64_t)d1 * sens/(1LL<<21) - off) / (1LL << 15);
    *pressure = (float)p / 100;
  }
  return true;
}

uint32_t BaroSensorClass::takeReading(uint8_t trigger_cmd, BaroOversampleLevel oversample_level)
{
  Wire.beginTransmission(BARO_ADDR);
  Wire.write(trigger_cmd);
  err = _endTransmission();

  if(err)
    return 0;
  uint8_t sampling_delay = pgm_read_byte(SamplingDelayMs + (int)oversample_level);
  delay(sampling_delay);

  Wire.beginTransmission(BARO_ADDR);
  Wire.write((uint8_t) CMD_READ_ADC);
  err = _endTransmission(false);
  if(err)
    return 0;
  int req = Wire.requestFrom(BARO_ADDR, 3);
  if(req != 3)
    req = Wire.requestFrom(BARO_ADDR, 3); // Sometimes first read fails...?
  if(req != 3) {
    err = ERR_BAD_READLEN;
    return 0;
  }
  uint32_t result = (uint32_t)Wire.read() << 16;
  result |= (uint32_t)Wire.read() << 8;
  result |= Wire.read();
  return result;
}

void BaroSensorClass::dumpDebugOutput()
{
  Serial.print(F("C1 = 0x"));
  Serial.println(c1, HEX);
  Serial.print(F("C2 = 0x"));
  Serial.println(c2, HEX);
  Serial.print(F("C3 = 0x"));
  Serial.println(c3, HEX);
  Serial.print(F("C4 = 0x"));
  Serial.println(c4, HEX);
  Serial.print(F("C5 = 0x"));
  Serial.println(c5, HEX);
  Serial.print(F("C6 = 0x"));
  Serial.println(c6, HEX);
  Serial.print(F("d1 first = 0x"));
  Serial.println(takeReading(CMD_START_D1(OSR_8192), OSR_8192));
  Serial.print(F("d2 first = 0x"));
  Serial.println(takeReading(CMD_START_D2(OSR_8192), OSR_8192));
  Serial.print(F("d1 second = 0x"));
  Serial.println(takeReading(CMD_START_D1(OSR_8192), OSR_8192));
  Serial.print(F("d2 second = 0x"));
  Serial.println(takeReading(CMD_START_D2(OSR_8192), OSR_8192));
  Serial.print(F("d1 third = 0x"));
  Serial.println(takeReading(CMD_START_D1(OSR_8192), OSR_8192));
  Serial.print(F("d2 third = 0x"));
  Serial.println(takeReading(CMD_START_D2(OSR_8192), OSR_8192));
  float temp, pressure;
  bool res = getTempAndPressure(&temp, &pressure);
  Serial.print(F("result (fourth) = "));
  Serial.println(res ? F("OK") : F("ERR"));
  Serial.print(F("Temp (fourth) = "));
  Serial.println(temp);
  Serial.print(F("Pressure (fourth) = "));
  Serial.println(pressure);
  Serial.print(F("Error (fourth) = "));
  Serial.println(err);
}
