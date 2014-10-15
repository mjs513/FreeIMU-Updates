/**
Visualize a cube which will assumes the orientation described
in a quaternion coming from the serial port.

INSTRUCTIONS: 
This program has to be run when you have the FreeIMU_serial
program running on your Arduino and the Arduino connected to your PC.
Remember to set the serialPort variable below to point to the name the
Arduino serial port has in your system. You can get the port using the
Arduino IDE from Tools->Serial Port: the selected entry is what you have
to use as serialPort variable.


Copyright (C) 2011-2012 Fabio Varesano - http://www.varesano.net/

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

import processing.serial.*;
import processing.opengl.*;

Serial myPort;  // Create object from Serial class

final String serialPort = "COM4"; // replace this with your serial port. On windows you will need something like "COM1".
int BaudRate=57600;

//Settup Stop Watch
StopWatchTimer sw = new StopWatchTimer();

// These are needed for the moving average calculation
float[] data = new float[32];
float total = 0, average = 0;
int p = 0, n = 0;

//LPF
float filterFactor = 0.05;

//Moving average Heading
float corr_heading;

//set motiondetect types
float accnorm,accnorm_var_test;
int accnorm_test, omegax, omegay, omegaz, omega_test, motionDetect;

//Movingaverage filters for motion detection
//float accnorm_test_avg, accnorm_var,motion_detect_ma, accnorm_avg;

MovingAverage accnorm_test_avg = new MovingAverage(5);
MovingAverage accnorm_var = new MovingAverage(7);
MovingAverage motion_detect_ma = new MovingAverage(7);
MovingAverage accnorm_avg = new MovingAverage(5);

float [] q = new float [4];
float [] acc = new float [3];
float [] gyro = new float [3];
float [] magn = new float [3];
float [] ypr = new float [3];
float temp; float press; float altitude; 
float dt, heading;

// Altitude - Accel Complimentary filter setup
float[] dyn_acc = new float[3];
float fused_alt;

float S;
float A;

String seapresscmd = "99";
float STATIONALTFT = 36.0;
float sea_press = 1013.25;            //Input local sea level pressure
float declinationAngle = -13.1603;   //Flushing, NY magnetic declination in degrees
float SEA_PRESS  = 1013.25;          //default sea level pressure level in mb
float KNOWNALT   = 65.0;            //default known altitude, 
float INHG       = 0.02952998751;    //convert mb to in/Hg constant
float MB         = 33.8638815;       //convert in/Hg to mb constant
float FTMETERS   = 0.3048;
float METERS2FT  = 3.2808399;
float PI         = 3.14159;

float [] hq = null;
float [] Euler = new float [3]; // psi, theta, phi

int lf = 10; // 10 is '\n' in ASCII
byte[] inBuffer = new byte[22]; // this is the number of chars on each line from the Arduino (including /r/n)

//---------------------------------------------------
float Sample_X;
float Sample_Y; 
float Sample_Z; 
long [] Sensor_Data = new long [8]; 
short countx; short county ;

float [] accelerationx = new float [2];
float [] accelerationy = new float [2];
float [] velocityx = new float [2];
float [] velocityy = new float [2];
float [] positionX= new float [2]; 
float [] positionY = new float [2]; 
float [] positionZ = new float [2]; 
long direction; 
float sstatex; float sstatey;

int calib = 1;
int cube_odo = 0;
//-------------------------------------

PFont font;
final int VIEW_SIZE_X = 900, VIEW_SIZE_Y = 600;

final int burst = 32;
int count = 0;

void myDelay(int time) {
  try {
    Thread.sleep(time);
  } catch (InterruptedException e) { }
}

void setup() 
{
  size(VIEW_SIZE_X, VIEW_SIZE_Y, OPENGL);

  
  myPort = new Serial(this, serialPort, BaudRate);
  myDelay(1000);
  
  // The font must be located in the sketch's "data" directory to load successfully
  font = loadFont("CourierNew36.vlw"); 
  
  println("Waiting IMU..");

  myPort.clear();
  myDelay(1000);
  while (myPort.available() == 0) {
    myPort.write("v");
    myDelay(1000);
    sw.start();
  }
  //println(myPort.readStringUntil('\n'));
  myPort.write("f");
  myPort.write("z" + char(burst));
  myPort.bufferUntil('\n');
}

float decodeFloat(String inString) {
  byte [] inData = new byte[4];
  
  if(inString.length() == 8) {
    inData[0] = (byte) unhex(inString.substring(0, 2));
    inData[1] = (byte) unhex(inString.substring(2, 4));
    inData[2] = (byte) unhex(inString.substring(4, 6));
    inData[3] = (byte) unhex(inString.substring(6, 8));
  }
      
  int intbits = (inData[3] << 24) | ((inData[2] & 0xff) << 16) | ((inData[1] & 0xff) << 8) | (inData[0] & 0xff);
  return Float.intBitsToFloat(intbits);
}

void serialEvent(Serial p) {
  if(p.available() >= 17) {
    String inputString = p.readStringUntil('\n');
    //print(inputString);
    if (inputString != null && inputString.length() > 0) {
      String [] inputStringArr = split(inputString, ",");
      if(inputStringArr.length >= 17) { // q1,q2,q3,q4,\r\n so we have 5 elements
        q[0] = decodeFloat(inputStringArr[0]);
        q[1] = decodeFloat(inputStringArr[1]);
        q[2] = decodeFloat(inputStringArr[2]);
        q[3] = decodeFloat(inputStringArr[3]);
	acc[0] = decodeFloat(inputStringArr[4]);
	acc[1] = decodeFloat(inputStringArr[5]);
	acc[2] = decodeFloat(inputStringArr[6]);
	gyro[0] = decodeFloat(inputStringArr[7]);
	gyro[1] = decodeFloat(inputStringArr[8]);
	gyro[2] = decodeFloat(inputStringArr[9]);
	magn[0] = decodeFloat(inputStringArr[10]);
	magn[1] = decodeFloat(inputStringArr[11]);		
	magn[2] = decodeFloat(inputStringArr[12]);
	temp = decodeFloat(inputStringArr[13]);
	press = decodeFloat(inputStringArr[14]);
        dt = (1./decodeFloat(inputStringArr[15]));
        heading = decodeFloat(inputStringArr[16]);
        if(heading < -9990) {
            heading = 0;
        }
        altitude = decodeFloat(inputStringArr[17]);

      }
    }
    count = count + 1;
    if(burst == count) { // ask more data when burst completed
      //1 = RESET MPU-6050, 2 = RESET Q Matrix
      if(key == 'q') {
         myPort.clear();
         myPort.write("2");
         sw.start();
         println("pressed 2");
         key = '0';
      } else if(key == 'r') {
            myPort.clear();
            myPort.write("1");
            sw.start();
            println("pressed 1");
            key = '0';
      } else if(key == 'g') {
            myPort.clear();
            myPort.write("g");
            sw.start();
            println("pressed g");
            key = '0';            
      } else if(key == 'R') {
            myPort.clear();
            calib = 0;
            sea_press = 1013.25;
            setup();
      } 
      
      if(seapresscmd != "99"){
         myPort.clear();
         myPort.write(seapresscmd);
         seapresscmd =  "99";    
      }   
      
      if(calib == 0) {
         myPort.clear();
         myPort.write("f");
         sw.start();
         calib = 99;
      }    
      if(calib == 1) {
         myPort.clear();
         myPort.write("t");
         sw.start();
         calib = 99;
      }

      myDelay(100);
      p.write("z" + char(burst));
      count = 0;
    }
  }
}


void buildBoxShape() {
  //box(60, 10, 40);
  noStroke();
  beginShape(QUADS);
  
  //Z+ (to the drawing area)
  fill(#00ff00);
  vertex(-30, -5, 20);
  vertex(30, -5, 20);
  vertex(30, 5, 20);
  vertex(-30, 5, 20);
  
  //Z-
  fill(#0000ff);
  vertex(-30, -5, -20);
  vertex(30, -5, -20);
  vertex(30, 5, -20);
  vertex(-30, 5, -20);
  
  //X-
  fill(#ff0000);
  vertex(-30, -5, -20);
  vertex(-30, -5, 20);
  vertex(-30, 5, 20);
  vertex(-30, 5, -20);
  
  //X+
  fill(#ffff00);
  vertex(30, -5, -20);
  vertex(30, -5, 20);
  vertex(30, 5, 20);
  vertex(30, 5, -20);
  
  //Y-
  fill(#ff00ff);
  vertex(-30, -5, -20);
  vertex(30, -5, -20);
  vertex(30, -5, 20);
  vertex(-30, -5, 20);
  
  //Y+
  fill(#00ffff);
  vertex(-30, 5, -20);
  vertex(30, 5, -20);
  vertex(30, 5, 20);
  vertex(-30, 5, 20);
  
  endShape();
}


void drawCube() {  
  pushMatrix();
    translate(VIEW_SIZE_X/2, VIEW_SIZE_Y/2 + 50, 0);
    scale(5,5,5);
    
    // a demonstration of the following is at 
    // http://www.varesano.net/blog/fabio/ahrs-sensor-fusion-orientation-filter-3d-graphical-rotating-cube
    rotateZ(-Euler[2]);
    rotateX(-Euler[1]);
    rotateY(-Euler[0]);
    
    buildBoxShape();
    
  popMatrix();
}


void draw() {
  background(#000000);
  fill(#ffffff);
  
  
  if(hq != null) { // use home quaternion
    quaternionToEuler(quatProd(hq, q), Euler);
	//println(quatProd(hq,q));println();
    text("Disable home position by pressing \"n\"", 20, VIEW_SIZE_Y - 30);
  }
  else {
    quaternionToEuler(q, Euler);
    text("Point FreeIMU's X axis to your monitor then press \"h\"", 20, VIEW_SIZE_Y - 30);
  }

  fused_alt = altitude + STATIONALTFT/METERS2FT;

  text("Temp: " + temp + "\n" + "Press: " + press + "\n" + "   Alt: " + nfp((fused_alt),3,2), 20, VIEW_SIZE_Y - 110);
  text("DeltaT: " + dt, 180, VIEW_SIZE_Y - 110);
  
  textFont(font, 20);
  textAlign(LEFT, TOP);
  text("Q:\n" + q[0] + "\n" + q[1] + "\n" + q[2] + "\n" + q[3], 20, 20);
  text("Euler Angles:\nYaw (psi)  : " + nfp(degrees(Euler[0]),3,2) + "\nPitch (theta): " + nfp(degrees(Euler[1]),3,2) + "\nRoll (phi)  : " + nfp(degrees(Euler[2]),3,2), 200, 20);

  //Compass averaging
  //currentAngle = myAtan2(mouseY-height/2, mouseX-width/2) + radians(myNoise); 
  addItemsToHistoryBuffers(radians(heading));
  calculateMathematicalAverageOfHistory();
  calculateYamartinoAverageOfHistory(); 
  
  //corr_heading = heading;
  corr_heading = degrees(yamartinoAverageAngle);
  
  text("Heading " + nfp(((corr_heading)),4,1),400,20); 

  text( "Elapsed Time: " + sw.hour() + ":" + sw.minute() + ":" + sw.second(), 500, 40);
  
  text("Acc:\n" + nfp(acc[0],1,6) + "\n" + nfp(acc[1],1,6) + "\n" + nfp(acc[2],1,6) + "\n", 20, 130);
  text("Gyro:\n" + nfp(gyro[0],1,6) + "\n" + nfp(gyro[1],1,6) + "\n" + nfp(gyro[2],1,6) + "\n", 20, 220);
  text("Magn:\n" + nfp(magn[0],1,6) + "\n" + nfp(magn[1],1,6) + "\n" + nfp(magn[2],1,6) + "\n", 20, 310);

  text(MotionDetect(),VIEW_SIZE_X-125,VIEW_SIZE_Y-125) ;
  if(MotionDetect() > 0 ){
    fill(#FF0000);
  } else {
    fill(#FFFFFF)
  ; }
  rect(VIEW_SIZE_X-100,VIEW_SIZE_Y-100,50,50);

  if(cube_odo == 0) { 
	drawCube(); }
    else {
	position();
        text("px:  " + positionX[0] + "\n" + "py:  " + positionY[0], 200, 200);
   }
  //myPort.write("q" + 1);
}


void keyPressed() {
  if(key == 'h') {
    println("pressed h");
    
    // set hq the home quaternion as the quatnion conjugate coming from the sensor fusion
    hq = quatConjugate(q);
    sw.start();
  }
  else if(key == 'n') {
    println("pressed n");
    hq = null;
  }
  else if(key == 's') {
    println("pressed s"); 
    sw.start();
  }  
}

// See Sebastian O.H. Madwick report 
// "An efficient orientation filter for inertial and intertial/magnetic sensor arrays" Chapter 2 Quaternion representation

void quaternionToEuler(float [] q, float [] euler) {
  euler[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1); // psi
  euler[1] = -asin(2 * q[1] * q[3] + 2 * q[0] * q[2]); // theta
  euler[2] = atan2(2 * q[2] * q[3] - 2 * q[0] * q[1], 2 * q[0] * q[0] + 2 * q[3] * q[3] - 1); // phi
}

float [] quatProd(float [] a, float [] b) {
  float [] q = new float[4];
  
  q[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3];
  q[1] = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2];
  q[2] = a[0] * b[2] - a[1] * b[3] + a[2] * b[0] + a[3] * b[1];
  q[3] = a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + a[3] * b[0];
  
  return q;
}

// returns a quaternion from an axis angle representation
float [] quatAxisAngle(float [] axis, float angle) {
  float [] q = new float[4];
  
  float halfAngle = angle / 2.0;
  float sinHalfAngle = sin(halfAngle);
  q[0] = cos(halfAngle);
  q[1] = -axis[0] * sinHalfAngle;
  q[2] = -axis[1] * sinHalfAngle;
  q[3] = -axis[2] * sinHalfAngle;
  
  return q;
}

// return the quaternion conjugate of quat
float [] quatConjugate(float [] quat) {
  float [] conj = new float[4];
  
  conj[0] = quat[0];
  conj[1] = -quat[1];
  conj[2] = -quat[2];
  conj[3] = -quat[3];
  
  return conj;
}

void getYawPitchRollRad() {
  //float q[4]; // quaternion
  float gx, gy, gz; // estimated gravity direction
  
  gx = 2 * (q[1]*q[3] - q[0]*q[2]);
  gy = 2 * (q[0]*q[1] + q[2]*q[3]);
  gz = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];
  
  ypr[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1);
  ypr[1] = atan(gx / sqrt(gy*gy + gz*gz));
  ypr[2] = atan(gy / sqrt(gx*gx + gz*gz));
}

//=============================================================
void gravityCompensateDynAcc() {
  float[] g = new float[3];
  
  // get expected direction of gravity in the sensor frame
  g[0] = 2 * (q[1] * q[3] - q[0] * q[2]);
  g[1] = 2 * (q[0] * q[1] + q[2] * q[3]);
  g[2] = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
  
  // compensate accelerometer readings with the expected direction of gravity
  dyn_acc[0] = acc[0] - g[0];
  dyn_acc[1] = acc[1] - g[1];
  dyn_acc[2] = acc[2] - g[2];
}
 

//=============================================================
// converted from Michael Shimniok Data Bus code
// http://mbed.org/users/shimniok/code/AVC_20110423/

float clamp360(float x) {
    while ((x) >= 360.0) (x) -= 360.0; 
    while ((x) < 0) (x) += 360.0; 
    return x;
}



//==============================================================
//
float HeadingAvgCorr(float newx, float oldx) {
    while ((newx + 180.0) < oldx) (newx) += 360.0;
    while ((newx - 180.0) > oldx) (newx) -= 360.0;
    while ((newx) == 360.0) (newx) = 0.0;
    return newx;
}

//==============================================================
//SMA filter
// Use the next value and calculate the 
// moving average 
public void AddNewValue(float value){
  total -= data[p];
  data[p] = value;
  total += value;
  p = ++p % data.length;
  if(n < data.length) n++;
  average = total / n;
} 

//=======================================
public float iround(float number, float decimal) {
  int ix;
  ix = round(number*pow(10, decimal));
  return float(ix)/pow(10, decimal);
} 
