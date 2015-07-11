/**
 * The most complicated example so far. This will display the Mandelbrot
 * set in a separate window. Draw a rectangle over the image to create 
 * a new Mandelbrot in a new window ...
 *
 * (c) 2012 by Peter Lager
 */

import g4p_controls.*;
import java.awt.Color;

final int MAX_ITERATE = 128;

final int MAX_WIDTH = 400;
final int MAX_HEIGHT = 400;

final int MIN_WIDTH = 30;
final int MIN_HEIGHT = 30;

int[] colors = new int[MAX_ITERATE];

GButton btnStart;
GLabel label;

int locX = 100, locY = 100;

String t0, t1, t2;


public void setup() {
  size(300, 160);
  G4P.setGlobalColorScheme(G4P.ORANGE_SCHEME);
  btnStart = new GButton(this, (width - 80)/2, height - 30, 80, 20, "START");
  t0 = "MANDELBROT GALORE\n";
  t1 = "Click on the START button to see the Mandelbrot set in a separate window. ";
  t2 = " To zoom into the set drag a box on the area you are interested in and ";
  t2 += "he enlarged image will appear in a new window.";
  label = new GLabel(this, 20, 20, 260, 200, t0 + t1 + t2);
  label.setTextAlign(GAlign.CENTER, GAlign.TOP);
  createColours();
}

/**
 * Draw for the main sketch window
 */
public void draw() {
  background(230, 230, 230);
}

/**
 Create the colours to be used.
 */
public void createColours() {
  colors[MAX_ITERATE-1] = color(0);
  float hue = 0.0f;
  float bright = 1.0f;
  float binc = 0.009f/MAX_ITERATE;
  for (int i = 0; i < MAX_ITERATE - 2; i++) {
    colors[i] = Color.HSBtoRGB(hue, 1.0f, bright);
    hue = (hue + 0.1309f) % 1.0f ;
    bright -= binc;
  }
}

/**
 * Create a PImage of the correct size and using the data
 * for a window create the Mandelbrot image. Signal when
 * complete by setting imgDone = true
 * 
 * @param data
 */
public void calcMandlebrot(MyWinData data) {
  double x0, x1, y0, y1, deltaX, deltaY;
  double colX, rowY;
  x0 = data.sx;
  x1 = data.ex;
  y0 = data.sy;
  y1 = data.ey;
  deltaX = (x1 - x0)/data.w;
  deltaY = (y1 - y0)/data.h;
  data.img.loadPixels();
  int count = 0;
  Complex c = new Complex();
  Complex z = new Complex();

  colX = x0;
  rowY = y0;
  int minC = 999, maxC = -999;
  for (int row = 0; row < data.h; row++) {
    colX = x0;
    for (int col = 0; col < data.w; col++) {
      count = 0;
      c.set(colX, rowY);
      z.set(colX, rowY);
      while (count < MAX_ITERATE-1 && z.sizeSquared () < 4.0) {
        z = z.squared().add(c); 
        count++;
      }
      if (count < minC) minC = count;
      if (count > maxC) maxC = count;
      data.img.pixels[col + row * data.w] = colors[count];
      colX += deltaX;
    }
    rowY += deltaY;
  }
  data.img.updatePixels();
  data.imgDone = true;
}

/**
 * Create and display a new Frame for a Mandelbrot
 * make it as big as possible to fit MAX_WIDTH and MAX_HEIGHT
 * but reject if this results in the width or height being less
 * than the minimum vales (MIN_WIDTH, MIN_HEIGHT.
 * 
 * @param nsx
 * @param nex
 * @param nsy
 * @param ney
 * @return
 */
public boolean makeNewBrotWindow(double nsx, double nex, double nsy, double ney, int action) {
  MyWinData mydata = new MyWinData();
  GWindow window = null;
  // Using doubles for greater magnification range
  double mag = 2.5 / (nex-nsx);
  String s = String.format("%.2E", mag);
  String title = "Mandelbrot (x "+s+")";
  float ratio = (float) ((nex-nsx)/(ney-nsy));
  if (ratio > MAX_WIDTH/MAX_HEIGHT) {
    mydata.w = MAX_WIDTH;
    mydata.h = (int)( MAX_HEIGHT / ratio);
  }
  else {
    mydata.h = MAX_HEIGHT;
    mydata.w = (int)( MAX_WIDTH * ratio);
  }
  mydata.sx = nsx;
  mydata.sy = nsy;
  mydata.ex = nex;
  mydata.ey = ney;
  mydata.imgDone = false;

  if (mydata.w >= MIN_WIDTH && mydata.h >= MIN_HEIGHT) {
    mydata.img = new PImage(mydata.w, mydata.h);
    window = new GWindow(this, title, locX, locY, mydata.w, mydata.h, false, null);
    // Need to replace with screen.width and screen.height for
    // Processing 1.5.1
    locX = (locX + mydata.w + 20)%(displayWidth - MAX_WIDTH);
    locY = (locY + 20)%(displayHeight - MAX_HEIGHT);
    window.addData(mydata);
    window.addDrawHandler(this, "windowDraw");
    window.addMouseHandler(this, "windowMouse");
    window.setActionOnClose(action);
    calcMandlebrot(mydata);
    return true;
  }
  return false;
}


