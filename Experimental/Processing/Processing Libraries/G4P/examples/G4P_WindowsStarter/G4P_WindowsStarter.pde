/*
 Demonstration of how to create and use multiple windows with
 the G4P (GUI for Processing) library.
 
 (c)2012 Peter Lager
 
 Since each window has its own unique data set they can all 
 share the code needed for mouse handling and drawing.
 It is a complex example and it is recommended that you read
 a detailed explanation of the code used which can be found at:
   http://www.lagers.org.uk/g4p/applets/g4p_windowsstarter
   
 */

import g4p_controls.*;

GWindow[] window;
GButton btnStart;
GLabel lblInstr;

void setup() {
  size(256, 128);
  btnStart = new GButton(this, 4, 34, 120, 60, "Create 3 Windows");
  lblInstr = new GLabel(this, 132, 34, 120, 60, "Use the mouse to draw a rectangle in any of the 3 windows");
  lblInstr.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  lblInstr.setVisible(false);
}

/**
 * Draw for the main window
 */
void draw() {
  background(240);
}

/**
Create the three windows so that they share mouse handling 
and drawing code.
*/
void createWindows() {
  int col;
  window = new GWindow[3];
  for (int i = 0; i < 3; i++) {
    col = (128 << (i * 8)) | 0xff000000;
    window[i] = new GWindow(this, "Window "+i, 70+i*220, 160+i*50, 200, 200, false, JAVA2D);
    window[i].setBackground(col);
    window[i].addData(new MyWinData());
    window[i].addDrawHandler(this, "windowDraw");
    window[i].addMouseHandler(this, "windowMouse");
  }
}

/**
 * Click the button to create the windows.
 * @param button
 */
void handleButtonEvents(GButton button, GEvent event) {
  if (window == null && event == GEvent.CLICKED) {
    createWindows();
    lblInstr.setVisible(true);
    button.setEnabled(false);
  }
}

/**
 * Handles mouse events for ALL GWindow objects
 *  
 * @param appc the PApplet object embeded into the frame
 * @param data the data for the GWindow being used
 * @param event the mouse event
 */
void windowMouse(GWinApplet appc, GWinData data, MouseEvent event) {
  MyWinData data2 = (MyWinData)data;
  switch(event.getAction()) {
  case MouseEvent.PRESS:
    data2.sx = data2.ex = appc.mouseX;
    data2.sy = data2.ey = appc.mouseY;
    data2.done = false;
    break;
  case MouseEvent.RELEASE:
    data2.ex = appc.mouseX;
    data2.ey = appc.mouseY;
    data2.done = true;
    break;
  case MouseEvent.DRAG:
    data2.ex = appc.mouseX;
    data2.ey = appc.mouseY;
    break;
  }
}

/**
 * Handles drawing to the windows PApplet area
 * 
 * @param appc the PApplet object embeded into the frame
 * @param data the data for the GWindow being used
 */
void windowDraw(GWinApplet appc, GWinData data) {
  MyWinData data2 = (MyWinData)data;
  if (!(data2.sx == data2.ex && data2.ey == data2.ey)) {
    appc.stroke(255);
    appc.strokeWeight(2);
    appc.noFill();
    if (data2.done) {
      appc.fill(128);
    }
    appc.rectMode(CORNERS);
    appc.rect(data2.sx, data2.sy, data2.ex, data2.ey);
  }
}  

/**
 * Simple class that extends GWinData and holds the data 
 * that is specific to a particular window.
 * 
 * @author Peter Lager
 */
class MyWinData extends GWinData {
  int sx, sy, ex, ey;
  boolean done;
}