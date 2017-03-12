// Colors
color yellow_ = color(200, 200, 20), green_ = color(30, 120, 30), red_ = color(120, 30, 30), blue_ = color(50, 50, 100),
       grey_ = color(30, 30, 30),black_ = color(0, 0, 0),orange_ =color(200,128,0);

Slider scaleSlider, axSlider, aySlider, azSlider, gxSlider, gySlider, gzSlider,
       magxSlider, magySlider, magzSlider, yawSlider, pitchSlider, rollSlider, altSlider;

Toggle  tACC_ROLL, tACC_PITCH, tACC_Z, tGYRO_ROLL, tGYRO_PITCH, tGYRO_YAW, tMAGX, tMAGY, tMAGZ,
        tYAW, tPITCH, tROLL, tBARO;

Button buttonSTART, buttonSTOP;

cDataArray accxData   = new cDataArray(200), accyData    = new cDataArray(200), acczData     = new cDataArray(200),
           gyroxData  = new cDataArray(200), gyroyData   = new cDataArray(200), gyrozData    = new cDataArray(200),
           magxData   = new cDataArray(200), magyData    = new cDataArray(200), magzData     = new cDataArray(200),
           altData    = new cDataArray(200),
           yawData = new cDataArray(200), pitchData = new cDataArray(200), rollData = new cDataArray(200),
           debug4Data = new cDataArray(200);

boolean axGraph =true,ayGraph=true,azGraph=true,gxGraph=true,gyGraph=true,gzGraph=true,altGraph=true,headGraph=true, magxGraph =true,magyGraph=true,magzGraph=true,
        yawGraph = true,pitchGraph = true,rollGraph = true,debug4Graph = false,hideDraw=false,GraphicsInited=false;

