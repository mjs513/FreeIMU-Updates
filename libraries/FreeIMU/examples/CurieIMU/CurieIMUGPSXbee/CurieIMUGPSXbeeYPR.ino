/*
  ===============================================
  Example sketch for CurieImu library for Intel(R) Curie(TM) devices.
  Copyright (c) 2015 Intel Corporation.  All rights reserved.

  Based on I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050
  class by Jeff Rowberg: https://github.com/jrowberg/i2cdevlib

  ===============================================
  I2Cdev device library code is placed under the MIT license
  Copyright (c) 2011 Jeff Rowberg

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  ===============================================
*/

#include "CurieImu.h"
#include "MadgwickAHRS.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "CommunicationUtils.h"

const int ledPin = 13;      // activity LED pin
boolean blinkState = false; // state of the LED

Madgwick filter; // initialise Madgwick object
int ax, ay, az;
int gx, gy, gz;
float yaw;
float pitch;
float roll;
float val[8];

int factor = 65.536; // 64.536variable by which to divide gyroscope values, used to control sensitivity
// note that an increased baud rate requires an increase in value of factor

#define rad2deg 180.0/PI

int calibrateOffsets = 1; // int to determine whether calibration takes place or not


/*
   This sample code demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 38400;
#define telemBaud 57600

// The TinyGPS++ object
  TinyGPSPlus gps;

  // Setup GPS Serial and load config from i2c eeprom
  boolean gpsStatus[] = {false, false, false, false, false, false, false};
  unsigned long start;
  
  // The serial connection to the GPS device
  //SoftwareSerial gpsSerial(RXPin, TXPin);
  #define gpsSerial Serial1

//------------------------------------------
/*  #include <XBee.h>
  #define telemSerial Serial1

  XBee xbee = XBee();

  XBeeResponse response = XBeeResponse();
  // create reusable response objects for responses we expect to handle
  ZBRxResponse rx = ZBRxResponse();
  //Address set to receiving Xbee (one attached to PC)
  XBeeAddress64 Broadcast = XBeeAddress64(0x0013A200, 0x40C04290);
*/
  char Message[230], charBuf[20];
  String cmd, Payload;
  char cmd1[9];

void setup() {
  Serial.begin(57600); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open

    gpsSerial.begin(GPSBaud);

    // For Galileo,DUE and Teensy use Serial port 1
    //Load configuration from i2c eeprom - this assumes you have saved
    //a default configuration to the eeprom or permanent storage.
    //If you do not have this setup you will have to remove the
    //following lines and the additional code at the end of the sketch.
    //gpsSerial.begin(9600);
    
    //Settings Array
    //Code based on http://playground.arduino.cc/UBlox/GPS
    byte settingsArray[] = {0x04}; // Not really used for this example
    configureUblox(settingsArray);

  //telemSerial.begin(telemBaud);
  //xbee.setSerial(telemSerial);
  //xbee.setAPImode(1);

   //xbee.readPacket();
   //while(!xbee.getResponse().isAvailable()) {
   //   xbee.readPacket();
   //} 
   //getMessage();
  
  // initialize device
  //Serial.println("Initializing IMU device...");
  CurieImu.initialize();
  CurieImu.setFullScaleAccelRange(BMI160_ACCEL_RANGE_2G);
  CurieImu.setAccelRate(BMI160_ACCEL_RATE_50HZ);
  CurieImu.setAccelDLPFMode(BMI160_DLPF_MODE_OSR2);
  CurieImu.setFullScaleGyroRange(BMI160_GYRO_RANGE_2000);
  CurieImu.setGyroRate(BMI160_GYRO_RATE_50HZ);
  CurieImu.setGyroDLPFMode(BMI160_DLPF_MODE_OSR2);
  //factor = 16.4f;
  
  // verify connection
  //Serial.println("Testing device connections...");
  if (CurieImu.testConnection()) {
    //Serial.println("CurieImu connection successful");
  } else {
    //Serial.println("CurieImu connection failed");
  }
  
  // use the code below to calibrate accel/gyro offset values
/*  Serial.println("Internal sensor offsets BEFORE calibration...");
  Serial.print(CurieImu.getXAccelOffset()); 
  Serial.print("\t"); // -76
  Serial.print(CurieImu.getYAccelOffset()); 
  Serial.print("\t"); // -235
  Serial.print(CurieImu.getZAccelOffset()); 
  Serial.print("\t"); // 168
  Serial.print(CurieImu.getXGyroOffset()); 
  Serial.print("\t"); // 0
  Serial.print(CurieImu.getYGyroOffset()); 
  Serial.print("\t"); // 0
  Serial.println(CurieImu.getZGyroOffset());
*/
  // To manually configure offset compensation values, 
  // use the following methods instead of the autoCalibrate...() methods below
  //    CurieImu.setXGyroOffset(220);
  //    CurieImu.setYGyroOffset(76);
  //    CurieImu.setZGyroOffset(-85);
  //    CurieImu.setXAccelOffset(-76);
  //    CurieImu.setYAccelOffset(-235);
  //    CurieImu.setZAccelOffset(168);
  
  //Serial.println("About to calibrate. Make sure your board is stable and upright");
  //delay(5000);

  if(calibrateOffsets == 1) {
    // The board must be resting in a horizontal position for 
    // the following calibration procedure to work correctly!
    //Serial.print("Starting Gyroscope calibration...");
    CurieImu.autoCalibrateGyroOffset();
    //Serial.println(" Done");
    //Serial.print("Starting Acceleration calibration...");
    CurieImu.autoCalibrateXAccelOffset(0);
    CurieImu.autoCalibrateYAccelOffset(0);
    CurieImu.autoCalibrateZAccelOffset(1);
    //Serial.println(" Done");
  
    //Serial.println("Internal sensor offsets AFTER calibration...");
    //Serial.print(CurieImu.getXAccelOffset());
    //Serial.print("\t"); // -76
    //Serial.print(CurieImu.getYAccelOffset());
    //Serial.print("\t"); // -2359
    //Serial.print(CurieImu.getZAccelOffset());
    //Serial.print("\t"); // 1688
    //Serial.print(CurieImu.getXGyroOffset());
    //Serial.print("\t"); // 0
    //Serial.print(CurieImu.getYGyroOffset());
    //Serial.print("\t"); // 0
    //Serial.println(CurieImu.getZGyroOffset());
  
    //Serial.println("Enabling Gyroscope/Acceleration offset compensation");
    CurieImu.setGyroOffsetEnabled(true);
    CurieImu.setAccelOffsetEnabled(true);
  }
  
  

  // configure Arduino LED for activity indicator
  pinMode(ledPin, OUTPUT);
}

