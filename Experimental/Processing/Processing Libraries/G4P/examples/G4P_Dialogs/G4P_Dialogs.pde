/**
Simple sketch to experiment with the different dialogs
available to you in the G4P library.

The file diaologs will not work in an applet hosted on
a website unless you digitally sign the jars.

The file dialogs match the behaviour of those in Processing 
V1.5.1 i.e. immediate mode rather than on a seperate thread
as in 2.0

created by Peter Lager

*/

import java.awt.Rectangle;
import java.util.ArrayList;

import g4p_controls.*;

public void setup() {
  size(660, 420);
  rects = new ArrayList<Rectangle> ();
  createFileSystemGUI(340, 220, 300, 130, 6);
  createOptionDialogGUI(20, 20, 440, 184, 6);
  createMessageDialogGUI(20, 220, 300, 184, 6);
  createColorChooserGUI(480, 20, 160, 60, 6);
}

public void draw() {
  background(200, 200, 255);
  for (Rectangle r : rects)
    showFrame(r);
}

public void handleButtonEvents(GButton button, GEvent event) { 
  // Folder selection
  if (button == btnFolder || button == btnInput || button == btnOutput)
    handleFileDialog(button);
  else if (button == btnMdialog)
    handleMessageDialog();
  // Option dialog
  else if (button == btnOdialog)
    handleOptionDialog();
  // Color chooser
  else if (button == btnColor)
    handleColorChooser();
}

// G4P code for folder and file dialogs
public void handleFileDialog(GButton button) {
  String fname;
  // Folder selection
  if (button == btnFolder) {
    fname = G4P.selectFolder("Folder Dialog");
    lblFile.setText(fname);
  }
  // File input selection
  else if (button == btnInput) {
    // Use file filter if possible
    fname = G4P.selectInput("Input Dialog", "png,gif,jpg,jpeg", "Image files");
    lblFile.setText(fname);
  }
  // File output selection
  else if (button == btnOutput) {
    fname = G4P.selectOutput("Output Dialog");
    lblFile.setText(fname);
  }
}

// G4P code for message dialogs
public void handleMessageDialog() {
  // Determine message type
  int mtype;
  switch(md_mtype) {
  default:
  case 0: 
    mtype = G4P.PLAIN; 
    break;
  case 1: 
    mtype = G4P.ERROR; 
    break;
  case 2: 
    mtype = G4P.INFO; 
    break;
  case 3: 
    mtype = G4P.WARNING; 
    break;
  case 4: 
    mtype = G4P.QUERY; 
    break;
  }
  String message = txfSMMessage.getText();
  String title = txfMdTitle.getText();
  G4P.showMessage(this, message, title, mtype);
}

// G4P code for option dialogs
public void handleOptionDialog() {
  // Determine message type
  int mtype, otype;
  switch(od_mtype) {
  default:
  case 0: 
    mtype = G4P.PLAIN; 
    break;
  case 1: 
    mtype = G4P.ERROR; 
    break;
  case 2: 
    mtype = G4P.INFO; 
    break;
  case 3: 
    mtype = G4P.WARNING; 
    break;
  case 4: 
    mtype = G4P.QUERY; 
    break;
  }
  // Determine the option type
  switch(od_otype) {
  default:
  case 0: 
    otype = G4P.YES_NO; 
    break;
  case 1: 
    otype = G4P.YES_NO_CANCEL; 
    break;
  case 2: 
    otype = G4P.OK_CANCEL; 
    break;
  }
  String message = txfOdMessage.getText();
  String title = txfOdTitle.getText();
  int reply = G4P.selectOption(this, message, title, mtype, otype);
  switch(reply) {
  case G4P.OK: 
    lblReply.setText("OK / YES"); 
    break;
  case G4P.NO: 
    lblReply.setText("NO"); 
    break;
  case G4P.CANCEL: 
    lblReply.setText("CANCEL"); 
    break;
  case G4P.CLOSED: 
    lblReply.setText("CLOSED"); 
    break;
  }    
  /* 
   * Some comments on the returned value:
   * G4P.OK and G4P.YES have the same integer value so can
   * be used interchangeably.
   * G4P.CLOSED maybe returned if the dialog box is closed
   * although on some systems G4P.NO or G4P.CANCEL may be 
   * returned instead.
   * It is safer to tested for a positive response because
   * they have the same value. If you must test for a negative
   * response use !G4P.OK or !G4P.YES
   */
}

