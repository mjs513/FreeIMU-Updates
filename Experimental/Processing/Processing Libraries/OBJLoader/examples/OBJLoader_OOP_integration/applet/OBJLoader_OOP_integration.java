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

public class OBJLoader_OOP_integration extends PApplet {



SimpleClass s;

public void setup()
{
  
  size(400, 400, P3D);
  
  s = new SimpleClass("map_ground_path_s.obj", this);
}


public void draw()
{
  background(128);
  lights();
  
  translate(width/2, height/2, 0);

  rotateY(radians(frameCount)/2);
  
  s.draw();

}
class SimpleClass
{

  OBJModel model; 

  SimpleClass(String s, PApplet parent)
  {
    model = new OBJModel(parent, s, OBJModel.ABSOLUTE, QUADS);

    // this is done becaue the model was made in meters 1 = 1m
    // where processing is 1 = 1pixel
    model.scale(100);
    model.translateToCenter();
  }

  public void draw()
  {
      noStroke();
    model.draw(); 
  }
}

    static public void main(String args[]) {
        PApplet.main(new String[] { "--bgcolor=#FFFFFF", "OBJLoader_OOP_integration" });
    }
}
