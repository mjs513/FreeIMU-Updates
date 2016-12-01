# Arduino library for Pololu LPS pressure sensor boards

Version: 3.0.0<br>
Release date: 2016-08-17<br>
[![Build Status](https://travis-ci.org/pololu/lps-arduino.svg?branch=master)](https://travis-ci.org/pololu/lps-arduino)<br>
[www.pololu.com](https://www.pololu.com/)


## Summary

This is a library for an
[Arduino-compatible controller](https://www.pololu.com/arduino) that
interfaces with ST LPS25H and LPS331AP pressure sensors on Pololu
boards. The library makes it simple to read the raw pressure data from
these boards:

* [Pololu LPS25H pressure/altitude sensor carrier](https://www.pololu.com/catalog/product/2724)
* [Pololu LPS331AP pressure/altitude sensor carrier](https://www.pololu.com/catalog/product/2126)
* [AltIMU-10 v3 (L3GD20H, LSM303D, and LSM331AP carrier)](https://www.pololu.com/catalog/product/2469)
* [AltIMU-10 (L3GD20, LSM303DLHC, and LSM331AP carrier)](https://www.pololu.com/catalog/product/1269) (discontinued)

The library also provides functions to help calculate altitude based
on the measured pressure.


## Getting started

### Hardware

A LPS carrier can be purchased from Pololu's website. Before
continuing, careful reading of the product page as well as the chip
datasheet is recommended.

Make the following connections with wires between the Arduino and the
LPS board:

#### 5V Arduino boards

(including Arduino Uno, Leonardo, Mega; Pololu A-Star 32U4)

    Arduino   LPS board
    -------   ---------
         5V - VIN
        GND - GND
        SDA - SDA
        SCL - SCL

#### 3.3V Arduino boards

(including Arduino Due)

    Arduino   LPS board
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
2. Search for "LPS".
3. Click the LPS entry in the list.
4. Click "Install".

If this does not work, you can manually install the library:

1. Download the
   [latest release archive from GitHub](https://github.com/pololu/lps-arduino/releases)
   and decompress it.
2. Rename the folder "lps-arduino-xxxx" to "LPS".
3. Drag the "LPS" folder into the "libraries" directory inside your
   Arduino sketchbook directory. You can view your sketchbook location by
   opening the "File" menu and selecting "Preferences" in the Arduino IDE. If
   there is not already a "libraries" folder in that location, you should make
   the folder yourself.
4. After installing the library, restart the Arduino IDE.

## Examples

Several example sketches are available that show how to use the
library. You can access them from the Arduino IDE by opening the
"File" menu, selecting "Examples", and then selecting "LPS". If
you cannot find these examples, the library was probably installed
incorrectly and you should retry the installation instructions above.

### SerialMetric

This program continuously takes pressure and temperature readings from
the sensor and calculates an altitude from the pressure. It shows the
data in metric units: pressure in millibars (mbar) (which are
equivalent to hectopascals (hPa)), altitude in meters, and temperature
in degrees Celsius. The program's output is sent over the serial
interface, and you can display it with the Arduino Serial Monitor.

Example output:

    p: 931.93 mbar	a: 700.02 m	t: 29.92 deg C
    p: 931.85 mbar	a: 700.73 m	t: 29.92 deg C
    p: 931.75 mbar	a: 701.68 m	t: 29.89 deg C

### SerialUS

This program is the same as SerialMetric, except that it shows the
data in United States customary units: pressure in inches of mercury
(inHg), altitude in feet, and temperature in degrees Fahrenheit.

Example output:

    p: 27.52 inHg	a: 2296.55 ft	t: 83.19 deg F
    p: 27.52 inHg	a: 2295.50 ft	t: 83.18 deg F
    p: 27.52 inHg	a: 2296.42 ft	t: 83.17 deg F


## Library reference

- `bool init(deviceType device, byte sa0)` <br> Initializes the
  library with the device being used (`LPS::device_331AP`,
  `LPS::device_25H`, or `LPS::device_auto`) and the state of the SA0
  pin (`LPS::sa0_low`, `LPS::sa0_high`, or `LPS::sa0_auto`; `LOW` or
  `HIGH` also work), which determines the least significant bit of the
  sensor's address. Both of these arguments are optional; if they are
  not specified, the library will try to automatically detect the
  device and address. The return value is a boolean indicating whether
  a device was successfully detected.
- `deviceType getDeviceType(void)` <br> Returns the device type
  detected by `init()`.
- `byte getAddress(void)` <br> Returns the address detected by
  `init()`.
- `void enableDefault(void)` <br> Turns on the pressure sensor in a
  default configuration that gives continous output at 12.5 Hz.
- `void writeReg(int reg, byte value)` <br> Writes a pressure sensor
  register with the given value. Register addresses are defined by the
  regAddr enumeration type in LPS.h.  Example use:
  `ps.writeReg(LPS::CTRL_REG1, 0xE0);`
- `byte readReg(int reg)` <br> Reads a pressure sensor register and
  returns the value read.
- `float readPressureMillibars(void)` <br> Returns a pressure reading
  from the sensor in units of millibars (mbar)/hectopascals (hPa).
- `float readPressureInchesHg(void)` <br> Returns a pressure reading
  from the sensor in units of inches of mercury (inHg).
- `long readPressureRaw(void)` <br> Returns a raw 24-bit pressure
  reading from the sensor.
- `float readTemperatureC(void)` <br> Returns a temperature reading
  from the sensor in units of degrees Celsius.
- `float readTemperatureF(void)` <br> Returns a temperature reading
  from the sensor in units of degrees Fahrenheit.
- `int readTemperatureRaw(void)` <br> Returns a raw 16-bit temperature
  reading from the sensor.
- `float pressureToAltitudeMeters(float pressure_mbar, float
  altimeter_setting_mbar)` <br> Converts a pressure in mbar to an
  altitude in meters, using the 1976
  [U.S. Standard Atmosphere](https://en.wikipedia.org/wiki/U.S._Standard_Atmosphere)
  model (note that this formula only applies up to an altitude of 11
  km, or about 36000 ft). If the optional `altimeter_setting_mbar`
  argument is given, specifying an "altimeter setting" or "QNH"
  (barometric pressure adjusted to sea level, often used in aviation
  and obtained from a local weather monitoring station), this function
  returns an indicated altitude compensated for the actual regional
  pressure. Otherwise, it returns a pressure altitude above the
  standard pressure level of 1013.25 mbar (29.9213 inHg).
- `float pressureToAltitudeFeet(float pressure_inHg, float
  altimeter_setting_inHg)` <br> Converts a pressure in inHg to an
  altitude in feet. See the preceding description of
  `pressureToAltitudeMeters()` for details.


## Version history

* 3.0.0 (2016-08-17): Updated library to work with the Arduino Library Manager.
* 2.0.0 (2014-06-03): Major rewrite. List of significant changes:
   * Renamed library to LPS.
   * Added support for LPS25H and automatic detection of device type.
   * Library constants converted to enums.
* 1.0.1 (2014-01-08): Changed raw output byte combination logic to work properly on 32-bit microcontrollers and be more portable.
* 1.0.0 (2013-03-22): Original release of LPS331 library.
