/*
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

#include "CurieImu.h"
#include "ss_spi.h"
#include "interrupt.h"

#define CURIE_IMU_CHIP_ID 0xD1

#define BMI160_GPIN_AON_PIN 4

/******************************************************************************/

/** Power on and prepare for general usage.
 * This will prepare the SPI communication interface for accessing the BMI160
 * on the Curie module, before calling BMI160::initialize() to activate the
 * BMI160 accelerometer and gyroscpoe with default settings.
 */
bool CurieImuClass::begin()
{
    ss_spi_init(SPI_SENSING_1, 2000, SPI_BUSMODE_0, SPI_8_BIT, SPI_SE_1);

    /* Perform a dummy read from 0x7f to switch to spi interface */
    uint8_t dummy_reg = 0x7F;
    serial_buffer_transfer(&dummy_reg, 1, 1);

    /* The SPI interface is ready - now invoke the base class initialization */
    BMI160Class::initialize();

    /** Verify the SPI connection.
     * MakgetGyroRatee sure the device is connected and responds as expected.
     * @return True if connection is valid, false otherwise
     */
    return (CURIE_IMU_CHIP_ID == getDeviceID());
}

void CurieImuClass::end()
{
    ss_spi_disable(SPI_SENSING_1);
}

int CurieImuClass::getGyroRate()
{
    int rate;

    switch (BMI160Class::getGyroRate()) {
        case BMI160_GYRO_RATE_25HZ:
            rate = 25;
            break;

        case BMI160_GYRO_RATE_50HZ:
            rate = 50;
            break;

        case BMI160_GYRO_RATE_100HZ:
            rate = 100;
            break;

        case BMI160_GYRO_RATE_200HZ:
            rate = 200;
            break;

        case BMI160_GYRO_RATE_400HZ:
            rate = 400;
            break;

        case BMI160_GYRO_RATE_800HZ:
            rate = 800;
            break;

        case BMI160_GYRO_RATE_1600HZ:
            rate = 1600;
            break;

        case BMI160_GYRO_RATE_3200HZ:
        default:
            rate = 3200;
            break;
    }

    return rate;
}

void CurieImuClass::setGyroRate(int rate)
{
    BMI160GyroRate bmiRate;

    if (rate <= 25) {
        bmiRate = BMI160_GYRO_RATE_25HZ;
    } else if (rate <= 50) {
        bmiRate = BMI160_GYRO_RATE_50HZ;
    } else if (rate <= 100) {
        bmiRate = BMI160_GYRO_RATE_100HZ;
    } else if (rate <= 200) {
        bmiRate = BMI160_GYRO_RATE_200HZ;
    } else if (rate <= 400) {
        bmiRate = BMI160_GYRO_RATE_400HZ;
    } else if (rate <= 800) {
        bmiRate = BMI160_GYRO_RATE_800HZ;
    } else if (rate <= 1600) {
        bmiRate = BMI160_GYRO_RATE_1600HZ;
    } else {
        bmiRate = BMI160_GYRO_RATE_3200HZ;
    }

    BMI160Class::setGyroRate(bmiRate);
}

float CurieImuClass::getAccelerometerRate()
{
    float rate;

    switch (BMI160Class::getAccelRate()) {
        case BMI160_ACCEL_RATE_25_2HZ:
            rate = 12.5;
            break;

        case BMI160_ACCEL_RATE_25HZ:
            rate = 25;
            break;

        case BMI160_ACCEL_RATE_50HZ:
            rate = 50;
            break;

        case BMI160_ACCEL_RATE_100HZ:
            rate = 100;
            break;

        case BMI160_ACCEL_RATE_200HZ:
            rate = 200;
            break;

        case BMI160_ACCEL_RATE_400HZ:
            rate = 400;
            break;

        case BMI160_ACCEL_RATE_800HZ:
            rate = 800;
            break;

        case BMI160_ACCEL_RATE_1600HZ:
        default:
            rate = 1600;
            break;
    }

    return rate;
}

void CurieImuClass::setAccelerometerRate(float rate)
{
    BMI160AccelRate bmiRate;

    if (rate <= 12.5) {
        bmiRate = BMI160_ACCEL_RATE_25_2HZ;
    } else if (rate <= 25) {
        bmiRate = BMI160_ACCEL_RATE_25HZ;
    } else if (rate <= 50) {
        bmiRate = BMI160_ACCEL_RATE_50HZ;
    } else if (rate <= 100) {
        bmiRate = BMI160_ACCEL_RATE_100HZ;
    } else if (rate <= 200) {
        bmiRate = BMI160_ACCEL_RATE_200HZ;
    } else if (rate <= 400) {
        bmiRate = BMI160_ACCEL_RATE_400HZ;
    } else if (rate <= 800) {
        bmiRate = BMI160_ACCEL_RATE_800HZ;
    } else {
        bmiRate = BMI160_ACCEL_RATE_1600HZ;
    }

    setAccelRate(bmiRate);
}

int CurieImuClass::getGyroRange()
{
    int range;

    switch (getFullScaleGyroRange()) {
        case BMI160_GYRO_RANGE_2000:
            range = 2000;
            break;

        case BMI160_GYRO_RANGE_1000:
            range = 1000;
            break;

        case BMI160_GYRO_RANGE_500:
            range = 500;
            break;

        case BMI160_GYRO_RANGE_250:
            range = 250;
            break;

        case BMI160_GYRO_RANGE_125:
        default:
            range = 125;
            break;
    }

    return range;
}

void CurieImuClass::setGyroRange(int range)
{
    BMI160GyroRange bmiRange;

    if (range >= 2000) {
        bmiRange = BMI160_GYRO_RANGE_2000;
        gyro_range = 2000.0f;
    } else if (range >= 1000) {
        bmiRange = BMI160_GYRO_RANGE_1000;
        gyro_range = 1000.0f;
    } else if (range >= 500) {
        bmiRange = BMI160_GYRO_RANGE_500;
        gyro_range = 500.0f;
    } else if (range >= 250) {
        bmiRange = BMI160_GYRO_RANGE_250;
        gyro_range = 250.0f;
    } else {
        bmiRange = BMI160_GYRO_RANGE_125;
        gyro_range = 125.0f;
    }

    setFullScaleGyroRange(bmiRange);
}

