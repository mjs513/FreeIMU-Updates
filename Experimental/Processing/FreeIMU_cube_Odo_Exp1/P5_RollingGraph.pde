public void graphwin() {
  cf = addControlFrame("Sensor Data", 600,700);
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
      return (double)acc[0];
    }    
  }
  class eq2 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
      return (double)acc[1];
    }    
  }
  class eq3 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
       return (double)acc[2];
    }
  }
  
  class gy1 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
       return (double)gyro[0];
    }
  }  
  class gy2 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
       return (double)gyro[1];
    }
  }    
  class gy3 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
       return (double)gyro[2];
    }
  }    

  class mg1 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
       return (double)magn[0];
    }
  }  
  class mg2 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
       return (double)magn[1];
    }
  }  
  class mg3 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
       return (double)magn[2];
    }
  }  

  class Ta implements ILine2DEquation{
    public double computePoint(double x,int pos) {
       return (double)temp;
    }
  }  
  class Alt implements ILine2DEquation{
    public double computePoint(double x,int pos) {
       return (double)fused_alt;
    }
  }  
  
  class eq4 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
      return (double)degrees(Euler[0]);
    }    
  }
  class eq5 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
      return (double)degrees(Euler[1]);
    }    
  }
  class eq6 implements ILine2DEquation{
    public double computePoint(double x,int pos) {
      return (double)degrees(Euler[2]);
    }    
  }

  RollingLine2DTrace r,r2,r3,r4,r5,r6,G1,G2,G3,M1,M2,M3,T1,A1;
  
  Graph2D g, g1;
  CheckBox checkbox;
  PImage legend;
  
  public void setup() {
    size(w, h);
    //frameRate(25);
    
    startPlots = 0;
    
    legend = loadImage(skpath + "Legend.png");
    
    //Acceleration
    r  = new RollingLine2DTrace(new eq() ,100,0.1f);
    r.setTraceColour(228,26,28);
    r.setLineWidth(2);
  
    r2 = new RollingLine2DTrace(new eq2(),100,0.1f);
    r2.setTraceColour(77,175,74);
    r2.setLineWidth(2);
  
    r3 = new RollingLine2DTrace(new eq3(),100,0.1f);
    r3.setTraceColour(55,126,184);
    r3.setLineWidth(2);
 
    //Gyro
    G1 = new RollingLine2DTrace(new gy1(),100,0.1f);
    G1.setTraceColour(152,78,163);
    G1.setLineWidth(2);    
    
    G2 = new RollingLine2DTrace(new gy2(),100,0.1f);
    G2.setTraceColour(255,127,0);
    G2.setLineWidth(2);    

    G3 = new RollingLine2DTrace(new gy3(),100,0.1f);
    G3.setTraceColour(255,255,51);
    G3.setLineWidth(2);

    //Magnetometer
    M1 = new RollingLine2DTrace(new mg1(),100,0.1f);
    M1.setTraceColour(0, 0, 255);
    M1.setLineWidth(2);
    
    M2 = new RollingLine2DTrace(new mg2(),100,0.1f);
    M2.setTraceColour(166,86,40);
    M2.setLineWidth(2);
    
    M3 = new RollingLine2DTrace(new mg3(),100,0.1f);
    M3.setTraceColour(202,178,214);
    M3.setLineWidth(2);
    
    //Temp and Altitude
    T1 = new RollingLine2DTrace(new Ta(),100,0.1f);
    T1.setTraceColour(255, 0, 0);
    T1.setLineWidth(2); 
 
    A1 = new RollingLine2DTrace(new Alt(),100,0.1f);
    A1.setTraceColour(0, 0, 255);
    A1.setLineWidth(2);     
    
    //Yaw - Pitch - Roll Chart: Fixed for now  
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

    CheckBox checkbox;
    
    cp5.addTextlabel("label")
       .setText("Set Accel Range: ")
       .setPosition(10,10)
       .setColorValue(0xffffff00)
       .setFont(createFont("Arial bold", 14));   
    cp5.addSlider("Max")
       //.setSize(100,20)
       .setSize(180,20)
       //.setRange(0.05, 16)
       .setRange(0.05, 100)
       .setNumberOfTickMarks(75)
       .setPosition(140,10);
    cp5.addSlider("Min")
       //.setSize(100,20)
       .setSize(180,20)
       //.setRange(-16, -0.05)
       .setRange(-100,-0.05)
       .setNumberOfTickMarks(75)
       //.setPosition(290,10);
       .setPosition(370,10);
    //cp5.addSlider("def").plugTo(parent,"def").setRange(0, 255).setPosition(10,30);
    
    cp5.addTextlabel("label1")
       .setText("Set Attit. Range: ")
       .setPosition(10,40)
       .setColorValue(0xffffff00)
       .setFont(createFont("Arial bold", 14));   
    cp5.addSlider("Max Ang")
       .setSize(100,20)
       .setRange(0, 180)
       .setNumberOfTickMarks(32)
       .setPosition(140,40);
    cp5.addSlider("Min Ang")
       .setSize(100,20)
       .setRange(-180, 0)
       .setNumberOfTickMarks(32)
       .setPosition(310,40);
         
    cp5.addBang("Set Config")
       //.setPosition(175, 80)
       .setPosition(280, 160)
       .setSize(100, 20)
       .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);  

     cp5.addCheckBox("checkBox")
                .setPosition(100, 120)
                .setColorForeground(color(120))
                .setColorActive(color(255))
                .setColorLabel(color(255))
                .setSize(20, 20)
                .setItemsPerRow(3)
                .setSpacingColumn(30)
                .setSpacingRow(20)
                .addItem("Ax", 1)
                .addItem("Ay", 2)
                .addItem("Az", 3)
                .addItem("Gx", 11)
                .addItem("Gy", 12)
                .addItem("Gz", 13)
                .addItem("Mx", 21)
                .addItem("My", 22)
                .addItem("Mz", 23)
                .addItem("Ta", 23)
                .addItem("Alt", 23)
                ;       
   
  }

  public void draw() {
      //background(145);
      background(180);
      if(startPlots == 1){
          legend.resize(0,50);
          image(legend,110,45);  
          g.draw();
          g1.draw();  
      } 
  }

  void keyPressed() {
    if (key==' ') {
      cp5.get(CheckBox.class, "checkBox").deactivateAll();
    } 
  }
  
  void controlEvent(ControlEvent theEvent) {

    if (theEvent.controller().name()=="Set Config") { 
      //println(cp5.get(CheckBox.class, "checkBox").getArrayValue());
      //println(cp5.get(CheckBox.class, "checkBox").getArrayValue().length);
      cp5.hide();

      startPlots = 1;
      
      g = new Graph2D(this, 400, 200, false);

      for (int i=0;i<cp5.get(CheckBox.class, "checkBox").getArrayValue().length;i++) {
        int n = (int)cp5.get(CheckBox.class, "checkBox").getArrayValue()[i];
        if(n == 1){
          switch (i) {
            case 0:
              g.addTrace(r);
              break;
            case 1:
              g.addTrace(r2);
              break;
            case 2:
              g.addTrace(r3);
              break;            
            case 3:
              g.addTrace(G1);
              break;
            case 4:
              g.addTrace(G2);
              break;
            case 5:
              g.addTrace(G3);
              break;              
            case 6:
              g.addTrace(M1);
              break;
            case 7:
              g.addTrace(M2);
              break;
            case 8:
              g.addTrace(M3);
              break;              
            case 9:
              g.addTrace(T1);
              break;
            case 10:
              g.addTrace(A1);
              break;          
            default: 
              g.addTrace(r);
              g.addTrace(r2);
              g.addTrace(r3);
          }
        }
      }
    
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