// G4P code for colour chooser
public void handleColorChooser() {
  sel_col = G4P.selectColor();
  pg.beginDraw();
  pg.background(sel_col);
  pg.endDraw();
}

// Handles events from checkbox and option controls.
public void handleToggleControlEvents(GToggleControl checkbox, GEvent event) {
  if (checkbox == cbxUseNative) {
    PApplet.useNativeSelect = cbxUseNative.isSelected();
  }
  else if (checkbox.tagNo >= 6000 ) {
    switch(checkbox.tagNo / 1000) {
    case 6:
      od_mtype = checkbox.tagNo % 6000;
      break;
    case 7:
      od_otype = checkbox.tagNo % 7000;
      break;
    case 9:
      md_mtype = checkbox.tagNo % 9000;
      break;
    }
  }
}

// Simple graphical frame to group controls
public void showFrame(Rectangle r) {
  noFill();
  strokeWeight(1);
  stroke(color(240, 240, 255));
  rect(r.x, r.y, r.width, r.height);
  stroke(color(0));
  rect(r.x+1, r.y+1, r.width, r.height);
}

// The next 4 methods are simply to create the GUI. So there is
// no more code related to the various dialogs.
public void createColorChooserGUI(int x, int y, int w, int h, int border) {
  // Store picture frame
  rects.add(new Rectangle(x, y, w, h));
  // Set inner frame position
  x += border; 
  y += border;
  w -= 2*border; 
  h -= 2*border;
  GLabel title = new GLabel(this, x, y, w, 20);
  title.setText("Color picker dialog", GAlign.LEFT, GAlign.MIDDLE);
  title.setOpaque(true);
  title.setTextBold();
  btnColor = new GButton(this, x, y+26, 80, 20, "Choose");
  sel_col = color(255);
  pg = createGraphics(60, 20, JAVA2D);
  pg.beginDraw();
  pg.background(sel_col);
  pg.endDraw();
  spad = new GSketchPad(this, x+88, y+26, pg.width, pg.height);
  spad.setGraphic(pg);
}

public void createMessageDialogGUI(int x, int y, int w, int h, int border) {
  // Store picture frame
  rects.add(new Rectangle(x, y, w, h));
  // Set inner frame position
  x += border; 
  y += border;
  w -= 2*border; 
  h -= 2*border;
  GLabel title = new GLabel(this, x, y, w, 20);
  title.setText("Message dialogs", GAlign.LEFT, GAlign.MIDDLE);
  title.setOpaque(true);
  title.setTextBold();
  btnMdialog = new GButton(this, x, y+26, 80, 20, "Show");
  String[] t = new String[] { 
    "Plain", "Error", "Information", "Warning", "Question"
  };
  opgMmessType = new GToggleGroup();
  optMmessType = new GOption[t.length];
  for (int i = 0; i < optMmessType.length; i++) {
    optMmessType[i] = new GOption(this, x, y+60+i*18, 94, 18);
    optMmessType[i].setText(t[i]);
    optMmessType[i].tagNo = 9000 + i;
    opgMmessType.addControl(optMmessType[i]);
  }
  md_mtype = 0;
  optMmessType[md_mtype].setSelected(true);
  GLabel dtitle = new GLabel(this, x+w-190, y+20, 190, 20);
  dtitle.setText("Dialog title", GAlign.LEFT, GAlign.MIDDLE);
  txfMdTitle = new GTextField(this, x+w-190, y+40, 190, 20);
  txfMdTitle.setDefaultText("Enter dialog title");
  GLabel dmess = new GLabel(this, x+w-190, y+60, 190, 20);
  dmess.setText("Dialog message", GAlign.LEFT, GAlign.MIDDLE);
  txfSMMessage = new GTextArea(this, x+w-193, y+75, 196, 100);
  txfSMMessage.setOpaque(false);
  txfSMMessage.setDefaultText("Enter dialog message");
}

