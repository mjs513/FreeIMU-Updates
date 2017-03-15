#include <CurieIMU.h>
#include "BMI160.h"
#include <MadgwickAHRS.h>

Madgwick filter;
unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;

  int fifo_cnt;
  uint8_t data[1024];
  int16_t aix, aiy, aiz;
  int16_t gix, giy, giz;
  float ax, ay, az;
  float gx, gy, gz;
  float roll, pitch, heading;
  unsigned long microsNow;

void bmi160_intr(void)
{
  fifo_cnt = CurieIMU.getFIFOCount();
  fifo_cnt += 4;
  CurieIMU.getFIFOBytes(data, fifo_cnt);
  decode();
}

void decode(){
  gix = (((int16_t)data[13])  << 8) | data[12];
  aix = (((int16_t)data[19])  << 8) | data[18];

  giy = (((int16_t)data[15])  << 8) | data[14];
  aiy = (((int16_t)data[21])  << 8) | data[20];
  
  giz = (((int16_t)data[17])  << 8) | data[16];
  aiz = (((int16_t)data[23])  << 8) | data[22];

/*
  Serial.print("a/g:\t");
  Serial.print(aix);  Serial.print("\t"); Serial.print(aiy); Serial.print("\t");
  Serial.print(aiz);  Serial.print("\t");
  Serial.print(gix);  Serial.print("\t"); Serial.print(giy); Serial.print("\t");
  Serial.println(giz);
*/

  // check if it's time to read data and update the filter
  microsNow = micros();
  if (microsNow - microsPrevious >= microsPerReading) {

    // convert from raw data to gravity and degrees/second units
    ax = convertRawAcceleration(aix);
    ay = convertRawAcceleration(aiy);
    az = convertRawAcceleration(aiz);
    gx = convertRawGyro(gix);
    gy = convertRawGyro(giy);
    gz = convertRawGyro(giz);

/*
  Serial.print("a/g:\t");
  Serial.print(ax);  Serial.print("\t"); Serial.print(ay); Serial.print("\t");
  Serial.print(az);  Serial.print("\t");
  Serial.print(gx);  Serial.print("\t"); Serial.print(gy); Serial.print("\t");
  Serial.println(gz);
*/
    // update the filter, which computes orientation
    filter.updateIMU(gx, gy, gz, ax, ay, az);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);

    // increment previous time, so we keep proper pace
    microsPrevious = microsPrevious + microsPerReading;
  }
  
}

void setup() {
  Serial.begin(9600);

  // start the IMU and filter
  filter.begin(25);

  // initialize device
  Serial.println("Initializing IMU device...");
  CurieIMU.begin();

  // verify connection
  Serial.println("Testing device connections...");
  if (CurieIMU.begin()) {
    Serial.println("CurieIMU connection successful");
  } else {
    Serial.println("CurieIMU connection failed");
  }

  //CurieIMU.setGyroRate(25);
  CurieIMU.setAccelerometerRange(2);
  CurieIMU.setAccelRate(BMI160_ACCEL_RATE_1600HZ);
  CurieIMU.setGyroRange(250);
  CurieIMU.setGyroRate(BMI160_GYRO_RATE_1600HZ);

    Serial.println("About to calibrate. Make sure your board is stable and upright");
    delay(5000);

    // The board must be resting in a horizontal position for
    // the following calibration procedure to work correctly!
    Serial.print("Starting Gyroscope calibration and enabling offset compensation...");
    CurieIMU.autoCalibrateGyroOffset();
    Serial.println(" Done");

    Serial.print("Starting Acceleration calibration and enabling offset compensation...");
    CurieIMU.autoCalibrateAccelerometerOffset(X_AXIS, 0);
    CurieIMU.autoCalibrateAccelerometerOffset(Y_AXIS, 0);
    CurieIMU.autoCalibrateAccelerometerOffset(Z_AXIS, 1);
    Serial.println(" Done");

    Serial.println("Internal sensor offsets AFTER calibration...");
    Serial.print(CurieIMU.getAccelerometerOffset(X_AXIS));
    Serial.print("\t"); // -76
    Serial.print(CurieIMU.getAccelerometerOffset(Y_AXIS));
    Serial.print("\t"); // -2359
    Serial.print(CurieIMU.getAccelerometerOffset(Z_AXIS));
    Serial.print("\t"); // 1688
    Serial.print(CurieIMU.getGyroOffset(X_AXIS));
    Serial.print("\t"); // 0
    Serial.print(CurieIMU.getGyroOffset(Y_AXIS));
    Serial.print("\t"); // 0
    Serial.println(CurieIMU.getGyroOffset(Z_AXIS));
  
  CurieIMU.setFIFOHeaderModeEnabled(false);   // Use Headerless Mode
  CurieIMU.setAccelFIFOEnabled(true);         // Enable the accelerometer FIFO
  CurieIMU.setGyroFIFOEnabled(true);          // Enable the gyro FIFO

  #define BMI160_FIFO_TIME_EN_BIT 1           // Returns a sensor time after last valid frame
  CurieIMU.reg_write_bits(BMI160_RA_FIFO_CONFIG_1, 0x1, BMI160_FIFO_TIME_EN_BIT, 1);

  CurieIMU.attachInterrupt(bmi160_intr);      // Attach interrupt for when FIFO data greater than
                                              // watermark level
  //CurieIMU.reg_write(BMI160_RA_FIFO_CONFIG_0, 0b00000101); // set FIFO water mark level
  CurieIMU.reg_write(BMI160_RA_FIFO_CONFIG_0, 0b00001010); // set FIFO water mark level
  #define BMI160_FWM_INT_BIT 6                // Sets watermark interrupt
  CurieIMU.reg_write_bits(BMI160_RA_INT_EN_1, 0x1, BMI160_FWM_INT_BIT, 1);
  CurieIMU.resetFIFO();


  // initialize variables to pace updates to correct rate
  microsPerReading = 1000000 / 25;
  microsPrevious = micros();
}

void loop() {

}

float convertRawAcceleration(int aRaw) {
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767
  
  float a = (aRaw * 2.0) / 32768.0;
  return a;
}

float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}
