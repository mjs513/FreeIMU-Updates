#include <AP_Math_freeimu.h>
#include <Butter.h>    // Butterworth filter
#include <iCompass.h>
#include <MovingAvarageFilter.h>

/**
 * FreeIMU library serial communication protocol
*/

#include <HMC58X3.h>
#include <MS561101BA.h> //Comment out for APM 2.5
#include <I2Cdev.h>
#include <MPU60X0.h> 
#include <AP_Baro_MS5611.h>  //Uncomment for APM2.5

//#define DEBUG
#include "DebugUtils.h"
#include "CommunicationUtils.h"
#include "FreeIMU.h"
#include "FilteringScheme.h"
#include "RunningAverage.h"

#include <Wire.h>
#include <SPI.h>

#define BaudRate 57600

#define HAS_GPS 1
#define gpsSerial Serial1
static const unsigned long GPSBaud = 57600;

#define HAS_telem 1
#define telemSerial Serial2
#define telemBaud 57600

uint8_t count = 64;
float q[4];
int raw_values[11];
float ypr[3]; // yaw pitch roll
char str[128];
float val[12];
float val_array[19]; 

// Set the FreeIMU object and LSM303 Compass
FreeIMU my3IMU = FreeIMU();

#if HAS_GPS
  #include <TinyGPS++.h>
  // The TinyGPS++ object
  TinyGPSPlus gps;
  
  // Setup GPS Serial and load config from i2c eeprom
  boolean gpsStatus[] = {false, false, false, false, false, false, false};
  unsigned long start;
#endif

#if HAS_telem
  #include <XBee.h>

  XBee xbee = XBee();

  XBeeResponse response = XBeeResponse();
  // create reusable response objects for responses we expect to handle
  ZBRxResponse rx = ZBRxResponse();
  //Address set to receiving Xbee (one attached to PC)
  XBeeAddress64 Broadcast = XBeeAddress64(0x0013A200, 0x40C04290);
  char Message[230], charBuf[2];

#endif 

//The command from the PC
//char cmd, tempCorr;
char tempCorr;
char cmd1[9];
String cmd, Payload;


void setup() {
  Serial.begin(BaudRate);
  Wire.begin();
  #if HAS_telem
    telemSerial.begin(telemBaud);
    xbee.setSerial(telemSerial);
    xbee.setAPImode(1);
  #endif
  
  my3IMU.init(true);

  #if HAS_GPS
    // For Galileo,DUE and Teensy use Serial port 1
    //Load configuration from i2c eeprom - this assumes you have saved
    //a default configuration to the eeprom or permanent storage.
    //If you do not have this setup you will have to remove the
    //following lines and the additional code at the end of the sketch.
    gpsSerial.begin(9600);
    
    //Settings Array
    //Code based on http://playground.arduino.cc/UBlox/GPS
    byte settingsArray[] = {0x04}; // Not really used for this example
    configureUblox(settingsArray);
    
    //Retain this line
    gpsSerial.begin(GPSBaud); 
  #endif
  

  // LED
  pinMode(13, OUTPUT);
}

