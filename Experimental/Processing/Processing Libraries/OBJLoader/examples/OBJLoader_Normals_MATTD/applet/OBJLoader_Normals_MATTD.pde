import saito.objloader.*;


// declare that we need a OBJModel and we'll be calling it "model"
OBJModel model;
float rotX;
float rotY;



void setup()
{
    size(600, 600, P3D);

    // making an object called "model" that is a new instance of OBJModel
    model = new OBJModel(this, "cubic_sphere.obj", "relative", QUADS);

    // turning on the debug output (it's all the stuff that spews out in the black box down the bottom)
    model.enableDebug();

    model.scale(3);  

    noStroke();
}

void draw()
{
    background(32);
    lights();

    pushMatrix();
    translate(width/2, height/2, 0);
    rotateX(rotY);
    rotateY(rotX);


   // model.draw();

    for (int j = 0; j < model.getSegmentCount(); j++) {

        Segment segment = model.getSegment(j);
        Face[] faces = segment.getFaces();

        noFill();
        stroke(196, 64);

        beginShape(QUADS);

        for(int i = 0; i < faces.length; i ++)
        {
            PVector[] vs = faces[i].getVertices();
            PVector[] ns = faces[i].getNormals();

            for (int k = 0; k < vs.length; k++) {
                normal(ns[k].x, ns[k].y, ns[k].z);
                vertex(vs[k].x, vs[k].y, vs[k].z);
            }
        }

        endShape();

        // the face projected along the center normal    
        fill(255,0,255);
        noStroke();    

        beginShape(QUADS);

        for(int i = 0; i < faces.length; i ++)
        {
            PVector[] v = faces[i].getVertices();
            PVector n = faces[i].getNormal();
            
            float nor = abs(sin(radians((frameCount+i))) * 100);

            for (int k = 0; k < v.length; k++) {
                
                vertex(v[k].x + (n.x*nor), v[k].y + (n.y*nor), v[k].z + (n.z*nor));
            }
        }

       
        endShape();
    }

    popMatrix();
}


void mouseDragged()
{
    rotX += (mouseX - pmouseX) * 0.01;
    rotY -= (mouseY - pmouseY) * 0.01;
}

