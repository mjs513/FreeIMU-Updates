// ============================================
// Create the showcase controls
// ============================================

public void makeDemoControls() {
  // Create the panel of this size
  int pX = 10, pY = 30, pHeight = 240, pWidth = 400;
  pnlControls = new GPanel(this, pX, pY, pWidth, pHeight, "Panel Tab Text (drag to move : click to open/close)");
  pnlControls.setOpaque(true);
  pnlControls.setCollapsed(false);
  // Create the user action feedback label
  lblAction = new GLabel(this, 0, pHeight-20, pWidth, 20, "This is a GLabel control - for status info");
  lblAction.setOpaque(true);
  pnlControls.addControl(lblAction);    

  //Load some sample text
  String[] paragraphs = loadStrings("book.txt");
  startText = PApplet.join(paragraphs, '\n');

  // Create a text area with both horizontal and 
  // vertical scrollbars that automatically hide 
  // when not needed.
  txaDemo = new GTextArea(this, 4, 24, 220, 96, G4P.SCROLLBARS_BOTH | G4P.SCROLLBARS_AUTOHIDE);
  txaDemo.setText(startText, 250);
  txaDemo.setDefaultText("Please enter some text");
  pnlControls.addControl(txaDemo);
  // Create a text field with horizontal scrollbar
  txfDemo = new GTextField(this, 4, 126, 220, 30, G4P.SCROLLBAR_HORIZONTAL | G4P.SCROLLBARS_AUTOHIDE);
  txfDemo.setDefaultText("This is default text");
  pnlControls.addControl(txfDemo);
  // Add tab controls
  tt = new GTabManager();
  tt.addControls(txaDemo, txfDemo);

  // Checkbox
  cbxSticky = new GCheckbox(this, 230, 130, 120, 20, "Stick to ticks");
  pnlControls.addControl(cbxSticky);

  // Knob
  knbDemo = new GKnob(this, 230, 30, 90, 90, 0.76f);
  knbDemo.setLimits(0.0f, 0.0f, TWO_PI);
  knbDemo.setNbrTicks(10);
  knbDemo.setEasing(5);
  knbDemo.setShowArcOnly(true);
  knbDemo.setOverArcOnly(true);
  knbDemo.setSensitivity(2.0f);
  pnlControls.addControl(knbDemo);

  lblMC = new GLabel(this, 310, 22, 100, 50, "Knob turning control");
  lblMC.setTextAlign(GAlign.LEFT, null);
  lblMC.setTextAlign(GAlign.CENTER, GAlign.TOP);
  lblMC.setTextBold();
  optAngular = new GOption(this, 326, 60, 80, 18, "Angular");
  optXdrag = new GOption(this, 326, 80, 80, 18, "X drag");
  optYdrag = new GOption(this, 326, 100, 80, 18, "Y drag");
  tg = new GToggleGroup();
  tg.addControls(optAngular, optXdrag, optYdrag);
  optXdrag.setSelected(true);
  pnlControls.addControls(lblMC, optAngular, optXdrag, optYdrag);

  // Create timer button
  btnTimer = new GButton(this, pWidth-100, pHeight-60, 100, 40, "Start");
  btnTimer.setIcon("time.png", 1, null, null);
  tmrTimer = new GTimer(this, this, "myTimerFunction", 300);  
  pnlControls.addControl(btnTimer);
  // When the timer is on we will see it in a GSketchPad control
  // Create the GSkethPad control to position and display the graphic
  spad = new GSketchPad(this, pWidth-210, pHeight-60, 100, 40);
  // Now create and clear graphic to be used (JAVA parameter
  // parameter is only needed for Processing 1.5.1)
  pg = createGraphics(100, 46, JAVA2D);
  // Add the graphic to the control. 
  spad.setGraphic(pg);
  updateGraphic(3); // Method for drawing the graphic
  pnlControls.addControl(spad);
  // 4-way stick control
  astick = new GStick(this, pWidth - 290, pHeight - 80, 60, 60);
  pnlControls.addControl(astick);
  // Create button to open the control window
  btnControl = new GButton(this, 2, pHeight - 60, 100, 40, "G4P config window");
  pnlControls.addControl(btnControl);
}
