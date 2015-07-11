// =============================================
// Create the configuration window and controls
// =============================================

GWindow windControl;  
GButton[] btnColours = new GButton[8];
GSlider sdrAlpha;
GKnob knbAngle;
GLabel lblAlpha1, lblAlpha2;
GSlider2D sdrPanelPos;

public void createControlWindow() {
  windControl = new GWindow(this, "Controls", 600, 400, 400, 300, false, JAVA2D);
  PApplet app = windControl.papplet; // save some typing
  // Create colour scheme selectors
  int x = 8;
  int y = 50; // app.height - 24;
  for (int i = 0; i < btnColours.length; i++) {
    btnColours[i] = new GButton(app, x  + i * 34, y, 30, 18, "" + (i+1));
    btnColours[i].tag = "Button: " + (i+1);
    btnColours[i].setLocalColorScheme(i);
    btnColours[i].tagNo = 1000+i;
  }  
  y = 0; //app.height - 90;    
  sdrAlpha = new GSlider(app, x, y, 268, 60, 12);
  sdrAlpha.setLimits(255, 0, 255);
//  sdrAlpha.setRotation(-PI/2);
//  sdrAlpha.setTextOrientation(G4P.ORIENT_RIGHT);
  sdrAlpha.setEasing(20);
  sdrAlpha.setShowValue(true);
  sdrAlpha.setShowTicks(true);

  x = 290; 
  y = 16;
  lblAlpha1 = new GLabel(app, x, y, 120, 26, "Alpha Slider");
  lblAlpha1.setTextBold();
  lblAlpha2 = new GLabel(app, x, y + 30, 110, 80, "When alpha falls below " + G4P.ALPHA_BLOCK + " it will disable the controls in the main sketch.");

  x = app.width - 120; 
  y = app.height - 120;
  knbAngle = new GKnob(app, x, y, 100, 100, 0.75f);
  knbAngle.setTurnRange(0, 360);
  knbAngle.setLimits(0.0f, 0.0f, TWO_PI);
  knbAngle.setTurnMode(G4P.CTRL_ANGULAR);
  knbAngle.setIncludeOverBezel(true);
  knbAngle.setNbrTicks(13);
  knbAngle.setStickToTicks(true);

  sdrPanelPos = new GSlider2D(app, 40, 84, 200, 200);
  sdrPanelPos.setLimitsX(pnlControls.getX(), 0, 200);
  sdrPanelPos.setLimitsY(pnlControls.getY(), 0, 200);
  
  windControl.addDrawHandler(this, "drawController");
}

/*
   * The draw handler for the control window
 */
public void drawController(GWinApplet appc, GWinData data) {
  appc.background(227, 230, 255);
}