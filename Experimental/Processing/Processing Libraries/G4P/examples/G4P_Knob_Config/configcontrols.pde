// The code in this tab is used to create all the other
// controls needed to configure the knob control.

GToggleGroup tg = new GToggleGroup();
GOption optAngular, optXdrag, optYdrag;
GCheckbox cbxOpaque, cbxOverArc, cbxOverAll;
GCheckbox cbxShowTicks, cbxShowArcOnly, cbxShowTrack;
GCheckbox cbxSticky;
GSlider sdrBack, sdrSense, sdrNbrTicks, sdrEasing, sdrGripRatio;
GLabel lblMC, lblVisible, lblSense, lblNbrTicks;
GLabel lblEasing, lblValue, lblAngles, lblGripAmount;
GKnob knbStart, knbEnd;
GButton btnMakeCode;
GButton[] btnColours = new GButton[8];

public void makeKnobConfigControls() {
  // Create colour scheme selectors
  int x = width - 42, y = 2;
  for (int i = 0; i < btnColours.length; i++) {
    btnColours[i] = new GButton(this, x, y + i * 20, 40, 18, "" + (i+1));
    btnColours[i].tag = "Button: " + (i+1);
    btnColours[i].setLocalColorScheme(i);
    btnColours[i].tagNo = 1000+i;
  }  
  // Create sliders
  x = width-100; 
  y = 162;
  bgcol = 200; 
  sdrBack = new GSlider(this, x, y, 162, 80, 12);
  sdrBack.setLimits(bgcol, 0, 255);
  sdrBack.setRotation(-PI/2);
  sdrBack.setTextOrientation(G4P.ORIENT_RIGHT);
  sdrBack.setEasing(20);
  sdrBack.setShowValue(true);
  sdrBack.setShowTicks(true);

  x = width - 180; 
  y = 250;
  sdrSense = new GSlider(this, x, y, 80, 40, 12);
  sdrSense.setLimits(1.0f, 0.2f, 5.0f);
  sdrSense.setShowValue(true);
  lblSense = new GLabel(this, x + 82, y, 100, 40, "XY drag rate");
  lblSense.setTextAlign(GAlign.LEFT, null);
  lblSense.setTextItalic();

  sdrEasing = new GSlider(this, x, y + 34, 80, 40, 12);
  sdrEasing.setLimits(1.0f, 1.0f, 30.0f);
  sdrEasing.setShowValue(true);
  lblEasing = new GLabel(this, x + 82, y + 34, 80, 40, "Easing");
  lblEasing.setTextAlign(GAlign.LEFT, null);
  lblEasing.setTextItalic();

  sdrNbrTicks = new GSlider(this, x, y + 68, 80, 40, 12);
  sdrNbrTicks.setLimits(2, 2, 15);
  sdrNbrTicks.setShowValue(true);
  lblNbrTicks = new GLabel(this, x + 82, y + 68, 80, 40, "No. of ticks");
  lblNbrTicks.setTextAlign(GAlign.LEFT, null);
  lblNbrTicks.setTextItalic();

  x = width - 180; 
  y = 164;
  lblMC = new GLabel(this, x, y, 178, 18, "Knob turning control");
  lblMC.setTextAlign(GAlign.LEFT, null);
  lblMC.setTextBold();
  optAngular = new GOption(this, x, y + 20, 80, 18, "Angular");
  optXdrag = new GOption(this, x, y + 40, 80, 18, "X drag");
  optYdrag = new GOption(this, x, y + 60, 80, 18, "Y drag");
  tg.addControls(optAngular, optXdrag, optYdrag);
  optXdrag.setSelected(true);
  cbxOverArc = new GCheckbox(this, x + 90, y + 20, 90, 18, "Arc only");
  cbxOverAll = new GCheckbox(this, x + 90, y + 40, 90, 18, "Incl. Bezel");

  x = width - 180; 
  y = 2;
  lblVisible = new GLabel(this, x, y, 70, 18, "Visible");
  lblVisible.setTextBold();
  cbxShowTicks = new GCheckbox(this, x, y + 20, 70, 18, "Ticks");
  cbxShowTicks.setSelected(true);
  cbxShowTrack = new GCheckbox(this, x, y + 40, 70, 18, "Track");
  cbxShowTrack.setSelected(true);
  cbxShowArcOnly = new GCheckbox(this, x, y + 60, 80, 18, "Arc Only");
  cbxOpaque = new GCheckbox(this, x, y + 80, 70, 18, "Opaque");
  cbxSticky = new GCheckbox(this, x, y + 110, 70, 40, "Stick to ticks");

  x = (int) (kb.getCX() - 40);
  y = (int) (kb.getY() -40);
  lblValue = new GLabel(this, x, y, 80, 30);
  lblValue.setOpaque(true);
  lblValue.setText("" + kb.getValueS());

  x = 5; 
  y = height - 80;
  knbStart = new GKnob(this, x, y, 50, 50, 1);
  knbStart.setTurnRange(0, 360);
  knbStart.setLimits(110, 0, 360);
  knbStart.setTurnMode(G4P.CTRL_ANGULAR);
  knbEnd = new GKnob(this, x + 50, y, 50, 50, 1);
  knbEnd.setTurnRange(0, 360);
  knbEnd.setLimits(70, 0, 360);
  knbEnd.setTurnMode(G4P.CTRL_ANGULAR);
  lblAngles = new GLabel(this, 0, y + 55, 110, 20, "Turn Limits");
  sdrGripRatio = new GSlider(this, x + 110, y, 120, 40, 12);
  sdrGripRatio.setShowDecor(false, false, true, false);
  sdrGripRatio.setValue(0.8f);
  lblGripAmount = new GLabel(this, x + 226, y, 80, 40, "Grip Ratio");
  btnMakeCode = new GButton(this, x + 110, y + 36, 182, 38, "Place code for existing configuration on clipboard");
}