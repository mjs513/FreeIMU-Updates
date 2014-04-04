import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.serial.*; 
import processing.opengl.*; 
import controlP5.*; 
import java.util.LinkedList; 
import java.util.Queue; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class FreeIMU_cube_Odo_Exp extends PApplet {

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




ControlP5 cp5;

Serial myPort;  // Create object from Serial class

//setting a 1D Kalman filter
MyKalman pressK = new MyKalman();

//Settup Stop Watch
StopWatchTimer sw = new StopWatchTimer();

// These are needed for the moving average calculation
float[] data = new float[32];
float total = 0, average = 0;
int p = 0, n = 0;

//LPF
float filterFactor = 0.05f;
float heading_f = 0.f;

//Moving average Heading
float corr_heading;
float heading_avg;
float heading = 0;
float oldHeading = 0.0f;
int windSize = 96;
MovingAverage HeadingAvg = new MovingAverage(windSize);

//set motiondetect types
float accnorm,accnorm_var_test;
int accnorm_test, omegax, omegay, omegaz, omega_test, motionDetect;

//Movingaverage filters for motion detection
MovingAverage accnorm_test_avg = new MovingAverage(5);
MovingAverage accnorm_var = new MovingAverage(7);
MovingAverage motion_detect_ma = new MovingAverage(7);
MovingAverage accnorm_avg = new MovingAverage(5);

final String serialPort = "COM4"; // replace this with your serial port. On windows you will need something like "COM1".

float [] q = new float [4];
float [] acc = new float [3];
float [] gyro = new float [3];
float [] magn = new float [3];
float [] ypr = new float [3];
float temp; float press; float altitude; 
float EstAlt;
float dt;
float tnew;
float told = 0;

// Altitude - Accel Complimentary filter setup
float[] dyn_acc = new float[3];
float fused_alt;
Quaternion dyn_acc_q;
Quaternion q1;
Quaternion multQ;
Quaternion dyn_acc_q_earth;
Quaternion conQ;
AltitudeComplementary altitudeFilter = new AltitudeComplementary();

// Sphere Variables
float R = 150;
int xDetail = 40;
int yDetail = 30;
float[] xGrid = new float[xDetail+1];
float[] yGrid = new float[yDetail+1];
float[][][] allPoints = new float[xDetail+1][yDetail+1][3];

// Rotation Variables
float camDistance = -90;
float rotationX = 100;
float rotationY = 170;
float velocityX = 0;
float velocityY = 0;

// Texture
PImage texmap, Aimage, cmpRng, cmpAP;
float angx, angy, angz, angyLevelControl;

float S;
float A;

float sea_press = 1013.25f;            //Input local sea level pressure
float declinationAngle = -13.1603f;   //Flushing, NY magnetic declination in degrees
float SEA_PRESS  = 1013.25f;          //default sea level pressure level in mb
float KNOWNALT   = 65.0f;            //default known altitude, 
float INHG       = 0.02952998751f;    //convert mb to in/Hg constant
float MB         = 33.8638815f;       //convert in/Hg to mb constant
float FTMETERS   = 0.3048f;
float METERS2FT  = 3.2808399f;
float PI         = 3.14159f;
float deg2rads   = PI/180;
int calib = 1;
int ArtHorFlg = 0;

//FreeIMY setup
float [] hq = null;
float [] Euler = new float [3]; // psi, theta, phi

int lf = 10; // 10 is '\n' in ASCII
byte[] inBuffer = new byte[22]; // this is the number of chars on each line from the Arduino (including /r/n)

//---------------------------------------------------
// types needed for ODO implementation
//
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

// Switch for ODO
int cube_odo = 0;

//-------------------------------------
//
PFont font, font8, font9, font12, font15, letters, lcd;

final int VIEW_SIZE_X = 1000, VIEW_SIZE_Y = 600;
int xCompass    = 865;        int yCompass    = 365;
int xLevelObj   = 723;        int yLevelObj   = 90+0;

final int burst = 32;
int count = 0;

public void myDelay(int time) {
  try {
    Thread.sleep(time);
  } catch (InterruptedException e) { }
}

public void setup() 
{
  size(VIEW_SIZE_X, VIEW_SIZE_Y, OPENGL);
  //frameRate(120);
  
  // The font must be located in the sketch's "data" directory to load successfully
  font = loadFont("CourierNew36.vlw"); 
  lcd = loadFont("LiquidCrystal-128.vlw");
  font8 = createFont("Arial bold",8,false);
  font9 = createFont("Arial bold",9,false);
  font12 = createFont("Arial bold",12,false);
  font15 = createFont("Arial bold",15,false);

  //Quaternion library calls for complimentary altitude filter  
  dyn_acc_q = new Quaternion();
  q1 = new Quaternion();
  multQ = new Quaternion();
  dyn_acc_q_earth = new Quaternion();
  conQ = new Quaternion();

  cp5 = new ControlP5(this);
  cp5.setControlFont(font,18);
  cp5.setColorValue(color(255, 255, 0));
  cp5.setColorLabel(color(255, 255, 0));
  
  setValues();

  //setup attitdude indicator
  noStroke();
  imageMode(CENTER);
  cmpRng = loadImage("CompassRing.PNG");
  cmpAP  = loadImage("CompassAP.PNG");
  texmap = loadImage("sphere_bckgrnd.png");
  Aimage = loadImage("AttInd.PNG");
  setupSphere(R, xDetail, yDetail);
  
  //serial port set up
  myPort = new Serial(this, serialPort, 38400);

  //elapsed time start call
  sw.start();
  
  println("Waiting IMU..");

  while (myPort.available() > 0) {
    myPort.write("v");
    myPort.write("1");
    myDelay(1000);
  }
  //println(myPort.readStringUntil('\n'));
  myPort.write("z" + PApplet.parseChar(burst));
  myPort.bufferUntil('\n');
  
}

public float decodeFloat(String inString) {
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

public void serialEvent(Serial p) {
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
        tnew = decodeFloat(inputStringArr[15]);
        heading = decodeFloat(inputStringArr[16]);
        dt = tnew - told;
        told = tnew;
        //getYawPitchRollRad();
      }
    }
    count = count + 1;
    if(burst == count) { // ask more data when burst completed
      //1 = RESET MPU-6050, 2 = RESET Q Matrix
      if(key == '2') {
         myPort.clear();
         myPort.write("2");
         sw.start();
         println("pressed 2");
         key = '0';
      } else if(key == '1') {
            myPort.clear();
            myPort.write("1");
            sw.start();
            println("pressed 1");
            key = '0';
      } else if(key == 'r') {
            myPort.clear();
            ArtHorFlg = 0;
            calib = 1;
            sea_press = 1013.25f;
            setup();
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
      p.write("z" + PApplet.parseChar(burst));
      count = 0;
    }
  }
}


