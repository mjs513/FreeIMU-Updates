#include "CurieIMU.h"
#include "BMI160.h"

  int fifo_cnt;
  uint8_t data[1024];
  int16_t ax, ay, az;         // accelerometer values
  int16_t gx, gy, gz;         // gyrometer values

void bmi160_intr(void)
{
  fifo_cnt = CurieIMU.getFIFOCount();
  fifo_cnt += 4;
  CurieIMU.getFIFOBytes(data, fifo_cnt);
  decode();
  //show_fifo_frames();
}

void decode(){
  gx = (((int16_t)data[13])  << 8) | data[12];
  ax = (((int16_t)data[19])  << 8) | data[18];

  gy = (((int16_t)data[15])  << 8) | data[14];
  ay = (((int16_t)data[21])  << 8) | data[20];
  
  gz = (((int16_t)data[17])  << 8) | data[16];
  az = (((int16_t)data[23])  << 8) | data[22];
  // display tab-separated accel/gyro x/y/z values
  Serial.print("a/g:\t");
  Serial.print(ax);  Serial.print("\t"); Serial.print(ay); Serial.print("\t");
  Serial.print(az);  Serial.print("\t");
  Serial.print(gx);  Serial.print("\t"); Serial.print(gy); Serial.print("\t");
  Serial.println(gz);
}

void show_fifo_frames(){
  //Serial.println(fifo_cnt);
  for(int i = 0; i < fifo_cnt; i++){
    Serial.print(data[i]); Serial.print(", ");
  }
  Serial.println("----------------------");
}


void setup() {
// initialize device
  Serial.begin(9600); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open

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

  // Some debug info to verify register writes
  //Serial.println(CurieIMU.reg_read(BMI160_RA_FIFO_CONFIG_0), BIN);
  //Serial.println(CurieIMU.reg_read(BMI160_RA_FIFO_CONFIG_1), BIN);

}

void loop() {

  delay(1);
}
