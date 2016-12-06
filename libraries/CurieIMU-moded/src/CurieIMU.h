/*
 * BMI160 accelerometer/gyroscope library for Intel(R) Curie(TM) devices.
 *
 * Copyright (c) 2015 Intel Corporation.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef _CurieImu_H_
#define _CurieImu_H_

#include "BMI160.h"

/**
 * axis options
 *@see autoCalibrateAccelerometerOffset()
 *@see get/setGyroOffset()
 *@see get/setAccelerometerOffset()
 *@see readAcceleration()
 *@see readRotation()
 *@see shockDetected()
 *@see tapDetected()
 *@see motionDetected()
 */
typedef enum{
    X_AXIS = 0,
    Y_AXIS,
    Z_AXIS,
} CurieImuAxis;

/**
 *direction options
 *@see shockDetected()
 *@see tapDetected()
 *@see motionDetected()
 */
typedef enum{
    POSITIVE,
    NEGATIVE,
} CurieImuDirection;

 /**
 * Features for getThreshold(), getDuration() functions,
 */
typedef enum {
    CURIE_IMU_FREEFALL = 0,
    CURIE_IMU_SHOCK,
    CURIE_IMU_MOTION,
    CURIE_IMU_ZERO_MOTION,
    CURIE_IMU_STEP,
    CURIE_IMU_TAP,
    CURIE_IMU_TAP_SHOCK,
    CURIE_IMU_TAP_QUIET,
    CURIE_IMU_DOUBLE_TAP,
    CURIE_IMU_FIFO_FULL,
    CURIE_IMU_DATA_READY,
} CurieImuFeature;

/**
 * Step Detection Mode options
 * @see setStepDetectionMode()
 */
typedef enum {
    CURIE_IMU_STEP_MODE_NORMAL = BMI160_STEP_MODE_NORMAL,
    CURIE_IMU_STEP_MODE_SENSITIVE = BMI160_STEP_MODE_SENSITIVE,
    CURIE_IMU_STEP_MODE_ROBUST = BMI160_STEP_MODE_ROBUST,
    CURIE_IMU_STEP_MODE_UNKNOWN = BMI160_STEP_MODE_UNKNOWN
} CurieImuStepMode;

/* Note that this CurieImuClass class inherits methods from the BMI160Class which
 * is defined in BMI160.h.  BMI160Class provides methods for configuring and
 * accessing features of the BMI160 IMU device.  This CurieImuClass extends that
 * class with implementation of details specific to the integration of the BMI160
 * device on the Intel Curie module, such as the serial communication interface
 * and interrupt signalling.
 *
 * Please refer to the respective .cpp files for documentation on each of the
 * methods provided by these classes.
 */
class CurieImuClass : public BMI160Class {
    friend void bmi160_pin1_isr(void);

    public:
        bool begin(void);
        void end(void);

        // supported values: 25, 50, 100, 200, 400, 800, 1600, 3200 (Hz)
        int getGyroRate();
        void setGyroRate(int rate);

        // supported values: 12.5, 25, 50, 100, 200, 400, 800, 1600 (Hz)
        float getAccelerometerRate();
        void setAccelerometerRate(float rate);

        // supported values: 125, 250, 500, 1000, 2000 (degrees/second)
        int getGyroRange();
        void setGyroRange(int range);

        // supported values: 2, 4, 8, 16 (G)
        int getAccelerometerRange();
        void setAccelerometerRange(int range);

        void autoCalibrateGyroOffset();
        void autoCalibrateAccelerometerOffset(int axis, int target);

        void noGyroOffset();
        void noAccelerometerOffset();
        bool gyroOffsetEnabled();
        bool accelerometerOffsetEnabled();

        float getGyroOffset(int axis);
        float getAccelerometerOffset(int axis);

        // supported values: -31.171 to 31.171 (degrees/second), in steps of 0.061 degrees/second
        void setGyroOffset(int axis, float offset);

        // supported values: -495.3 (mg) to 495.3 (mg), in steps of 3.9 mg
        void setAccelerometerOffset(int axis, float offset);

        // supported values:
        //   CURIE_IMU_FREEFALL: 3.91 to 1995.46 (mg), in steps of 7.81 mg
        //   CURIE_IMU_SHOCK:
        //       2G: 3.91 to 1995.46 (mg), in steps of 7.81 mg
        //       4G: 7.81 to 3993.46 (mg), in steps of 15.63 mg
        //       8G: 15.63 to 7984.38 (mg), in steps of 31.25 mg 
        //       16G: 31.25 to 15968.75 (mg), in steps of 62.50 mg
        //   CURIE_IMU_MOTION:
        //       2G: 0 to 997.05 (mg), in steps of 3.91 mg
        //       4G: 0 to 1991.55 (mg), in steps of 7.81 mg
        //       8G: 0 to 3985.65 (mg), in steps of 15.63 mg 
        //       16G: 0 to 7968.75 (mg), in steps of 31.25 mg
        //   CURIE_IMU_ZERO_MOTION:
        //       2G: 0 to 997.05 (mg), in steps of 3.91 mg
        //       4G: 0 to 1991.55 (mg), in steps of 7.81 mg
        //       8G: 0 to 3985.65 (mg), in steps of 15.63 mg 
        //       16G: 0 to 7968.75 (mg), in steps of 31.25 mg
        //   CURIE_IMU_TAP:
        //       2G: 31.25 to 7968.75 (mg), in steps of 62.5 mg
        //       4G: 62.50 to 31937.50 (mg), in steps of 125.0 mg
        //       8G: 125.0 to 63875.00 (mg), in steps of 250.0 mg 
        //       16G: 250.0 to 127750.00 (mg), in steps of 500 mg
        float getDetectionThreshold(int feature);
        void setDetectionThreshold(int feature, float threshold);