void graphSetup(){
  g_graph  = new cGraph(xGraph+110,yGraph, 480, 200);
  
  scaleSlider = cp5.addSlider("SCALE",0,10,1,xGraph+515,yGraph-20,75,20).setLabel("");// GraphScaler

  buttonSTART  = cp5.addButton("bSTART",1,xGraph+110,yGraph-25,45,19).setLabel("START").setColorBackground(red_);
  buttonSTOP   = cp5.addButton("bSTOP",1,xGraph+160,yGraph-25,45,19).setLabel("STOP").setColorBackground(green_);


  color c,black;
  black = color(0,0,0);
  //int xo = xGraph-7;
  int xo = xGraph-7;
  int x = xGraph+40;
  int y1= yGraph+10;  //ACC
  int y2= yGraph+55;  //GYRO
  int y5= yGraph+100; //MAG
  int y3= yGraph+150; //ALT
  int y4= yGraph+165; //HEAD
  int y7= yGraph+185; //GPS
  int y6= yGraph+205; //DEBUG

  tACC_ROLL   = cp5.addToggle("ACC_ROLL",true,x,y1+10,20,10).setColorActive(color(255, 0, 0)).setColorBackground(black).setLabel(""); 
  tACC_PITCH  = cp5.addToggle("ACC_PITCH",true,x,y1+20,20,10).setColorActive(color(0, 255, 0)).setColorBackground(black).setLabel(""); 
  tACC_Z      = cp5.addToggle("ACC_Z",true,x,y1+30,20,10).setColorActive(color(0, 0, 255)).setColorBackground(black).setLabel(""); 
  tGYRO_ROLL  = cp5.addToggle("GYRO_ROLL",true,x,y2+10,20,10).setColorActive(color(200, 200, 0)).setColorBackground(black).setLabel(""); 
  tGYRO_PITCH = cp5.addToggle("GYRO_PITCH",true,x,y2+20,20,10).setColorActive(color(0, 255, 255)).setColorBackground(black).setLabel(""); 
  tGYRO_YAW   = cp5.addToggle("GYRO_YAW",true,x,y2+30,20,10).setColorActive(color(255, 0, 255)).setColorBackground(black).setLabel(""); 
  tMAGX       = cp5.addToggle("MAGX",true,x,y5+10,20,10).setColorActive(color(50, 100, 150)).setColorBackground(black).setLabel(""); 
  tMAGY       = cp5.addToggle("MAGY",true,x,y5+20,20,10).setColorActive(color(100, 50, 150)).setColorBackground(black).setLabel(""); 
  tMAGZ       = cp5.addToggle("MAGZ",true,x,y5+30,20,10).setColorActive(color(150, 100, 50)).setColorBackground(black).setLabel(""); 
  tYAW        = cp5.addToggle("YAW",true,x+110,y6,20,10).setColorActive(color(200, 50, 0)).setColorBackground(black).setLabel("");
  tPITCH      = cp5.addToggle("PITCH",true,x+230,y6,20,10).setColorActive(color(0, 200, 50)).setColorBackground(black).setLabel("");
  tROLL       = cp5.addToggle("ROLL",true,x+350,y6,20,10).setColorActive(color(50, 0, 200)).setColorBackground(black).setLabel("");
  tBARO       = cp5.addToggle("BARO",true,x,y3 ,30,10).setColorActive(color(125, 125, 125)).setColorBackground(black).setLabel(""); 

  cp5.addTextlabel("acclabel","ACC",xo,y1 -4);
  cp5.addTextlabel("accrolllabel","      ax",xo,y1+4).setFont(font10);
  cp5.addTextlabel("accpitchlabel","      ay",xo,y1+14).setFont(font10);
  cp5.addTextlabel("acczlabel","      az",xo,y1+24).setFont(font10);
  cp5.addTextlabel("gyrolabel","GYRO",xo,y2 -4);
  cp5.addTextlabel("gyrorolllabel","      gx",xo,y2+4).setFont(font10);
  cp5.addTextlabel("gyropitchlabel","      gy",xo,y2+14).setFont(font10);
  cp5.addTextlabel("gyroyawlabel","      gz",xo,y2+24).setFont(font10);
  cp5.addTextlabel("maglabel","MAG",xo,y5 -4);
  cp5.addTextlabel("magrolllabel","      mx",xo,y5+4).setFont(font10);
  cp5.addTextlabel("magpitchlabel","      my",xo,y5+14).setFont(font10);
  cp5.addTextlabel("magyawlabel","      mz",xo,y5+24).setFont(font10);
  cp5.addTextlabel("altitudelabel","ALT",xo,y3 -4);

  cp5.addTextlabel("Yaw","Yaw",x+80,y6 -2).setFont(font10);
  cp5.addTextlabel("Pitch","Pitch",x+195,y6 -2).setFont(font10);
  cp5.addTextlabel("Roll","Roll",x+320,y6 -2).setFont(font10);
  //cp5.addTextlabel("debug4","debug4",x+450,y6 -2).setFont(font9);

  axSlider = cp5.addSlider("axSlider",-1000,+1000,0,x+20,y1+10,50,10).setDecimalPrecision(0).setLabel("");
  aySlider = cp5.addSlider("aySlider",-1000,+1000,0,x+20,y1+20,50,10).setDecimalPrecision(0).setLabel("");
  azSlider = cp5.addSlider("azSlider",-1000,+1000,0,x+20,y1+30,50,10).setDecimalPrecision(0).setLabel("");
  gxSlider = cp5.addSlider("gxSlider",-5000,+5000,0,x+20,y2+10,50,10).setDecimalPrecision(0).setLabel("");
  gySlider = cp5.addSlider("gySlider",-5000,+5000,0,x+20,y2+20,50,10).setDecimalPrecision(0).setLabel("");
  gzSlider = cp5.addSlider("gzSlider",-5000,+5000,0,x+20,y2+30,50,10).setDecimalPrecision(0).setLabel("");
  altSlider = cp5.addSlider("altSlider",-30000,+30000,0,x+20,y3 ,50,10).setDecimalPrecision(2).setLabel("");
  magxSlider = cp5.addSlider("magxSlider",-5000,+5000,0,x+20,y5+10,50,10).setDecimalPrecision(0).setLabel("");
  magySlider = cp5.addSlider("magySlider",-5000,+5000,0,x+20,y5+20,50,10).setDecimalPrecision(0).setLabel("");
  magzSlider = cp5.addSlider("magzSlider",-5000,+5000,0,x+20,y5+30,50,10).setDecimalPrecision(0).setLabel("");
  yawSlider = cp5.addSlider("yawSlider",-180,+180,0,x+130,y6,50,10).setDecimalPrecision(0).setLabel("");
  pitchSlider = cp5.addSlider("pitchSlider",-90,+90,0,x+250,y6,50,10).setDecimalPrecision(0).setLabel("");
  rollSlider = cp5.addSlider("rollSlider",-90,+90,0,x+370,y6,50,10).setDecimalPrecision(0).setLabel("");
  //cp5.addSlider("debug4Slider",-32768,+32767,0,x+490,y6,50,10).setDecimalPrecision(0).setLabel("");

}

void graph(){
  // ---------------------------------------------------------------------------------------------
  // GRAPH
  // ---------------------------------------------------------------------------------------------

  strokeWeight(1);
  fill(255, 255, 255);
  g_graph.drawGraphBox();
  
  strokeWeight(1.5);
  stroke(255, 0, 0); if (axGraph) g_graph.drawLine(accxData, -10, +10);
  stroke(0, 255, 0); if (ayGraph) g_graph.drawLine(accyData, -10, +10);
  stroke(0, 0, 255);
  if (azGraph) {
    if (scaleSlider.getValue()<2){ g_graph.drawLine(acczData, -16, +16);
    } else{ g_graph.drawLine(acczData, 200*scaleSlider.getValue()-1000,200*scaleSlider.getValue()+500);}
  }
  
  float altMin = (altData.getMinVal() + altData.getRange() / 2) - 100;
  float altMax = (altData.getMaxVal() + altData.getRange() / 2) + 100;

  stroke(200, 200, 0);  if (gxGraph)   g_graph.drawLine(gyroxData, -10, +10);
  stroke(0, 255, 255);  if (gyGraph)   g_graph.drawLine(gyroyData, -10, +10);
  stroke(255, 0, 255);  if (gzGraph)   g_graph.drawLine(gyrozData, -10, +10);
  stroke(125, 125, 125);if (altGraph)  g_graph.drawLine(altData, altMin, altMax);
  //stroke(225, 225, 125);if (headGraph) g_graph.drawLine(headData, -370, +370);
  stroke(50, 100, 150); if (magxGraph) g_graph.drawLine(magxData, -10, +10);
  stroke(100, 50, 150); if (magyGraph) g_graph.drawLine(magyData, -10, +10);
  stroke(150, 100, 50); if (magzGraph) g_graph.drawLine(magzData, -10, +10);

  stroke(200, 50, 0); if (yawGraph)  g_graph.drawLine(yawData, -180, +180);
  stroke(0, 200, 50); if (pitchGraph)  g_graph.drawLine(pitchData, -90, +90);
  stroke(50, 0, 200); if (rollGraph)  g_graph.drawLine(rollData, -90, +90);
  //stroke(0, 0, 0);    if (debug4Graph)  g_graph.drawLine(debug4Data, -5000, +5000);

}


