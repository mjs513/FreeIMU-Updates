# Arduino library for Pololu L3G boards

Version: 3.0.0 <br>
Release date: 2016-08-19 <br>
[![Build Status](https://travis-ci.org/pololu/l3g-arduino.svg?branch=master)](https://travis-ci.org/pololu/l3g-arduino) <br>
[www.pololu.com](https://www.pololu.com/)

## Summary

This is a library for an
[Arduino-compatible controller](https://www.pololu.com/arduino) that
interfaces with L3GD20H, L3GD20, and L3G4200D gyros on Pololu
boards. It makes it simple to read the raw gyro data from these
boards:

* [L3GD20H 3-axis gyro carrier](https://www.pololu.com/catalog/product/2129)
* [L3GD20 3-axis gyro carrier](https://www.pololu.com/catalog/product/2125)
* [MinIMU-9 v3 (L3GD20H and LSM303D carrier)](https://www.pololu.com/catalog/product/2468)
* [AltIMU-10 v4 (L3GD20H, LSM303D, and LPS25H carrier)](https://www.pololu.com/catalog/product/2470)
* [AltIMU-10 v3 (L3GD20H, LSM303D, and LSM331AP carrier)](https://www.pololu.com/catalog/product/2469)
* [L3G4200D 3-axis gyro carrier](https://www.pololu.com/catalog/product/1272) (discontinued)
* [MinIMU-9 v2 (L3GD20 and LSM303DLHC carrier)](https://www.pololu.com/catalog/product/1268) (discontinued)
* [MinIMU-9 (L3G4200D and LSM303DLM carrier)](https://www.pololu.com/catalog/product/1265) (discontinued)
* [MinIMU-9 (L3G4200D and LSM303DLH carrier)](https://www.pololu.com/catalog/product/1264) (discontinued)
* [AltIMU-10 (L3GD20, LSM303DLHC, and LPS331AP carrier)](https://www.pololu.com/catalog/product/1269) (discontinued)

## Getting Started

### Hardware


A L3G carrier can be purchased from Pololu's website. Before
continuing, careful reading of the product page as well as the chip
datasheet is recommended.

Make the following connections with wires between the Arduino and the
L3G board:

#### 5V Arduino boards

(including Arduino Uno, Leonardo, Mega; Pololu A-Star 32U4)

    Arduino   L3G board
    -------   ---------
         5V - VIN
        GND - GND
        SDA - SDA
        SCL - SCL

#### 3.3V Arduino boards

(including Arduino Due)

    Arduino   L3G board
    -------   ---------
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
2. Search for "L3G".
3. Click the L3G entry in the list.
4. Click "Install".

If this does not work, you can manually install the library:

1. Download the
   [latest release archive from GitHub](https://github.com/pololu/l3g-arduino/releases)
   and decompress it.
2. Rename the folder "l3g-arduino-xxxx" to "L3G".
3. Drag the "L3G" folder into the "libraries" directory inside your
   Arduino sketchbook directory. You can view your sketchbook location by
   opening the "File" menu and selecting "Preferences" in the Arduino IDE. If
   there is not already a "libraries" folder in that location, you should make
   the folder yourself.
4. After installing the library, restart the Arduino IDE.

## Example program

An example sketch is available that shows how to use the library.  You
can access it from the Arduino IDE by opening the "File" menu,
selecting "Examples", and then selecting "L3G".  If you cannot find
these examples, the library was probably installed incorrectly and you
should retry the installation instructions above.

### Serial

This program continuously reads the gyro, communicating the readings
over the serial interface. You can display the readings with the
Arduino Serial Monitor.

Example output:

    G X: 188 Y: -10 Z: -47
    G X: 138 Y: -40 Z: -26
    G X: 110 Y: -55 Z: 4

See the comments in this sketch for some notes on how to convert the
raw sensor values to units of dps (degrees per second).

### Other library applications

These programs make use of the L3G library but are not included in the
library archive or repository.

- [MinIMU-9 + Arduino AHRS](https://github.com/pololu/minimu-9-ahrs-arduino)
  <br> This sketch allows an Arduino connectd to a MinIMU-9 or
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

## Documentation

- `vector<int16_t> g` <br> The last values read from the gyro.
- `byte last_status` <br> The status of the last I2C transmission. See
  the
  [`Wire.endTransmission()` documentation](https://arduino.cc/en/Reference/WireEndTransmission)
  for return values.
- `L3G(void)` <br> Constructor.
- `bool init(byte device, byte sa0)` <br> Initializes the library with
  the device being used (`device_4200D`, `device_D20`, `device_D20H`,
  or `device_auto`) and the state of the SA0 pin (`sa0_low`,
  `sa0_high`, or `sa0_auto`), which determines the least significant
  bit of the I²C slave address. Constants for these arguments are
  defined in L3G.h. Both of these arguments are optional; if they are
  not specified, the library will try to automatically detect the
  device and slave address, and it will return a boolean indicating
  whether the type of L3G device was successfully determined (if
  necessary).
- `byte getDeviceType(void)` <br> Returns the device type specified to
  or detected by `init()`.
- `void enableDefault(void)` <br> Turns on the gyro and enables a
  consistent set of default settings.  This function will set the
  gyro's full scale to be +/-250 dps (degrees per second), which means
  that a reading of 114 corresponds to approximately 1 dps. See the
  comments in L3G.cpp for a full explanation of these settings.
- `void writeReg(byte reg, byte value)` <br> Writes a gyro register
  with the given value. Register address constants are defined by the
  regAddr enumeration type in L3G.h.  Example use:
  `gyro.writeReg(L3G::CTRL_REG1, 0x6F);`
- `byte readReg(byte reg)` <br> Reads a gyro register and returns the
  value read.
- `void read(void)` <br> Takes a reading from the gyro and stores the
  values in the vector `g`.
- `void setTimeout(unsigned int timeout)` <br> Sets a timeout period
  for `read()`, in milliseconds, after which it will abort if no data
  is received. A value of 0 disables the timeout.
- `unsigned int getTimeout(void)` <br> Returns the current timeout
  period setting.
- `bool timeoutOccurred(void)` <br> Returns a boolean indicating
  whether a call to `read()` has timed out since the last call to
  `timeoutOccurred()`.

## Version history

* 3.0.0 (2016-08-19): Updated library to work with the Arduino Library Manager.
* 2.0.0 (2015-02-05): Major rewrite, making library more consistent
  with LSM303 library. List of significant changes:
    * `enableDefault()` behavior changed to be more explicit and consistent across devices.
    * Library constants converted to enums.
    * L3GD20H is now detected as a distinct device (instead of being
      treated as an L3GD20) and constants have been added for the
      register names specified in its datasheet.
    * Added timeout functions from LSM303 library.
* 1.2.2 (2014-02-05): Added support for L3GD20H.
* 1.2.1 (2012-10-31): Cast sensor readings to 16-bit ints for better portability.
* 1.2.0 (2012-07-06): Renamed library to L3G and added support for L3GD20.
    * Besides the name change, the main difference from the L3G4200D
      library as originally released is that you need to call the
      `init()` function before using any of the other library
      functions, typically from within the Arduino `setup()`
      function. While the older library only works with the Pololu
      boards' default gyro slave address of 1101001b, the new library
      allows you to specify the slave address with the `init()`
      function.
* 1.1.0 (2011-12-12): Arduino 1.0 compatibility.
* 1.0.0 (2011-09-14): Original release of L3G4200D library.