        // supported values:
        //   CURIE_IMU_FREEFALL: 2.5 to 637.5 (ms), in steps of 2.5 ms
        //   CURIE_IMU_SHOCK: 50, 75 (ms)
        //   CURIE_IMU_MOTION: [1 - 4] / getAccelerometerRate() S
        //   CURIE_IMU_ZERO_MOTION: 1.28, 2.56, 3.84, 5.12, 6.40, 7.68, 8.96, 
        //                          10.24, 11.52, 12.80, 14.08, 15.36, 16.64,
        //                          17.92, 19.20, 20.48, 25.60, 30.72, 35.84,
        //                          40.96, 46.08, 51.20, 56.32, 61.44, 66.56,
        //                          71.68, 76.80, 81.92, 87.04, 92.16, 97.28, 
        //                          102.40, 112.64, 122.88, 133.12, 143.36,
        //                          153.60, 163.84, 174.08, 184.32, 194.56, 
        //                          204.80, 215.04, 225.28, 235.52, 245.76, 
        //                          256.00, 266.24, 276.48, 286.72, 296.96, 
        //                          307.20, 317.44, 327.68, 337.92, 348.16, 
        //                          358.40, 368.64, 378.88, 389.12, 399.36,
        //                          409.60, 419.84, 430.08 S
        //   CURIE_IMU_DOUBLE_TAP: 50, 100, 150, 200, 250, 275, 500, 700 ms
        //   CURIE_IMU_TAP_SHOCK: 50, 75 ms
        //   CURIE_IMU_TAP_QUIET: 20, 30 ms
        float getDetectionDuration(int feature);
        void setDetectionDuration(int feature, float value); //value duration

        void interrupts(int feature);
        void noInterrupts(int feature);
        bool interruptsEnabled(int feature);

        bool getInterruptStatus(int feature);

        CurieImuStepMode getStepDetectionMode();
        void setStepDetectionMode(int mode);

        void readMotionSensor(int& ax, int& ay, int& az, int& gx, int& gy, int& gz);
        void readMotionSensorScaled(float& ax, float& ay, float& az, float& gx, float& gy, float& gz);
        void readAccelerometer(int& x, int& y, int& z);
        void readAccelerometerScaled(float& x, float& y, float& z);
        void readGyro(int& x, int& y, int& z);
        void readGyroScaled(float& x, float& y, float& z);
        int readAccelerometer(int axis);
        float readAccelerometerScaled(int axis);
        int readGyro(int axis);
        float readGyroScaled(int axis);
        int readTemperature();

        bool shockDetected(int axis, int direction);
        bool motionDetected(int axis, int direction);
        bool tapDetected(int axis, int direction);
        bool stepsDetected();

        void attachInterrupt(void (*callback)(void));
        void detachInterrupt(void);

    private:
        int serial_buffer_transfer(uint8_t *buf, unsigned tx_cnt, unsigned rx_cnt);

        float accel_range;
        float gyro_range;

        float getFreefallDetectionThreshold();
        void setFreefallDetectionThreshold(float threshold);
        float getShockDetectionThreshold();
        void setShockDetectionThreshold(float threshold);
        float getMotionDetectionThreshold();
        void setMotionDetectionThreshold(float threshold);
        float getZeroMotionDetectionThreshold();
        void setZeroMotionDetectionThreshold(float threshold);
        float getTapDetectionThreshold();
        void setTapDetectionThreshold(float threshold);

        float getFreefallDetectionDuration();
        void setFreefallDetectionDuration(float duration);
        int getShockDetectionDuration();
        void setShockDetectionDuration(int duration);
        float getMotionDetectionDuration();
        void setMotionDetectionDuration(float duration);
        float getZeroMotionDetectionDuration();
        void setZeroMotionDetectionDuration(float duration);
        int getTapShockDuration();
        void setTapShockDuration(int duration);
        int getTapQuietDuration();
        void setTapQuietDuration(int duration);
        int getDoubleTapDetectionDuration();
        void setDoubleTapDetectionDuration(int duration);

        float convertRaw(int16_t raw, float range_abs);

        void enableInterrupt(int feature, bool enabled);

        void (*_user_callback)(void);
};


#endif /* _CurieImu_H_ */
