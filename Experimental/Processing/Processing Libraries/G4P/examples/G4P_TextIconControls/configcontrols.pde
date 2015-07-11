  // The code in this tab is used to create all the other
  // controls needed to configure the text+icon controls.
  
  GToggleGroup tgStyle = new GToggleGroup();
  GOption optPlain, optBold, optItalic, optBoldItalic;
  GCheckbox cbxOpaque;
  GSlider sdrBack;
  GLabel lblTestStyle, lblVisible, lblNbrTicks, lblEasing;
  GLabel lblTextA, lblIconA;
  GKnob knbAngle;
  GButton[] btnColours = new GButton[8];
  GDropList textH, textV, iconH, iconV;

  public void makeTextIconConfigControls(){
    // Create colour scheme selectors
    int x = width - 42, y = 2;
    for(int i = 0; i < btnColours.length; i++){
      btnColours[i] = new GButton(this, x, y + i * 20, 40, 18, "" + (i+1));
      btnColours[i].tag = "Button: " + (i+1);
      btnColours[i].setLocalColorScheme(i);
      btnColours[i].tagNo = 1000+i;
    }  
    // Create sliders
    x = width-100; y = 162;
    sdrBack = new GSlider(this, x, y, 162, 80, 12);
    sdrBack.setLimits(bgcol, 0, 255);
    sdrBack.setRotation(-PI/2);
    sdrBack.setTextOrientation(G4P.ORIENT_RIGHT);
    sdrBack.setEasing(20);
    sdrBack.setShowValue(true);
    sdrBack.setShowTicks(true);
    
    x = width - 180; y = 2;
    lblVisible = new GLabel(this, x, y, 80, 18, "Visible");
    lblVisible.setTextBold();
    cbxOpaque = new GCheckbox(this, x, y + 20 , 70, 18, "Opaque");
  
    x = width - 180; y = 52;
    lblTestStyle = new GLabel(this, x, y, 80, 18, "Text Style");
    lblTestStyle.setTextBold();
    optPlain = new GOption(this, x, y + 20, 80, 18, "Plain");
    optBold = new GOption(this, x, y + 40, 80, 18, "Bold");
    optItalic = new GOption(this, x, y + 60, 80, 18, "Italic");
    optBoldItalic = new GOption(this, x, y + 80, 100, 18, "Bold-Italic");
    
    tgStyle.addControls(optPlain, optBold, optItalic, optBoldItalic);
    optPlain.setSelected(true);

    // Allignment droplists
    String[] items;  
    x = width - 180; y = 170;
    lblTextA = new GLabel(this, x, y, 80, 38, "Text Alignment");
    lblTextA.setTextBold();
    
    items = new String[] { "LEFT", "CENTER", "RIGHT", "JUSTIFY"};
    textH = new GDropList(this, x, y + 40, 80, 90, 4);
    textH.setItems(items, 0);
    textH.tag = "textH";
    
    items = new String[] { "TOP", "MIDDLE", "BOTTOM"};
    textV = new GDropList(this, x, y + 60, 80, 90, 4);
    textV.setItems(items, 1);
    textV.tag = "textV";

    x = width - 90; y = 170;
    lblIconA = new GLabel(this, x, y, 80, 38, "Icon Alignment");
    lblIconA.setTextBold();
    
    items = new String[] { "LEFT", "RIGHT"};
    iconH = new GDropList(this, x, y + 40, 80, 90, 4);
    iconH.setItems(items, 0);
    iconH.tag = "iconH";
    
    items = new String[] { "TOP", "MIDDLE", "BOTTOM"};
    iconV = new GDropList(this, x, y + 60, 80, 90, 4);
    iconV.setItems(items, 1);
    iconV.tag = "iconV";

    x = width - 130; y = height - 80;
    knbAngle = new GKnob(this, x, y, 70, 70, 0.6f);
    knbAngle.setTurnRange(0, 360);
    knbAngle.setLimits(0.0f, 0.0f, TWO_PI);
    knbAngle.setTurnMode(G4P.CTRL_ANGULAR);
    knbAngle.setIncludeOverBezel(true);
    knbAngle.setNbrTicks(13);
    knbAngle.setStickToTicks(true);    
  }
