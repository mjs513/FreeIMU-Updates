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

public class OBJLoader_FaceLocation_MATTD extends PApplet {



OBJModel model;

float rotX;
float rotY;

float normLength = -25;

PVector pos = new PVector(0,0,0);
PVector dir = new PVector(1,0,0);

AABB volume;

public void setup() {
    size(600, 600, P3D);

    model = new OBJModel(this, "cubic_sphere.obj");

    model.scale(3);
    model.translateToCenter();

    volume = new AABB(0,-150,-150, 40,150,150);
}

public void draw() {

    pos.add(dir);

    if(pos.x > 200 || pos.x < -200) {

        dir.mult (-1);
    }

    volume.moveCenterTo(pos);


    background(32);
    lights();

    pushMatrix();
    translate(width / 2, height / 2, 0); 

    rotateX(rotY);
    rotateY(rotX);

    volume.draw();

    noStroke();

    for (int j = 0; j < model.getSegmentCount(); j++)
    {

        Segment segment = model.getSegment(j);

        Face[] faces = segment.getFaces();

        beginShape(QUADS);

        for (int i = 0; i < faces.length; i++) {
            Face f = faces[i];

            if(volume.isColliding( f.getCenter() )) {

                PVector[] vs = f.getVertices();
                PVector[] ns = f.getNormals();
                for (int k = 0; k < vs.length; k++) {
                    normal(ns[k].x, ns[k].y, ns[k].z);
                    vertex(vs[k].x, vs[k].y, vs[k].z);
                }
            }
        }

        endShape();
    }
    popMatrix();
}

public void mouseDragged() {
    rotX += (mouseX - pmouseX) * 0.01f;
    rotY -= (mouseY - pmouseY) * 0.01f;
}












class AABB
{
  PVector min = new PVector();
  PVector max = new PVector();

  AABB()
  {

    min.set(0, 0, 0);

    max.set(0, 0, 0);
  }


  AABB(float x1, float y1, float z1, float x2, float y2, float z2)
  {

    reset();

    addPoint(x1, y1, z1);

    addPoint(x2, y2, z2);
  }

  AABB(PVector p1, PVector p2)
  {
    reset();

    addPoint(p1);

    addPoint(p2);
  }

  public void reset()
  {
    min.x = MAX_FLOAT;
    min.y = MAX_FLOAT;
    min.z = MAX_FLOAT;
    max.x = MIN_FLOAT;
    max.y = MIN_FLOAT;
    max.z = MIN_FLOAT;
  }


  public void addPoint(PVector point)
  {
    addPoint(point.x, point.y, point.z);
  }


  public void addPoint(float x, float y, float z)
  {
    if(x < min.x) {
      min.x = x;
    }
    if(x > max.x) {
      max.x = x;
    }
    if(y < min.y) {
      min.y = y;
    }
    if(y > max.y) {
      max.y = y;
    }
    if(z < min.z) {
      min.z = z;
    }
    if(z > max.z) {
      max.z = z;
    }		
  }


  public void moveCenterTo(PVector p)
  {
    // get the center of the volume
    PVector a = new PVector(min.x, min.y, min.z);
    PVector b = new PVector(max.x, max.y, max.z);    

    b.sub(a);
    b.div(2);
    b.add(min);

    // get the position difference
    PVector c = new PVector(p.x, p.y, p.z);
    c.sub(b);


    min.add(c);
    max.add(c);

  }

  public void draw()
  {
    pushStyle();

    noFill();

    stroke(255, 0, 255);

    pushMatrix();

    PVector center = new PVector(max.x, max.y, max.z);
    center.sub(min);
    center.div(2);
    center.add(min);

    PVector whd = new PVector(max.x, max.y, max.z);
    whd.sub(min);

    translate(center.x, center.y, center.z);

    box(whd.x, whd.y, whd.z);


    popMatrix();

    popStyle();

  }



  public boolean isColliding(PVector p)
  {
    return isColliding(p.x, p.y, p.z);
  }


  public boolean isColliding(float x, float y, float z)
  {
    if(min.x > x){
      return false;
    }
    if(max.x < x){
      return false;
    }
    if(min.y > y){
      return false;
    }
    if(max.y < y){
      return false;
    }
    if(min.z > z){
      return false;
    }
    if(max.z < z){
      return false;
    }		
    return true;
  }

}


    static public void main(String args[]) {
        PApplet.main(new String[] { "--bgcolor=#FFFFFF", "OBJLoader_FaceLocation_MATTD" });
    }
}
