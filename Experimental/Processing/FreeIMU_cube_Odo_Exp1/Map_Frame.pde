public void gpscam() {
  f = new PFrame();
  cp5.get(Button.class, "gpscam").setVisible(false);
}

public class PFrame extends Frame {
    public PFrame() {
        setBounds(100,100,900,600);
        mesg = new secondApplet();
        add(mesg);
        mesg.init();
        show();
    }
}



// the ControlFrame class extends PApplet, so we 
// are creating a new processing applet inside a
// new frame with a controlP5 object loaded
public class secondApplet extends PApplet {

  int w, h;
  int abc = 100;
  import peasy.*;
  
  //GLOBAL VARIABLES
  PeasyCam cam;
 
  //screen limits in lat, lon and elevation
  float west = -73.9;
  float east = -73.8;
  float north = 40.8;
  float south = 40.7;
  float lowest = 20;
  float highest = 30;

  //screen display point coordinates
  float screen_X;
  float screen_Y;
  float screen_Z;

  //to draw lines in chronological order
  int totalCount  = 0;
  int toDrawCount = 0; 

  public void setup() {
  size(900, 600, P3D);
  background(0);
  colorMode(HSB, 360, 100, 100);
  fill(180); //colour of title & instructions

  //initialize font for text
  //instructions = loadFont("CenturyGothic-24.vlw");

  //initial peasycam settings
  cam = new PeasyCam(this, width/2, height/2, 0, width*0.6);
  cam.lookAt(width/2, height/2, 0);
  
  }

  public void draw() {
    background(0);
    noStroke();

    int count = 0;
    boolean finished = false;
  
    // map lat and long to height & width of screen display
    float screen_X = map(longt, west, east, 0, width);
    float screen_Y = map(lat, north, south, 0, height);

    //map elevation to appropriate vertical scale for z-axis
    float screen_Z = map(gpsalt, lowest, highest, 0, 50);

    // call function to display the points
    drawPoints(screen_X, screen_Y, screen_Z, count > toDrawCount - 10); 
  }

   //Drawing points is much much faster than drawing spheres
   void drawPoints(float screen_X, float screen_Y, float screen_Z, boolean highlight) {
      if (highlight) {
        stroke(255); // colour most recent points white
      } else {
        stroke(map(screen_Z, 0, 50, 240, 50), 99, 99); //otherwise map point colour to elevation
      }
      point(screen_X, screen_Y, screen_Z);
    }
    
    void keyPressed() {
      if (key=='x') {
        key=0;
        f.dispose();
  }
}

}

