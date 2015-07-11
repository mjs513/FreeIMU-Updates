import g4p_controls.*;

GPanel pnl;
GSketchPad spad;
GButton btnClear;
PGraphics pg;

void setup() {
  size(400, 300);
  // Now create and clear graphic to be used (JAVA parameter
  // parameter is only needed for Processing 1.5.1)
  pg = createGraphics(200, 160, JAVA2D);
  clearGraphic();
  // Create the G4P control to position and display the graphic
  spad = new GSketchPad(this, 0, 0, 100, 80);
  // Set the graphic for this control. 
  // The graphic will be scaled to fit the control.
  spad.setGraphic(pg);
  // Create the button to clear the graphic
  btnClear = new GButton(this, 0, 0, 80, 20, "Clear");
  // Create the panel and add the controls
  pnl = new GPanel(this, 20, 30, 160, 140, "Sketch Test");
  pnl.addControl(btnClear, 10, 24);
  pnl.addControl(spad, 10, 50);
  // Expand the panel
  pnl.setCollapsed(false);
}

void draw() {
  background(240);
  // Every 10th frame update the sketchpad graphic
  if (frameCount % 10 == 0)
    updateGraphic();
}

void handleButtonEvents(GButton button, GEvent event) { 
  clearGraphic();
}

// Clear the sketchpad graphic
void clearGraphic() {
  pg.beginDraw();
  pg.background(255, 255, 200);
  pg.noFill();
  pg.ellipseMode(CORNERS);
  pg.endDraw();
}

// Add a line or ellipse to the sketchpad graphic
void updateGraphic() {
  float x0 = random(10, pg.width-10);
  float x1 = random(10, pg.width-10);
  float y0 = random(10, pg.height-10);
  float y1 = random(10, pg.height-10);
  int col = color(random(64, 255), random(64, 255), random(64, 255));
  pg.beginDraw();
  pg.stroke(col);
  pg.strokeWeight(random(2, 5));
  if (random(0, 1) < 0.5f)
    pg.line(x0, y0, x1, y1);
  else
    pg.ellipse(x0, y0, x1, y1);  
  pg.endDraw();
}

