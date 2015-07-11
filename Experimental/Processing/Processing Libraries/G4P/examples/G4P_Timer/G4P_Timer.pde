/**
Balls of Vesuvius.  
 
A simple program to demonstrate the use of the GTimer
class which is part of the G4P (GUI for Processing)
library.

(c)2012 Peter Lager

*/

import g4p_controls.*;

GSlider sdrRate;
GButton btnStart, btnStop;
GTimer timer;

ArrayList liveBalls, deadBalls;
int rate;
PImage rear, front;

void setup(){
  size(768,600);
  // Create 2 buttons to start and stop the balls
  btnStart = new GButton(this, 10, 10, 100, 20, "Start");
  btnStop = new GButton(this, 120, 10, 100, 20, "Stop");
  // Create a slider to control rate of balls erupted.
  sdrRate = new GSlider(this, 230, 10, 360, 20, 10);
  sdrRate.setLimits(50, 10, 120); // (init, min, max)
  sdrRate.setEasing(5);
  
  // Get timer interval based on initial slider value and limits
  rate = 130 - sdrRate.getValueI();
  // Create a GTimer object that will call the method
  // fireBall 
  // Parameter 1 : the PApplet class i.e. 'this'
  //           2 : the object that has the method to call
  //           3 : the name of the method (parameterless) to call
  //           4 : the interval in millisecs bewteen method calls
  timer = new GTimer(this, this, "fireBall", rate);

  // Balls in the air alive
  liveBalls = new ArrayList(2000);
  // Balls that are below the level of the window
  deadBalls = new ArrayList(100);

  front = loadImage("vfront.png");
  rear = loadImage("vrear.jpg");

  // try and keep it at 30fps
  frameRate(30);

  // Register the pre() method for this class. Pick the line
  // to match the version of Processing being used.
  // registerPre(this); // Use this for PS 1.5.1
  registerMethod("pre", this); // Use this for PS 2.0b6
}

// This method is now called before each call to draw()
public void pre(){
  Ball b;
  int i;
  // Update all live balls
  for(i = 0; i < liveBalls.size(); i++){
    b = (Ball)liveBalls.get(i);
    b.update();
    // See if this ball should die if so remember it
    if(b.y > height + 20)
      deadBalls.add(b);
  }
  // Remove dead balls from the list of live balls
  for(i = 0; i < deadBalls.size(); i++){
    liveBalls.remove(deadBalls.get(i));
  }
  // Done with dead balls
  deadBalls.clear();
}

void draw(){
  int i;
  Ball b;

  background(rear);
  for(i = 0; i < liveBalls.size(); i++){
    b = (Ball)liveBalls.get(i);
    b.display();
  }
  image(front,0,0);
}

// This is called when the user drags on the slider
void handleSliderEvents(GValueControl slider, GEvent event){
  rate = 130 - sdrRate.getValueI();
  timer.setInterval(rate);
}

// This method is called when a button is clicked
void handleButtonEvents(GButton button, GEvent event){
  if(button == btnStart && event == GEvent.CLICKED)
    timer.start();
  if(button == btnStop && event == GEvent.CLICKED)
    timer.stop();
}

// This method is called by the timer
void fireBall(GTimer timer){
  Ball ball = new Ball();
  liveBalls.add(ball);
}

// Simple class to represent a ball
class Ball {
  public float radius;
  public int col;
  public float x, y;
  public float vx, vy;
  public float gravity = 0.07f;
  public float drag = 0.99;
  public float shrink = 0.999;

  public Ball(){
    x = random(500,540);
    y = 290;
    col = color(random(200,255),random(20,55),0);
    radius = random(3,10);
    vx = random(-3.0, 1.9);
    vy = random(5.5, 8.2);
  }

  public void update(){
    x += vx;
    y -= vy;
    vy -= gravity;
    if(vy < 0)
      vx *= drag;
    radius *= shrink;
  }

  public void display(){
    noStroke();
    fill(col);
    ellipse(x,y,radius,radius);
  }

}