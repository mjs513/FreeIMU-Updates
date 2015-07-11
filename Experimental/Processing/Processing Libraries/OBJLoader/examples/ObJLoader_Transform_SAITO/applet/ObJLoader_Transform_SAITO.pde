import saito.objloader.*;
OBJModel model;
OBJModel tmpmodel;
float rotX;
float rotY;
void setup()
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
void draw()
{
  background(128);
  lights();
  pushMatrix();
  translate(width/2, height/2, 0);
  rotateX(rotY);
  rotateY(rotX);
  scale(20.0);

  // renders the temporary model
  tmpmodel.draw();

  popMatrix();

  animation();
}
// transformation parameter
float k = 0.0;
// transforms the orignal model shape and stores transformed shape 
// into temporary model storage
void animation(){

  for(int i = 0; i < model.getVertexCount(); i++){
    PVector orgv = model.getVertex(i);
    PVector tmpv = new PVector();
    tmpv.x = orgv.x * (abs(sin(k+i*0.04)) * 0.3 + 1.0);
    tmpv.y = orgv.y * (abs(cos(k+i*0.04)) * 0.3 + 1.0);
    tmpv.z = orgv.z * (abs(cos(k/5.)) * 0.3 + 1.0);
    tmpmodel.setVertex(i, tmpv);
  }
  k+=0.1;
}

void mouseDragged()
{
  rotX += (mouseX - pmouseX) * 0.01;
  rotY -= (mouseY - pmouseY) * 0.01;
}