void loop() {
   
  smartDelay(300);
  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));

  // read raw accel/gyro measurements from device
  //CurieImu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  // read raw accel/gyro measurements from device
   ax = CurieImu.getAccelerationX();
   ay = CurieImu.getAccelerationY();
   az = CurieImu.getAccelerationZ();
   gx = CurieImu.getRotationX();
   gy = CurieImu.getRotationY();
   gz = CurieImu.getRotationZ();

   // use function from MagdwickAHRS.h to return quaternions
   filter.updateIMU(gx / factor, gy / factor , gz / factor , ax, ay, az);

   // functions to find yaw roll and pitch from quaternions

   yaw = filter.getYaw()*rad2deg;
   roll = filter.getRoll()*rad2deg;
   pitch = filter.getPitch()*rad2deg;
   val[0] = yaw;
   val[1] = pitch;
   val[2] = roll;
 
  //Serial.print(yaw);
  //Serial.print("\t");
  //Serial.print(roll);
  //Serial.print("\t");
  //Serial.println(pitch);
   
   
  // these methods (and a few others) are also available
  //CurieImu.getAcceleration(&ax, &ay, &az);
  //CurieImu.getRotation(&gx, &gy, &gz);

  // display tab-separated accel/gyro x/y/z values
  //Serial.print("a/g:\t");
  //Serial.print(ax);
  //Serial.print("\t");
  //Serial.print(ay);
  //Serial.print("\t");
  //Serial.print(az);
  //Serial.print("\t");
  //Serial.print(gx);
  //Serial.print("\t");
  //Serial.print(gy);
  //Serial.print("\t");
  //Serial.println(gz);
  
  //XBeeSerialPrintInt(ax);  XBeeSerialPrintInt(ay);  XBeeSerialPrintInt(az);
  //XBeeSerialPrintInt(gx);  XBeeSerialPrintInt(gy);  XBeeSerialPrintInt(gz);
  //XBeeSerialPrintFloat(yaw);  XBeeSerialPrintFloat(pitch);  XBeeSerialPrintFloat(roll);
 
  //XBeeSerialPrintFloat(gps.location.lat()); XBeeSerialPrintFloat(gps.location.lng());
  //XBeeSerialPrintFloat(gps.altitude.meters()); XBeeSerialPrintFloat(gps.course.deg());
  //XBeeSerialPrintFloat(gps.speed.kmph()); 

  val[3] = gps.location.lat();
  val[4] = gps.location.lng();  
  val[5] = gps.altitude.meters();
  val[6] = gps.course.deg();
  val[7] = gps.speed.kmph();

  serialPrintFloatArr(val,3);
  Serial.print('\n');
  
  //Serial.println();strcat(Message, "\n");Serial.println(Message);
  //sendPayload();
  //Message[0] = (char)0;  

  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(ledPin, blinkState);
}

void XBeeSerialPrintFloat(float floatVal) {

  dtostrf(floatVal, 4, 4, charBuf);  //4 is mininum width, 4 is precision; float value is copied onto buff
  //display character array
  strcat(Message, charBuf); strcat(Message,",");
  //Serial.print("stringVal: ");Serial.println(stringVal); //display string
}

void XBeeSerialPrintInt(int Val) {

  dtostrf((float) Val, 6, 0, charBuf);  //4 is mininum width, 4 is precision; float value is copied onto buff
  //display character array
  strcat(Message, charBuf); strcat(Message,",");
  //Serial.print("stringVal: ");Serial.println(stringVal); //display string

}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (gpsSerial.available())
      gps.encode(gpsSerial.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}

