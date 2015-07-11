import processing.core.*; 
import processing.xml.*; 

import processing.opengl.*; 
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

public class OBJLoader_BoundingBox extends PApplet {





OBJModel model;

// BoundingBox is a class within OBJModel. Check docs for all it can do.
BoundingBox bbox;

public void setup()
{

  size(600,400,OPENGL);

  model = new OBJModel(this, "map_ground_path_s.obj", "relative", QUADS);
  model.enableDebug();

  model.scale(100);
  model.translateToCenter();

  bbox = new BoundingBox(this, model);
}


public void draw(){
  background(128);
  lights();
  translate(width/2, height/2, 0);
  //rotateX(radians(frameCount)/2);
  rotateY(radians(frameCount)/2);


  for(int i = -1; i < 2; i ++){
    pushMatrix();
    translate(0,0,i*bbox.getWHD().z);
    model.draw();
    popMatrix();
  }


  noFill();
  stroke(255,0,255);
  bbox.draw();
  noStroke();
}

    static public void main(String args[]) {
        PApplet.main(new String[] { "--bgcolor=#FFFFFF", "OBJLoader_BoundingBox" });
    }
}
