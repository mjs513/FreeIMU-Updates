//////////////////////////////////////////////////////////
//
// Based on the work found here:
//           http://angela.homelinux.net:8080/drupal/code
//
//////////////////////////////////////////////////////////

void rotComp(){

  pushMatrix();
  //translate(width/2, height/2);
  translate(850,420);
  scale(0.9);
  rotateZ(-radians(corr_heading));
  image(cmpRng,0,0);
  scale(1/0.9);
  popMatrix();
  
  pushMatrix();
  translate(850,420);
  scale(0.75);
  image(cmpAP, 0, 0);
  scale(1/0.75);
  popMatrix();
}

