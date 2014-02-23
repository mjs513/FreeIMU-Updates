public void graphwin() {
  cf = addControlFrame("Sensor Data", 550,700);
  cp5.get(Button.class, "graphwin").setVisible(false);
}


ControlFrame addControlFrame(String theName, int theWidth, int theHeight) {
  final Frame f = new Frame(theName);
  final ControlFrame p = new ControlFrame(this, theWidth, theHeight);
  f.add(p);
  p.init();
  f.setTitle(theName);
  f.setSize(p.w, p.h);
  f.setLocation(100, 100);
  f.setResizable(false);
  f.setVisible(true);
  f.addWindowListener(new WindowAdapter() {
    public void windowClosing(WindowEvent we) {
      p.dispose();
      f.dispose();
      cp5.get(Button.class, "graphwin").setVisible(true);
    }
  }
  );
  return p;
}

// the ControlFrame class extends PApplet, so we 
// are creating a new processing applet inside a
// new frame with a controlP5 object loaded
public class ControlFrame extends PApplet {

  int w, h;
  int abc = 100;
  int startPlots;

  /**
  * RollingGraph
  * This sketch makes ise of the RollingLine2DTrace object to
  * draw a dynamically updated plot.
  */

  import org.gwoptics.graphics.graph2D.Graph2D;
  import org.gwoptics.graphics.graph2D.traces.ILine2DEquation;
  import org.gwoptics.graphics.graph2D.traces.RollingLine2DTrace;

  class eq implements ILine2DEquation{
    public double computePoint(double x,int pos) {
      return (double)dyn_acc[0];
    }    
  }
  class eq2 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
      return (double)dyn_acc[1];
    }    
  }
  class eq3 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
       return (double)dyn_acc[2];
    }
  }
  class eq4 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
      return (double)degrees(Euler[0]);
    }    
  }
  class eq5 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
      return (double)degrees(ypr[1]);
    }    
  }
  class eq6 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
      return (double)degrees(ypr[2]);
    }    
  }

  RollingLine2DTrace r,r2,r3,r4,r5,r6;
  Graph2D g, g1;
  
  public void setup() {
    size(w, h);
    //frameRate(25);
    
    startPlots = 0;
    
    r  = new RollingLine2DTrace(new eq() ,100,0.1f);
    r.setTraceColour(255, 0, 0);
    r.setLineWidth(2);
  
    r2 = new RollingLine2DTrace(new eq2(),100,0.1f);
    r2.setTraceColour(0, 255, 0);
    r2.setLineWidth(2);
  
    r3 = new RollingLine2DTrace(new eq3(),100,0.1f);
    r3.setTraceColour(0, 0, 255);
    r3.setLineWidth(2);
   
    r4  = new RollingLine2DTrace(new eq4() ,100,0.1f);
    r4.setTraceColour(255, 0, 0);
    r4.setLineWidth(2);
  
    r5 = new RollingLine2DTrace(new eq5(),100,0.1f);
    r5.setTraceColour(0, 255, 0);
    r5.setLineWidth(2);
  
    r6 = new RollingLine2DTrace(new eq6(),100,0.1f);
    r6.setTraceColour(0, 0, 255);
    r6.setLineWidth(2);    
 
    cp5 = new ControlP5(this);
    PFont p = createFont("Arial bold",12); 
    cp5.setControlFont(p, 12);
    
    cp5.addTextlabel("label")
       .setText("Set Accel Range: ")
       .setPosition(10,10)
       .setColorValue(0xffffff00)
       .setFont(createFont("Arial bold", 14));   
    cp5.addSlider("Max")
       .setSize(100,20)
       .setRange(0.05, 16)
       .setNumberOfTickMarks(10)
       .setPosition(140,10);
    cp5.addSlider("Min")
       .setSize(100,20)
       .setRange(-16, -0.05)
       .setNumberOfTickMarks(10)
       .setPosition(290,10);
    //cp5.addSlider("def").plugTo(parent,"def").setRange(0, 255).setPosition(10,30);
    
    cp5.addTextlabel("label1")
       .setText("Set Attit. Range: ")
       .setPosition(10,40)
       .setColorValue(0xffffff00)
       .setFont(createFont("Arial bold", 14));   
    cp5.addSlider("Max Ang")
       .setSize(100,20)
       .setRange(90, 180)
       .setNumberOfTickMarks(8)
       .setPosition(140,40);
    cp5.addSlider("Min Ang")
       .setSize(100,20)
       .setRange(-180, -90)
       .setNumberOfTickMarks(8)
       .setPosition(310,40);
         
    cp5.addBang("Set Rngs")
       .setPosition(175, 80)
       .setSize(100, 20)
       .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);  
   
  }

  public void draw() {
      background(145);
      if(startPlots == 1){
          g.draw();
          g1.draw();  
      } 
  }
  
  void controlEvent(ControlEvent theEvent) {
    if (theEvent.controller().name()=="Set Rngs") { 
      
      startPlots = 1;
      
      g = new Graph2D(this, 400, 200, false);
      g.addTrace(r);
      g.addTrace(r2);
      g.addTrace(r3);
      g.position.y = 110;
      g.position.x = 90;

      g.setXAxisMax(5f);
      g.setXAxisLabelFont("Arial bold", 14, false);
      g.setYAxisLabelFont("Arial bold", 14, false);
      g.setXAxisTickFont("Arial bold", 12, false);
      g.setYAxisTickFont("Arial bold", 12, false);
      g.setYAxisMax(cp5.get(Slider.class, "Max").getValue());
      g.setYAxisMin(cp5.get(Slider.class, "Min").getValue());
      g.setYAxisTickSpacing((cp5.get(Slider.class, "Max").getValue()-cp5.get(Slider.class, "Min").getValue())/10);
      g.setYAxisLabelAccuracy(2); 
      g.setYAxisLabel("Acceleration (g)");
      
      g1 = new Graph2D(this, 400, 200, false);
      g1.addTrace(r4);
      g1.addTrace(r5);      
      g1.addTrace(r6);      
      g1.position.y = 370;
      g1.position.x = 90;

      g1.setXAxisMax(5f);  
      g1.setXAxisLabelFont("Arial bold", 14, false);
      g1.setYAxisLabelFont("Arial bold", 14, false);    
      g1.setXAxisTickFont("Arial bold", 12, false);
      g1.setYAxisTickFont("Arial bold", 12, false);
      g1.setYAxisMax(cp5.get(Slider.class, "Max Ang").getValue());
      g1.setYAxisMin(cp5.get(Slider.class, "Min Ang").getValue());
      g1.setYAxisTickSpacing((cp5.get(Slider.class, "Max Ang").getValue()-cp5.get(Slider.class, "Min Ang").getValue())/10);
      g1.setYAxisLabelAccuracy(2);
      g1.setYAxisLabel("Yaw/Pitch/Roll (deg)");
     }
  }
  
  private ControlFrame() {
  }

  public ControlFrame(Object theParent, int theWidth, int theHeight) {
    parent = theParent;
    w = theWidth;
    h = theHeight;
  }


  public ControlP5 control() {
    return cp5;
  }

  
  ControlP5 cp5;
  Object parent;
 
}

