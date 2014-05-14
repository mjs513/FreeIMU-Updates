public void GPStrace() {
  //frameRate(120);
  eSketch = new EmbeddedSketch(child);
  cp5.get(Button.class, "GPStrace").setVisible(false);
}

//The JFrame which will contain the child applet
public class EmbeddedSketch extends JFrame {
  PApplet sketch;
  public EmbeddedSketch(PApplet p) {
    add(p);
    p.init();
    sketch = p;
    setVisible(true);
    setBounds(100, 100, 900, 600);
            
    setLocation(100, 100);
    //setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
  }
}


public class ChildApplet extends PApplet {
 
    public void setup() {
      size(900, 600, P3D);
      smooth();
      background(0);
      colorMode(HSB, 360, 100, 100);
      fill(180); //colour of title & instructions

      cam = new PeasyCam(this, 100);
      cam.reset();
      //cam.setMinimumDistance(50);
      //cam.setMaximumDistance(500);
    }

    public void draw() {
      background(0);
      println(lat);
      box(50, 200, 100);
      // map lat and long to height & width of screen display
      /*float screen_X = map(longt*10000, west*10000, east*10000, 0, width);
      float screen_Y = map(lat*10000, north*10000, south*10000, 0, height);

      //map elevation to appropriate vertical scale for z-axis
      float screen_Z = map(gpsalt, lowest, highest, 0, 50);

      stroke(map(screen_Z, 0, 50, 240, 50), 99, 99); //otherwise map point colour to elevation

      // call function to display the points
      point(screen_X, screen_Y, screen_Z);*/
      
    }
} 
