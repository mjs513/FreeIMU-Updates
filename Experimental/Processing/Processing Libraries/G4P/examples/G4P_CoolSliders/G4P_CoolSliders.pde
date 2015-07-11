/*
 The GCustomSlider control replaces the GWSlider, a control
 created as part of the Processing library `gwoptics' 
 http://www.gwoptics.org/processing/gwoptics_p5lib/

 The GCustomSlider has all the capabilities of the original
 control but also has the capabilities of the GSlider in that
 it can be rotated to any angle and user controllable text
 orientation.
 
 This sketch simply demonstrates some of the features and 
 skins available with this library.
 
 To try out different configurations trye the example
 G4P_CustomSlider_Config.
 
 (c) 2012 Peter Lager
 
 */

import g4p_controls.*;

GCustomSlider sdr1, sdr2, sdr3, sdr4, sdr5, sdr6, sdr7;

void setup() {
  size(600, 280);  

  //=============================================================
  // Simple default slider,
  // constructor is `Parent applet', the x, y position and length
  sdr1 = new GCustomSlider(this, 20, 20, 260, 50, null);
  // show          opaque  ticks value limits
  sdr1.setShowDecor(false, true, true, true);
  sdr1.setNbrTicks(5);
  sdr1.setLimits(40, -100, 100);
  //=============================================================
  // Slider with a custom skin, check the data folder to find 
  // the `blue18px' folder which stores the used image files.
  sdr2 = new GCustomSlider(this, 20, 80, 260, 50, "blue18px");
  // show          opaque  ticks value limits
  sdr2.setShowDecor(false, true, false, true);
  // there are 3 types
  // GCustomSlider.DECIMAL  e.g.  0.002
  // GCustomSlider.EXPONENT e.g.  2E-3
  // GCustomSlider.INTEGER
  sdr2.setNumberFormat(G4P.DECIMAL, 3);
  sdr2.setLimits(0.5f, 0f, 1.0f);
  sdr2.setShowValue(false); 

  //=============================================================
  // Slider with another custom skin
  sdr3 = new GCustomSlider(this, 20, 140, 260, 50, "red_yellow18px");
  // show          opaque  ticks value limits
  sdr3.setShowDecor(false, true, false, true);
  sdr3.setEasing(25);
  
  //=============================================================
  // Slider with another custom skin
  sdr4 = new GCustomSlider(this, 20, 200, 260, 50, "purple18px");
  // show          opaque  ticks value limits
  sdr4.setShowDecor(false, true, true, true);
  sdr4.setNbrTicks(6);
  sdr4.setStickToTicks(true);
  sdr4.setEasing(20);

  //=============================================================
  // Standard slider with labels switched off
  sdr5 = new GCustomSlider(this, 320, 20, 260, 80, null);
  // show          opaque  ticks value limits
  sdr5.setShowDecor(false, true, false, false);
  sdr5.setNumberFormat(GCustomSlider.DECIMAL, 2);
  sdr5.setLimits(0.5f, 0f, 1.0f);
  sdr5.setNbrTicks(6);
  sdr5.setStickToTicks(true);  //false by default 
  // `Stick to ticks' enforces that the handle can only rest 
  // at a tick position.

  //=============================================================
  // This example shows small float numbers used and settings
  // the accuracy of the display labels
  sdr6 = new GCustomSlider(this, 320, 80, 260, 80, null);
  // show          opaque  ticks value limits
  sdr6.setShowDecor(false, true, true, true);
  sdr6.setNumberFormat(G4P.EXPONENT, 1);
  sdr6.setLimits(3E-2f, 2.0E-2f, 3.5E-2f);
  sdr6.setNbrTicks(16); 
  sdr6.setEasing(15);
  sdr6.setStickToTicks(true);

  //=============================================================
  // We can also add custom labels to ticks
  // Note: 
  // setTickLabels() changes the number of ticks previously 
  //                 set with setTickCount() to match the 
  //                 number of labels in the array.
  // setTickCount()  cancels labels that were previously set 
  //                 with setTickLabels()
  String[] tickLabels = new String[] {
    "A", "B", "C", "D", "E"
  };
  sdr7 = new GCustomSlider(this, 320, 160, 260, 70, "green_red20px");
  // show          opaque  ticks value limits
  sdr7.setShowDecor(false, true, false, false);
  sdr7.setTickLabels(tickLabels);
  // notice that we are setting a value that is not 
  // exactly a tick when `stick to tick' is true, 
  // setValue will stick to nearest tick value
}

void draw() {
  background(200, 200, 255);
}

void handleSliderEvents(GSlider slider) {
  println("integer value:" + slider.getValueI() + " float value:" + slider.getValueF());
}