public void buildBoxShape() {
  //box(60, 10, 40);
  noStroke();
  beginShape(QUADS);

  //Z+ (to the drawing area)
  fill(0xff00ff00);
  vertex(-30, -5, 20);
  vertex(30, -5, 20);
  vertex(30, 5, 20);
  vertex(-30, 5, 20);
  
  //Z-
  fill(0xff0000ff);
  vertex(-30, -5, -20);
  vertex(30, -5, -20);
  vertex(30, 5, -20);
  vertex(-30, 5, -20);
  
  //X-
  fill(0xffff0000);
  vertex(-30, -5, -20);
  vertex(-30, -5, 20);
  vertex(-30, 5, 20);
  vertex(-30, 5, -20);
  
  //X+
  fill(0xffffff00);
  vertex(30, -5, -20);
  vertex(30, -5, 20);
  vertex(30, 5, 20);
  vertex(30, 5, -20);
  
  //Y-
  fill(0xffff00ff);
  vertex(-30, -5, -20);
  vertex(30, -5, -20);
  vertex(30, -5, 20);
  vertex(-30, -5, 20);
  
  //Y+
  fill(0xff00ffff);
  vertex(-30, 5, -20);
  vertex(30, 5, -20);
  vertex(30, 5, 20);
  vertex(-30, 5, 20);
  
  endShape();
}


public void drawCube() {  
    pushMatrix();
    translate(VIEW_SIZE_X/2, VIEW_SIZE_Y/2 + 150, +80);
    scale(2,2,2);
    // a demonstration of the following is at 
    // http://www.varesano.net/blog/fabio/ahrs-sensor-fusion-orientation-filter-3d-graphical-rotating-cube
    rotateZ(-Euler[2]);
    rotateX(-Euler[1]+radians(17));
    rotateY(-Euler[0]);
    
    buildBoxShape();
    
  popMatrix();
}


public void draw() {
  //background(#585858);
  background(0xff000000);
  textFont(font, 18);
  textAlign(LEFT, TOP);
  strokeWeight(3);
  fill(0xffffffff);
  if(hq != null) { // use home quaternion
    quaternionToEuler(quatProd(hq, q), Euler);
    text("Disable home position by pressing \"n\"", 20, VIEW_SIZE_Y - 30);
 }
  else {
    quaternionToEuler(q, Euler);
    text("Point FreeIMU's X axis to your monitor then press \"h\"", 20, VIEW_SIZE_Y - 30);
  }

  fill(0xffFFFF00);
  float press1 = pressK.update(press);
  altitude = ((pow((sea_press / press1), 1/5.257f) - 1.0f) * (temp + 273.15f)) / 0.0065f;
  altitude = altitude + 36/METERS2FT;
  EstimatedAltitude();
  text("Temp: " + temp + "\n" + "Press: " + press + "\n" , 20, VIEW_SIZE_Y - 110);
  textFont(font,24);
  text("ALT:",(VIEW_SIZE_X/2)-75,40); 
  fill(0xff00CF00);
  textFont(font,36);
  text(nfp((fused_alt),3,2),(VIEW_SIZE_X/2)-75,70); 
  textFont(font,18);
  fill(0xffffff00);
  text("DeltaT: " + dt, 180, VIEW_SIZE_Y - 110);
  
  text("Q:\n" + q[0] + "\n" + q[1] + "\n" + q[2] + "\n" + q[3], 20, 20);

  textFont(font, 24);
  text("Pitch:\n", xLevelObj-50, yLevelObj + 45);
  text("Roll:\n", xLevelObj-50, yLevelObj + 105);
  text("Yaw:\n", xLevelObj-50, yLevelObj + 165);
  text("Heading:\n",850,255);
  
  //text(nfp(degrees(Euler[1]),3,2) + "  " + nfp(degrees(Euler[2]),3,2), xLevelObj + 85, yLevelObj + 125);
  getYawPitchRollRad();
  
  fill(0xff00CF00);
  //text(nfp(degrees(Euler[1]),3,2), xLevelObj-40, yLevelObj + 75);
  //text(nfp(degrees(Euler[2]),3,2), xLevelObj-40, yLevelObj + 135);
  //text(nfp(degrees(Euler[0]),3,2), xLevelObj-40, yLevelObj + 195);
  text(nfp(degrees(ypr[1]),3,2), xLevelObj-40, yLevelObj + 75);
  text(nfp(degrees(ypr[2]),3,2), xLevelObj-40, yLevelObj + 135);
  text(nfp(degrees(Euler[0]),3,2), xLevelObj-40, yLevelObj + 195);
  
  textFont(font, 18);
  fill(0xffFFFF00);
  noFill();
  stroke(204, 102, 0);
  rect(10, 17, 145, 95, 7);
  //angx = Euler[2];
  //angy = Euler[1];
  angx = ypr[2];
  angy = ypr[1];
  
  float head1 = iround(heading,1);
  corr_heading = clamp360(head1+declinationAngle);
  HeadingAvg.newNum(corr_heading);
  //HeadingAvg.newNum(HeadingAvgCorr(corr_heading, oldHeading));
  //oldHeading = corr_heading;
  corr_heading = HeadingAvg.getAvg();
  //text("Heading " + nfp(((corr_heading)),4,1),400,20);
  //buildCompass();
  rotComp();
  
  textFont(font, 24);
  fill(0xff00CF00);
  text(nfp(((corr_heading)),4,1),850,285);
  textFont(font, 18);
  fill(0xffFFFF00);
  
  noFill();
  stroke(204, 102, 0);
  rect(10, 125, 145, 85, 7);  
  text("Acc:\n" + nfp(acc[0],1,6) + "\n" + nfp(acc[1],1,6) + "\n" + nfp(acc[2],1,6) + "\n", 20, 130);
  //rect(10, 210, 145, 85, 7); 
  //text("Gyro:\n" + nfp(gyro[0],1,6) + "\n" + nfp(gyro[1],1,6) + "\n" + nfp(gyro[2],1,6) + "\n", 20, 220);
  rect(170, 20, 145, 85, 7);
  text("Gyro:\n" + nfp(gyro[0],1,6) + "\n" + nfp(gyro[1],1,6) + "\n" + nfp(gyro[2],1,6) + "\n", 180, 25);
  //rect(10, 295, 145, 90, 7); 
  //text("Magn:\n" + nfp(magn[0],1,6) + "\n" + nfp(magn[1],1,6) + "\n" + nfp(magn[2],1,6) + "\n", 20, 310);
  rect(170, 125, 145, 85, 7); 
  text("Magn:\n" + nfp(magn[0],1,6) + "\n" + nfp(magn[1],1,6) + "\n" + nfp(magn[2],1,6) + "\n", 180, 130);
  
  //text(MotionDetect(),VIEW_SIZE_X-75,VIEW_SIZE_Y-75) ;
  if(MotionDetect() > 0 ){
    fill(0xffFF0000);
  } else {
    fill(0xffFFFFFF)
  ; }
  rect(VIEW_SIZE_X-75,VIEW_SIZE_Y-55,35,35);

  if(cube_odo == 0) { 
	drawCube(); }
    else {
	position();
        text("px:  " + positionX[0] + "\n" + "py:  " + positionY[0], 200, 200);
   }
  
  textFont(font,20);
  fill(0xffffff00);
  text("Elapsed\n" + "Time", 680,40);   
  textFont(lcd, 48);
  fill(0xffff0000);
  text(sw.hour() + ":" + sw.minute() + ":" + sw.second(), 823, 40);
  
  if(ArtHorFlg == 1) {
      NewHorizon();
  }

}


