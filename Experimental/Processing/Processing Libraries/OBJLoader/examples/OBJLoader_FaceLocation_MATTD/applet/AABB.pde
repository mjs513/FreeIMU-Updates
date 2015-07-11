
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

  void reset()
  {
    min.x = MAX_FLOAT;
    min.y = MAX_FLOAT;
    min.z = MAX_FLOAT;
    max.x = MIN_FLOAT;
    max.y = MIN_FLOAT;
    max.z = MIN_FLOAT;
  }


  void addPoint(PVector point)
  {
    addPoint(point.x, point.y, point.z);
  }


  void addPoint(float x, float y, float z)
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


  void moveCenterTo(PVector p)
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

  void draw()
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



  boolean isColliding(PVector p)
  {
    return isColliding(p.x, p.y, p.z);
  }


  boolean isColliding(float x, float y, float z)
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


