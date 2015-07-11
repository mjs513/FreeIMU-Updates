/**
 * Sketch to demonstrate the GTextArea class. 
 * 
 * This component allows the user to enter and edit text.
 * 
 * In this demonstration the text width > the display width
 * so it can show off its' horizontal scrollbar.
 * 
 * The control has been setup with some default text and 
 * this becomes visible when the control is empty.
 * 
 * The buttons on the side will change the font style
 * for the selected text. If no text has been selected
 * then the buttons are ignored.
 
 * Note that super/sub script may not be available on your
 * system.
 * 
 * @author Peter Lager
 *
 */

import g4p_controls.*;


GTextArea txaSample;
int bgcol = 32;
String startText;

public void setup() {
  size(700, 360);

  //Load some sample text
  String[] paragraphs = loadStrings("book3.txt");
  startText = PApplet.join(paragraphs, '\n');

  // Create a text area with both horizontal and 
  // vertical scrollbars that automatically hide 
  // when not needed.
  txaSample = new GTextArea(this, 80, 20, 290, 300, G4P.SCROLLBARS_BOTH | G4P.SCROLLBARS_AUTOHIDE);
  txaSample.setText(startText, 310);
  // Set some default text
  txaSample.setDefaultText("Please enter some text");

  createConfigControls();
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

public void handleTextEvents(GEditableTextControl textarea, GEvent event) {
  println(event);
}

public void handleButtonEvents(GButton button, GEvent event) { 
  if (event == GEvent.CLICKED) {
    if (button.tagNo >= 100)
      txaSample.setLocalColorScheme(button.tagNo - 1000);
    if (button == btnItalic)
      txaSample.setSelectedTextStyle(G4P.POSTURE, G4P.POSTURE_OBLIQUE);
    if (button == btnPlain)
      txaSample.clearStyles();
    if (button == btnBold)
      txaSample.setSelectedTextStyle(G4P.WEIGHT, G4P.WEIGHT_BOLD);
    if (button == btnSuper)
      txaSample.setSelectedTextStyle(G4P.SUPERSCRIPT, G4P.SUPERSCRIPT_SUPER);
    if (button == btnSub)
      txaSample.setSelectedTextStyle(G4P.SUPERSCRIPT, G4P.SUPERSCRIPT_SUB);
    if (button == btnText)
      txaSample.setText(startText, 310);
  }
}

public void handleToggleControlEvents(GToggleControl checkbox, GEvent event) { 
  if (checkbox == cbxJustify)
    txaSample.setJustify(checkbox.isSelected());
}

public void handleKnobEvents(GValueControl knob, GEvent event) {
  txaSample.setRotation(knbAngle.getValueF(), GControlMode.CENTER);
}