void loop() {
  #if HAS_telem
    xbee.readPacket();
    if (xbee.getResponse().isAvailable()) {
      cmd1[0] = '\0';
      getMessage();
      } else if (xbee.getResponse().isError()) {
        // some kind of error happened, I put the stars in so
        // it could easily be found
        //Serial.print("************************************* error code:");
        //Serial.println(xbee.getResponse().getErrorCode(),DEC);
       }  
  #else
    if(Serial.available()) {
      cmd1[0] = '\0';
      cmd1[0] = Serial.read(); }
  #endif
    if(cmd1[0]=='v') {
      sprintf(str, "FreeIMU library by %s, FREQ:%s, LIB_VERSION: %s, IMU: %s", FREEIMU_DEVELOPER, FREEIMU_FREQ, FREEIMU_LIB_VERSION, FREEIMU_ID);
      #if HAS_telem
        strcpy(Message, str);
        sendPayload();
      #else
        Serial.print(str);
        Serial.print('\n');
      #endif
    }
    else if(cmd1[0]=='1'){
      my3IMU.init(true);
    }
    else if(cmd1[0]=='2'){
      my3IMU.RESET_Q();           
    }
    else if(cmd1[0]=='g'){
      my3IMU.initGyros();
      //my3IMU.zeroGyro();      
    }
    else if(cmd1[0]=='t'){
      //available opttions temp_corr_on, instability_fix
      my3IMU.setTempCalib(1);   
    }
    else if(cmd1[0]=='f'){
      //available opttions temp_corr_on, instability_fix
      my3IMU.initGyros();
      my3IMU.setTempCalib(0);
    }
    else if(cmd1[0]=='p'){
      //set sea level pressure
      long sea_press = Serial.parseInt();        
      my3IMU.setSeaPress(sea_press/100.0);
      //Serial.println(sea_press);
    }	
    else if(cmd1[0]=='r') {
      //uint8_t count = serial_busy_wait();
      for(uint8_t i=0; i<count; i++) {
        //my3IMU.getUnfilteredRawValues(raw_values);
        my3IMU.getRawValues(raw_values);
        sprintf(str, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,", raw_values[0], raw_values[1], raw_values[2], raw_values[3], raw_values[4], raw_values[5], raw_values[6], raw_values[7], raw_values[8], raw_values[9]);
        Serial.print(str);
        #if (HAS_MS5611() || HAS_BMP085() || HAS_LPS331())
          Serial.print(my3IMU.getBaroTemperature()); Serial.print(",");
          Serial.print(my3IMU.getBaroPressure()); Serial.print(",");
        #endif
        Serial.print(millis()); Serial.print(",");
        Serial.println("\r\n");
     }
    }
    else if(cmd1[0]=='b') {
      //uint8_t count = serial_busy_wait();
      for(uint8_t i=0; i<count; i++) {
        #if HAS_ITG3200()
          my3IMU.acc.readAccel(&raw_values[0], &raw_values[1], &raw_values[2]);
          my3IMU.gyro.readGyroRaw(&raw_values[3], &raw_values[4], &raw_values[5]);
          writeArr(raw_values, 6, sizeof(int)); // writes accelerometer, gyro values & mag if 9150
        #elif HAS_MPU9150()  || HAS_MPU9250()
          my3IMU.getRawValues(raw_values);
          writeArr(raw_values, 9, sizeof(int)); // writes accelerometer, gyro values & mag if 9150
        #elif HAS_MPU6050() || HAS_MPU6000()   // MPU6050
          //my3IMU.accgyro.getMotion6(&raw_values[0], &raw_values[1], &raw_values[2], &raw_values[3], &raw_values[4], &raw_values[5]);
          my3IMU.getRawValues(raw_values);
          writeArr(raw_values, 6, sizeof(int)); // writes accelerometer, gyro values & mag if 9150
        #elif HAS_ALTIMU10()
          my3IMU.getRawValues(raw_values);
          writeArr(raw_values, 9, sizeof(int)); // writes accelerometer, gyro values & mag of Altimu 10        
        #endif
        //writeArr(raw_values, 6, sizeof(int)); // writes accelerometer, gyro values & mag if 9150
        
        #if IS_9DOM() && (!HAS_MPU9150()  && !HAS_MPU9250() && !HAS_ALTIMU10())
          my3IMU.magn.getValues(&raw_values[0], &raw_values[1], &raw_values[2]);
          writeArr(raw_values, 3, sizeof(int));
        #endif
        Serial.println();
      }
    }
    else if(cmd1[0] == 'z') {
      float val_array[19] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
      //uint8_t count = serial_busy_wait();
      for(uint8_t i=0; i<count; i++) {
        my3IMU.getQ(q, val);
		val_array[15] = my3IMU.sampleFreq;        
        //my3IMU.getValues(val);       
        val_array[7] = (val[3] * M_PI/180);
        val_array[8] = (val[4] * M_PI/180);
        val_array[9] = (val[5] * M_PI/180);
        val_array[4] = (val[0]);
        val_array[5] = (val[1]);
        val_array[6] = (val[2]);
        val_array[10] = (val[6]);
        val_array[11] = (val[7]);
        val_array[12] = (val[8]);
        val_array[0] = (q[0]);
        val_array[1] = (q[1]);
        val_array[2] = (q[2]);
        val_array[3] = (q[3]);
        //val_array[15] = millis();
        val_array[16] = val[9];
        val_array[18] = val[11];
	
        #if HAS_PRESS()
           // with baro
           val_array[17] = val[10];
           val_array[13] = (my3IMU.getBaroTemperature());
           val_array[14] = (my3IMU.getBaroPressure());
        #elif HAS_MPU6050()
           val_array[13] = (my3IMU.DTemp/340.) + 35.;
		#elif HAS_MPU9150()  || HAS_MPU9250()
           val_array[13] = ((float) my3IMU.DTemp) / 333.87 + 21.0;
        #elif HAS_ITG3200()
           val_array[13] = my3IMU.rt;
        #endif
		
        #if HAS_telem
          //serialPrintFloatArr(val_array,19);
          //Serial.println('\n');
          Message[0] = '\0';
          XBeeSerialPrintFloatArr(val_array,19);
        #else
          serialPrintFloatArr(val_array,19);
        #endif
        
        #if HAS_GPS
          val_array[0] = (float) gps.hdop.value();
          val_array[1] = (float) gps.location.lat();
          val_array[2] = (float) gps.location.lng();
          val_array[3] = (float) gps.altitude.meters();
          val_array[4] = (float) gps.course.deg();
          val_array[5] = (float) gps.speed.kmph();
		  
          #if HAS_telem
	    XBeeSerialPrintFloatArr(val_array,6);
          #else
	    serialPrintFloatArr(val_array,6);
	    Serial.print('\n');			
          #endif
          smartDelay(20);
        #else
          #if HAS_telem
	    strcat(Message, "\n");
	  #else
            Serial.print('\n');
	  #endif
        #endif
		
        #if HAS_telem
	  sendPayload();
          Serial.print(Message);
        #endif
      }
    }    
  cmd1[0] = '\0';
}

char serial_busy_wait() {
  #if !HAS_telem
    while(!Serial.available()) {
      ; // do nothing until ready
    }
    return Serial.read();
  #else
    delay(1);
  #endif
}


// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  #if HAS_GPS
  unsigned long start = millis();
  do 
  {
    while (gpsSerial.available())
      gps.encode(gpsSerial.read());
  } while (millis() - start < ms);
  #endif
}

#if HAS_GPS

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

#endif

#if HAS_telem
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

 void getMessage(){   
       //Serial.print("Frame Type is ");
       //Serial.println(xbee.getResponse().getApiId(), HEX);
       if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
           xbee.getResponse().getZBRxResponse(rx);
          if (rx.getOption() & ZB_PACKET_ACKNOWLEDGED)
              //the sender got an ACK
              //Serial.println("packet acknowledged");
         
         cmd = "";
	 cmd1[0] = '\0';
         // this is the actual data you sent
         //Serial.println("Received Data: ");
         for (int i = 0; i < rx.getDataLength(); i++) {
           //print8Bits(rx.getData()[i]);
           cmd1[i] = (char) rx.getData()[i];
           cmd += cmd1[i];
         }
         //Serial.println(cmd1[0]);
         //Serial.println();
       } 
 }

 void sendPayload(){
	ZBTxRequest zbtx = ZBTxRequest(Broadcast, (uint8_t *) Message, strlen(Message));
	xbee.send(zbtx);
 }
 
#endif