int CurieImuClass::getAccelerometerRange()
{
    int range;

    switch (getFullScaleAccelRange()) {
        case BMI160_ACCEL_RANGE_2G:
            range = 2;
            break;

        case BMI160_ACCEL_RANGE_4G:
            range = 4;
            break;

        case BMI160_ACCEL_RANGE_8G:
            range = 8;
            break;

        case BMI160_ACCEL_RANGE_16G:
        default:
            range = 16;
            break;
    }

    return range;
}

void CurieImuClass::setAccelerometerRange(int range)
{
    BMI160AccelRange bmiRange;

    if (range <= 2) {
        bmiRange = BMI160_ACCEL_RANGE_2G;
        accel_range = 2.0f;
    } else if (range <= 4) {
        bmiRange = BMI160_ACCEL_RANGE_4G;
        accel_range = 4.0f;
    } else if (range <= 8) {
        bmiRange = BMI160_ACCEL_RANGE_8G;
        accel_range = 8.0f;
    } else {
        bmiRange = BMI160_ACCEL_RANGE_16G;
        accel_range = 16.0f;
    }

    setFullScaleAccelRange(bmiRange);
}

void CurieImuClass::autoCalibrateGyroOffset()
{
    BMI160Class::autoCalibrateGyroOffset();

    setGyroOffsetEnabled(true);
}

void CurieImuClass::autoCalibrateAccelerometerOffset(int axis, int target)
{
    switch (axis) {
        case X_AXIS:
            autoCalibrateXAccelOffset(target);
            break;

        case Y_AXIS:
            autoCalibrateYAccelOffset(target);
            break;

        case Z_AXIS:
            autoCalibrateZAccelOffset(target);
            break;

        default:
            break;
    }

    setAccelOffsetEnabled(true);
}

void CurieImuClass::noGyroOffset()
{
    setGyroOffsetEnabled(false);
}

void CurieImuClass::noAccelerometerOffset()
{
    setAccelOffsetEnabled(false);
}

bool CurieImuClass::gyroOffsetEnabled()
{
    return getGyroOffsetEnabled();
}

bool CurieImuClass::accelerometerOffsetEnabled()
{
    return getAccelOffsetEnabled();
}

float CurieImuClass::getGyroOffset(int axis)
{
    int bmiOffset;

    if (axis == X_AXIS) {
        bmiOffset = getXGyroOffset();
    } else if (axis == Y_AXIS) {
        bmiOffset = getYGyroOffset();
    } else if (axis == Z_AXIS) {
        bmiOffset = getZGyroOffset();
    } else {
        return -1;
    }

    return (bmiOffset * 0.061);
}

float CurieImuClass::getAccelerometerOffset(int axis)
{
    int bmiOffset;

    if (axis == X_AXIS) {
        bmiOffset = getXAccelOffset();
    } else if (axis == Y_AXIS) {
        bmiOffset = getYAccelOffset();
    } else if (axis == Z_AXIS) {
        bmiOffset = getZAccelOffset();
    } else {
        return -1;
    }

    return (bmiOffset * 3.9);
}

void CurieImuClass::setGyroOffset(int axis, float offset)
{
    int bmiOffset = offset / 0.061;

    if (bmiOffset < -512) {
        bmiOffset = -512;
    } else if (bmiOffset > 511) {
        bmiOffset = 511;
    }

    if (axis == X_AXIS) {
        setXGyroOffset(bmiOffset);
    } else if (axis == Y_AXIS) {
        setYGyroOffset(bmiOffset);
    } else if (axis == Z_AXIS) {
        setZGyroOffset(bmiOffset);
    }

    setGyroOffsetEnabled(true);
}

void CurieImuClass::setAccelerometerOffset(int axis, float offset)
{
    int bmiOffset = offset / 3.9;

    if (bmiOffset < -128) {
        bmiOffset = -128;
    } else if (bmiOffset > 127) {
        bmiOffset = 127;
    }

    if (axis == X_AXIS) {
        setXAccelOffset(bmiOffset);
    } else if (axis == Y_AXIS) {
        setYAccelOffset(bmiOffset);
    } else if (axis == Z_AXIS) {
        setZAccelOffset(bmiOffset);
    }

    setAccelOffsetEnabled(true);
}

float CurieImuClass::getDetectionThreshold(int feature)
{
    switch (feature) {
        case CURIE_IMU_FREEFALL:
            return getFreefallDetectionThreshold();

        case CURIE_IMU_SHOCK:
            return getShockDetectionThreshold();

        case CURIE_IMU_MOTION:
            return getMotionDetectionThreshold();

        case CURIE_IMU_ZERO_MOTION:
            return getZeroMotionDetectionThreshold();

        case CURIE_IMU_TAP:
            return getTapDetectionThreshold();

        case CURIE_IMU_STEP:
        case CURIE_IMU_DOUBLE_TAP:
        case CURIE_IMU_FIFO_FULL:
        case CURIE_IMU_DATA_READY:
        default:
            return -1;
    }
}

void CurieImuClass::setDetectionThreshold(int feature, float threshold)
{
    switch (feature) {
        case CURIE_IMU_FREEFALL:
            setFreefallDetectionThreshold(threshold);
            break;

        case CURIE_IMU_SHOCK:
            setShockDetectionThreshold(threshold);
            break;

        case CURIE_IMU_MOTION:
            setMotionDetectionThreshold(threshold);
            break;

        case CURIE_IMU_ZERO_MOTION:
            setZeroMotionDetectionThreshold(threshold);
            break;

        case CURIE_IMU_TAP:
            setTapDetectionThreshold(threshold);
            break;

        case CURIE_IMU_STEP:
        case CURIE_IMU_DOUBLE_TAP:
        case CURIE_IMU_FIFO_FULL:
        case CURIE_IMU_DATA_READY:
        default:
            break;
    }
}

