import processing.core.*; 
import processing.xml.*; 

import saito.objloader.*; 

import java.applet.*; 
import java.awt.Dimension; 
import java.awt.Frame; 
import java.awt.event.MouseEvent; 
import java.awt.event.KeyEvent; 
import java.awt.event.FocusEvent; 
import java.awt.Image; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class ObJLoader_Transform_SAITO extends PApplet {


OBJModel model;
OBJModel tmpmodel;
float rotX;
float rotY;
public void setup()
{
  size(600, 600, P3D);
  frameRate(30);
  model = new OBJModel(this);
  tmpmodel = new OBJModel(this);
  model.enableDebug();
  model.load("dma.obj");
  tmpmodel.load("dma.obj");
  stroke(255);
}
public void draw()
{
  background(128);
  lights();
  pushMatrix();
  translate(width/2, height/2, 0);
  rotateX(rotY);
  rotateY(rotX);
  scale(20.0f);

  // renders the temporary model
  tmpmodel.draw();

  popMatrix();

  animation();
}
// transformation parameter
float k = 0.0f;
// transforms the orignal model shape and stores transformed shape 
// into temporary model storage
public void animation(){

  for(int i = 0; i < model.getVertexCount(); i++){
    PVector orgv = model.getVertex(i);
    PVector tmpv = new PVector();
    tmpv.x = orgv.x * (abs(sin(k+i*0.04f)) * 0.3f + 1.0f);
    tmpv.y = orgv.y * (abs(cos(k+i*0.04f)) * 0.3f + 1.0f);
    tmpv.z = orgv.z * (abs(cos(k/5.f)) * 0.3f + 1.0f);
    tmpmodel.setVertex(i, tmpv);
  }
  k+=0.1f;
}

public void mouseDragged()
{
  rotX += (mouseX - pmouseX) * 0.01f;
  rotY -= (mouseY - pmouseY) * 0.01f;
}


    static public void main(String args[]) {
        PApplet.main(new String[] { "--bgcolor=#FFFFFF", "ObJLoader_Transform_SAITO" });
    }
}
