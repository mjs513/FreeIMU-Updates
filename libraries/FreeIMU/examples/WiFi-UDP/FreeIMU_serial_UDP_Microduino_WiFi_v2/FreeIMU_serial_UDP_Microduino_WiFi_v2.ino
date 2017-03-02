/**
 * FreeIMU library serial communication protocol
 * validated on LinkNode D1
*/
//These are optional depending on your IMU configuration
#include <I2Cdev.h>
#include <HMC58X3.h>
#include <BMP085.h>
#include <MPU60X0.h>


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

#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <elapsedMillis.h>

elapsedMillis sendData1;

char ssid[] = "ssid";            // your network SSID (name)
char pass[] = "password";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

unsigned int localPort = 4210;  // local port to listen on
unsigned int localPortS = 4211;
char packetBuffer[255];  // buffer for incoming packets
String payload;

//The command from the PC
char tempCorr;
String cmd;
uint8_t cmd1;

WiFiEspUDP Udp;
WiFiEspUDP UdpS;

char ReplyBuffer[] = "ACK";      // a string to send back
#define sendInterval 50

void setup() {
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(115200);
  
  Wire.begin();
  Wire.setClock(400000L);

  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  
  Serial.println("Connected to wifi");
  printWifiStatus();
  
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
  UdpS.begin(localPortS);
  
  Serial.print("Listening on port(s) ");
  Serial.println(localPort);
  Serial.println(localPortS);
  
  my3IMU.init(true);

  Serial.println("Ready for commands");
  // LED
  pinMode(13, OUTPUT);  //
  //pinMode(5, OUTPUT);  //for eps32 thing from sparkfun
}

void loop() {

  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());
    
    int len = Udp.read(packetBuffer, 255);
    if (len > 0)
    {
      packetBuffer[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    	
    cmd = packetBuffer[0];
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
      WiFi.disconnect();
      Udp.stop();
      Serial.println("Disconnected");
    }
    else if(cmd=="s"){
      cmd1 = 1;
    }
  }
    
    if(!UdpS.remotePort()){
        //nobody have connected yet
        return;
    }
    
  if(sendData1 > sendInterval){
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
        char message[payloadLen];
        payload.toCharArray(message, payloadLen);

        // send a reply, to the IP address and port that sent us the packet we received
        UdpS.beginPacket(UdpS.remoteIP(), UdpS.remotePort());
        UdpS.write(message,sizeof(message));
        UdpS.endPacket();

        sendData1 = 0;
  }
  //delay(sendInterval);

}


//==========================================================
void serialPayloadFloatArr(float * arr, int length) {
  for(int i=0; i<length; i++) {
    serialPayloadPrint(arr[i]);
    payload += ",";
  }
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


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