float CurieImuClass::getDetectionDuration(int feature)
{
    switch (feature) {
        case CURIE_IMU_FREEFALL:
            return getFreefallDetectionDuration();

        case CURIE_IMU_SHOCK:
            return getShockDetectionDuration();

        case CURIE_IMU_MOTION:
            return getMotionDetectionDuration();

        case CURIE_IMU_TAP_SHOCK:
            return getTapShockDuration();

        case CURIE_IMU_ZERO_MOTION:
            return getZeroMotionDetectionDuration();

        case CURIE_IMU_TAP_QUIET:
            return getTapQuietDuration();

        case CURIE_IMU_DOUBLE_TAP:
            return getDoubleTapDetectionDuration();

        case CURIE_IMU_TAP:
        case CURIE_IMU_STEP:
        case CURIE_IMU_FIFO_FULL:
        case CURIE_IMU_DATA_READY:
        default:
            return -1;
    }
}

float CurieImuClass::getFreefallDetectionThreshold()
{
    int bmiThreshold = BMI160Class::getFreefallDetectionThreshold();

    return (bmiThreshold * 7.81) + 3.91;
}

void CurieImuClass::setFreefallDetectionThreshold(float threshold)
{
    int bmiThreshold = (threshold - 3.91) / 7.81;

    if (bmiThreshold < 0) {
        bmiThreshold = 0;
    } else if (bmiThreshold > 255) {
        bmiThreshold = 255;
    }

    BMI160Class::setFreefallDetectionThreshold(bmiThreshold);
}

float CurieImuClass::getShockDetectionThreshold()
{
    int bmiThreshold = BMI160Class::getShockDetectionThreshold();
    float step;
    float min;

    switch (getAccelerometerRange()) {
        case 2:
            step = 7.81;
            min = 3.91;
            break;

        case 4:
            step = 15.63;
            min = 7.81;
            break;

        case 8:
            step = 31.25;
            min = 15.63;
            break;

        case 16:
        default:
            step = 62.50;
            min = 31.25;
            break;
    }

    return (bmiThreshold * step) + min;
}

void CurieImuClass::setShockDetectionThreshold(float threshold)
{
    int bmiThreshold;

    switch (getAccelerometerRange()) {
        case 2:
            bmiThreshold = (threshold - 3.91) / 7.81;
            break;

        case 4:
            bmiThreshold = (threshold - 7.81) / 15.6;
            break;

        case 8:
            bmiThreshold = (threshold - 15.63) / 31.25;
            break;

        case 16:
        default:
            bmiThreshold = (threshold - 31.25) / 62.50;
            break;
    }

    if (bmiThreshold < 0) {
        bmiThreshold = 0;
    } else if (bmiThreshold > 255) {
        bmiThreshold = 255;
    }

    BMI160Class::setShockDetectionThreshold(bmiThreshold);
}

float CurieImuClass::getMotionDetectionThreshold()
{
    int bmiThreshold = BMI160Class::getMotionDetectionThreshold();
    float step;

    switch (getAccelerometerRange()) {
        case 2:
            step = 3.91;
            break;

        case 4:
            step = 7.81;
            break;

        case 8:
            step = 15.63;
            break;

        case 16:
        default:
            step = 31.25;
            break;
    }

    return (bmiThreshold * step);
}

void CurieImuClass::setMotionDetectionThreshold(float threshold)
{
    int bmiThreshold;

    switch (getAccelerometerRange()) {
        case 2:
            bmiThreshold = threshold / 3.91;
            break;

        case 4:
            bmiThreshold = threshold / 7.81;
            break;

        case 8:
            bmiThreshold = threshold / 15.63;
            break;

        case 16:
        default:
            bmiThreshold = threshold / 31.25;
            break;
    }

    if (bmiThreshold < 0) {
        bmiThreshold = 0;
    } else if (bmiThreshold > 255) {
        bmiThreshold = 255;
    }

    BMI160Class::setMotionDetectionThreshold(bmiThreshold);
}

float CurieImuClass::getZeroMotionDetectionThreshold()
{
    int bmiThreshold = BMI160Class::getZeroMotionDetectionThreshold();
    float step;

    switch (getAccelerometerRange()) {
        case 2:
            step = 3.91;
            break;

        case 4:
            step = 7.81;
            break;

        case 8:
            step = 15.63;
            break;

        case 16:
        default:
            step = 31.25;
            break;
    }

    return (bmiThreshold * step);
}

void CurieImuClass::setZeroMotionDetectionThreshold(float threshold)
{
    int bmiThreshold;

    switch (getAccelerometerRange()) {
        case 2:
            bmiThreshold = threshold / 3.91;
            break;

        case 4:
            bmiThreshold = threshold / 7.81;
            break;

        case 8:
            bmiThreshold = threshold / 15.63;
            break;

        case 16:
        default:
            bmiThreshold = threshold / 31.25;
            break;
    }

    if (bmiThreshold < 0) {
        bmiThreshold = 0;
    } else if (bmiThreshold > 255) {
        bmiThreshold = 255;
    }

    BMI160Class::setZeroMotionDetectionThreshold(bmiThreshold);
}

float CurieImuClass::getTapDetectionThreshold()
{
    int bmiThreshold = BMI160Class::getTapDetectionThreshold();
    float step;
    float min;

    switch (getAccelerometerRange()) {
        case 2:
            step = 62.5;
            min = 31.25;
            break;

        case 4:
            step = 125.0;
            min = 62.5;
            break;

        case 8:
            step = 250.0;
            min = 125.0;
            break;

        case 16:
        default:
            step = 500.0;
            min = 250.0;
            break;
    }

    return (bmiThreshold * step) + min;
}

