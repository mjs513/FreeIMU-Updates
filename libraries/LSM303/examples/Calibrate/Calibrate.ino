#include <Wire.h>
#include <LSM303.h>

LSM303 compass;
LSM303::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = {-32768, -32768, -32768};

char report[80];

void setup() {
  Serial.begin(9600);
  Wire.begin();
  compass.init();
  compass.enableDefault();
}

void loop() {  
  compass.read();
  
  running_min.x = min(running_min.x, compass.m.x);
  running_min.y = min(running_min.y, compass.m.y);
  running_min.z = min(running_min.z, compass.m.z);

  running_max.x = max(running_max.x, compass.m.x);
  running_max.y = max(running_max.y, compass.m.y);
  running_max.z = max(running_max.z, compass.m.z);
  
  snprintf(report, sizeof(report), "min: {%+6d, %+6d, %+6d}    max: {%+6d, %+6d, %+6d}",
    running_min.x, running_min.y, running_min.z,
    running_max.x, running_max.y, running_max.z);
  Serial.println(report);
  
  delay(100);
}