/**
 * Click the button to create the windows.
 * @param button
 */
public void handleButtonEvents(GButton button, GEvent event) {
  if (btnStart == button) {
    makeNewBrotWindow(-2.0f, 0.5f, -1.25f, 1.25f, GWindow.KEEP_OPEN);
    btnStart.setVisible(false);
    label.setText(t0 + t2);
  }
}

/**
 * Handles mouse events for ALL GWindow objects
 * 
 * @param appc the PApplet object embeded into the frame
 * @param data the data for the GWindow being used
 * @param event the mouse event
 */
public void windowMouse(GWinApplet appc, GWinData data, MouseEvent event) {
  MyWinData d = (MyWinData)data;
  if (d.imgDone == false)
    return;
  switch(event.getAction()) {
  case MouseEvent.PRESS:
    d.msx = d.mex = appc.mouseX;
    d.msy = d.mey = appc.mouseY;
    appc.loop();
    appc.frameRate(30);
    break;
  case MouseEvent.RELEASE:
    d.mex = appc.mouseX;
    d.mey = appc.mouseY;
    // Make sure the coordinates are top left / bottom left
    int temp;
    if (d.msx > d.mex) {
      temp = d.msx; 
      d.msx = d.mex; 
      d.mex = temp;
    }
    if (d.msy > d.mey) {
      temp = d.msy; 
      d.msy = d.mey; 
      d.mey = temp;
    }
    // Calculate the new Mandelbrot plane coordinates
    double nsx, nex, nsy, ney;
    nsx = dmap((double)d.msx, (double)0, (double)d.w, d.sx, d.ex);
    nex = dmap((double)d.mex, (double)0, (double)d.w, d.sx, d.ex);
    nsy = dmap((double)d.msy, (double)0, (double)d.h, d.sy, d.ey);
    ney = dmap((double)d.mey, (double)0, (double)d.h, d.sy, d.ey);
    makeNewBrotWindow(nsx, nex, nsy, ney, GWindow.CLOSE_WINDOW);
    d.msx = d.mex = d.msy = d.mey = 0;
    appc.noLoop();
    break;
  case MouseEvent.DRAG:
    d.mex = appc.mouseX;
    d.mey = appc.mouseY;
    break;
  }
}

/**
 * Copied from PApplet and converted floats to doubles 
 * @param value
 * @param istart
 * @param istop
 * @param ostart
 * @param ostop
 * @return
 */
double dmap(double value, double istart, double istop, double ostart, double ostop) {
  return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

/**
 * Handles drawing to the windows PApplet area
 * 
 * @param appc the PApplet object embeded into the frame
 * @param data the data for the GWindow being used
 */
public void windowDraw(GWinApplet appc, GWinData data) {
  MyWinData d = (MyWinData)data;
  if (d.imgDone) {
    appc.image(d.img, 0, 0);
    if (d.msx != d.mex || d.msy != d.mey) {
      appc.strokeWeight(2);
      appc.stroke(255);
      appc.noFill();
      appc.rectMode(CORNERS);
      appc.rect(d.msx, d.msy, d.mex, d.mey);
    }
  }
}

/**
 A basic complex number class.
 */
class Complex {
  public double real;
  public double img;

  Complex() {
    real = img = 0.0f;
  }

  public Complex(float r, float i) {
    super();
    this.real = r;
    this.img = i;
  }

  public void set(double colX, double rowY) {
    real = colX;
    img = rowY;
  }

  public Complex add(Complex c) {
    real += c.real;
    img += c.img;
    return this;
  }

  public void mult(Complex c) {
    double nReal = real * c.real - img * c.img;
    double nImg = real * c.img + img * c.real;
    real = nReal;
    img = nImg;
  }

  public Complex squared() {
    double nReal = (real - img)*(real + img);
    double nImg = 2 * real * img;
    real = nReal;
    img = nImg;  
    return this;
  }

  public double sizeSquared() {
    return real * real + img * img;
  }
}

/**
 * Simple class that extends GWinData and holds the data that is specific
 * to a particular window.
 */
class MyWinData extends GWinData {

  public int msx, msy, mex, mey;
  public double sx, sy, ex, ey;
  public int w, h;
  public PImage img;
  public boolean imgDone;
}
