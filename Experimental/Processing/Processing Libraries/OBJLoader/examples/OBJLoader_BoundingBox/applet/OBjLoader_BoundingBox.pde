import processing.opengl.*;

import saito.objloader.*;

OBJModel model;

// BoundingBox is a class within OBJModel. Check docs for all it can do.
BoundingBox bbox;

void setup()
{

  size(600,400,OPENGL);

  model = new OBJModel(this, "map_ground_path_s.obj", "relative", QUADS);
  model.enableDebug();

  model.scale(100);
  model.translateToCenter();

  bbox = new BoundingBox(this, model);
}


void draw(){
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

