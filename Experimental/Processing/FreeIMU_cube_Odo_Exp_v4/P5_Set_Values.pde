////////////////////////////////////////////////////////////////
//
//  Setups ControlP5 controllers
//
////////////////////////////////////////////////////////////////

void setValues(){

  cp5.addTextfield("SL Press(mb)")
    .setPosition(50,230)    
    .setSize(100, 30)
    .setAutoClear(false)
    .setVisible(true)
    .setValue(str(sea_press));
 
   //cp5.addToggle("Calib",true, 60, 320, 20,20).setLabel("Calib(on)");
   cp5.addTextfield("Calib(on/off)")
    .setPosition(50,290)
    .setSize(100, 30)
    .setAutoClear(false)
    .setVisible(true)
    .setValue(str(calib));
    
  cp5.addTextfield("Sta Alt(ft)")
    .setPosition(170,230)    
    .setSize(100, 30)
    .setAutoClear(false)
    .setVisible(true)
    .setValue(str(STATIONALTFT));

  cp5.addTextfield("ODO(Y/N)")
    .setPosition(170,290)    
    .setSize(100, 30)
    .setAutoClear(false)
    .setVisible(true)
    .setValue(str(cube_odo));  
    
  cp5.addTextfield("DataFile")
    .setPosition(170,350)    
    .setSize(100, 30)
    .setAutoClear(false)
    .setVisible(true)
    .setValue(str(PrintOutput));     
    
   cp5.addBang("START")
    .setPosition(50, 350)
    .setSize(100, 50)
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER); 
  
}

void controlEvent(ControlEvent theEvent) {
  /*   
  if (theEvent.isFrom("GraphWin")) {   
       runSketch( new String[] { "--display=1",
                            "--location=20,50",
                            "--sketch-path=" + sketchPath(""),
                            "" },
                            rollingGraph = new GraphWin() );
  }
  */
  
  if (theEvent.isFrom("START")) {
    sea_press = float(cp5.get(Textfield.class, "SL Press(mb)").getText());
    calib = int(cp5.get(Textfield.class, "Calib(on/off)").getText());
    cube_odo = int(cp5.get(Textfield.class, "ODO(Y/N)").getText());
    STATIONALTFT = float(cp5.get(Textfield.class, "Sta Alt(ft)").getText());
    PrintOutput = int(cp5.get(Textfield.class, "DataFile").getText());
    cp5.get(Textfield.class, "SL Press(mb)").setVisible(false);
    cp5.get(Textfield.class, "Calib(on/off)").setVisible(false);
    cp5.get(Textfield.class, "ODO(Y/N)").setVisible(false);
    cp5.get(Textfield.class, "Sta Alt(ft)").setVisible(false);
    cp5.get(Textfield.class, "DataFile").setVisible(false);
    cp5.get(Bang.class, "START").setVisible(false);
   
    ArtHorFlg = 1;
  
    }
    
  //println(calib);
  if(sea_press !=1013.25) {
        seapresscmd ="p" + str((int) (sea_press*100));
   } else {
        seapresscmd = "99";
   }
}

/**
 * Click the button to create the windows.
 * @param button
 */
public void handleButtonEvents(GButton button, GEvent event) {
  if (button == btnStart) {
       runSketch( new String[] { "--display=1",
                            "--location=20,50",
                            "--sketch-path=" + sketchPath(""),
                            "" },
                            rollingGraph = new GraphWin() );
  } else if(button == btnExit) {
    exit();
  }
}