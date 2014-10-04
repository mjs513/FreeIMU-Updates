FreeIMU-Updates
===============

Change Log
Monday, Sept 22, 2014

10-03-14 Updated FreeIMU.cpp to allow easier access to change accelerometer/gyro/magneter default range settings to match description in wiki page

09-22-14

Changed LSM303 library default sampling rate to 100hz for accel and magnetometer to adjust for use with Teensy 3.1

Updated (int) to (int16_t) in lps331 library for getTemperatureRaw.

FreeIMU examples updated accordingly based on compass averaging change.

09-20-14

Deleted iCompass and RunningAverage from FreeIMU. Compass averaging is done in the Processing sketch using Yarmartino compass averaging method which is more address issue of discontinuity at 0/360 degrees. Tests on the Altimu10 v3 are much more stable. The processing code was adapted from Christopher Baker's code at  https://github.com/SAIC-ATS/Algorithms.git. Will leave runningaveage library in case anyone wishes to use it for other reasons.

iCompass deleted as tilt compensation was not working properly. Tilt compensation is still done from within FreeIMU.cpp using a different method.

FreeIMU_serial.ino updated to reflect changes to implementation of altidue complimentary filter from within the library as opposed to calling from the serial sketch.

Remaining sketches are in the process of being updated.

09-12-14 - Updated tuning for Altimu10v3, MPU9250 breakout board from Innvense, and FreeIMUv0.4.3.

09-03-14 - As a result of a conversation with Dan Barzilay did a little more research and determined that using Seb Madgwicks MARG gradient descent filter would provide an improvement to the drifting that I was still seeing. Preliminary tests with the FreeIMU v4 board were promising as well as additional testing with several other boards.  As a result the MadgwickAHRS code was added to FreeIMU library directly from the X-IO website with some minor coding changes to allow you to select either or of the implementations.

Please consider this a better release as still have some additional testing to perform.

See video: http://youtu.be/VCxzleg9KA4
Refs:
http://www.hindawi.com/journals/ijdsn/2014/864768/
http://code.google.com/p/uav-helicoptor-ece4007/wiki/MadgwickAHRS

-----------------------------------------------------------

08-29-14 - incorporated changes proposed by Dan Barzilay which incorporates the Altitude Complimentary filter and the Heading calcs that was in the Procressing sketch directly into the library.

Arduino serial sketches were updated to output the heading and altimeter accordingly. As a result of the changes the Processing sketch was simplified and the changes incorporated into the FreeIMU_cube_Odo_Exp_v2 Gui in the experimental folder. 

Additional changes to the FreeIMU_cube_Odo Gui will be made shortly.

My thanks to Dan for incorporating these changes.

---------------------------------------------------------------

08-22-14 Added same serial options in the Yaw-Pitch-Roll processing sketch as available with the processing GUI.

----------------------------------------------------------------

08-21-14 Update all Processing sketches with additional delay.

Uploaded processing version 2.0b7 which plays nicely with opengl to a separate library under mjs513.
-----------------------------------------------------------------

08-20-14 Added delay to processing sketch before serial read loop in set-up. Uploaded Processing 2.0b7 which works with the GUIs, no issue.

-----------------------------------------------------------

08-18-14 Reload of Freeimu.cpp, freeimu.h, calibration.h, freeimu_serial.ino and freeimu calib.  Tommasso report errors but I could not duplicate with my test versions so i am re uploading.

NOTE: With the advent of Processing 2.2 the GUI's do not seem to work anymore - issue with OPENGL. Will research and see if I can get operational again.
-----------------------------------------------------------

06-20-14
-------- Based on Kris Winer's work on the MPU-9250 and the AK8963 I was able to create a basic AK8963 library
         modeled after that of the AK8975 library from Jeff Rowberg. This was then incorporated into
		 FreeIMU library.

05-24-14
-------- Corrected test for AltIMU-10 v3 and updated Kp/Ki
-------- Created a new Serial Example for when using a Arduino Due - AltSoftSerial does not work for Due.
-------- Minor correction to Processing sketch
--------------------------------------------------------------------------

05-17-14
-------- updated FreeIMU examples to work with current library
-------- Added support for the Pololu AltIMU-10 v3 IMU
--------------------------------------------------------------------------

05-13-14
-------- Deleted 9150 library and added support for MPU-9150/9250 using Jeff Rowbergs AK8975 library for the magnetometer and maintained integrity of original 60X0 library
-------- Replaced Median Filter with Butterworh filters
-------- Added Filter library from ArduIMU (nice addition)
-------- Reduced size of serial sketch to fit on arduino uno for calibration
-------- Added azimuth code for heading when magnetometer and accelerometer available by extracting the code segment from the Pololu LSM303D library. Made a library called iCompass so it can be used elsewhere.
-------- In process of modifing code for use with Pololu AltIMU-10 v3.
---------------------------------------------------------------------------