void configureUblox(byte *settingsArrayPointer) {
    byte gpsSetSuccess = 0;
    //Serial.println("Configuring u-Blox GPS initial state...");

    //Generate the configuration string for loading from i2c eeprom
    byte setCFG[] = {0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x04, 0x1E,
  0xB4 };
    calcChecksum(&setCFG[2], sizeof(setCFG) - 4);

    delay(2500);

    gpsSetSuccess = 0;
    while(gpsSetSuccess < 3) {
      //Serial.print("Loading permanent configuration... ");
      sendUBX(&setCFG[0], sizeof(setCFG));  //Send UBX Packet
      gpsSetSuccess += getUBX_ACK(&setCFG[2]); 
      //Passes Class ID and Message ID to the ACK Receive function      
      if (gpsSetSuccess == 10) gpsStatus[1] = true;
      if (gpsSetSuccess == 5 | gpsSetSuccess == 6) gpsSetSuccess -= 4;
    }
    
    if (gpsSetSuccess == 3) Serial.println("Config update failed.");
      gpsSetSuccess = 0;
}

void calcChecksum(byte *checksumPayload, byte payloadSize) {
    byte CK_A = 0, CK_B = 0;
    for (int i = 0; i < payloadSize ;i++) {
      CK_A = CK_A + *checksumPayload;
      CK_B = CK_B + CK_A;
      checksumPayload++;
    }
    *checksumPayload = CK_A;
    checksumPayload++;
    *checksumPayload = CK_B;
}

void sendUBX(byte *UBXmsg, byte msgLength) {
    for(int i = 0; i < msgLength; i++) {
      gpsSerial.write(UBXmsg[i]);
      gpsSerial.flush();
    }
    gpsSerial.println();
    gpsSerial.flush();
}

byte getUBX_ACK(byte *msgID) {
    byte CK_A = 0, CK_B = 0;
    byte incoming_char;
    boolean headerReceived = false;
    unsigned long ackWait = millis();
    byte ackPacket[10] = {0xB5, 0x62, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    int i = 0;
      while (1) {
        if (gpsSerial.available()) {
          incoming_char = gpsSerial.read();
          if (incoming_char == ackPacket[i]) {
            i++;
          } else if (i > 2) {
              ackPacket[i] = incoming_char;
              i++;
           }
  }
        if (i > 9) break;
        if ((millis() - ackWait) > 1500) {
          //Serial.println("ACK Timeout");
          return 5;
  }
  if (i == 4 && ackPacket[3] == 0x00) {
    //Serial.println("NAK Received");
    return 1;
        }
      }

      for (i = 2; i < 8 ;i++) {
        CK_A = CK_A + ackPacket[i];
        CK_B = CK_B + CK_A;
      }
  
      if(msgID[0] == ackPacket[6] && msgID[1] == ackPacket[7] && CK_A == ackPacket[8] && CK_B == ackPacket[9]) {
        //Serial.println("Success!");
        //Serial.print("ACK Received! ");
        //printHex(ackPacket, sizeof(ackPacket));
        return 10;
      } else {
          //Serial.print("ACK Checksum Failure: ");
          //printHex(ackPacket, sizeof(ackPacket));
          delay(1000);
          return 1;
        }
}

/*
void XBeeSerialPrintFloatArr(float * arr, int len) {

  for(uint8_t i=0; i<len; i++) {
    XBeeSerialFloatPrint(arr[i]);
    strcat(Message, ",");
  }
}


void XBeeSerialFloatPrint(float f) {
  byte * b = (byte *) &f;
  for(uint8_t i=0; i<4; i++) {
      byte b1 = (b[i] >> 4) & 0x0f;
      byte b2 = (b[i] & 0x0f);
    
      char c1 = (b1 < 10) ? ('0' + b1) : 'A' + b1 - 10;
      char c2 = (b2 < 10) ? ('0' + b2) : 'A' + b2 - 10;
      
      String(c1).toCharArray(charBuf, 2); strcat(Message, charBuf);
      String(c2).toCharArray(charBuf, 2); strcat(Message, charBuf);
    }
}

void sendPayload(){
  ZBTxRequest zbtx = ZBTxRequest(Broadcast, (uint8_t *) Message, strlen(Message));
  xbee.send(zbtx);
}


 //void getMessage(uint8_t *data, uint8_t length){
 void getMessage(){   

       //Serial.print("Frame Type is ");
       //Serial.println(xbee.getResponse().getApiId(), HEX);
       if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
           xbee.getResponse().getZBRxResponse(rx);
          if (rx.getOption() & ZB_PACKET_ACKNOWLEDGED)
              //the sender got an ACK
             //Serial.println("packet acknowledged");
         
         cmd = "";
         // this is the actual data you sent
         //Serial.println("Received Data: ");
         for (int i = 0; i < rx.getDataLength(); i++) {
           //print8Bits(rx.getData()[i]);
           cmd1[i] = (char) rx.getData()[i];
           cmd += cmd1[i];
         }
         //Serial.println(cmd);
         //Serial.println();
       } 
 }
*/