public void createOptionDialogGUI(int x, int y, int w, int h, int border) {
  // Store picture frame
  rects.add(new Rectangle(x, y, w, h));
  // Set inner frame position
  x += border; 
  y += border;
  w -= 2*border; 
  h -= 2*border;
  GLabel title = new GLabel(this, x, y, w, 20);
  title.setText("Option dialogs", GAlign.LEFT, GAlign.MIDDLE);
  title.setOpaque(true);
  title.setTextBold();
  btnOdialog = new GButton(this, x, y+26, 80, 20, "Show");
  String[] t = new String[] { 
    "Plain", "Error", "Information", "Warning", "Question"
  };
  opgOmessType = new GToggleGroup();
  optOmessType = new GOption[t.length];
  for (int i = 0; i < optOmessType.length; i++) {
    optOmessType[i] = new GOption(this, x, y+60+i*18, 94, 18);
    optOmessType[i].setText(t[i]);
    optOmessType[i].tagNo = 6000 + i;
    opgOmessType.addControl(optOmessType[i]);
  }
  od_mtype = 0;
  optOmessType[md_mtype].setSelected(true);
  t = new String[] { 
    "Yes / No", "Yes / No / Cancel", "Ok / Cancel"
  };
  opgOoptType = new GToggleGroup();
  optOoptType = new GOption[t.length];
  for (int i = 0; i < optOoptType.length; i++) {
    optOoptType[i] = new GOption(this, x+104, y+60+i*18, 140, 18);
    optOoptType[i].setText(t[i]);
    optOoptType[i].tagNo = 7000 + i;
    opgOoptType.addControl(optOoptType[i]);
  }
  od_otype = 0;
  optOoptType[md_mtype].setSelected(true);
  lblReply = new GLabel(this, x+104, y+26, 120, 20);
  lblReply.setLocalColorScheme(G4P.GREEN_SCHEME);
  lblReply.setOpaque(true);
  lblReply.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  GLabel dtitle = new GLabel(this, x+w-190, y+20, 190, 20);
  dtitle.setText("Dialog title", GAlign.LEFT, GAlign.MIDDLE);
  txfOdTitle = new GTextField(this, x+w-190, y+40, 190, 20);
  txfOdTitle.setDefaultText("Enter dialog title");
  GLabel dmess = new GLabel(this, x+w-190, y+60, 190, 20);
  dmess.setText("Dialog message", GAlign.LEFT, GAlign.MIDDLE);
  txfOdMessage = new GTextArea(this, x+w-193, y+75, 196, 100);
  txfOdMessage.setOpaque(false);
  txfOdMessage.setDefaultText("Enter dialog message");
}

public void createFileSystemGUI(int x, int y, int w, int h, int border) {
  // Store picture frame
  rects.add(new Rectangle(x, y, w, h));
  // Set inner frame position
  x += border; 
  y += border;
  w -= 2*border; 
  h -= 2*border;
  GLabel title = new GLabel(this, x, y, w, 20);
  title.setText("File system dialogs", GAlign.LEFT, GAlign.MIDDLE);
  title.setOpaque(true);
  title.setTextBold();
  // Create buttons
  int bgap = 8;
  int bw = round((w - 2 * bgap) / 3.0f);
  int bs = bgap + bw;
  btnFolder = new GButton(this, x, y+30, bw, 20, "Folder");
  btnInput = new GButton(this, x+bs, y+30, bw, 20, "Input");
  btnOutput = new GButton(this, x+2*bs, y+30, bw, 20, "Output");
  lblFile = new GLabel(this, x, y+60, w, 44);
  lblFile.setTextAlign(GAlign.LEFT, GAlign.MIDDLE);
  lblFile.setOpaque(true);
  lblFile.setLocalColorScheme(G4P.GREEN_SCHEME);
  // Use native or Java Swing dialogs
  cbxUseNative = new GCheckbox(this, x, y + h - 14, w, 20, "Use native controls");
  cbxUseNative.setSelected(true);
  cbxUseNative.setTextItalic();
}

// Controls used for file dialog GUI 
GButton btnFolder, btnInput, btnOutput;
GLabel lblFile;
GCheckbox cbxUseNative;

// Controls used for message dialog GUI 
GButton btnMdialog;
GOption[] optMmessType;
GToggleGroup opgMmessType;
GTextField txfMdTitle;
GTextArea txfSMMessage;
int md_mtype;

// Controls used for option dialog GUI 
GButton btnOdialog;
GOption[] optOmessType, optOoptType;
GToggleGroup opgOmessType, opgOoptType;
GTextField txfOdTitle;
GTextArea txfOdMessage;
GLabel lblReply;
int od_mtype, od_otype;

// Controls used for colour chooser dialog GUI 
GButton btnColor;
GSketchPad spad;
PGraphics pg;
int sel_col = -1;

// Graphic frames used to group controls
ArrayList<Rectangle> rects ;


