/**
 * ControlP5 ControlEvent.
 * every control event is automatically forwarded to the function controlEvent(ControlEvent)
 * inside a sketch if such function is available. For further details about the API of 
 * the ControlEvent class, please refer to the documentation.
 *
 *
 * find a list of public methods available for ControlEvent
 * at the bottom of this sketch's source code
 *
 * by Andreas Schlegel, 2012
 * www.sojamo.de/libraries/controlP5
 *
 */

import controlP5.*;

ControlP5 cp5;

public int myColorRect = 200;

public int myColorBackground = 100;


void setup() {
  size(400, 400);
  noStroke();
  
  cp5 = new ControlP5(this);
  cp5.addNumberbox("n1")
     .setValue(myColorRect)
     .setPosition(100, 160)
     .setSize(100, 14)
     .setId(1);
     
  cp5.addNumberbox("n2")
     .setValue(myColorBackground)
     .setPosition(100, 200)
     .setSize(100, 14)
     .setId(2);
     
  cp5.addTextfield("n3")
     .setPosition(100, 240)
     .setSize(100, 20)
     .setId(3);
     
}

void draw() {
  background(myColorBackground);
  fill(myColorRect);
  rect(0, 0, width, 100);
}


void controlEvent(ControlEvent theEvent) {
  println("got a control event from controller with id "+theEvent.getController().getId());
  
  if (theEvent.isFrom(cp5.getController("n1"))) {
    println("this event was triggered by Controller n1");
  }
  
  switch(theEvent.getController().getId()) {
    case(1):
    myColorRect = (int)(theEvent.getController().getValue());
    break;
    case(2):
    myColorBackground = (int)(theEvent.getController().getValue());
    break;
    case(3):
    println(theEvent.getController().getStringValue());
    break;
  }
}


/*
 a list of all methods available for ControlEvent
 use ControlP5.printPublicMethodsFor(ControlEvent.class);
 to print the following list into the console.
 
 You can find further details about class ControlEvent in the javadoc.
 
 Format:
 ClassName : returnType methodName(parameter type)
 
 controlP5.ControlEvent : ControlGroup getGroup() 
 controlP5.ControlEvent : Controller getController() 
 controlP5.ControlEvent : String getLabel() 
 controlP5.ControlEvent : String getName() 
 controlP5.ControlEvent : String getStringValue() 
 controlP5.ControlEvent : Tab getTab() 
 controlP5.ControlEvent : boolean isController() 
 controlP5.ControlEvent : boolean isFrom(ControllerInterface) 
 controlP5.ControlEvent : boolean isFrom(String) 
 controlP5.ControlEvent : boolean isGroup() 
 controlP5.ControlEvent : boolean isTab() 
 controlP5.ControlEvent : float getValue() 
 controlP5.ControlEvent : float[] getArrayValue() 
 controlP5.ControlEvent : int getId() 
 controlP5.ControlEvent : int getType() 
 java.lang.Object : String toString() 
 java.lang.Object : boolean equals(Object) 
 */
 
 