void CurieImuClass::setTapDetectionThreshold(float threshold)
{
    int bmiThreshold;

    switch (getAccelerometerRange()) {
        case 2:
            bmiThreshold = (threshold - 31.25) / 62.5;
            break;

        case 4:
            bmiThreshold = (threshold - 62.5) / 125.0;
            break;

        case 8:
            bmiThreshold = (threshold - 125.0) / 250.0;
            break;

        case 16:
        default:
            bmiThreshold = (threshold - 2500) / 500.0;
            break;
    }

    if (bmiThreshold < 0) {
        bmiThreshold = 0;
    } else if (bmiThreshold > 255) {
        bmiThreshold = 255;
    }

    BMI160Class::setTapDetectionThreshold(bmiThreshold);
}

void CurieImuClass::setDetectionDuration(int feature, float value)
{
    switch (feature) {
        case CURIE_IMU_FREEFALL:
            setFreefallDetectionDuration(value);
            break;

        case CURIE_IMU_SHOCK:
            setShockDetectionDuration(value);
            break;

        case CURIE_IMU_MOTION:
            setMotionDetectionDuration(value);
            break;

        case CURIE_IMU_TAP_SHOCK:
            setTapShockDuration(value);
            break;

        case CURIE_IMU_ZERO_MOTION:
            setZeroMotionDetectionDuration(value);
            break;

        case CURIE_IMU_TAP_QUIET:
            setTapQuietDuration(value);
            break;

        case CURIE_IMU_DOUBLE_TAP:
            setDoubleTapDetectionDuration(value);
            break;

        case CURIE_IMU_TAP:
        case CURIE_IMU_STEP:
        case CURIE_IMU_FIFO_FULL:
        case CURIE_IMU_DATA_READY:
        default:
            break;
    }
}

float CurieImuClass::getFreefallDetectionDuration()
{
    int bmiDuration = BMI160Class::getFreefallDetectionDuration();

    return ((bmiDuration + 1) * 2.5);
}

void CurieImuClass::setFreefallDetectionDuration(float duration)
{
    int bmiDuration = (duration - 2.5) / 2.5;

    if (bmiDuration < 0) {
        bmiDuration = 0;
    } else if (bmiDuration > 255) {
        bmiDuration = 255;
    }

    BMI160Class::setFreefallDetectionDuration(bmiDuration);
}

int CurieImuClass::getShockDetectionDuration()
{
    int duration;

    switch (BMI160Class::getShockDetectionDuration()) {
        case BMI160_TAP_SHOCK_DURATION_50MS:
            duration = 50;
            break;

        case BMI160_TAP_SHOCK_DURATION_75MS:
        default:
            duration = 75;
            break;
    }

    return duration;
}
void CurieImuClass::setShockDetectionDuration(int duration)
{
    BMI160TapShockDuration bmiDuration;

    if (duration <= 50) {
        bmiDuration = BMI160_TAP_SHOCK_DURATION_50MS;
    } else {
        bmiDuration = BMI160_TAP_SHOCK_DURATION_75MS;
    }

    BMI160Class::setShockDetectionDuration(bmiDuration);
}

float CurieImuClass::getMotionDetectionDuration()
{
    int bmiDuration = BMI160Class::getMotionDetectionDuration();

    return (bmiDuration / getAccelerometerRate());
}
void CurieImuClass::setMotionDetectionDuration(float duration)
{
    int bmiDuration = (duration * getAccelerometerRate());

    if (bmiDuration < 1) {
        bmiDuration = 1;
    } else if (bmiDuration > 4) {
        bmiDuration = 4;
    }

    BMI160Class::setMotionDetectionDuration(bmiDuration);
}

