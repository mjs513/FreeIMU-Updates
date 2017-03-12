void flci() {  // ---------------------------------------------------------------------------------------------
  // Fly Level Control Instruments
  // ---------------------------------------------------------------------------------------------
  // info angles
  fill(255,255,127);
  textFont(font12);
  text(nfp(degrees(ypr[1]),3,2) + "°", xLevelObj+48, yLevelObj+62); //pitch
  text(nfp(degrees(ypr[2]),3,2) + "°", xLevelObj-62, yLevelObj+62); //roll

  pushMatrix();
  translate(xLevelObj-34,yLevelObj+112);
  fill(50,50,50);
  noStroke();
  ellipse(0,0,66,66);
  rotate(ypr[2]);
  fill(255,255,127);
  textFont(font12);text("ROLL", -13, 5);
  strokeWeight(1.5);
  stroke(127,127,127);
  line(-30,1,30,1);
  stroke(255,255,255);
  line(-30,0,+30,0);line(0,0,0,-10);
  popMatrix();
  
  pushMatrix();
  translate(xLevelObj+34,yLevelObj+112);
  fill(50,50,50);
  noStroke();
  ellipse(0,0,66,66);
  rotate(ypr[1]);
  fill(255,255,127);
  textFont(font12);text("PITCH", -18, 5);
  strokeWeight(1.5);
  stroke(127,127,127);
  line(-30,1,30,1);
  stroke(255,255,255);
  line(-30,0,30,0);line(30,0,30-size/6 ,size/6);line(+30,0,30-size/6 ,-size/6);  
  popMatrix();
}

void magnetron(){
  // ---------------------------------------------------------------------------------------------
  // Magnetron Combi Fly Level Control
  // ---------------------------------------------------------------------------------------------
  angy = ypr[1] * 180/PI;
  horizonInstrSize=68;
  angyLevelControl=((angy<-horizonInstrSize) ? -horizonInstrSize : (angy>horizonInstrSize) ? horizonInstrSize : angy);
  pushMatrix();
  translate(xLevelObj,yLevelObj);
  noStroke();
  // instrument background
  fill(50,50,50);
  ellipse(0,0,150,150);
  // full instrument
  rotate(-ypr[2]);
  rectMode(CORNER);
  // outer border
  strokeWeight(1);
  stroke(90,90,90);
  //border ext
  arc(0,0,140,140,0,TWO_PI);
  stroke(190,190,190);
  //border int
  arc(0,0,138,138,0,TWO_PI);
  // inner quadrant
  strokeWeight(1);
  stroke(255,255,255);
  fill(124,73,31);
  //earth
  float angle = acos(angyLevelControl/horizonInstrSize);
  arc(0,0,136,136,0,TWO_PI);
  fill(38,139,224); 
  //sky 
  arc(0,0,136,136,HALF_PI-angle+PI,HALF_PI+angle+PI);
  float x = sin(angle)*horizonInstrSize;
  if (angy>0) 
    fill(124,73,31);
  noStroke();   
  triangle(0,0,x,-angyLevelControl,-x,-angyLevelControl);
  // inner lines
  strokeWeight(1);
  for(i=0;i<8;i++) {
    j=i*15;
    if (angy<=(35-j) && angy>=(-65-j)) {
      stroke(255,255,255); line(-30,-15-j-angy,30,-15-j-angy); // up line
      fill(255,255,255);
      textFont(font9);
      text("+" + (i+1) + "0", 34, -20-j-angy); //  up value
      text("+" + (i+1) + "0", -48, -20-j-angy); //  up value
    }
    if (angy<=(42-j) && angy>=(-58-j)) {
      stroke(167,167,167); line(-20,-7-j-angy,20,-7-j-angy); // up semi-line
    }
    if (angy<=(65+j) && angy>=(-35+j)) {
      stroke(255,255,255); line(-30,15+j-angy,30,15+j-angy); // down line
      fill(255,255,255);
      textFont(font9);
      text("-" + (i+1) + "0", 34, 8+j-angy); //  down value
      text("-" + (i+1) + "0", -48, 8+j-angy); //  down value
    }
    if (angy<=(58+j) && angy>=(-42+j)) {
      stroke(127,127,127); line(-20,7+j-angy,20,7+j-angy); // down semi-line
    }
  }
  strokeWeight(2);
  stroke(255,255,255);
  if (angy<=50 && angy>=-50) {
    line(-40,-angy,40,-angy); //center line
    fill(255,255,255);
    textFont(font9);
    text("0", 34, -5-angy); // center
    text("0", -39, -5-angy); // center
  }

  // lateral arrows
  strokeWeight(1);
  // down fixed triangle
  stroke(60,60,60);
  fill(180,180,180,255);

  triangle(-horizonInstrSize,-8,-horizonInstrSize,8,-55,0);
  triangle(horizonInstrSize,-8,horizonInstrSize,8,55,0);

  // center
  strokeWeight(1);
  stroke(255,0,0);
  line(-20,0,-5,0); line(-5,0,-5,5);
  line(5,0,20,0); line(5,0,5,5);
  line(0,-5,0,5);
  popMatrix();

}

