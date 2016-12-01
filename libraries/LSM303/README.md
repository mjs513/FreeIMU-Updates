# Arduino library for Pololu LSM303 boards

Version: 3.0.1 <br>
Release date: 2016-08-17 <br>
[![Build Status](https://travis-ci.org/pololu/lsm303-arduino.svg?branch=master)](https://travis-ci.org/pololu/lsm303-arduino)<br>
[www.pololu.com](https://www.pololu.com/)

## Summary

This is a library for an
[Arduino-compatible controller](https://www.pololu.com/arduino) that
interfaces with LSM303D, LSM303DLHC, LSM303DLM, and LSM303DLH 3D
compass and accelerometer ICs on Pololu boards. It makes it simple to
read the raw accelerometer and magnetometer data from these boards:

* [LSM303D 3D compass and accelerometer carrier](https://www.pololu.com/catalog/product/2127)
* [MinIMU-9 v3 (L3GD20H and LSM303D carrier)](https://www.pololu.com/catalog/product/2468)
* [AltIMU-10 v4 (L3GD20H, LSM303D, and LPS25H carrier)](https://www.pololu.com/catalog/product/2470)
* [AltIMU-10 v3 (L3GD20H, LSM303D, and LSM331AP carrier)](https://www.pololu.com/catalog/product/2469)
* [LSM303DLM 3D compass and accelerometer carrier](https://www.pololu.com/catalog/product/1273) (discontinued)
* [LSM303DLHC 3D compass and accelerometer carrier](https://www.pololu.com/catalog/product/2124) (discontinued)
* [LSM303DLH 3D compass and accelerometer carrier](https://www.pololu.com/catalog/product/1250) (discontinued)
* [MinIMU-9 v2 (L3GD20 and LSM303DLHC carrier)](https://www.pololu.com/catalog/product/1268) (discontinued)
* [MinIMU-9 (L3G4200D and LSM303DLM carrier)](https://www.pololu.com/catalog/product/1265) (discontinued)
* [MinIMU-9 (L3G4200D and LSM303DLH carrier)](https://www.pololu.com/catalog/product/1264) (discontinued)
* [AltIMU-10 (L3GD20, LSM303DLHC, and LSM331AP carrier)](https://www.pololu.com/catalog/product/1269) (discontinued)

The library also includes a function for computing the
tilt-compensated heading for those looking to use the LSM303 as a
tilt-compensated compass.

## Getting started

### Hardware

A LSM303 carrier can be purchased from Pololu's website. Before
continuing, careful reading of the product page as well as the chip
datasheet is recommended.

Make the following connections with wires between the Arduino and the
LSM303 board:

#### 5V Arduino boards

(including Arduino Uno, Leonardo, Mega; Pololu A-Star 32U4)

    Arduino   LSM303 board
    -------   ------------
         5V - VIN
        GND - GND
        SDA - SDA
        SCL - SCL

#### 3.3V Arduino boards

(including Arduino Due)

    Arduino   LSM303 board
    -------   ------------
        3V3 - VIN
        GND - GND
        SDA - SDA
        SCL - SCL

### Software

If you are using version 1.6.2 or later of the
[Arduino software (IDE)](https://www.arduino.cc/en/Main/Software), you can use
the Library Manager to install this library:

1. In the Arduino IDE, open the "Sketch" menu, select "Include Library", then
   "Manage Libraries...".
2. Search for "LSM303".
3. Click the LSM303 entry in the list.
4. Click "Install".

If this does not work, you can manually install the library:

1. Download the
   [latest release archive from GitHub](https://github.com/pololu/lsm303-arduino/releases)
   and decompress it.
2. Rename the folder "lps-arduino-xxxx" to "LSM303".
3. Drag the "LSM303" folder into the "libraries" directory inside your
   Arduino sketchbook directory. You can view your sketchbook location by
   opening the "File" menu and selecting "Preferences" in the Arduino IDE. If
   there is not already a "libraries" folder in that location, you should make
   the folder yourself.
4. After installing the library, restart the Arduino IDE.

## Examples

Several example sketches are available that show how to use the
library. You can access them from the Arduino IDE by opening the
"File" menu, selecting "Examples", and then selecting "LSM303". If
you cannot find these examples, the library was probably installed
incorrectly and you should retry the installation instructions above.

### Serial

This program continuously reads the accelerometer and magnetometer,
communicating the readings over the serial interface. You can display
the readings with the Arduino Serial Monitor.

Example output:

    A:    192  -1040 -17168    M:   -512     27    144
    A:    288  -1040 -17232    M:   -511     26    143
    A:     16  -1104 -17216    M:   -511     27    144

See the comments in this sketch for some notes on how to convert the
raw sensor values to units of g and gauss.

### Calibrate

This program is similar to the Serial example, but instead of printing
the most recent readings, it prints a running minimum and maximum of
the readings from each magnetometer axis. These values can be used to
calibrate the `heading()` functions and the Heading example after
moving the LSM303 through every possible orientation.

### Heading

This program uses readings from the accelerometer and magnetometer to
calculate a tilt-compensated compass heading (in degrees relative to a
default vector), which is communicated serially and can be displayed
with the Arduino Serial Monitor. The default vector is chosen to point
along the surface of the PCB, in the direction of the top of the text
on the silkscreen. (This is the +X axis on the Pololu LSM303D carrier
and the -Y axis on the Pololu LSM303DLHC, LSM303DLM, and LSM303DLH
carriers.) See the comments if you want to use a different reference
vector.

For the most accurate results, you should replace the values of
`m_min` and `m_max` assigned in the `setup()` function with your own
values obtained from the Calibrate example.

## Other library applications

These programs make use of the LSM303 library but are not included in
the library archive or repository.

- [MinIMU-9 + Arduino AHRS](https://github.com/pololu/minimu-9-ahrs-arduino)
  <br> This sketch allows an Arduino connected to a MinIMU-9 or
  AltIMU-10 to function as an attitude and heading reference system,
  calculating estimated roll, pitch, and yaw angles from sensor
  readings that can be visualized with a 3D test program on a PC. It
  is based on the work of Jordi Munoz, William Premerlani, Jose Julio,
  and Doug Weibel.
- [Pololu_Open_IMU](https://github.com/mikeshub/Pololu_Open_IMU)
  by mikeshub <br> This is an alternative AHRS implementation that
  uses the
  [Madgwick algorithm](http://www.x-io.co.uk/open-source-imu-and-ahrs-algorithms/).
- [ascii_graph](https://gist.github.com/drewtm/9081341) by drewtm <br>
  This sketch outputs a text-based graph of LSM303 accelerometer and
  L3G gyro data, providing a quick way to check whether the sensors
  are working as expected.

## Library reference

- `vector<int16_t> a` <br> The last values read from the
  accelerometer.
- `vector<int16_t> m` <br> The last values read from the magnetometer.
- `vector<int16_t> m_min` <br> Lower bounds (minimum values) for the
  magnetometer readings on each axis; set this appropriately to
  calibrate `heading()`.
- `vector<int16_t> m_max` <br> Upper bounds (maximum values) for the
  magnetometer readings on each axis; set this appropriately to
  calibrate `heading()`.
- `byte last_status` <br> The status of the last I2C transmission. See
  the [`Wire.endTransmission()`
  documentation](https://arduino.cc/en/Reference/WireEndTransmission)
  for return values.
- `LSM303(void)` <br> Constructor; initializes `m_min` and `m_max`
  with placeholder values.
- `bool init(deviceType device, sa0State sa0)` <br> Initializes the
  library with the device being used (`device_DLH`, `device_DLM`,
  `device_DLHC`, `device_D`, or `device_auto`) and the state of the
  SA0 pin (`sa0_low`, `sa0_high`, or `sa0_auto`), which determines the
  least significant bit(s) of the I²C slave address (on some devices,
  and only for the accelerometer in some cases). Constants for these
  arguments are defined in LSM303.h. Both of these arguments are
  optional; if they are not specified, the library will try to
  automatically detect the device and accelerometer
  address[[1]](#f1). A boolean is returned indicating whether
  the type of LSM303 device was successfully determined (if
  necessary).
- `byte getDeviceType(void)` <br> Returns the device type specified to
  or detected by `init()`.
- `void enableDefault(void)` <br> Turns on the accelerometer and
  magnetometer and enables a consistent set of default settings.  This
  function will set the accelerometer's full scale to be +/-2 g, which
  means that a reading of 16384 corresponds to approximately 1 g. The
  magnetometer's full scale is set to +/-4 gauss for the LSM303D or
  +/-1.3 gauss on all other devices. See the comments in LSM303.cpp
  for a full explanation of these settings.
- `void writeReg(byte reg, byte value)` <br> Writes an accelerometer
  or magnetometer register with the given value. Register addresses
  are defined by the regAddr enumeration type in LSM303.h.  Example
  use: `compass.writeReg(LSM303::CTRL_REG1_A, 0x57);`
- `void readReg(int reg)` <br> Reads an accelerometer or magnetometer
  register and returns the value read.[[2]](#f2)[[3]](#f3)
- `void writeAccReg(byte reg, byte value)` <br> Writes an
  accelerometer register with the given value.
- `byte readAccReg(byte reg)` <br> Reads an accelerometer register and
  returns the value read.
- `void writeMagReg(byte reg, byte value)` <br> Writes a magnetometer
  register with the given value.
- `byte readMagReg(int reg)` <br> Reads a magnetometer register and
  returns the value read.[[3]](#f3)
- `void readAcc(void)` <br> Takes a reading from the accelerometer and
  stores the values in the vector `a`. Conversion of the readings to
  units of g depends on the accelerometer's selected gain (full scale
  setting).  Note that in the LSM303DLHC, LSM303DLM, and LSM303DLH,
  the acceleration data registers actually contain a left-aligned
  12-bit number, so the lowest 4 bits are always 0, and the values in
  `a` should be shifted right by 4 bits (divided by 16) to be
  consistent with the conversion factors specified in the datasheets.
- `void readMag(void)` <br> Takes a reading from the magnetometer and
  stores the values in the vector `m`. Conversion of the readings to
  units of gauss depends on the magnetometer's selected gain (full
  scale setting).
- `void read(void)` <br> Takes a reading from both the accelerometer
  and magnetometer and stores the values in the vectors `a` and `m`.
- `void setTimeout(unsigned int timeout)` <br> Sets a timeout period
  for `readAcc()` and `readMag()`, in milliseconds, after which they
  will abort if no data is received. A value of 0 disables the
  timeout.
- `unsigned int getTimeout(void)` <br> Returns the current timeout
  period setting.
- `bool timeoutOccurred(void)` <br> Returns a boolean indicating
  whether a call to `readAcc()` or `readMag()` has timed out since the
  last call to `timeoutOccurred()`.
- `float heading(void)` <br> Returns the tilt-compensated heading of a
  default vector in degrees (the angular difference in the horizontal
  plane between the default vector and north). The default vector is
  chosen to point along the surface of the PCB, in the direction of
  the top of the text on the silkscreen. This is the +X axis on the
  Pololu LSM303D carrier and the -Y axis on the Pololu LSM303DLHC,
  LSM303DLM, and LSM303DLH carriers.
- `float heading(vector from)` <br> Returns the tilt-compensated
  heading of the given vector in degrees (the angular difference in
  the horizontal plane between `from` and north).

<b name="f1">1</b> The automatic detection might fail if you do not
use the Pololu boards' default accelerometer address, so you should
specify your particular device if you change the state of the SA0 pin.

<b name="f2">2</b> This function will not work for reading
TEMP_OUT_H_M and TEMP_OUT_L_M on the LSM303DLHC. To read those two
registers, use `readMagReg()` instead.

<b name="f3">3</b> If the magnetometer data registers are read using
register address constants without a specific device prefix
(e.g. `OUT_Y_H_M`), these functions will automatically use the correct
register addresses depending on the device type.

## Version history

* 3.0.1 (2016-08-17): Used PI instead of M_PI because M_PI is not
  defined for Arduino/Genuino 101.
* 3.0.0 (2016-08-17): Updated library to work with the Arduino Library Manager.
* 2.1.0 (2015-02-05): Improved autodetect behavior in `init()`;
  reverted argument types in register access functions to allow
  numeric register addresses; other miscellaneous fixes and
  optimizations.
* 2.0.0 (2013-11-27): Major rewrite. List of significant changes:
    * Added support for LSM303D.
    * Lowest 4 bits of accelerometer readings are no longer dropped
      before being stored in `a`; this makes the values returned by
      the library more consistent between the LSM303D and older
      sensors.
    * `enableDefault()` behavior changed to be more consistent across devices.
    * `heading()` now returns a float instead of an int.
    * Library constants converted to enums.
    * Added `writeReg()` and `readReg()`, which should be usable in
      place of `writeAccReg()`, `readAccReg()`, `writeMagReg()`, and
      `readMagReg()` in most situations.
    * `timeoutOccurred()` now reports whether a timeout occurred since
      it was last called instead of only on the most recent
      `readAcc()` or `readMag()` call.
    * Magnetometer gain functions removed; unfortunately, they would
      have been hard to update to support the LSM303D.
* 1.4.4 (2013-07-22): Corrected comments explaining `heading()` function.
* 1.4.3 (2013-03-15): Enable high resolution mode for LSM303DLHC
  accelerometer in `enableDefault()`.
* 1.4.2 (2012-10-31): Cast sensor readings to 16-bit ints for better
  portability.
* 1.4.1 (2012-07-06): Added `getDeviceType()` function for programs
  that need to autodetect devices and distinguish between them.
* 1.4.0 (2012-05-24): Added magnetometer gain functions and reading
  timeout feature; thanks to Joshua Hogendorn and Eric Brundick for
  these contributions. Added keywords.txt and changed file extensions
  of examples to .ino.
* 1.3.0 (2011-12-12): Arduino 1.0 compatibility.
* 1.2.0 (2011-11-15): Original release. (numbered to avoid confusion
  with our earlier
  [LSM303DLH library](https://github.com/pololu/LSM303DLH))
    * Besides the name change, the main difference in this library is
      that you need to call the `init()` function before using any of
      the other library functions, typically from within the Arduino
      `setup()` function. While the older library only works with the
      Pololu boards' default accelerometer slave address of 0011000b,
      this library allows you to specify the slave address with the
      `init()` function.
