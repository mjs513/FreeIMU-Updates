/**
 Interative Toroid with GUI controls from the G4P
 library
 (c)2012 Peter Lager
 
 Modification of the example
 Interactive Toroid by Ira Greenberg. 
 
 The toroid code has been abstracted to its own tab to 
 separate it from the GUI code for clarity.
 
 Illustrates the geometric relationship between Toroid, Sphere,
 and Helix 3D primitives, as well as lathing principal.
 
 */

import g4p_controls.*;

GLabel lblSegs, lblERad, lblPts, lblLRad;
GCustomSlider sdrSegs, sdrERad, sdrPts, sdrLRad;
GCheckbox cbxWire;
GOption optTorroid, optHelix;
GToggleGroup optShape;
GPanel p;

Toroid t1;

void setup() {
  size(640, 480, P3D);
  t1 = new Toroid();

  // Sets the colour scheme for the GUI components 
  // The 8 schemes available are 
  // RED_SCHEME, GREEN_SCHEME, YELLOW_SCHEME, PURPLE_SCHEME
  // ORANGE_SCHEME, CYAN_SCHEME, BLUE_SCHEME, GOLD_SCHEME
  // Defaults to BLUE_SCHEME 
  G4P.setGlobalColorScheme(G4P.GOLD_SCHEME);

  // Create the various GUI components
  p = new GPanel(this, 2, height - 30, 460, 300, "Toroid Control Panel");
  lblSegs = new GLabel(this, 2, 40, 120,20, "Segment detail");
  lblPts = new GLabel(this, 2, 100, 120, 20 , "Ellipse detail");
  lblERad = new GLabel(this, 2, 160, 120, 20, "Ellipse Radius");
  lblLRad = new GLabel(this, 2, 220, 120, 20, "Toroid Radius");

  sdrSegs = new GCustomSlider(this, 110, 20, 325, 60, "purple18px");
  sdrSegs.setLimits(60, 3, 60);
  sdrSegs.setNbrTicks(58);
  sdrSegs.setStickToTicks(true);
  
  sdrPts = new GCustomSlider(this, 110, 80, 325, 60, "purple18px");
  sdrPts.setLimits(32, 3, 32);
  sdrPts.setNbrTicks(30);
  sdrPts.setStickToTicks(true);

  sdrERad = new GCustomSlider(this, 110, 140, 325, 60, null);
  sdrERad.setLimits(60.0, 10.0, 100.0);  
  sdrERad.setEasing(20);
  
  sdrLRad = new GCustomSlider(this, 110, 200, 325, 60, null);
  sdrLRad.setLimits(140.0, 10.0, 240.0);
  sdrLRad.setEasing(20);

  // Various options
  optTorroid = new GOption(this, 110, 260, 80, 20, "Toroid?");
  optHelix = new GOption(this, 200, 260, 80, 20 , "Helix?");
  cbxWire = new GCheckbox(this, 330, 260, 100, 20, "Wire frame?");

  // Torroid / helix option group
  optShape = new GToggleGroup();
  optShape.addControl(optTorroid);
  optShape.addControl(optHelix);
  optTorroid.setSelected(true);

  p.addControl(lblSegs);
  p.addControl(lblPts);
  p.addControl(lblERad);
  p.addControl(lblLRad);
  p.addControl(sdrSegs);
  p.addControl(sdrPts);
  p.addControl(sdrERad);
  p.addControl(sdrLRad);
  p.addControl(optHelix);
  p.addControl(optTorroid);
  p.addControl(cbxWire);
  // Set the alpha after adding the controls.
  // The true will mean it will be applied to 
  // anything already added to the panel.
  p.setAlpha(200, true);
}

public void handleSliderEvents(GValueControl slider, GEvent event) {
  if (slider == sdrSegs)
    t1.setSegmentDetail(sdrSegs.getValueI());
  if (slider == sdrPts)
    t1.setEllipseDetail(sdrPts.getValueI());
  if (slider == sdrERad)
    t1.setEllipseRadius(sdrERad.getValueF()); 
  if (slider == sdrLRad)
    t1.setLatheRadius(sdrLRad.getValueF());
}

public void handleToggleControlEvents(GToggleControl option, GEvent event) {
  if (option == cbxWire)
    t1.setIsWire(cbxWire.isSelected());
  if (option == optHelix)
    t1.setIsHelix(true);
  if (option == optTorroid)
    t1.setIsHelix(false);
}

void draw() {
  pushMatrix();
  background(200, 200, 255);
  // basic lighting setup
  lights();
  // 2 rendering styles
  //center and spin toroid
  translate(width/2, height/2, -200);

  rotateX(frameCount*PI/150);
  rotateY(frameCount*PI/170);
  rotateZ(frameCount*PI/90);

  // draw toroid
  t1.draw();
  popMatrix();
}
