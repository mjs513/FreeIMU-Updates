/**
 Demonstration of the GKnob component which is part of 
 the G4P (GUI for Processing) library.
 
 Simple program that creates 3 knobs but only shows a third
 of each one. They are used to control the RGB balance for 
 the rectangle fill.
 
 (c)2012 Peter Lager
 */

import g4p_controls.*;

GKnob knbRed, knbGreen, knbBlue;
GLabel label;

int r, g, b;
int kx, ky;

void setup() {
  size(400, 220);
  r = g = b = 160;
  kx = 20;
  ky = 20;

  label = new GLabel(this, 0, height - 26, width, 26, 
        "Drag mouse in a circular movement around the knobs centre");
  label.setOpaque(true);
  label.setLocalColorScheme(G4P.CYAN_SCHEME);
  
  knbRed = new GKnob(this, kx, ky, 150, 150, 0.8);
  knbRed.setTurnRange(150, 270);
  knbRed.setTurnMode(G4P.CTRL_ANGULAR);
  knbRed.setArcPolicy(true, true, true);
  knbRed.setLimits(r, 0, 255);
  knbRed.setNbrTicks(9);
  knbRed.setLocalColorScheme(G4P.RED_SCHEME);

  knbGreen = new GKnob(this, kx + 8, ky, 150, 150, 0.8);
  knbGreen.setTurnRange(270, 30);
  knbGreen.setTurnMode(G4P.CTRL_ANGULAR);
  knbGreen.setArcPolicy(true, true, true);
  knbGreen.setLimits(g, 0, 255);
  knbGreen.setNbrTicks(9);
  knbGreen.setLocalColorScheme(G4P.GREEN_SCHEME);

  knbBlue = new GKnob(this, kx + 4, ky + 9, 150, 150, 0.8);
  knbBlue.setTurnRange(30, 150);
  knbBlue.setTurnMode(G4P.CTRL_ANGULAR);
  knbBlue.setArcPolicy(true, true, true);
  knbBlue.setLimits(b, 0, 255);
  knbBlue.setNbrTicks(9);
  knbBlue.setLocalColorScheme(G4P.BLUE_SCHEME);

  stroke(0);
  strokeWeight(3);
  rectMode(CORNERS);
}

void draw() {
  background(220, 220, 255);
  // Color sampler
  fill(r, g, b);
  rect(kx + 190, 40, width - 40, height - 40);
}

void handleKnobEvents(GValueControl knob, GEvent event) {
  if (knob == knbRed) 
    r = knob.getValueI();
  else if (knob == knbGreen)
    g = knob.getValueI();
  else if (knob == knbBlue)
    b = knob.getValueI();
}
