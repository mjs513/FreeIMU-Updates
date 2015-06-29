#include <AP_Math_freeimu.h>
#include <Filter.h>    // Filter library
#include <Butter.h>    // Butterworth filter
#include <iCompass.h>

#include <ADXL345.h>
#include <bma180.h>
#include <HMC58X3.h>
//#include <FastSerial.h>
#include <ITG3200.h>
#include <MS561101BA.h>
#include <I2Cdev.h>
#include <MPU60X0.h>
#include <EEPROM.h>
//#include <AP_GPS_UBLOX.h>

#include "FilteringScheme.h"
#include "RunningAverage.h"
#include <MovingAvarageFilter.h>

//#define DEBUG
#include "DebugUtils.h"
#include "CommunicationUtils.h"
#include "FreeIMU.h"
#include <Wire.h>
#include <SPI.h>

#define SOFTWARE_VER "1.9"
unsigned int imu_health = 65012;

#define PRINT_MAGNETOMETER 1
#define PRINT_AccelGyro    0

float test = 0.000;
float euler_x;
float euler_y;
float euler_z;

float q[4], val[11];
int rawvals[11];

long convert_to_dec(float x)
{
  return x*10000000;
}

// Set the FreeIMU object
FreeIMU my3IMU = FreeIMU();

void setup() {
  //Serial.begin(115200);
  Serial.begin(38400);
  Wire.begin();
  
  delay(5);
  my3IMU.init();
  delay(5);
}


void loop() { 
	my3IMU.getQ(q, val);
	//my3IMU.getRawValues(rawvals);
 
	float sqw = q[0]*q[0];
	float sqx = q[1]*q[1];
	float sqy = q[2]*q[2];
	float sqz = q[3]*q[3];

	// q0 = q3, q1 = q0x    q2 = q1y   q3 = q2z
	float DCM_Matrix[3][3]=
	{		
		{(sqw+sqx-sqy-sqz), (2*q[1]*q[2]+2*q[0]*q[3]), (2*q[1]*q[3]-2*q[0]*q[2])},
		{(2*q[1]*q[2]-2*q[0]*q[3]), (sqw-sqx+sqy-sqz), (2*q[2]*q[3]+2*q[0]*q[1])},
		{(2*q[1]*q[3]+2*q[0]*q[2]), (2*q[2]*q[3]-2*q[0]*q[1]), (sqw-sqx-sqy+sqz)}  
	};

	if (DCM_Matrix[1][0] > 0.998){
		euler_y = atan2(DCM_Matrix[0][2], DCM_Matrix[2][2]);
		euler_z = M_PI/2;
		euler_x = 0;
	}
	if (DCM_Matrix[1][0] < -0.998){
		euler_y = atan2(DCM_Matrix[0][2], DCM_Matrix[2][2]);
		euler_z = -M_PI/2;
		euler_x = 0;
	}

    euler_x = atan2(DCM_Matrix[1][0],DCM_Matrix[0][0]); //yaw
    euler_y = atan2(DCM_Matrix[1][2], DCM_Matrix[2][2]); //roll 
    euler_z = -asin(DCM_Matrix[0][2]);  //pitch
  
    Serial.print("!");
    Serial.print("!!VER:");
    Serial.print(SOFTWARE_VER);  //output the software version
    Serial.print(",");
	
    #if PRINT_AccelGyro == 1
        Serial.print("AN0:");
        Serial.print(rawvals[0]); //Reversing the sign. 
        Serial.print(",AN1:");
        Serial.print(rawvals[1]);
        Serial.print(",AN2:");
        Serial.print(rawvals[2]);  
        Serial.print(",AN3:");
        Serial.print(rawvals[3]);
        Serial.print (",AN4:");
        Serial.print(rawvals[4]);
        Serial.print (",AN5:");
        Serial.print(rawvals[5]);
        Serial.print (",");
    #endif	
	
    Serial.print ("EX0:");
    Serial.print(convert_to_dec(DCM_Matrix[0][0]));
    Serial.print (",EX1:");
    Serial.print(convert_to_dec(DCM_Matrix[0][1]));
    Serial.print (",EX2:");
    Serial.print(convert_to_dec(DCM_Matrix[0][2]));
    Serial.print (",EX3:");
    Serial.print(convert_to_dec(DCM_Matrix[1][0]));
    Serial.print (",EX4:");
    Serial.print(convert_to_dec(DCM_Matrix[1][1]));
    Serial.print (",EX5:");
    Serial.print(convert_to_dec(DCM_Matrix[1][2]));
    Serial.print (",EX6:");
    Serial.print(convert_to_dec(DCM_Matrix[2][0]));
    Serial.print (",EX7:");
    Serial.print(convert_to_dec(DCM_Matrix[2][1]));
    Serial.print (",EX8:");
    Serial.print(convert_to_dec(DCM_Matrix[2][2]));
    Serial.print (",");
    Serial.print("RLL:");
    //Serial.print(roll * 180/M_PI);
    Serial.print(-euler_y *180/M_PI);
    Serial.print(",PCH:");
    //Serial.print(pitch * 180/M_PI);
    Serial.print(-euler_z*180/M_PI);
    Serial.print(",YAW:");
    //Serial.print(yaw * 180/M_PI);		
    Serial.print(-euler_x*180/M_PI);
    //Serial.print(ToDeg(yaw));
    Serial.print(",IMUH:");
    Serial.print((imu_health>>8)&0xff);
    Serial.print (",");
    //Serial.print("TOW:");
    //Serial.print(test);
    //Serial.println("***");

    #if PRINT_MAGNETOMETER == 1
        Serial.print("MGX:");
        Serial.print(val[6]);
        Serial.print (",MGY:");
        Serial.print(val[7]);
        Serial.print (",MGZ:");
        Serial.print(val[8]);
        Serial.print (",MGH:");
        Serial.print(val[9]);
        Serial.print (",");
    #endif	

    Serial.print("TOW:");
    Serial.print(test);
    Serial.println("***");	
	
    delay(20);

}

 

