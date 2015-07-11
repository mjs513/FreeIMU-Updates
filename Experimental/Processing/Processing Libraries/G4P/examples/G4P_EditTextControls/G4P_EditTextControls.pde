/**
 * This sketch is to demonstrate some features of the GTextField
 * and GTextArea controls. 
 * These features include
 * - Tabbing between controls
 * - Default text
 * - Copy and paste text
 * 
 * @author Peter Lager
 */
 
import g4p_controls.*;

GTextField txf1, txf2;
GTextArea txa1, txa2;
GTabManager tt;

public void setup() {
  size(500, 260);
  G4P.setGlobalColorScheme(GCScheme.RED_SCHEME);
  // Some start text
  String[] paragraphs = loadStrings("book3.txt");
  String startTextA = PApplet.join(paragraphs, '\n');
  String startTextF = "G4P is a GUI control library created by Peter Lager";

  txf1 = new GTextField(this, 10, 10, 200, 20);
  txf1.tag = "txf1";
  txf1.setDefaultText("Text field 1");

  txf2 = new GTextField(this, 290, 10, 200, 30, G4P.SCROLLBAR_HORIZONTAL);
  txf2.tag = "txf2";
  txf2.setDefaultText("Text field 2");
  txf2.setText(startTextF);

  txa1 = new GTextArea(this, 10, 80, 200, 160);
  txa1.tag = "txa1";
  txa1.setDefaultText("Text area 1");

  txa2 = new GTextArea(this, 290, 80, 200, 160, G4P.SCROLLBARS_BOTH);
  txa2.tag = "txa2";
  txa2.setDefaultText("Text area 2");
  txa2.setText(startTextA, 300);

  // Create the tab manager and add these controls to it
  tt = new GTabManager();
  tt.addControls(txf1, txa1, txf2, txa2);
}

public void draw() {
  background(240, 240, 200);
  // Draw tab order
  stroke(0);
  strokeWeight(2);
  line(txf1.getCX(), txf1.getCY(), txa1.getCX(), txa1.getCY());
  line(txa1.getCX(), txa1.getCY(), txf2.getCX(), txf2.getCY());
  line(txf2.getCX(), txf2.getCY(), txa2.getCX(), txa2.getCY());
}

public void handleTextEvents(GEditableTextControl tc, GEvent event) { 
  System.out.print("\n" + tc.tag + "   Event type: ");
  switch(event) {
  case CHANGED:
    System.out.println("CHANGED");
    break;
  case SELECTION_CHANGED:
    System.out.println("SELECTION_CHANGED");
    System.out.println(tc.getSelectedText() + "\n");
    break;
  case ENTERED:
    System.out.println("ENTER KEY TYPED");
    System.out.println(tc.getSelectedText() + "\n");
    break;
  default:
    System.out.println("UNKNOWN");
  }
}
