// The code in this tab is used to create all the other
// controls needed to configure the knob control.

GButton[] btnColours = new GButton[8];
GButton btnItalic, btnBold, btnPlain, btnSuper, btnSub, btnText;
GCheckbox cbxJustify;
GLabel lblStyleInstr;
GKnob knbAngle;
GSlider sdrBack;
StyledString ss;

public void createConfigControls() {
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
  sdrBack = new GSlider(this, x, y, 162, 80, 12);
  sdrBack.setLimits(bgcol, 0, 255);
  sdrBack.setRotation(-PI/2);
  sdrBack.setTextOrientation(G4P.ORIENT_RIGHT);
  sdrBack.setEasing(20);
  sdrBack.setShowValue(true);
  sdrBack.setShowTicks(true);

  x = width -180; 
  y = 2;
  btnPlain = new GButton(this, x, 2, 66, 20, "Clear");
  btnPlain.tag = "Button:  Clear";

  btnItalic = new GButton(this, x, 2 + 22, 66, 20, "Italic");
  btnItalic.tag = "Button:  Italic";
  ss = new StyledString("Italic"); 
  ss.addAttribute(G4P.POSTURE, G4P.POSTURE_OBLIQUE);
  btnItalic.setStyledText(ss);

  btnBold = new GButton(this, x, 2 + 44, 66, 20, "Bold");
  btnBold.tag = "Button:  Bold";
  ss = new StyledString("Bold"); 
  ss.addAttribute(G4P.WEIGHT, G4P.WEIGHT_BOLD);
  btnBold.setStyledText(ss);

  btnSuper = new GButton(this, x, 2 + 66, 66, 20, "Superscript");
  btnSuper.tag = "Button:  Superscript";
  ss = new StyledString("Superscript"); 
  ss.addAttribute(G4P.SUPERSCRIPT, G4P.SUPERSCRIPT_SUPER, 5, 11);
  btnSuper.setStyledText(ss);

  btnSub = new GButton(this, x, 2 + 88, 66, 20, "SubScript");
  btnSub.tag = "Button:  Subscript";
  ss = new StyledString("Subscript"); 
  ss.addAttribute(G4P.SUPERSCRIPT, G4P.SUPERSCRIPT_SUB, 3, 9);
  btnSub.setStyledText(ss);

  cbxJustify = new GCheckbox(this, x, 2 + 110, 66, 20, "Justify");
  cbxJustify.setOpaque(true);

  btnText = new GButton(this, x, 142, 66, 36, "RESET TEXT");

  x = width - 180; 
  y = 176;
  String si = "Use the keyboard (Windows shortcut keys) or the mouse to select some text then click on the style buttons to change the style of the selected text";
  lblStyleInstr = new GLabel(this, x, y, 170, 100, si);
  lblStyleInstr.setOpaque(true);

  x = width - 130; 
  y = height - 80;
  knbAngle = new GKnob(this, x, y, 70, 70, 0.6f);
  knbAngle.setTurnRange(0, 360);
  knbAngle.setLimits(0.0f, 0.0f, TWO_PI);
  knbAngle.setTurnMode(G4P.CTRL_ANGULAR);
  knbAngle.setIncludeOverBezel(true);
  knbAngle.setNbrTicks(13);
  knbAngle.setStickToTicks(true);
}