public void keyPressed() {
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
  else if(key == 'q') {
    exit();
  }
}

// See Sebastian O.H. Madwick report 
// "An efficient orientation filter for inertial and intertial/magnetic sensor arrays" Chapter 2 Quaternion representation

public void quaternionToEuler(float [] q, float [] euler) {
  euler[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1); // psi
  euler[1] = -asin(2 * q[1] * q[3] + 2 * q[0] * q[2]); // theta
  euler[2] = atan2(2 * q[2] * q[3] - 2 * q[0] * q[1], 2 * q[0] * q[0] + 2 * q[3] * q[3] - 1); // phi
}

public float [] quatProd(float [] a, float [] b) {
  float [] q = new float[4];
  
  q[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3];
  q[1] = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2];
  q[2] = a[0] * b[2] - a[1] * b[3] + a[2] * b[0] + a[3] * b[1];
  q[3] = a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + a[3] * b[0];
  
  return q;
}

// returns a quaternion from an axis angle representation
public float [] quatAxisAngle(float [] axis, float angle) {
  float [] q = new float[4];
  
  float halfAngle = angle / 2.0f;
  float sinHalfAngle = sin(halfAngle);
  q[0] = cos(halfAngle);
  q[1] = -axis[0] * sinHalfAngle;
  q[2] = -axis[1] * sinHalfAngle;
  q[3] = -axis[2] * sinHalfAngle;
  
  return q;
}

// return the quaternion conjugate of quat
public float [] quatConjugate(float [] quat) {
  float [] conj = new float[4];
  
  conj[0] = quat[0];
  conj[1] = -quat[1];
  conj[2] = -quat[2];
  conj[3] = -quat[3];
  
  return conj;
}

