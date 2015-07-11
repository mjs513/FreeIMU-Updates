/**
 This sketch is used show controls that uae a mixture of
 text and icons and allows you to experiment with text 
 and icon alignment options.
 
 It also uses the droplist control extensively.
 controls available in the G4P library.
 
 created by Peter Lager
 */

import g4p_controls.*;

GToggleGroup tg = new GToggleGroup();
GOption opt0, opt1, opt2;
GCheckbox cbx0;
GLabel lbl0, lbl1;
GButton btn0, btn1;

// Since all the classes for all these controls inherit 
// from GTextIconControl this arraylist will keep them as
// a collection so they can all be configured as a group
// more easily
ArrayList<GTextIconAlignBase> controls = new ArrayList<GTextIconAlignBase>();

int bgcol = 240;

public void setup() {
  size(560, 380);
  G4P.setCursorOff(CROSS);

  // Get some sample text for later
  String[] lines = loadStrings("tisample.txt");
  String text = join(lines, '\n');
  // Options and checkboxes have default settings :
  //   left/middle text & left/middle icon alignment
  // Labels and buttons have default settings :
  //   centre/middle text & right/middle icon alignment
  // The buttons and labels will have there alignment 
  // changed to match the labels so they all start
  // matched to the drop lists.
  lbl0 = new GLabel(this, 20, 10, 150, 350, text);
  lbl0.setTextAlign(GAlign.LEFT, null);
  controls.add(lbl0);

  lbl1 = new GLabel(this, 200, 100, 140, 80, "Labels can also have icons like this one");
  lbl1.setIcon("bugtest.png", 1, GAlign.LEFT, null);
  lbl1.setTextAlign(GAlign.LEFT, null);
  controls.add(lbl1);

  btn0 = new GButton(this, 200, 190, 150, 80, "Buttons always have an opaque background");
  btn0.setTextAlign(GAlign.LEFT, null);
  controls.add(btn0);

  btn1 = new GButton(this, 200, 280, 150, 80, "As well as text buttons can also have icons");
  btn1.setIcon("smile.png", 3, GAlign.LEFT, null);
  btn1.setTextAlign(GAlign.LEFT, null);
  controls.add(btn1);

  opt0 = new GOption(this, 200, 10, 100, 18, "Option 0");
  tg.addControl(opt0);
  controls.add(opt0);

  opt1 = new GOption(this, 200, 32, 100, 18, "Option 1");
  tg.addControl(opt1);
  controls.add(opt1);

  opt2 = new GOption(this, 200, 54, 100, 18, "Option 2");
  tg.addControl(opt2);
  controls.add(opt2);
  
  opt0.setSelected(true);
  tg.addControls(opt0, opt1, opt2);
  
  cbx0 = new GCheckbox(this, 200, 76, 100, 20, "Tick box");
  controls.add(cbx0);
  makeTextIconConfigControls();
}

public void draw() {
  background(bgcol);
  fill(227, 230, 255);
  noStroke();
  rect(width - 190, 0, 200, height);
}

public void handleSliderEvents(GValueControl slider, GEvent event) { 
  if (slider == sdrBack)
    bgcol = slider.getValueI();
}

public void handleKnobEvents(GValueControl knob, GEvent event) { 
  if (knbAngle == knob)
    for (GTextIconAlignBase control : controls)
      control.setRotation(knbAngle.getValueF(), GControlMode.CENTER);
}

public void handleButtonEvents(GButton button, GEvent event) { 
  if (button.tagNo >= 1000) {
    for (GTextIconAlignBase control : controls)
      control.setLocalColorScheme(button.tagNo - 1000);
  }
}


public void handleToggleControlEvents(GToggleControl option, GEvent event) {
  if (option == optPlain)
    for (GTextIconAlignBase control : controls)
      control.setTextPlain();
  else if (option == optBold)
    for (GTextIconAlignBase control : controls) {
      control.setTextPlain();
      control.setTextBold();
    }
  else if (option == optItalic)
    for (GTextIconAlignBase control : controls) {
      control.setTextPlain();
      control.setTextItalic();
    }
  else if (option == optItalic)
    for (GTextIconAlignBase control : controls) {
      control.setTextPlain();
      control.setTextItalic();
    }
  else if (option == optBoldItalic)
    for (GTextIconAlignBase control : controls) {
      control.setTextBold();
      control.setTextItalic();
    }
  else if (option == cbxOpaque)
    for (GTextIconAlignBase control : controls)
      control.setOpaque(cbxOpaque.isSelected());
}

public void handleDropListEvents(GDropList list, GEvent event) {
  GAlign na = GAlign.getFromText(list.getSelectedText());
  println(na);

  if (list == textH)
    for (GTextIconAlignBase control : controls)
      control.setTextAlign(na, null);
  if (list == textV)
    for (GTextIconAlignBase control : controls)
      control.setTextAlign(null, na);
  if (list == iconH)
    for (GTextIconAlignBase control : controls)
      control.setIconAlign(na, null);
  if (list == iconV) {
    for (GTextIconAlignBase control : controls)
      control.setIconAlign(null, na);
  }
}