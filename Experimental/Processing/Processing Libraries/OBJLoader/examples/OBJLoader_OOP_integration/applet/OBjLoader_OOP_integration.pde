import saito.objloader.*;

SimpleClass s;

void setup()
{
  
  size(400, 400, P3D);
  
  s = new SimpleClass("map_ground_path_s.obj", this);
}


void draw()
{
  background(128);
  lights();
  
  translate(width/2, height/2, 0);

  rotateY(radians(frameCount)/2);
  
  s.draw();

}