float CurieImuClass::getZeroMotionDetectionDuration()
{
    float duration;

    switch (BMI160Class::getZeroMotionDetectionDuration()) {
        case BMI160_ZERO_MOTION_DURATION_1_28S:
            duration = 1.28;
            break;

        case BMI160_ZERO_MOTION_DURATION_2_56S:
            duration = 2.56;
            break;

        case BMI160_ZERO_MOTION_DURATION_3_84S:
            duration = 3.84;
            break;

        case BMI160_ZERO_MOTION_DURATION_5_12S:
            duration = 5.12;
            break;

        case BMI160_ZERO_MOTION_DURATION_6_40S:
            duration = 6.40;
            break;

        case BMI160_ZERO_MOTION_DURATION_7_68S:
            duration = 7.68;
            break;

        case BMI160_ZERO_MOTION_DURATION_8_96S:
            duration = 8.96;
            break;

        case BMI160_ZERO_MOTION_DURATION_10_24S:
            duration = 10.24;
            break;

        case BMI160_ZERO_MOTION_DURATION_11_52S:
            duration = 11.52;
            break;

        case BMI160_ZERO_MOTION_DURATION_12_80S:
            duration = 12.80;
            break;

        case BMI160_ZERO_MOTION_DURATION_14_08S:
            duration = 14.08;
            break;

        case BMI160_ZERO_MOTION_DURATION_15_36S:
            duration = 15.36;
            break;

        case BMI160_ZERO_MOTION_DURATION_16_64S:
            duration = 16.64;
            break;

        case BMI160_ZERO_MOTION_DURATION_17_92S:
            duration = 17.92;
            break;

        case BMI160_ZERO_MOTION_DURATION_19_20S:
            duration = 19.20;
            break;

        case BMI160_ZERO_MOTION_DURATION_20_48S:
            duration = 20.48;
            break;

        case BMI160_ZERO_MOTION_DURATION_25_60S:
            duration = 25.60;
            break;

        case BMI160_ZERO_MOTION_DURATION_30_72S:
            duration = 30.72;
            break;

        case BMI160_ZERO_MOTION_DURATION_35_84S:
            duration = 35.84;
            break;

        case BMI160_ZERO_MOTION_DURATION_40_96S:
            duration = 40.96;
            break;

        case BMI160_ZERO_MOTION_DURATION_46_08S:
            duration = 46.08;
            break;

        case BMI160_ZERO_MOTION_DURATION_51_20S:
            duration = 51.20;
            break;

        case BMI160_ZERO_MOTION_DURATION_56_32S:
            duration = 56.32;
            break;

        case BMI160_ZERO_MOTION_DURATION_61_44S:
            duration = 61.44;
            break;

        case BMI160_ZERO_MOTION_DURATION_66_56S:
            duration = 66.56;
            break;

        case BMI160_ZERO_MOTION_DURATION_71_68S:
            duration = 71.68;
            break;

        case BMI160_ZERO_MOTION_DURATION_76_80S:
            duration = 76.80;
            break;

        case BMI160_ZERO_MOTION_DURATION_81_92S:
            duration = 81.92;
            break;

        case BMI160_ZERO_MOTION_DURATION_87_04S:
            duration = 87.04;
            break;

        case BMI160_ZERO_MOTION_DURATION_92_16S:
            duration = 92.16;
            break;

        case BMI160_ZERO_MOTION_DURATION_97_28S:
            duration = 97.28;
            break;

        case BMI160_ZERO_MOTION_DURATION_102_40S:
            duration = 102.40;
            break;

        case BMI160_ZERO_MOTION_DURATION_112_64S:
            duration = 112.64;
            break;

        case BMI160_ZERO_MOTION_DURATION_122_88S:
            duration = 122.88;
            break;

        case BMI160_ZERO_MOTION_DURATION_133_12S:
            duration = 133.12;
            break;

        case BMI160_ZERO_MOTION_DURATION_143_36S:
            duration = 143.36;
            break;

        case BMI160_ZERO_MOTION_DURATION_153_60S:
            duration = 153.60;
            break;

        case BMI160_ZERO_MOTION_DURATION_163_84S:
            duration = 163.84;
            break;

        case BMI160_ZERO_MOTION_DURATION_174_08S:
            duration = 174.08;
            break;

        case BMI160_ZERO_MOTION_DURATION_184_32S:
            duration = 184.32;
            break;

        case BMI160_ZERO_MOTION_DURATION_194_56S:
            duration = 194.56;
            break;

        case BMI160_ZERO_MOTION_DURATION_204_80S:
            duration = 204.80;
            break;

        case BMI160_ZERO_MOTION_DURATION_215_04S:
            duration = 215.04;
            break;

        case BMI160_ZERO_MOTION_DURATION_225_28S:
            duration = 225.28;
            break;

        case BMI160_ZERO_MOTION_DURATION_235_52S:
            duration = 235.52;
            break;

        case BMI160_ZERO_MOTION_DURATION_245_76S:
            duration = 245.76;
            break;

        case BMI160_ZERO_MOTION_DURATION_256_00S:
            duration = 256.00;
            break;

        case BMI160_ZERO_MOTION_DURATION_266_24S:
            duration = 266.24;
            break;

        case BMI160_ZERO_MOTION_DURATION_276_48S:
            duration = 276.48;
            break;

        case BMI160_ZERO_MOTION_DURATION_286_72S:
            duration = 286.72;
            break;

        case BMI160_ZERO_MOTION_DURATION_296_96S:
            duration = 296.96;
            break;

        case BMI160_ZERO_MOTION_DURATION_307_20S:
            duration = 307.20;
            break;

        case BMI160_ZERO_MOTION_DURATION_317_44S:
            duration = 317.44;
            break;

        case BMI160_ZERO_MOTION_DURATION_327_68S:
            duration = 327.68;
            break;

        case BMI160_ZERO_MOTION_DURATION_337_92S:
            duration = 337.92;
            break;

        case BMI160_ZERO_MOTION_DURATION_348_16S:
            duration = 348.16;
            break;

        case BMI160_ZERO_MOTION_DURATION_358_40S:
            duration = 358.40;
            break;

        case BMI160_ZERO_MOTION_DURATION_368_64S:
            duration = 368.64;
            break;

        case BMI160_ZERO_MOTION_DURATION_378_88S:
            duration = 378.88;
            break;

        case BMI160_ZERO_MOTION_DURATION_389_12S:
            duration = 389.12;
            break;

        case BMI160_ZERO_MOTION_DURATION_399_36S:
            duration = 399.36;
            break;

        case BMI160_ZERO_MOTION_DURATION_409_60S:
            duration = 409.60;
            break;

        case BMI160_ZERO_MOTION_DURATION_419_84S:
            duration = 419.84;
            break;

        case BMI160_ZERO_MOTION_DURATION_430_08S:
        default:
            duration = 430.08;
            break;
    }

    return duration;
}
void CurieImuClass::setZeroMotionDetectionDuration(float duration)
{
    BMI160ZeroMotionDuration bmiDuration;

    if (duration <= 1.28) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_1_28S;
    } else if (duration <= 2.56) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_2_56S;
    } else if (duration <= 3.84) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_3_84S;
    } else if (duration <= 5.12) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_5_12S;
    } else if (duration <= 6.40) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_6_40S;
    } else if (duration <= 7.68) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_7_68S;
    } else if (duration <= 8.96) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_8_96S;
    } else if (duration <= 10.24) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_10_24S;
    } else if (duration <= 11.52) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_11_52S;
    } else if (duration <= 12.80) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_12_80S;
    } else if (duration <= 14.08) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_14_08S;
    } else if (duration <= 15.36) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_15_36S;
    } else if (duration <= 16.64) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_16_64S;
    } else if (duration <= 17.92) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_17_92S;
    } else if (duration <= 19.20) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_19_20S;
    } else if (duration <= 20.48) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_20_48S;
    } else if (duration <= 25.60) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_25_60S;
    } else if (duration <= 30.72) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_30_72S;
    } else if (duration <= 35.84) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_35_84S;
    } else if (duration <= 40.96) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_40_96S;
    } else if (duration <= 46.08) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_46_08S;
    } else if (duration <= 51.20) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_51_20S;
    } else if (duration <= 56.32) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_56_32S;
    } else if (duration <= 61.44) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_61_44S;
    } else if (duration <= 66.56) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_66_56S;
    } else if (duration <= 71.68) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_71_68S;
    } else if (duration <= 76.80) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_76_80S;
    } else if (duration <= 81.92) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_81_92S;
    } else if (duration <= 87.04) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_87_04S;
    } else if (duration <= 92.16) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_92_16S;
    } else if (duration <= 97.28) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_97_28S;
    } else if (duration <= 102.40) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_102_40S;
    } else if (duration <= 112.64) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_112_64S;
    } else if (duration <= 122.88) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_122_88S;
    } else if (duration <= 133.12) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_133_12S;
    } else if (duration <= 143.36) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_143_36S;
    } else if (duration <= 153.60) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_153_60S;
    } else if (duration <= 163.84) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_163_84S;
    } else if (duration <= 174.08) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_174_08S;
    } else if (duration <= 184.32) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_184_32S;
    } else if (duration <= 194.56) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_194_56S;
    } else if (duration <= 204.80) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_204_80S;
    } else if (duration <= 215.04) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_215_04S;
    } else if (duration <= 225.28) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_225_28S;
    } else if (duration <= 235.52) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_235_52S;
    } else if (duration <= 245.76) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_245_76S;
    } else if (duration <= 256.00) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_256_00S;
    } else if (duration <= 266.24) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_266_24S;
    } else if (duration <= 276.48) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_276_48S;
    } else if (duration <= 286.72) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_286_72S;
    } else if (duration <= 296.96) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_296_96S;
    } else if (duration <= 307.20) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_307_20S;
    } else if (duration <= 317.44) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_317_44S;
    } else if (duration <= 327.68) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_327_68S;
    } else if (duration <= 337.92) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_337_92S;
    } else if (duration <= 348.16) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_348_16S;
    } else if (duration <= 358.40) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_358_40S;
    } else if (duration <= 368.64) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_368_64S;
    } else if (duration <= 378.88) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_378_88S;
    } else if (duration <= 389.12) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_389_12S;
    } else if (duration <= 399.36) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_399_36S;
    } else if (duration <= 409.60) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_409_60S;
    } else if (duration <= 419.84) {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_419_84S;
    } else {
        bmiDuration = BMI160_ZERO_MOTION_DURATION_430_08S;
    }

    BMI160Class::setZeroMotionDetectionDuration(bmiDuration);
}