public void getYawPitchRollRad() {
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
public void gravityCompensateDynAcc() {
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

public float clamp360(float x) {
    while ((x) >= 360.0f) (x) -= 360.0f; 
    while ((x) < 0) (x) += 360.0f; 
    return x;
}



//==============================================================
//
public float HeadingAvgCorr(float newx, float oldx) {
    while ((newx + 180.0f) < oldx) (newx) += 360.0f;
    while ((newx - 180.0f) > oldx) (newx) -= 360.0f;
    while ((newx) == 360.0f) (newx) = 0.0f;
    return newx;
}

//============================================================
public void EstimatedAltitude() {
  
  gravityCompensateDynAcc();
  dyn_acc_q.x = dyn_acc[0];
  dyn_acc_q.y = dyn_acc[1];  
  dyn_acc_q.z = dyn_acc[2];
  dyn_acc_q.w = 0;
  q1.x = q[1]; q1.y = q[2]; q1.z = q[3]; q1.w = q[0];
  multQ = Quaternion.multiply(q1, dyn_acc_q);
  //conQ = Quaternion.conjugate(q1);
  conQ.x = -q1.x; conQ.y = -q1.y; conQ.z = -q1.z; conQ.w = q1.w;
  dyn_acc_q_earth = Quaternion.multiply(multQ, conQ);
  fused_alt = altitudeFilter.update(dyn_acc_q_earth.z, altitude, dt);
  
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
  return PApplet.parseFloat(ix)/pow(10, decimal);
}


// complementary filter from Palla Software
// 
final float Kp1 = 0.55f;                // PI observer velocity gain 
final float Kp2 = 1.0f;                 // PI observer position gain
final float Ki  = 0.001f;               // PI observer integral gain (bias cancellation)

public class AltitudeComplementary {
  
    //self.inited = 0
    public float AltErrorI = 0;
    //self.AccScale  = 0.0
    //public int startTime = millis();
    public float AltError = 0;
    public float InstAcc = 0.0f;
    public float Delta = 0;
    public float EstVelocity = 0.0f;
    public float EstAlt = altitude;
    //public float lastTime = millis();
    
  // Computes the complementary estimation of Altitude based from the Barometer and Accelerometer
  // @param dyn_acc_z the Z componente of the dynamic acceleration expressed in the Earth frame
  // @BaroAlt the Baromether altitude reading as coming from the sensor expressed in meters with decimals
  public float update(float dyn_acc_z, float BaroAlt, float dt ){
    //int currentTime = millis();
    
    //println(EstAlt);
    //println(dyn_acc_z);
    // Estimation Error
    AltError = BaroAlt - EstAlt;
    AltErrorI += AltError;
    
    AltErrorI = constrain(AltErrorI,-150.00f,+150.00f);

    InstAcc = dyn_acc_z * 9.80665f; //+ ((self.AltErrorI / 1000) if self.AltErrorI != 0.0 else 0)
    if (AltErrorI != 0.0f){
      //self.InstAcc += self.AltErrorI / 10000.0
      InstAcc = AltErrorI * Ki;
    }
    if(Float.isNaN(InstAcc)){
      InstAcc = 0;
    }
    
    float dt1 = dt/1000.f;

    // Integrators
    Delta = InstAcc * dt1 + (Kp1 * dt1) * AltError;
    
    if (EstVelocity != 0.0f) {
      EstAlt += (EstVelocity/5.0f + Delta) * (dt1 / 2) + (Kp2 * dt1) * AltError;
    } else {
      EstAlt = Delta * (dt1 / 2) + (Kp2 * dt1) * AltError;
    }
    
    EstVelocity += Delta * 10.0f;
    
    //print (self.InstAcc, self.AltErrorI, dt, self.Delta)
    
    //lastTime = currentTime;
    
    return EstAlt;
  }
}    
public class MyKalman {
//private float Q = 0.001;
//private float R = 0.1;
private float Q = 0.0000005f;
private float R = 0.01f;
private float P = 1, X = 0, K;
 
private void measurementUpdate(){
K = (P + Q) / (P + Q + R);
P = R * (P + Q) / (R + P + Q);
}
 
public float update(float measurement){
measurementUpdate();
float result = X + (measurement - X) * K;
X = result;
 
return result;
}
} 
public float MotionDetect() {
  
    /*###################################################################
    
       accelerationsquared euclidean norm analysis
    
    ################################################################### */
    accnorm = (acc[0]*acc[0]+acc[1]*acc[1]+acc[2]*acc[2]);
    //if((accnorm >=0.96) && (accnorm <= 0.99)){
    if((accnorm >=0.96f) && (accnorm <= 0.995f)){  
        accnorm_test = 0;
    } else {
        accnorm_test = 1; }
    //take average of 5 to 10 points
    accnorm_avg.newNum(accnorm);
    accnorm_test_avg.newNum(accnorm_test);

    /* ####################################################################
    #
    #   squared norm analysis to determine suddenly changes signal
    #
    ##################################################################### */
    accnorm_var.newNum(sq(accnorm-accnorm_avg.getAvg()));
    if(accnorm_var.getAvg() < 0.00025f) {
        accnorm_var_test = 0;
    }else {
        accnorm_var_test = 1; }

    /*####################################################################
    #
    #   angular rate analysis in order to disregard linear acceleration
    #
    ################################################################### */
    if ((gyro[0] >=-0.0024f) && (gyro[0] <= 0.0f)) {
        omegax = 0;
    } else {
        omegax = 1; }
        
    if((gyro[1] >= -0.0012f) && (gyro[1] <= 0.00215f)) {
        omegay = 0;
    } else {
        omegay = 1; }
        
    if((gyro[2] >= -0.0012f) && (gyro[2] <= 0.00215f)) {
        omegaz = 0;
    } else {
        omegaz = 1; }
        
    if ((omegax+omegay+omegaz) > 0) {
        omega_test = 1;
    } else {
        omega_test = 0; }


    /* ####################################################################
    #
    # combined movement detector
    #
    #################################################################### */
    if ((accnorm_test_avg.getAvg() + omega_test + accnorm_var_test) > 0) {
        motionDetect = 1;
    } else {
        motionDetect = 0; }
    
    //####################################################################    
    
    motion_detect_ma.newNum(motionDetect);
    
    if(motion_detect_ma.getAvg() > 0.5f) {
      return 1;
    } else {
      return 0; }
      
     //return omegaz;
}

/**
 ** No Longer Used
 **
 /////////////////////////////////////////////////////////////////////////////////////////
  void buildCompass(){
  // ---------------------------------------------------------------------------------------------
  // Compass Section - Multiwii compass code
  // 1/22/14 MJS - left GPS info in place as GPS will eventually be added to GUI
  // ---------------------------------------------------------------------------------------------
  scale(1.05);
  pushMatrix();
  translate(xCompass,yCompass);
  // Compass Background
  fill(0, 0, 0);
  strokeWeight(3);stroke(204, 102, 0);
  rectMode(CORNERS);
  int size=29;
  rect(-size*2.5,-size*2.5,size*2.5,size*2.5);
  
  // GPS quadrant
  strokeWeight(1.5);stroke(204, 102, 0);
  //if (GPS_update == 1) {
  //  fill(125);stroke(125);
  //} else {
  //  fill(160);stroke(160);
  //}
  ellipse(0,  0,   4*size+7, 4*size+7);  
  
  // GPS rotating pointer
  //rotate(GPS_directionToHome*PI/180);
  //strokeWeight(4);stroke(255,255,100);line(0,0, 0,-2.4*size);line(0,-2.4*size, -5 ,-2.4*size+10); line(0,-2.4*size, +5 ,-2.4*size+10);  
  //rotate(-GPS_directionToHome*PI/180);
  // compass quadrant
  strokeWeight(1.5);fill(0);stroke(0);stroke(204, 102, 0);
  ellipse(0,  0,   2.6*size+7, 2.6*size+7);
  
  // Compass rotating pointer
  strokeWeight(3); stroke(200, 200, 20); //stroke(255);
  rotate(corr_heading*PI/180);
  line(0,size*0.2, 0,-size*1.3); line(0,-size*1.3, -5 ,-size*1.3+10); line(0,-size*1.3, +5 ,-size*1.3+10);
  popMatrix();
  
  // angles 
  for (int i=0;i<=12;i++) {
    float angCalc=i*PI/6;
    if (i%3!=0) {
      stroke(125);
      line(xCompass+cos(angCalc)*size*2,yCompass+sin(angCalc)*size*2,xCompass+cos(angCalc)*size*1.6,yCompass+sin(angCalc)*size*1.6);
    } else {
      stroke(255);
      line(xCompass+cos(angCalc)*size*2.2,yCompass+sin(angCalc)*size*2.2,xCompass+cos(angCalc)*size*1.9,yCompass+sin(angCalc)*size*1.9);
    }
  }
  fill(255,255,255);
  textFont(font15);
  text("N", xCompass-5, yCompass-27-size*0.9);text("S", xCompass-5, yCompass+32-15+size*0.9);
  text("W", xCompass-33+5-size*0.9, yCompass+6-13);text("E", xCompass+21-5+size*0.9, yCompass+6-13);
  
  // head indicator
  textFont(font12);
  noStroke();
  fill(80,80,80,130);
  rect(xCompass-22,yCompass-8,xCompass+22,yCompass+9);
  textFont(font,18);
  fill(255,255,127);
  text(nfp(corr_heading,3,1) + " \u00b0",xCompass-11-15-(corr_heading>=10.0 ? (corr_heading>=100.0 ? 6 : 3) : 0),yCompass+6+3);
  textFont(font12);
  
  // GPS direction indicator
  //fill(255,255,0);
  //text(GPS_directionToHome + "\u00c2\u00b0",xCompass-6-size*2.1,yCompass+7+size*2);
  
  // GPS fix
  //if (GPS_fix==0) {
  //   fill(127,0,0);
  //} else {
  //   fill(0,255,0);
  //}
  //ellipse(xCompass+3+size*2.1,yCompass+3+size*2,12,12);
  //rect(xCompass-28+size*2.1,yCompass+1+size*2,xCompass+9+size*2.1,yCompass+13+size*2);
  //textFont(font9);
  //if (GPS_fix==0) {
  //  fill(255,255,0);
  //} else {
  //  fill(0,50,0);
  //}
  //text("GPS_fix",xCompass-27+size*2.1,yCompass+10+size*2);
  
  //reset rectMode to default value
  rectMode(CORNER);
  scale(1/1.05);
  fill(#FFFF00);
  textFont(font, 18);
}

void DrawHorizon(){
  // ---------------------------------------------------------------------------------------------
  // Magnetron Combi Fly Level Control - extracted from Multiwii code
  // modified 1/22/14
  // ---------------------------------------------------------------------------------------------
  scale(1.25);
  int horizonInstrSize=68;
  float a=(angx);
  angy = degrees(angy);
  angyLevelControl=((angy<-horizonInstrSize) ? -horizonInstrSize : (angy>horizonInstrSize) ? horizonInstrSize : angy);
  pushMatrix();
  translate(xLevelObj,yLevelObj);
  noStroke();
  // instrument background
  fill(50,50,50);
  ellipse(0,0,150,150);
  // full instrument
  rotate(-a);
  rectMode(CORNER);
  // outer border
  strokeWeight(2);
  stroke(90,90,90);
  //border ext
  arc(0,0,140,140,0,TWO_PI);
  stroke(190,190,190);
  //border int
  arc(0,0,138,138,0,TWO_PI);
  // inner quadrant
  strokeWeight(3);
  stroke(255,255,255);
  fill(124,73,31);
  //earth
  float angle = acos(angyLevelControl/horizonInstrSize);
  arc(0,0,136,136,0,TWO_PI);
  fill(38,139,224); 
  //sky 
  arc(0,0,136,136,HALF_PI-angle+PI,HALF_PI+angle+PI);
  float x = sin(angle)*horizonInstrSize;
  if (angy>0) 
    fill(124,73,31);
  noStroke();   
  triangle(0,0,x,-angyLevelControl,-x,-angyLevelControl);
  // inner lines
  strokeWeight(1);
  for(int i=0;i<8;i++) {
    int j=i*15;
    if (angy<=(35-j) && angy>=(-65-j)) {
      stroke(255,255,255); line(-30,-15-j-angy,30,-15-j-angy); // up line
      fill(255,255,255);
      textFont(font9);
      text("+" + (i+1) + "0", 34, -12-j-angy); //  up value
      text("+" + (i+1) + "0", -48, -12-j-angy); //  up value
    }
    if (angy<=(42-j) && angy>=(-58-j)) {
      stroke(167,167,167); line(-20,-7-j-angy,20,-7-j-angy); // up semi-line
    }
    if (angy<=(65+j) && angy>=(-35+j)) {
      stroke(255,255,255); line(-30,15+j-angy,30,15+j-angy); // down line
      fill(255,255,255);
      textFont(font9);
      text("-" + (i+1) + "0", 34, 17+j-angy); //  down value
      text("-" + (i+1) + "0", -48, 17+j-angy); //  down value
    }
    if (angy<=(58+j) && angy>=(-42+j)) {
      stroke(127,127,127); line(-20,7+j-angy,20,7+j-angy); // down semi-line
    }
  }
  strokeWeight(2);
  stroke(255,255,255);
  if (angy<=50 && angy>=-50) {
    line(-40,-angy,40,-angy); //center line
    fill(255,255,255);
    textFont(font9);
    text("0", 34, 4-angy); // center
    text("0", -39, 4-angy); // center
  }

  // lateral arrows
  strokeWeight(1);
  // down fixed triangle
  stroke(60,60,60);
  fill(180,180,180,255);

  triangle(-horizonInstrSize,-8,-horizonInstrSize,8,-55,0);
  triangle(horizonInstrSize,-8,horizonInstrSize,8,55,0);

  // center
  strokeWeight(2);
  stroke(255,0,0);
  line(-20,0,-5,0); line(-5,0,-5,5);
  line(5,0,20,0); line(5,0,5,5);
  line(0,-5,0,5);
  popMatrix();
  scale(1/1.25);
  fill(#FFFF00);
  textFont(font, 18);
}

**/
//////////////////////////////////////////////////////////
//
// Based on the work found here:
//           http://angela.homelinux.net:8080/drupal/code
//
//////////////////////////////////////////////////////////

public void rotComp(){

  pushMatrix();
  //translate(width/2, height/2);
  translate(850,420);
  scale(0.9f);
  rotateZ(-radians(corr_heading));
  image(cmpRng,0,0);
  scale(1/0.9f);
  popMatrix();
  
  pushMatrix();
  translate(850,420);
  scale(0.75f);
  image(cmpAP, 0, 0);
  scale(1/0.75f);
  popMatrix();
}

////////////////////////////////////////////////////////////////////////
//
// Based on the work found here:
//       http://scholarworks.csun.edu/handle/10211.2/1456
//
// and sample code:
//       http://processing.org/discourse/beta/num_1274893475.html
//
/////////////////////////////////////////////////////////////////////
public void NewHorizon(){
 
  pushMatrix();
  translate(width/2.0f, height/2.0f, camDistance);
  rotateZ( radians(90)+ angx);    //rolls it
  //rotateX( radians(4-rotationX) );  
  rotateY( radians(95) + angy );  //pitch
  rotateX( radians(0 ));   //yaw
  rotate(radians(17));

  drawSphere(texmap);
  popMatrix();

  //frame
  translate(width/2.0f, height/2.0f, +30);
  //image(Aimage, -150, -140);
  image(Aimage, 0, 0);
  
  // center
  translate(0,5,90);
  scale(1.50f);
  strokeWeight(8);
  stroke(255,0,0);
  line(-20,0,-5,0); line(-5,0,-5,5);
  line(5,0,20,0); line(5,0,5,5);
  line(0,-5,0,5);
  scale(1/1.5f);
  noStroke();

}

////////////////////////////////////////////////////////////////////////
public void setupSphere(float R, int xDetail, int yDetail){

  // Create a 2D grid of standardized mercator coordinates
  for(int i = 0; i <= xDetail; i++){
    xGrid[i]= i / (float) xDetail;
  } 
  for(int i = 0; i <= yDetail; i++){
    yGrid[i]= i / (float) yDetail;
  }

  textureMode(NORMAL);

  // Transform the 2D grid into a grid of points on the sphere, using the inverse mercator projection
  for(int i = 0; i <= xDetail; i++){
    for(int j = 0; j <= yDetail; j++){
      allPoints[i][j] = mercatorPoint(R, xGrid[i], yGrid[j]);
    }
  }
}

////////////////////////////////////////////////////////////////////////
public float[] mercatorPoint(float R, float x, float y){

  float[] thisPoint = new float[3];
  float phi = x*2*PI;
  float theta = PI - y*PI;

  thisPoint[0] = R*sin(theta)*cos(phi);
  thisPoint[1] = R*sin(theta)*sin(phi);
  thisPoint[2] = R*cos(theta);

  return thisPoint;
}

////////////////////////////////////////////////////////////////////////
public void drawSphere(PImage Map){

  for(int j = 0; j < yDetail; j++){
    beginShape(TRIANGLE_STRIP);
    texture(Map);
    for(int i = 0; i <= xDetail; i++){
      vertex(allPoints[i][j+1][0],   allPoints[i][j+1][1],   allPoints[i][j+1][2],   xGrid[i],   yGrid[j+1]);
      vertex(allPoints[i][j][0],     allPoints[i][j][1],     allPoints[i][j][2],     xGrid[i],   yGrid[j]);
    }
    endShape(CLOSE);
  }
}
 

/**************************************************************************
//This function allows movement end detection. If a certain number of 
//acceleration samples are  equal to zero we can assume movement has stopped.
//Accumulated Error generated in the velocity calculations is eliminated by 
//resetting the velocity variables. This stops position increment and greatly
//eliminates position error.
//***************************************************************************/  

public void movement_end_check()   
{

  if (accelerationx[1] == 0)   //we count the number of acceleration samples that equals zero 
{ countx++;}
  else { countx = 0;}

  if (countx >= 25)             //if this number exceeds 25, we can assume that velocity is zero
   {    velocityx[1] = 0;
        velocityx[0] = 0;
    }
   
   if (accelerationy[1] == 0)   //we do the same for the Y axis
   { county++;}
   else {  county = 0;}
  
  if (county >= 25)
   { 
     velocityy[1] = 0;
     velocityy[0] = 0;
    } 
}

//***********************************************************************
//***********************************************************************
//This function transforms acceleration to a proportional position by 
//integrating the acceleration data twice. It also adjusts sensibility by   
//multiplying the "positionX" and "positionY" variables. 
//This integration algorithm carries error, which is compensated in the 
//"movenemt end check" subroutine. Faster sampling frequency implies less 
//error but requires more memory. Keep in  mind that the same process is 
//applied to the X and Y axis. 
//******************************************************************************/  
 
public void position()  
{
  int count2=0;
    
  do{
     //ADC GetAllAxis();
     gravity_compensate();
     
     accelerationx[0] = accelerationx[0] + Sample_X; //filtering routine for noise attenuation
     accelerationy[0] = accelerationy[0] + Sample_Y; //64 samples are averaged. The resulting 
     //average represents the acceleration of 
     //an instant
      count2++;                                       

    } while (count2 != 0x40);                   // 64 sums of the acceleration sample
  

  accelerationx[0]= accelerationx[0]/64;       // division by 64
  accelerationy[0]= accelerationy[0]/64;
  
  accelerationx[0] = accelerationx[0] - (int)sstatex;  //eliminating zero reference 
                                                       //offset of the acceleration data
  accelerationy[0] = accelerationy[0] - (int)sstatey;  //to obtain positive and negative 
                                                       //acceleration
  
  if ((accelerationx[0] <=0.010f)&&(accelerationx[0] >= -0.010f))  //Discrimination window applied 
          {accelerationx[0] = 0;}                        // to the X axis acceleration 
                                                         //variable
               
  if ((accelerationy[0] <=0.010f) && (accelerationy[0] >= -0.010f)) 
                {accelerationy[0] = 0;} 
  
  //first X integration:
  velocityx[0]= velocityx[0]+ accelerationx[0]+ ((accelerationx[1] - accelerationx[0] )/2);  

  //second X integration:
  positionX[0]= positionX[0] + velocityx[0] + ((velocityx[1] - velocityx[0])/2);

  //first Y integration:       
  velocityy[0] = velocityy[0] + accelerationy[0] + ((accelerationy[1] -accelerationy[0])/2);

  //second Y integration:       
  positionY[0] = positionY[0] + velocityy[0] + ((velocityy[1] - velocityy[0])/2);  

  accelerationx[1] = accelerationx[0];   //The current acceleration value must be sent 
                                         //to the previous acceleration 
  accelerationy[1] = accelerationy[0];   //variable in order to introduce the new 
                                         //acceleration value.
    
  velocityx[1] = velocityx[0];         //Same done for the velocity variable
  velocityy[1] = velocityy[0];
  
  /* positionX[1] = positionX[1]*262144;       //The idea behind this shifting (multiplication) 
                                         //is a sensibility adjustment. <<18
  positionY[1] = positionY[1]*262144;      //Some applications require adjustments to a 
                                        //particular situation 
                                        //i.e. mouse application 
  */
  
  //data_transfer();
  
  /*  
  positionX[1] = positionX[1]/262144;      //once the variables are sent them must return to 
  positionY[1] = positionY[1]/262144;      //their original state
  */
  //text("px:  " + positionX[0] + "\n" + "py:  " + positionY[0], 200, 200);
  
  movement_end_check();
  
  /*  
  positionX[0] = positionX[1];          //actual position data must be sent to the  
  positionY[0] = positionY[1];     //previous position
  */  

  direction = 0;                        // data variable to direction variable reset


}    

//*****************************************************************************/

// compensate the accelerometer readings from gravity. 
// @param q the quaternion representing the orientation of a 9DOM MARG sensor array
// @param acc the readings coming from an accelerometer expressed in g
//
// Code snippet from Fabio Versano - blog on same topic
//
// @return a 3d vector representing dynamic acceleration expressed in g
public void gravity_compensate(){
  float [] g = {0.0f, 0.0f, 0.0f};
  
  // get expected direction of gravity
  g[0] = 2 * (q[1] * q[3] - q[0] * q[2]);
  g[1] = 2 * (q[0] * q[1] + q[2] * q[3]);
  g[2] = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
  
  // compensate accelerometer readings with the expected direction of gravity
  //return [acc[0] - g[0], acc[1] - g[1], acc[2] - g[2]]
  Sample_X = acc[0] - g[0];
  Sample_Y = acc[1] - g[1];

 }





public class MovingAverage {
    private final Queue<Float> window = new LinkedList<Float>();
    private final int period;
    private float sum;
 
    public MovingAverage(int period) {
        assert period > 0 : "Period must be a positive integer";
        this.period = period;
    }
 
    public void newNum(float num) {
        sum += num;
        window.add(num);
        if (window.size() > period) {
            sum -= window.remove();
        }
    }
 
    public float getAvg() {
        if (window.isEmpty()) return 0; // technically the average is undefined
        return sum / window.size();
    }
}

public class EMA {
    private float alpha;
    private float oldValue;
    public EMA(float alpha) {
        this.alpha = alpha;
    }

    public float getAvg(float value){
        //if(oldValue == null) {
        //    oldValue = value;
        //    return value;
        //}
        float newValue = oldValue + alpha * (value - oldValue);
        oldValue = newValue;
        return newValue;
    }
}


////////////////////////////////////////////////////////////////
//
//  Setups ControlP5 controllers
//
////////////////////////////////////////////////////////////////

public void setValues(){

  //input field for orbital radius....
  cp5.addTextfield("SL Press(mb)")
    .setPosition(50,250)
    .setSize(100, 40)
    .setAutoClear(false)
    .setVisible(true)
    .setValue(str(sea_press));
 
   //cp5.addToggle("Calib",true, 60, 320, 20,20).setLabel("Calib(on)");
   cp5.addTextfield("Calib(on/off)")
    .setPosition(50,320)
    .setSize(100, 40)
    .setAutoClear(false)
    .setVisible(true)
    .setValue(str(calib));
    
   cp5.addBang("START")
    .setPosition(50, 380)
    .setSize(80, 40)
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER); 
  
}

public void controlEvent(ControlEvent theEvent) {
  if (theEvent.controller().name()=="START") {
    sea_press = PApplet.parseFloat(cp5.get(Textfield.class, "SL Press(mb)").getText());
    calib = PApplet.parseInt(cp5.get(Textfield.class, "Calib(on/off)").getText());
    ArtHorFlg = 1;
  }
  println(calib);
}

// This class based on code found here: http://www.goldb.org/stopwatchjava.html
// http://forum.processing.org/one/topic/timer-in-processing.html

class StopWatchTimer {
  int startTime = 0, stopTime = 0;
  boolean running = false;  
   
    public void start() {
        startTime = millis();
        running = true;
    }
    public void stop() {
        stopTime = millis();
        running = false;
    }
    public int getElapsedTime() {
        int elapsed;
        if (running) {
             elapsed = (millis() - startTime);
        }
        else {
            elapsed = (stopTime - startTime);
        }
        return elapsed;
    }
    public int second() {
      return (getElapsedTime() / 1000) % 60;
    }
    public int minute() {
      return (getElapsedTime() / (1000*60)) % 60;
    }
    public int hour() {
      return (getElapsedTime() / (1000*60*60)) % 24;
    }
}

/*
//****************************************************************************
//The purpose of the calibration routine is to obtain the value of the 
//reference threshold. It consists on a 1024 samples average in no-movement 
//condition.
//*****************************************************************************

void Calibrate() 
{
  int count1;
  count1 = 0;
  do {  
  //ADC_GetAllAxis();
  gravity_compensate();
  sstatex = sstatex + Sample_X;             // Accumulate Samples
  sstatey = sstatey + Sample_Y;
  println(Sample_X);
  count1++;
  } while(count1 != 0x0400);                   // 1024 times

  sstatex=sstatex / 1024;                      // division between l024
  sstatey=sstatey / 1024;
  
}

*/

/* 
//**************************************************************************/
//**************************************************************************/ 
//This function obtains magnitude and direction 
//In   this   particular   protocol   direction   and   magnitude   are   sent   in   separate   variables. 
//Management can be done in many other different ways.
//**************************************************************************/  

/* void data_transfer() 
{
 
 float positionXbkp; 
 float positionYbkp;
 int delay;
 float [] posx_seg = new float [4];
 float [] posy_seg = new float [4]; 
 
 
  if (positionX[1] >= 0) {                          //This line compares the sign of the X direction data 
    direction = (direction | 0x10);                 //if its positive the most significant byte 
    posx_seg[0]= positionX[1] & 0x000000FF;         //is set to 1 else it is set to 8
    posx_seg[1]= (positionX[1]>>8) & 0x000000FF;    //the data is also managed in the 
                                                    // subsequent lines in order to 
    posx_seg[2]= (positionX[1]>>16) & 0x000000FF;   // be sent. The 32 bit variable must be 
    posx_seg[3]= (positionX[1]>>24) & 0x000000FF;   // split into 4 different 8 bit 
                                                    // variables in order to be sent via   
                                                    // the 8 bit SCI frame

      }

    else {direction = (direction | 0x80);
             positionXbkp = positionX[1]-1;
             positionXbkp = positionXbkp & 0xFFFFFFFF;
             posx_seg[0] = positionXbkp & 0x000000FF;
             posx_seg[1] = (positionXbkp >> 8) & 0x000000FF;
             posx_seg[2] = (positionXbkp >> 16) & 0x000000FF;
             posx_seg[3] = (positionXbkp >> 24) & 0x000000FF;
             } 
  
  if (positionY[1] >=0 ) {                            //  Same management than in the previous case 
             direction= (direction | 0x08);        // but with the Y data.
             posy_seg[0]= positionY[1] & 0x000000FF;
             posy_seg[1]= (positionY[1]>>8) & 0x000000FF;
             posy_seg[2]= (positionY[1]>>16) & 0x000000FF;
             posy_seg[3]= (positionY[1]>>24) & 0x000000FF;
             }
  
  else {direction= (direction | 0x01);
             positionYbkp = positionY[1]-1;
             positionYbkp = positionYbkp & 0xFFFFFFFF;
             posy_seg[0] = positionYbkp & 0x000000FF;
             posy_seg[1] = (positionYbkp >> 8) & 0x000000FF;
             posy_seg[2] = (positionYbkp >> 16) & 0x000000FF;
             posy_seg[3] = (positionYbkp >> 24) & 0x000000FF;
             } 

  delay =0x0100;
  
  Sensor_Data[0] = 0x03;
  Sensor_Data[1] = direction;
  Sensor_Data[2] = posx_seg[3];
  Sensor_Data[3] = posy_seg[3];
  Sensor_Data[4] = 0x01;
  Sensor_Data[7] = 0x01;
  Sensor_Data[6] = 9999;
  
  //while (--delay);                                   
  
  //SCITxMsg(Sensor_Data);         //  Data transferring function   
  //while (SCIC2 & 0x08);

} */


//************************************************************************
//************************************************************************
//This function returns data format to its original state. When obtaining 
//the magnitude and  direction of the position, an inverse two's complement 
//is made. This function makes the two's complement in order to return the 
//data to it original state.
//It is important to notice that the sensibility adjustment is greatly 
//impacted here, the amount of "ones" inserted in the mask must be equivalent
//to the "ones" lost in the shifting made in the previous function upon the 
//sensibility modification. 
//************************************************************************

/* void data_reintegration() 
{
  if (direction >= 10) 
   {
    positionX[1]= positionX[1] | 0xFFFFC000; }   // l8 "ones" inserted. Same size as the 
                  //amount of shifts 
    
    direction = direction & 0x01;
    if (direction == 1)   
      {  positionY[1]= positionY[1] | 0xFFFFC000;  }
 } */

  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "FreeIMU_cube_Odo_Exp" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
