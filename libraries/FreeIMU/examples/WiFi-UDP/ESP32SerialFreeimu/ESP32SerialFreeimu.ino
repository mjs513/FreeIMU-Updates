/**
 * FreeIMU library UDP communication protocol
 * Validated with ESP32 Dev Mod by Onehorse on Tindie
 */
//These are optional depending on your IMU configuration

#include <ADXL345.h>
#include <HMC58X3.h>
#include <LSM303.h>
#include <LPS.h> 
#include <L3G.h>
#include <ITG3200.h> //note LPS library must come before ITG lib
#include <bma180.h>
#include <MS561101BA.h> //Comment out for APM 2.5
#include <BMP085.h>
#include <I2Cdev.h>
#include <MPU60X0.h>
#include <AK8975.h>
#include <AK8963.h>
//#include <SparkFunLSM9DS1.h>  // Uncomment for LSM9DS1
#include <SFE_LSM9DS0.h>  // Uncomment for LSM9DS0 Chosse one or the othe ST IMUs
#include <BaroSensor.h>
//#include <AP_Baro_MS5611.h>  //Uncomment for APM2.5


//These are mandatory
#include <AP_Math_freeimu.h>
#include <Butter.h>    // Butterworth filter
#include <iCompass.h>
#include <MovingAvarageFilter.h>

#include <Wire.h>
#include <SPI.h>

//#define DEBUG
#include "DebugUtils.h"
#include "FreeIMU.h"
#include "DCM.h"
#include "FilteringScheme.h"
#include "RunningAverage.h"


//Intel Edison, Arduino 101, Arduino Due, Arduino Zero: no eeprom 
#if defined(__SAMD21G18A__) || defined(__SAM3X8E__) || defined(__ARDUINO_ARC__) || defined(__SAMD21G18A__) || defined(ESP32)
#define HAS_EEPPROM 0
#else
#include <EEPROM.h>
#define HAS_EEPPROM 1
#endif

#define M_PI 3.14159
#define BaudRate 115200


float q[4];
int raw_values[11];
float ypr[3]; // yaw pitch roll
char str[128];
float val[14];
float val_array[21]; 


// Set the FreeIMU object and LSM303 Compass
FreeIMU my3IMU = FreeIMU();

//#include <ESP8266WiFi.h>  //use for ESP8266
#include <WiFi.h>       // use for ESP32
#include <WiFiUdp.h>

#define sendInterval 50

const char* ssid = "CyberPalin";
const char* password = "michaelmariano";
WiFiUDP Udp;
static IPAddress remoteIp = IPAddress();
static uint16_t remotePort = 4210;

unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[256];  // buffer for incoming packets
String payload;

//The command from the PC
char tempCorr;
String cmd;

void setup() {
    Serial.begin(BaudRate);
    Wire.begin(21, 22, 4000000);
    //Wire.setClock(400000L);

    Serial.println();
    Serial.printf("Connecting to %s ", ssid);
    
    // delete old config
    WiFi.disconnect(true);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" connected");
    Udp.begin(localUdpPort);
    Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
    WiFi.localIP().toString().c_str(), localUdpPort;
    
  
    //#if HAS_MPU6050()
    //    my3IMU.RESET();
    //#endif
    
    my3IMU.init(true);
    

    // LED
    //pinMode(13, OUTPUT);  //
    pinMode(5, OUTPUT);  //for eps32 thing from sparkfun
}

void serialPayloadPrint(float f) {
    byte * b = (byte *) &f;
    for(int i=0; i<4; i++) {
        
        byte b1 = (b[i] >> 4) & 0x0f;
        byte b2 = (b[i] & 0x0f);
        
        char c1 = (b1 < 10) ? ('0' + b1) : 'A' + b1 - 10;
        char c2 = (b2 < 10) ? ('0' + b2) : 'A' + b2 - 10;
        
        payload += (c1);
        payload += (c2);
    }
}

void serialPayloadFloatArr(float * arr, int length) {
    for(int i=0; i<length; i++) {
        serialPayloadPrint(arr[i]);
        payload += ",";
    }
}

void loop() {
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        // receive incoming UDP packets
        remoteIp = Udp.remoteIP();
        remotePort = Udp.remotePort();
        Serial.printf("Received %d bytes from %s, port %d\n", packetSize, remoteIp.toString().c_str(), remotePort);
        int len = Udp.read(incomingPacket, 255);
        incomingPacket[len] = 0;
        Serial.printf("UDP packet contents: %s\n", incomingPacket);
        
        cmd = incomingPacket[0];
        if(cmd=="v") {
            sprintf(str, "FreeIMU library by %s, FREQ:%s, LIB_VERSION: %s, IMU: %s", FREEIMU_DEVELOPER, FREEIMU_FREQ, FREEIMU_LIB_VERSION, FREEIMU_ID);
            Serial.print(str);
            Serial.print('\n');
        }
        else if(cmd=="1"){
            my3IMU.init(true);
        }
        else if(cmd=="2"){
            my3IMU.RESET_Q();           
        }
        else if(cmd=="g"){
            my3IMU.initGyros();   
        }
        else if(cmd=="t"){
            //available opttions temp_corr_on, instability_fix
            my3IMU.setTempCalib(1);   
        }
        else if(cmd=="f"){
            //available opttions temp_corr_on, instability_fix
            my3IMU.initGyros();
            my3IMU.setTempCalib(0);
        }
        else if(cmd=="x"){
        WiFi.disconnect(true);
        Serial.println("Disconnected");
      }
    }
    
    if(!remotePort){
        //nobody have connected yet
        return;
    }
    
    float val_array[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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
    val_array[19] = val[12];
    
#if HAS_PRESS()
    // with baro
    val_array[17] = val[10];
    val_array[13] = (my3IMU.getBaroTemperature());
    val_array[14] = (my3IMU.getBaroPressure());
    #elif HAS_MPU6050()
        val_array[13] = (my3IMU.DTemp/340.) + 35.;
    #elif HAS_MPU9150()  || HAS_MPU9250()
        val_array[13] = ((float) my3IMU.DTemp) / 333.87 + 21.0;
    #elif HAS_LSM9DS0()
        val_array[13] = 21.0 + (float) my3IMU.DTemp/8.; //degrees C
    #elif HAS_ITG3200()
        val_array[13] = my3IMU.rt;
    #elif HAS_CURIE()
        val_array[13] = (my3IMU.DTemp/512.0) + 23.0;
    #endif
    
    payload = "";
    serialPayloadFloatArr(val_array, 20);
    payload += "\r\n";
  
    int payloadLen = payload.length();
    byte message[payloadLen];  
    payload.getBytes(message,payloadLen);
    
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(message,sizeof(message));
    Udp.endPacket();

   delay(sendInterval);
}
