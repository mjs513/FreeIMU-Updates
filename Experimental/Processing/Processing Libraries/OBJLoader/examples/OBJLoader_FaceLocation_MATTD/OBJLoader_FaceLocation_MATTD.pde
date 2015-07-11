import saito.objloader.*;

OBJModel model;

float rotX;
float rotY;

float normLength = -25;

PVector pos = new PVector(0,0,0);
PVector dir = new PVector(1,0,0);

AABB volume;

void setup() {
    size(600, 600, P3D);

    model = new OBJModel(this, "cubic_sphere.obj");

    model.scale(3);
    model.translateToCenter();

    volume = new AABB(0,-150,-150, 40,150,150);
}

void draw() {

    pos.add(dir);

    if(pos.x > 200 || pos.x < -200) {

        dir.mult (-1);
    }

    volume.moveCenterTo(pos);


    background(32);
    lights();

    pushMatrix();
    translate(width / 2, height / 2, 0); 

    rotateX(rotY);
    rotateY(rotX);

    volume.draw();

    noStroke();

    for (int j = 0; j < model.getSegmentCount(); j++)
    {

        Segment segment = model.getSegment(j);

        Face[] faces = segment.getFaces();

        beginShape(QUADS);

        for (int i = 0; i < faces.length; i++) {
            Face f = faces[i];

            if(volume.isColliding( f.getCenter() )) {

                PVector[] vs = f.getVertices();
                PVector[] ns = f.getNormals();
                for (int k = 0; k < vs.length; k++) {
                    normal(ns[k].x, ns[k].y, ns[k].z);
                    vertex(vs[k].x, vs[k].y, vs[k].z);
                }
            }
        }

        endShape();
    }
    popMatrix();
}

void mouseDragged() {
    rotX += (mouseX - pmouseX) * 0.01;
    rotY -= (mouseY - pmouseY) * 0.01;
}