// This class takes the data and helps graph it
class cGraph {
  float m_gWidth, m_gHeight, m_gLeft, m_gBottom, m_gRight, m_gTop;
  
  cGraph(float x, float y, float w, float h) {
    m_gWidth     = w; m_gHeight    = h;
    m_gLeft      = x; m_gBottom    = y;
    m_gRight     = x + w;
    m_gTop       = y + h;
  }
  
  void drawGraphBox() {
    stroke(0, 0, 0);
    rectMode(CORNERS);
    rect(m_gLeft, m_gBottom, m_gRight, m_gTop);
  }
  
  void drawLine(cDataArray data, float minRange, float maxRange) {
    float graphMultX = m_gWidth/data.getMaxSize();
    float graphMultY = m_gHeight/(maxRange-minRange);
    
    for( i=0; i<data.getCurSize()-1; ++i) {
      float x0 = i*graphMultX+m_gLeft;
      float y0 = m_gTop-(((data.getVal(i)-(maxRange+minRange)/2)*scaleSlider.getValue()+(maxRange-minRange)/2)*graphMultY);
      float x1 = (i+1)*graphMultX+m_gLeft;
      float y1 = m_gTop-(((data.getVal(i+1)-(maxRange+minRange)/2 )*scaleSlider.getValue()+(maxRange-minRange)/2)*graphMultY);
      line(x0, y0, x1, y1);
    }
  }
}

//********************************************************
//******************************************************
//********************************************************

class cDataArray {
  float[] m_data;
  int m_maxSize, m_startIndex = 0, m_endIndex = 0, m_curSize;
  
  cDataArray(int maxSize){
    m_maxSize = maxSize;
    m_data = new float[maxSize];
  }
  void addVal(float val) {
    m_data[m_endIndex] = val;
    m_endIndex = (m_endIndex+1)%m_maxSize;
    if (m_curSize == m_maxSize) {
      m_startIndex = (m_startIndex+1)%m_maxSize;
    } else {
      m_curSize++;
    }
  }
  float getVal(int index) {return m_data[(m_startIndex+index)%m_maxSize];}
  int getCurSize(){return m_curSize;}
  int getMaxSize() {return m_maxSize;}
  float getMaxVal() {
    float res = 0.0;
    for( i=0; i<m_curSize-1; i++) if ((m_data[i] > res) || (i==0)) res = m_data[i];
    return res;
  }
  float getMinVal() {
    float res = 0.0;
    for( i=0; i<m_curSize-1; i++) if ((m_data[i] < res) || (i==0)) res = m_data[i];
    return res;
  }
  float getRange() {return getMaxVal() - getMinVal();}
}

void ACC_ROLL(boolean theFlag) {axGraph = theFlag;}
void ACC_PITCH(boolean theFlag) {ayGraph = theFlag;}
void ACC_Z(boolean theFlag) {azGraph = theFlag;}
void GYRO_ROLL(boolean theFlag) {gxGraph = theFlag;}
void GYRO_PITCH(boolean theFlag) {gyGraph = theFlag;}
void GYRO_YAW(boolean theFlag) {gzGraph = theFlag;}
//void BARO(boolean theFlag) {altGraph = theFlag;}
void HEAD(boolean theFlag) {headGraph = theFlag;}
void MAGX(boolean theFlag) {magxGraph = theFlag;}
void MAGY(boolean theFlag) {magyGraph = theFlag;}
void MAGZ(boolean theFlag) {magzGraph = theFlag;}
void YAW(boolean theFlag) {yawGraph = theFlag;}
void PICTH(boolean theFlag) {pitchGraph = theFlag;}
void ROLL(boolean theFlag) {rollGraph = theFlag;}
//void DEBUG4(boolean theFlag) {debug4Graph = theFlag;}

public void bSTART() {
  //if(graphEnabled == false) {return;}
  buttonSTART.setColorBackground(green_);
  buttonSTOP.setColorBackground(red_);
  graph_on=1;
}

public void bSTOP() {
  buttonSTART.setColorBackground(red_);
  buttonSTOP.setColorBackground(green_);
  graph_on=0;
}