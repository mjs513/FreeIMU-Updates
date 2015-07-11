/**
 Demonstration of the GSlider2D control to position a
 gun-sight over a street scene.
 
 created by Peter Lager
 
 */
import g4p_controls.*;


GSlider2D sdrSniper;

PImage imgScene, imgSight;
// Lens properties
int lsize = 40, lsize2 = lsize * lsize;
float mag = 2.0f;
float k = -0.00002;

int sightX, sightY;
int border, borderViaLens;

public void setup() {
  size(540, 460);
  cursor(CROSS);
  imageMode(CENTER);
  imgScene = loadImage("backdrop.jpg");
  imgSight = loadImage("sight.png");

  // border colours
  borderViaLens = color(0, 80, 0);
  sdrSniper = new GSlider2D(this, 180, 340, 180, 100);
  sdrSniper.setLimitsX(180, 60, 480);
  sdrSniper.setLimitsY(150, 60, 280);
  sdrSniper.setEasing(8);
  // sdrSniper.setOpaque(false);
  sightX = sdrSniper.getValueXI();
  sightY = sdrSniper.getValueYI();
}

public void draw() {
  background(imgScene);
  showLens(sightX, sightY);
  image(imgSight, sightX, sightY);
}

public void handleSlider2DEvents(GSlider2D slider2d, GEvent event) {
  if (slider2d == sdrSniper) {
    sightX = sdrSniper.getValueXI();
    sightY = sdrSniper.getValueYI();
  }
}

public void showLens(int x, int y) {
  int u, v, r2;
  float f;
  for (int vd = - lsize; vd < lsize; vd++) {
    for (int ud = - lsize; ud < lsize; ud++) {
      r2 = ud*ud + vd*vd;
      if (r2 <= lsize2) {
        f = mag + k * r2;
        u = floor(ud/f) + x;
        v = floor(vd/f) + y;
        if (u >= 0 && u < imgScene.width && v >=0 && v < imgScene.height)
          set(ud + x, vd + y, imgScene.get(u, v));
        else 
          set(ud + x, vd + y, borderViaLens);
      }
    }
  }
}