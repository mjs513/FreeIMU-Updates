// The code in this tab is used to create all the other
// controls needed to configure the slider control.

GToggleGroup tg = new GToggleGroup();
GOption optLeft, optRight, optTrack;
GCheckbox cbxOpaque, cbxShowTicks, cbxValue, cbxLimits, cbxSticky;
GSlider sdrBack, sdrNbrTicks, sdrEasing;
GLabel lblOrient, lblVisible, lblNbrTicks, lblEasing;
GKnob knbAngle;
GButton btnMakeCode;
GButton[] btnColours = new GButton[8];

public void makeSliderConfigControls() {
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
  lblOrient = new GLabel(this, x, y, 178, 18, "Orient Text");
  lblOrient.setTextAlign(GAlign.LEFT, null);
  lblOrient.setTextBold();
  optLeft = new GOption(this, x, y + 20, 80, 18, "Left");
  optRight = new GOption(this, x, y + 40, 80, 18, "Right");
  optTrack = new GOption(this, x, y + 60, 80, 18, "Track");
  tg.addControls(optLeft, optRight, optTrack);
  optTrack.setSelected(true);

  x = width - 180; 
  y = 2;
  lblVisible = new GLabel(this, x, y, 70, 18, "Visible");
  lblVisible.setTextBold();
  cbxShowTicks = new GCheckbox(this, x, y + 20, 70, 18, "Ticks");
  cbxLimits = new GCheckbox(this, x, y + 40, 70, 18, "Limits");
  cbxValue = new GCheckbox(this, x, y + 60, 70, 18, "Value");
  cbxOpaque = new GCheckbox(this, x, y + 80, 70, 18, "Opaque");
  cbxSticky = new GCheckbox(this, x, y + 110, 70, 40, "Stick to ticks");

  x = 10; 
  y = height - 80;
  knbAngle = new GKnob(this, x, y, 70, 70, 0.6f);
  knbAngle.setTurnRange(0, 360);
  knbAngle.setLimits(0.0f, 0.0f, TWO_PI);
  knbAngle.setTurnMode(G4P.CTRL_ANGULAR);
  knbAngle.setIncludeOverBezel(true);
  knbAngle.setNbrTicks(13);
  knbAngle.setStickToTicks(true);

  btnMakeCode = new GButton(this, x + 100, y + 20, 182, 38, "Place code for existing configuration on clipboard");
}