04-19-14
------- Added support for GPS in Arduino and Processing Sketch using TinyGPS++ and AltSerialSoft libraries.
------- TinyGPS++ : http://arduiniana.org/libraries/tinygpsplus/
------- AltSoftwareSerial : https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html

04-18-14
------- Added 9150 library - 6050 library with getMotion9 function fully operational
------- Fixed heading function so heading is aligned with the IMU axis alignment
------- Put median filter on mag values
---------------------------------------

04-09-14	1. Deleted Fabio's implementation of the AHRS filter and inserted the complete version from 	Seb Madgwick's X-io website. Seemed to fix the mag issue dramatically.
2. Put median filter back in using a 5 point filter instead of 7
3. Fixed zero gyro issue when using the ADXL345

04-07-14 Added initGyros() routing from Ardupilot as well as filtering library so a median filter can be used for ADXL345 accelerometer. Code added to allow multiple board configuration Kp/Ki gains. Added ADXL345 temp cal for the DFROBOT board. Noticed I have to fix GUI for when you don't have a magnetometer on board. Next step drift correction.

03-05-14
Uploaded automated calibration code and methodology using Seb Madgwick calibration rig. His original paper is also posted on construction.

03-04-14
Fixed issue with increased drift when magnetometer enabled by updating Fabio's code to the Madgwick
code posted on his X-IO website.

03-02-14
1. Default to temp correction off.
2. Updating code as temp correction only applicable to MPU-6050 or 9150 at this point.
3. Clarified GenMPU6050 and DFROBOT defines
4. Added code for BMP085 altimeter
 
02-27-14	Fixed code issues with generic MPU6050 output for raw values. 
 
10-27-2013	1. Modified incorporate temperature correction for gyro and accelerometer
2. Corrected __AVR__ to __AVR_ATmega128__
3. Added call to get temperature from MPU-6050 in getRawValues
4. 1D Kalman Filter on q-values in sketch
	   
10-29-2013	Corrected temp coefficient array for temp
 	   
11-01-2013	 Added new inverse sqrt code obtained from articles:
http://pizer.wordpress.com/2008/10/12/fast-inverse-square-root/ and
http://www.diydrones.com/forum/topics/madgwick-imu-ahrs-and-fast-inverse-square-root
 	   
11-05-13	1. Add code to reset MPU-6050 on start-up, implemented in the Arduino sketch as well as option to manually reset in processing sketch.
2. Added LPF code line, currently set to MPU60X0_DLPF_BW_42
 	   
11-23-13	1. Changed twoKidef from 0.1 to 0.0f -12-12 => twoKiDef changed from 0f to 0.0025f based on article in DIYDrones
2. Added bypass caps (0.1 and 0.01 ceramic caps on the power pins of the freeIMU
2a. Added a 10 uF tantalum on the power bus
3. Yaw drift stabilized very nicely
4. Updated processing sketch to calculate heading and display based on the freescale application note and as implemented:
   http://diydrones.com/profiles/blogs/tiltcompensated-heading
5. update RESET function to zero out initial values
 	   
12-06-13	1. added LSM303D Polulo board instead of  calculating heading from FreeIMU
2. added option to turn on/off temp correction, temp_corr_on
3. added variable to set temperature break where calibration turns off, temp_break, note: temp_break in units directly from MPU-6050
 	   
01-02-14 to
  01-05-14	1. Recalibrated 6050 using Madgewick automated calibration rig - see paper
2. Corrected temperature calibration coefficients to ensure that gyros are zeroed for whole temp range when at rest - deleted necessary to run zerogyro routine.
3. Acceleration coefficients adjusted so Ax = 0, Ay = 0 and Az = 1g when at rest
4. Had to tune Ki to reduce drift - now set to 0.165
5. Changed Kp to 1.0 - drift almost zero
 	   
01-08-14	1. Completed coding ZARA ZUPT for Zero velocity detemination - added code to the processing sketch
 	   
01-09-14	1. Added a Reset Quaternion matrix only - option 2 in the sketch - to allow reset of matrix after rapid movement.
 	   
1-20-14	Summary of Processing sketch 
1.	updated to incorporate Altitude-Accelerometer complimentary filter
2.	motion detection indicator based on Zara-Zupt
3.	read additional fields from Arduino sketch
4.	Heading stability routine for LSM303 heading.
5.	Reset MPU-6050/Quaternion matrix by sending serial commands 1 or 2 respectfully.	 