void compass(){
  // ---------------------------------------------------------------------------------------------
  // Compass Section
  // ---------------------------------------------------------------------------------------------
  pushMatrix();
  translate(xCompass,yCompass);
  // Compass Background
  fill(0, 0, 0);
  strokeWeight(3);stroke(0);
  rectMode(CORNERS);
  size=29;
  rect(-size*2.5,-size*2.5,size*2.5,size*2.5);
  // GPS quadrant
  strokeWeight(1.5);
  if (GPS_update == 1) {
    fill(125);stroke(125);
  } else {
    fill(160);stroke(160);
  }
  ellipse(0,  0,   4*size+7, 4*size+7);
  // GPS rotating pointer
  rotate(GPS_directionToHome*PI/180);
  strokeWeight(4);stroke(255,255,100);line(0,0, 0,-2.4*size);line(0,-2.4*size, -5 ,-2.4*size+10); line(0,-2.4*size, +5 ,-2.4*size+10);  
  rotate(-GPS_directionToHome*PI/180);
  // compass quadrant
  strokeWeight(1.5);fill(0);stroke(0);
  ellipse(0,  0,   2.6*size+7, 2.6*size+7);
  // Compass rotating pointer
  stroke(255);
  rotate(corr_heading*PI/180);
  line(0,size*0.2, 0,-size*1.3); line(0,-size*1.3, -5 ,-size*1.3+10); line(0,-size*1.3, +5 ,-size*1.3+10);
  popMatrix();
  // angles 
  for (i=0;i<=12;i++) {
    angCalc=i*PI/6;
    if (i%3!=0) {
      stroke(75);
      line(xCompass+cos(angCalc)*size*2,yCompass+sin(angCalc)*size*2,xCompass+cos(angCalc)*size*1.6,yCompass+sin(angCalc)*size*1.6);
    } else {
      stroke(255);
      line(xCompass+cos(angCalc)*size*2.2,yCompass+sin(angCalc)*size*2.2,xCompass+cos(angCalc)*size*1.9,yCompass+sin(angCalc)*size*1.9);
    }
  }
  textFont(font15);
  text("N", xCompass-5, yCompass-33-size*0.9);text("S", xCompass-5, yCompass+19+size*0.9);
  text("W", xCompass-33-size*0.9, yCompass-9);text("E", xCompass+21+size*0.9, yCompass-9);
  // head indicator
  textFont(font12);
  noStroke();
  fill(80,80,80,130);
  rect(xCompass-22,yCompass-8,xCompass+22,yCompass+9);
  fill(255,255,127);
  text(corr_heading + "°",xCompass-11-(corr_heading>=10.0 ? (corr_heading>=100.0 ? 6 : 3) : 0),yCompass-6);
  // GPS direction indicator
  fill(255,255,0);
  text(GPS_directionToHome + "°",xCompass-6-size*2.1,yCompass-7+size*2);
  // GPS fix
  if (GPS_fix==0) {
     fill(127,0,0);
  } else {
     fill(0,255,0);
  }
  //ellipse(xCompass+3+size*2.1,yCompass+3+size*2,12,12);
  rect(xCompass-28+size*2.1,yCompass+1+size*2,xCompass+9+size*2.1,yCompass+13+size*2);
  textFont(font9);
  if (GPS_fix==0) {
    fill(255,255,0);
  } else {
    fill(0,50,0);
  }
  text("GPS_fix",xCompass-27+size*2.1,yCompass+0+size*2);
  rectMode(CORNER);

}

//////////////////////////////////////////////////////////////////////////
void drawCube() {  
    pushMatrix();
    translate(VIEW_SIZE_X/2, VIEW_SIZE_Y/2 - 130, +80);
    scale(2,2,2);
    // a demonstration of the following is at 
    // http://www.varesano.net/blog/fabio/ahrs-sensor-fusion-orientation-filter-3d-graphical-rotating-cube
    rotateZ(-Euler[2]);
    rotateX(-Euler[1]+radians(-17));
    rotateY(-Euler[0]);
    
    buildBoxShape();
    
  popMatrix();
}

//////////////////////////////////////////////////////////////////////////
void buildBoxShape() {
  //box(60, 10, 40);
  //noStroke();
  
  //Z+ (to the drawing area)   FRONT
  beginShape(QUADS);
  fill(#00ff00);
  //texture(Top);
  vertex(-30, -5, 20);
  vertex(30, -5, 20);
  vertex(30, 5, 20);
  vertex(-30, 5, 20);
  endShape();
  
  beginShape(QUADS);  
  //Z-
  fill(#0000ff);
  vertex(-30, -5, -20);
  vertex(30, -5, -20);
  vertex(30, 5, -20);
  vertex(-30, 5, -20);
  endShape();
  
  beginShape(QUADS);  
  //X-
  fill(#ff0000);
  vertex(-30, -5, -20);
  vertex(-30, -5, 20);
  vertex(-30, 5, 20);
  vertex(-30, 5, -20);
  endShape();
  
  beginShape(QUADS);  
  //X+ RIGHT SIDE
  fill(#ffff00);
  vertex(30, -5, -20);
  vertex(30, -5, 20);
  vertex(30, 5, 20);
  vertex(30, 5, -20);
  endShape();
    
  beginShape(QUADS);  
  //Y-
  //fill(#ff00ff);
  texture(Top);
  vertex(-30, -5, -20, 0, 0);
  vertex(30, -5, -20, 1, 0);
  vertex(30, -5, 20, 1, 1);
  vertex(-30, -5, 20, 0, 1);
  endShape();
  
  beginShape(QUADS);  
  //Y+ Bottom
  //fill(#00ffff);
  texture(Bottom);
  vertex(-30, 5, -20, 0, 0);
  vertex(30, 5, -20, 1, 0);
  vertex(30, 5, 20, 1, 1);
  vertex(-30, 5, 20, 0, 1);
  endShape();
}