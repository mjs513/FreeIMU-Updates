class SimpleClass
{

  OBJModel model; 

  SimpleClass(String s, PApplet parent)
  {
    model = new OBJModel(parent, s, OBJModel.ABSOLUTE, QUADS);

    // this is done becaue the model was made in meters 1 = 1m
    // where processing is 1 = 1pixel
    model.scale(100);
    model.translateToCenter();
  }

  void draw()
  {
      noStroke();
    model.draw(); 
  }
}

