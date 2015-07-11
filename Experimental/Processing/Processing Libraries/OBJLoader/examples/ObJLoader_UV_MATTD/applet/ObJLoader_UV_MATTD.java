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

public class ObJLoader_UV_MATTD extends PApplet {



// declare that we need a OBJModel and we'll be calling it "model"
OBJModel model;
OBJModel tmpmodel;
float rotX;
float rotY;


public void setup()
{
  size(600, 600, P3D);

  // making an object called "model" that is a new instance of OBJModel
  model = new OBJModel(this, "cube_sphere_test.obj");
  tmpmodel = new OBJModel(this, "cube_sphere_test.obj");

  // turning on the debug output (it's all the stuff that spews out in the black box down the bottom)
  model.enableDebug();

  noStroke();
}

int count = 0;

public void draw()
{
  background(128);
  lights();

  pushMatrix();
  translate(width/2, height/2, 0);
  rotateX(rotY);
  rotateY(rotX);
  scale(3);

  model.draw();
  popMatrix();
  
  for(int i = 0; i < model.getUVCount(); i ++)
  {

    PVector u = model.getUV(i);
    PVector stable_u = tmpmodel.getUV(i);

    u.x = stable_u.x + sin(radians(frameCount))/2;
    
  }
  
}


public void mouseDragged()
{
  rotX += (mouseX - pmouseX) * 0.01f;
  rotY -= (mouseY - pmouseY) * 0.01f;
}


    static public void main(String args[]) {
        PApplet.main(new String[] { "--bgcolor=#FFFFFF", "ObJLoader_UV_MATTD" });
    }
}