int CurieImuClass::getTapShockDuration()
{
    int duration;

    switch (BMI160Class::getTapShockDuration()) {
        case BMI160_TAP_SHOCK_DURATION_50MS:
            duration = 50;
            break;

        case BMI160_TAP_SHOCK_DURATION_75MS:
        default:
            duration = 75;
            break;
    }

    return duration;
}

void CurieImuClass::setTapShockDuration(int duration)
{
    BMI160TapShockDuration bmiDuration;

    if (duration <= 50) {
        bmiDuration = BMI160_TAP_SHOCK_DURATION_50MS;
    } else {
        bmiDuration = BMI160_TAP_SHOCK_DURATION_75MS;
    }

    BMI160Class::setTapShockDuration(bmiDuration);
}

int CurieImuClass::getTapQuietDuration()
{
    int duration;

    switch (BMI160Class::getTapQuietDuration()) {
        case BMI160_TAP_QUIET_DURATION_30MS:
            duration = 30;
            break;

        case BMI160_TAP_QUIET_DURATION_20MS:
        default:
            duration = 20;
            break;
    }

    return duration;
}

void CurieImuClass::setTapQuietDuration(int duration)
{
    BMI160TapQuietDuration bmiDuration;

    if (duration >= 30) {
        bmiDuration = BMI160_TAP_QUIET_DURATION_30MS;
    } else {
        bmiDuration = BMI160_TAP_QUIET_DURATION_20MS;
    }

    BMI160Class::setTapQuietDuration(bmiDuration);
}

int CurieImuClass::getDoubleTapDetectionDuration()
{
    int duration;

    switch (BMI160Class::getDoubleTapDetectionDuration()) {
        case BMI160_DOUBLE_TAP_DURATION_50MS:
            duration = 50;
            break;

        case BMI160_DOUBLE_TAP_DURATION_100MS:
            duration = 100;
            break;

        case BMI160_DOUBLE_TAP_DURATION_150MS:
            duration = 150;
            break;

        case BMI160_DOUBLE_TAP_DURATION_200MS:
            duration = 200;
            break;

        case BMI160_DOUBLE_TAP_DURATION_250MS:
            duration = 250;
            break;

        case BMI160_DOUBLE_TAP_DURATION_375MS:
            duration = 375;
            break;

        case BMI160_DOUBLE_TAP_DURATION_500MS:
            duration = 500;
            break;

        case BMI160_DOUBLE_TAP_DURATION_700MS:
        default:
            duration = 700;
            break;
    }

    return duration;
}
void CurieImuClass::setDoubleTapDetectionDuration(int duration)
{
    BMI160DoubleTapDuration bmiDuration;

    if (duration <= 50) {
        bmiDuration = BMI160_DOUBLE_TAP_DURATION_50MS;
    } else if (duration <= 100) {
        bmiDuration = BMI160_DOUBLE_TAP_DURATION_100MS;
    } else if (duration <= 150) {
        bmiDuration = BMI160_DOUBLE_TAP_DURATION_150MS;
    } else if (duration <= 200) {
        bmiDuration = BMI160_DOUBLE_TAP_DURATION_200MS;
    } else if (duration <= 250) {
        bmiDuration = BMI160_DOUBLE_TAP_DURATION_250MS;
    } else if (duration <= 375) {
        bmiDuration = BMI160_DOUBLE_TAP_DURATION_375MS;
    } else if (duration <= 500) {
        bmiDuration = BMI160_DOUBLE_TAP_DURATION_500MS;
    } else {
        bmiDuration = BMI160_DOUBLE_TAP_DURATION_700MS;
    }

    BMI160Class::setDoubleTapDetectionDuration(bmiDuration);
}

