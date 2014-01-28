////////////////////////////////////////////////////////////////////////
//
// Based on the work found here:
//       http://scholarworks.csun.edu/handle/10211.2/1456
//
// and sample code:
//       http://processing.org/discourse/beta/num_1274893475.html
//
/////////////////////////////////////////////////////////////////////
void NewHorizon(){
 
  pushMatrix();
  translate(width/2.0, height/2.0, camDistance);
  rotateZ( radians(90)+ angx);    //rolls it
  //rotateX( radians(4-rotationX) );  
  rotateY( radians(95) + angy );  //pitch
  rotateX( radians(0 ));   //yaw
  rotate(radians(17));

  drawSphere(texmap);
  popMatrix();

  //frame
  translate(width/2.0, height/2.0, +30);
  //image(Aimage, -150, -140);
  image(Aimage, 0, 0);
  
  // center
  translate(0,5,90);
  scale(1.50);
  strokeWeight(8);
  stroke(255,0,0);
  line(-20,0,-5,0); line(-5,0,-5,5);
  line(5,0,20,0); line(5,0,5,5);
  line(0,-5,0,5);
  scale(1/1.5);
  noStroke();

}

////////////////////////////////////////////////////////////////////////
void setupSphere(float R, int xDetail, int yDetail){

  // Create a 2D grid of standardized mercator coordinates
  for(int i = 0; i <= xDetail; i++){
    xGrid[i]= i / (float) xDetail;
  } 
  for(int i = 0; i <= yDetail; i++){
    yGrid[i]= i / (float) yDetail;
  }

  textureMode(NORMAL);

  // Transform the 2D grid into a grid of points on the sphere, using the inverse mercator projection
  for(int i = 0; i <= xDetail; i++){
    for(int j = 0; j <= yDetail; j++){
      allPoints[i][j] = mercatorPoint(R, xGrid[i], yGrid[j]);
    }
  }
}

////////////////////////////////////////////////////////////////////////
float[] mercatorPoint(float R, float x, float y){

  float[] thisPoint = new float[3];
  float phi = x*2*PI;
  float theta = PI - y*PI;

  thisPoint[0] = R*sin(theta)*cos(phi);
  thisPoint[1] = R*sin(theta)*sin(phi);
  thisPoint[2] = R*cos(theta);

  return thisPoint;
}

////////////////////////////////////////////////////////////////////////
void drawSphere(PImage Map){

  for(int j = 0; j < yDetail; j++){
    beginShape(TRIANGLE_STRIP);
    texture(Map);
    for(int i = 0; i <= xDetail; i++){
      vertex(allPoints[i][j+1][0],   allPoints[i][j+1][1],   allPoints[i][j+1][2],   xGrid[i],   yGrid[j+1]);
      vertex(allPoints[i][j][0],     allPoints[i][j][1],     allPoints[i][j][2],     xGrid[i],   yGrid[j]);
    }
    endShape(CLOSE);
  }
}
 
