/**
 Demonstration of image buttons available in the G4P (GUI for
 Processing) library. The button face comes from user supplied
 image files.
 
 (c) 2012 Peter Lager
 */

import g4p_controls.*;

GImageButton btnGhost, btnCoins, btnTJ, btnInfo;
GLabel lblOut;
long timer;

void setup() {
  size(580, 220);
  cursor(CROSS);
  String[] files;

  files = new String[] { 
    "infooff.png", "infoover.png", "infodown.png"
  };
  btnInfo = new GImageButton(this, 20, 14, files, "infomask.png");

  files = new String[] { 
    "ghost0.png", "ghost1.png", "ghost2.png"
  };
  btnGhost = new GImageButton(this, 40, 90, files);

  files = new String[] { 
    "tjoff.jpg", "tjover.jpg", "tjdown.jpg"
  };
  btnTJ = new GImageButton(this, 150, 10, files, "tjmask.png");

  files = new String[] { 
    "coins0.png", "coins1.png", "coins2.png"
  };
  btnCoins = new GImageButton(this, 400, 20, files);

  lblOut = new GLabel(this, 10, 190, 560, 20, "");
  lblOut.setTextAlign(GAlign.CENTER, null);
  timer = millis() - 5000;
}

void draw() {
  background(220, 220, 255);
  // Only display button info for 4 seconds
  if (millis() - timer > 4000) {
    lblOut.setText("CLICK ON A BUTTON");
  }
}

// When a button has been clicked give information aout the 
// button
void handleButtonEvents(GImageButton button, GEvent event) {
  if (button == btnGhost)
    lblOut.setText("Ghosts - png images using transparency");
  else if (button == btnCoins)
    lblOut.setText("Coins -  png images using transparency");  
  else if (button == btnTJ)
    lblOut.setText("Tom & Jerry - jpg images using png mask");
  else if (button == btnInfo)
    lblOut.setText("Info - png images using png mask");
  timer = millis();
}