void CurieImuClass::interrupts(int feature)
{
    enableInterrupt(feature, true);
}

void CurieImuClass::noInterrupts(int feature)
{
    enableInterrupt(feature, false);
}

void CurieImuClass::enableInterrupt(int feature, bool enabled)
{
    switch (feature) {
        case CURIE_IMU_FREEFALL:
            setIntFreefallEnabled(enabled);
            break;

        case CURIE_IMU_SHOCK:
            setIntShockEnabled(enabled);
            break;

        case CURIE_IMU_STEP:
            setIntStepEnabled(enabled);
            break;

        case CURIE_IMU_MOTION:
            setIntMotionEnabled(enabled);
            break;

        case CURIE_IMU_ZERO_MOTION:
            setIntZeroMotionEnabled(enabled);
            break;

        case CURIE_IMU_TAP:
            setIntTapEnabled(enabled);
            break;

        case CURIE_IMU_DOUBLE_TAP:
            setIntDoubleTapEnabled(enabled);
            break;

        case CURIE_IMU_FIFO_FULL:
            setIntFIFOBufferFullEnabled(enabled);
            break;

        case CURIE_IMU_DATA_READY:
            setIntDataReadyEnabled(enabled);
            break;

        default:
            break;
    }
}

bool CurieImuClass::interruptsEnabled(int feature)
{
    switch (feature) {
        case CURIE_IMU_FREEFALL:
            return getIntFreefallEnabled();

        case CURIE_IMU_SHOCK:
            return getIntShockEnabled();

        case CURIE_IMU_STEP:
            return getIntStepEnabled();

        case CURIE_IMU_MOTION:
            return getIntMotionEnabled();

        case CURIE_IMU_ZERO_MOTION:
            return getIntZeroMotionEnabled();

        case CURIE_IMU_TAP:
            return getIntTapEnabled();

        case CURIE_IMU_DOUBLE_TAP:
            return getIntDoubleTapEnabled();

        case CURIE_IMU_FIFO_FULL:
            return getIntFIFOBufferFullEnabled();

        case CURIE_IMU_DATA_READY:
            return getIntDataReadyEnabled();

        default:
            return false;
    }
}

bool CurieImuClass::getInterruptStatus(int feature)
{
    switch (feature) {
        case CURIE_IMU_FREEFALL:
            return getIntFreefallStatus();

        case CURIE_IMU_SHOCK:
            return getIntShockStatus();

        case CURIE_IMU_STEP:
            return getIntStepStatus();

        case CURIE_IMU_MOTION:
            return getIntMotionStatus();

        case CURIE_IMU_ZERO_MOTION:
            return getIntZeroMotionStatus();

        case CURIE_IMU_TAP:
            return getIntTapStatus();

        case CURIE_IMU_DOUBLE_TAP:
            return getIntDoubleTapStatus();

        case CURIE_IMU_FIFO_FULL:
            return getIntFIFOBufferFullStatus();

        case CURIE_IMU_DATA_READY:
            return getIntDataReadyStatus();

        default:
            return false;
    }
}

CurieImuStepMode CurieImuClass::getStepDetectionMode()
{
    return (CurieImuStepMode)BMI160Class::getStepDetectionMode();
}

void CurieImuClass::setStepDetectionMode(int mode)
{
    BMI160Class::setStepDetectionMode((BMI160StepMode)mode);
}

float CurieImuClass::convertRaw(int16_t raw, float range_abs)
{
    float slope;
    float val;

    /* Input range will be -32768 to 32767
     * Output range must be -range_abs to range_abs */
    val = (float)raw;
    slope = (range_abs * 2.0f) / BMI160_SENSOR_RANGE;
    return -(range_abs) + slope * (val + BMI160_SENSOR_LOW);
}

void CurieImuClass::readMotionSensor(int &ax, int &ay, int &az, int &gx,
                                     int &gy, int &gz)
{
    short sax, say, saz, sgx, sgy, sgz;

    getMotion6(&sax, &say, &saz, &sgx, &sgy, &sgz);

    ax = sax;
    ay = say;
    az = saz;
    gx = sgx;
    gy = sgy;
    gz = sgz;
}

void CurieImuClass::readMotionSensorScaled(float &ax, float &ay, float &az,
                                           float &gx, float &gy, float &gz)
{
    int16_t sax, say, saz, sgx, sgy, sgz;

    getMotion6(&sax, &say, &saz, &sgx, &sgy, &sgz);

    ax = convertRaw(sax, accel_range);
    ay = convertRaw(say, accel_range);
    az = convertRaw(saz, accel_range);
    gx = convertRaw(sgx, gyro_range);
    gy = convertRaw(sgy, gyro_range);
    gz = convertRaw(sgz, gyro_range);
}

void CurieImuClass::readAccelerometer(int &x, int &y, int &z)
{
    short sx, sy, sz;

    getAcceleration(&sx, &sy, &sz);

    x = sx;
    y = sy;
    z = sz;
}

void CurieImuClass::readAccelerometerScaled(float &x, float &y, float &z)
{
    int16_t sx, sy, sz;

    getAcceleration(&sx, &sy, &sz);

    x = convertRaw(sx, accel_range);
    y = convertRaw(sy, accel_range);
    z = convertRaw(sz, accel_range);
}

void CurieImuClass::readGyro(int &x, int &y, int &z)
{
    short sx, sy, sz;

    getRotation(&sx, &sy, &sz);

    x = sx;
    y = sy;
    z = sz;
}

void CurieImuClass::readGyroScaled(float &x, float &y, float &z)
{
    int16_t sx, sy, sz;

    getRotation(&sx, &sy, &sz);

    x = convertRaw(sx, gyro_range);
    y = convertRaw(sy, gyro_range);
    z = convertRaw(sz, gyro_range);
}

int CurieImuClass::readAccelerometer(int axis)
{
    if (axis == X_AXIS) {
        return getAccelerationX();
    } else if (axis == Y_AXIS) {
        return getAccelerationY();
    } else if (axis == Z_AXIS) {
        return getAccelerationZ();
    }

    return 0;
}

