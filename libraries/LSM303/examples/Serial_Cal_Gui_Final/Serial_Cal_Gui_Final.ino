#include <Wire.h>
#include <LSM303.h>
#include "CommunicationUtils.h"

LSM303 compass;

int raw_values[9];

//The command from the PC
char cmd;

void setup() {
  Serial.begin(38400);
  Wire.begin();
  compass.init();
  compass.enableDefault();
}

void loop() {
  if(Serial.available()) {
    cmd = Serial.read();
    if(cmd=='v') {
      Serial.println("LSM303 Calibration");
    }
    else if(cmd=='b') {
      uint8_t count = serial_busy_wait();
      for(uint8_t i=0; i<count; i++) {
        compass.read();
        raw_values[0] = (int)compass.a.x;
        raw_values[1] = (int)compass.a.y;
        raw_values[2] = (int)compass.a.z;
        raw_values[3] = 0;
        raw_values[4] = 0;
        raw_values[5] = 0;
        raw_values[6] = (int)compass.m.x;
        raw_values[7] = (int)compass.m.y;
        raw_values[8] = (int)compass.m.z;

        writeArr(raw_values, 9, sizeof(int));
        Serial.println();
      }
     }
  }
}

char serial_busy_wait() {
  while(!Serial.available()) {
    ; // do nothing until ready
  }
  return Serial.read();
}

