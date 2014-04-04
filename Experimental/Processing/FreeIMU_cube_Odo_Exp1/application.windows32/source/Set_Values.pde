////////////////////////////////////////////////////////////////
//
//  Setups ControlP5 controllers
//
////////////////////////////////////////////////////////////////

void setValues(){

  //input field for orbital radius....
  cp5.addTextfield("SL Press(mb)")
    .setPosition(50,250)
    .setSize(100, 40)
    .setAutoClear(false)
    .setVisible(true)
    .setValue(str(sea_press));
 
   //cp5.addToggle("Calib",true, 60, 320, 20,20).setLabel("Calib(on)");
   cp5.addTextfield("Calib(on/off)")
    .setPosition(50,320)
    .setSize(100, 40)
    .setAutoClear(false)
    .setVisible(true)
    .setValue(str(calib));
    
   cp5.addBang("START")
    .setPosition(50, 380)
    .setSize(80, 40)
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER); 
  
}

void controlEvent(ControlEvent theEvent) {
  if (theEvent.controller().name()=="START") {
    sea_press = float(cp5.get(Textfield.class, "SL Press(mb)").getText());
    calib = int(cp5.get(Textfield.class, "Calib(on/off)").getText());
    ArtHorFlg = 1;
  }
  println(calib);
}