float CurieImuClass::readAccelerometerScaled(int axis)
{
    int16_t raw;

    if (axis == X_AXIS) {
        raw = getAccelerationX();
    } else if (axis == Y_AXIS) {
        raw = getAccelerationY();
    } else if (axis == Z_AXIS) {
        raw = getAccelerationZ();
    } else {
        return 0;
    }

    return convertRaw(raw, accel_range);
}

int CurieImuClass::readGyro(int axis)
{
    if (axis == X_AXIS) {
        return getRotationX();
    } else if (axis == Y_AXIS) {
        return getRotationY();
    } else if (axis == Z_AXIS) {
        return getRotationZ();
    }

    return 0;
}

float CurieImuClass::readGyroScaled(int axis)
{
    int16_t raw;

    if (axis == X_AXIS) {
        raw = getRotationX();
    } else if (axis == Y_AXIS) {
        raw = getRotationY();
    } else if (axis == Z_AXIS) {
        raw = getRotationZ();
    } else {
        return 0;
    }

    return convertRaw(raw, gyro_range);
}

int CurieImuClass::readTemperature()
{
    return getTemperature();
}

bool CurieImuClass::shockDetected(int axis, int direction)
{
    if (direction == POSITIVE) {
        if (axis == X_AXIS) {
            return getXPosShockDetected();
        } else if (axis == Y_AXIS) {
            return getYPosShockDetected();
        } else if (axis == Z_AXIS) {
            return getZPosShockDetected();
        }
    } else if (direction == NEGATIVE) {
        if (axis == X_AXIS) {
            return getXNegShockDetected();
        } else if (axis == Y_AXIS) {
            return getYNegShockDetected();
        } else if (axis == Z_AXIS) {
            return getZNegShockDetected();
        }
    }

    return false;
}

bool CurieImuClass::motionDetected(int axis, int direction)
{
    if (direction == POSITIVE) {
        if (axis == X_AXIS) {
            return getXPosMotionDetected();
        } else if (axis == Y_AXIS) {
            return getYPosMotionDetected();
        } else if (axis == Z_AXIS) {
            return getZPosMotionDetected();
        }
    } else if (direction == NEGATIVE) {
        if (axis == X_AXIS) {
            return getXNegMotionDetected();
        } else if (axis == Y_AXIS) {
            return getYNegMotionDetected();
        } else if (axis == Z_AXIS) {
            return getZNegMotionDetected();
        }
    }

    return false;
}

bool CurieImuClass::tapDetected(int axis, int direction)
{
    if (direction == POSITIVE) {
        if (axis == X_AXIS) {
            return getXPosTapDetected();
        } else if (axis == Y_AXIS) {
            return getYPosTapDetected();
        } else if (axis == Z_AXIS) {
            return getZPosTapDetected();
        }
    } else if (direction == NEGATIVE) {
        if (axis == X_AXIS) {
            return getXNegTapDetected();
        } else if (axis == Y_AXIS) {
            return getYNegTapDetected();
        } else if (axis == Z_AXIS) {
            return getZNegTapDetected();
        }
    }

    return false;
}

bool CurieImuClass::stepsDetected()
{
    return getIntStepStatus();
}

/** Provides a serial buffer transfer implementation for the BMI160 base class
 *  to use for accessing device registers.  This implementation uses the SPI
 *  bus on the Intel Curie module to communicate with the BMI160.
 */
int CurieImuClass::serial_buffer_transfer(uint8_t *buf, unsigned tx_cnt,
                                          unsigned rx_cnt)
{
    int flags, status;

    if (rx_cnt) /* For read transfers, assume 1st byte contains register address */
        buf[0] |= (1 << BMI160_SPI_READ_BIT);

    /* Lock interrupts here to
     * - avoid concurrent access to the SPI bus
     * - avoid delays in SPI transfer due to unrelated interrupts
     */
    flags = interrupt_lock();
    status = ss_spi_xfer(SPI_SENSING_1, buf, tx_cnt, rx_cnt);
    interrupt_unlock(flags);

    return status;
}

/** Interrupt handler for interrupts from PIN1 on the BMI160
 *  Calls a user callback if available.  The user callback is
 *  responsible for checking the source of the interrupt using
 *  the relevant API functions from the BMI160Class base class.
 */
void bmi160_pin1_isr(void)
{
    soc_gpio_mask_interrupt(SOC_GPIO_AON, BMI160_GPIN_AON_PIN);
    //if (CurieImu._user_callback)
    //    CurieImu._user_callback();
    soc_gpio_unmask_interrupt(SOC_GPIO_AON, BMI160_GPIN_AON_PIN);
}

/** Stores a user callback, and enables PIN1 interrupts from the
 *  BMI160 module.
 */
void CurieImuClass::attachInterrupt(void (*callback)(void))
{
    gpio_cfg_data_t cfg;

    _user_callback = callback;

    memset(&cfg, 0, sizeof(gpio_cfg_data_t));
    cfg.gpio_type = GPIO_INTERRUPT;
    cfg.int_type = EDGE;
    cfg.int_polarity = ACTIVE_LOW;
    cfg.int_debounce = DEBOUNCE_ON;
    cfg.gpio_cb = bmi160_pin1_isr;
    soc_gpio_set_config(SOC_GPIO_AON, BMI160_GPIN_AON_PIN, &cfg);

    setInterruptMode(1);                        // Active-Low
    setInterruptDrive(0);                       // Push-Pull
    setInterruptLatch(BMI160_LATCH_MODE_10_MS); // 10ms pulse
    setIntEnabled(true);
}

/** Disables PIN1 interrupts from the BMI160 module.
 */
void CurieImuClass::detachInterrupt(void)
{
    setIntEnabled(false);

    soc_gpio_deconfig(SOC_GPIO_AON, BMI160_GPIN_AON_PIN);
}

/* Pre-instantiated Object for this class */
CurieImuClass CurieImu;
