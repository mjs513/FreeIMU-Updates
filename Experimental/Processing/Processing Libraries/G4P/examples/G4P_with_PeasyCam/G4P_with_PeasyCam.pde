/**
 * PeasyCam with GUI control
 *
 * Extends PeasyCam example to use with G4P by providing
 * sliders to control the angles.
 *
 * When the panel is open you can drag the sliders to rotate
 * the cube. When the panel is closed then dragging the mouse
 * over the display will rotate the cube as per PeaseyCam
 * example.
 *
 * The control code is quite complex because PeasyCam does 
 * not provide methods to set the rotation angles abso;utely
 * only relatively.
 *
 * Click on the panel tab to open/close the panel.
 * 
 * by Peter Lager
 */

import g4p_controls.*;
import peasy.*;

PeasyCam cam;

GPanel pnl;
GSlider sx, sy, sz;
GLabel label;
int ax, ay, az;

// These are needed to remember PeasyCam offset and distance
float[] offsets = new float[3];
float[] rotations = new float[3];
double distance = 0.0f;

// Remember last slider values
// PeasyCam does not provide methods to set the absolute
// rotation angles, rotateX(ang) rotates about the X axis
// by ang radians
int lastSx, lastSy, lastSz;
int currSx, currSy, currSz;

public void setup() {
  size(400, 400, P3D);

  // This is only required if you are going to create something
  // that changes the initial matrix (g) BEFORE creating a G4P 
  // control. PeasyCam is just such an object, but if it is 
  // created after one of the G4P controls then you would not 
  // this statement.
  G4P.registerSketch(this);

  // Create a PeasyCam object
  cam = new PeasyCam(this, 100);
  cam.setMinimumDistance(50);
  cam.setMaximumDistance(500);

  // Sets the colour scheme for the GUI components 
  // The 8 schemes available are 
  // RED_SCHEME, GREEN_SCHEME, YELLOW_SCHEME, PURPLE_SCHEME
  // ORANGE_SCHEME, CYAN_SCHEME, BLUE_SCHEME, GOLD_SCHEME
  // Defaults to BLUE_SCHEME 
  G4P.setGlobalColorScheme(GCScheme.RED_SCHEME);

  // Create a collapsible panel
  // (this, tab title, x, y, width, height)
  pnl = new GPanel(this, 10, 300, 300, 88, "Rotate Cube");
  pnl.setCollapsed(false);
  // Create a horizontal slider
  // (this, x, y, width, height)
  // default value limits 0-100 and initial value 50
  sx = new GSlider(this, 10, 22, 280, 20, 13);
  // set slider value limits (initial value, min, max)
  sx.setLimits(0, -180, 180);
  // Hide tick marks
  sx.setShowTicks(false);
  // Set thumb easing for nice visual effect - acceptable
  // values 1 to 100 (default = 1  i.e. no easing)
  sx.setEasing(15);

  sy = new GSlider(this, 10, 42, 280, 20, 13);
  sy.setLimits(0, -180, 180);
  sy.setShowTicks(false);
  sy.setEasing(15);

  sz = new GSlider(this, 10, 62, 280, 20, 13);
  sz.setLimits(0, -180, 180);
  sz.setShowTicks(false);
  sz.setEasing(15);

  // Add the sliders to the panel x,y coordinates are now 
  // relative to the top left of the panel open area below
  // the tab
  pnl.addControl(sx);
  pnl.addControl(sy);
  pnl.addControl(sz);

  // Create a label across the top of the screen
  String s = "Drag the slider thumb or click on the ";
  s += "track to rotate the cube. The panel can be collapsed/";
  s += "expanded by clicking on the panel title bar";
  label = new GLabel(this, 0, 0, width, 60, s);
  label.setOpaque(true);
  // Align the text both horizontally and vertically
  label.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
}

public void draw() {
  // Switch off PeasyCam mouse control if the panel is being
  // dragged else turn it on
  if (pnl.isCollapsed()) // Panel is collapsed
      cam.setMouseControlled(!pnl.isDragging());
  else // panel open must be using sliders
  cam.setMouseControlled(false);  
  rotateX(-.5f);
  rotateY(-.5f);
  background(0);
  // Draw big box
  strokeWeight(2);
  stroke(255, 255, 0);
  fill(255, 0, 0);
  box(30);
  // Draw little box
  translate(0, 0, 20);
  fill(0, 0, 255);
  box(5);
  // Synchronise the actual rotations and slider positions
  syncSliders();
}


/*
  This function displays how we can create a HUD with PeasyCam.
 */
void syncSliders() {
  // Get the current PeasyCam details to restore later
  rotations = cam.getRotations();

  // If necessary update slider positions
  if (pnl.isCollapsed()) {
    // Update slider positions
    currSx = lastSx = (int)Math.toDegrees(rotations[0]);
    currSy = lastSy = (int)Math.toDegrees(rotations[1]);
    currSz = lastSz = (int)Math.toDegrees(rotations[2]);

    // There are 2 methods to set the value of the slider
    // setValue(value); it takes into account any inertia
    // setValue(value, ignore); where ignore is a boolean value
    // which if true will set the value and move the thumb
    // immediately ignoring any inertia value
    sx.setValue((int)Math.toDegrees(rotations[0]));
    sy.setValue((int)Math.toDegrees(rotations[1]));
    sz.setValue((int)Math.toDegrees(rotations[2]));
  }
  else { // Use sliders to control rotation
    if (currSx != lastSx) {
      cam.rotateX(Math.toRadians(currSx - lastSx));
      lastSx = currSx;
    }
    if (currSy != lastSy) {
      cam.rotateY(Math.toRadians(currSy - lastSy));
      lastSy = currSy;
    }
    if (currSz != lastSz) {
      cam.rotateZ(Math.toRadians(currSz - lastSz));
      lastSz = currSz;
    }
  }
}

// Handle panels events i.e. when a panel is opened or
// collapsed
void handlePanelEvents(GPanel panel, GEvent event) {
  // Intended to detect panel events but ended up not
  // needing it. Left the following code as an example
  switch(event) {
  case COLLAPSED:
    pnl.setAvailableChildren(false);
    println("Panel has collapsed");
    break;
  case EXPANDED:
    pnl.setAvailableChildren(true);
    println("Panel has expanded");
    break;
  case DRAGGED:
    print("The panel has been dragged to ");
    println(pnl.getX() + ", " + pnl.getY());
    break;
  }
}

// Handles slider events for both horizontal and
// vertical sliders
void handleSliderEvents(GValueControl slider, GEvent event) {
  if (slider == sx)
    currSx = slider.getValueI();
  if (slider == sy)
    currSy = slider.getValueI(); 
  if (slider == sz)
    currSz = slider.getValueI();
}
