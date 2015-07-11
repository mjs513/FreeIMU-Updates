import saito.objloader.*;

OBJModel model;

float rotX;
float rotY;

float normLength = -25;

PVector pos;

void setup() {
    size(600, 600, P3D);

    model = new OBJModel(this, "cubic_sphere.obj", "relative", QUADS);

    model.scale(3);
    model.translateToCenter();

    pos = new PVector();

}

void draw() {
    background(32);
    lights();

    pos.x = sin(radians(frameCount)) * 200;
    pos.y = cos(radians(frameCount)) * 200;  

    pushMatrix();

    translate(width / 2, height / 2, 0); 

    rotateX(rotY);
    rotateY(rotX);

    pushMatrix();
    
    drawPoint(pos);

    popMatrix();

    //we have to get the faces out of each segment.
    // a segment is all verts of the one material
    for (int j = 0; j < model.getSegmentCount(); j++) {

        Segment segment = model.getSegment(j);
        Face[] faces = segment.getFaces();

        drawFaces( faces );

        drawNormals( faces );
        
    }
    
    popMatrix();
}


void drawFaces(Face[] fc) {

    // draw faces
    noStroke();

    beginShape(QUADS);

    for (int i = 0; i < fc.length; i++)
    {
        PVector[] vs = fc[i].getVertices();
        PVector[] ns = fc[i].getNormals();

        // if the majority of the face is pointing to the position we draw it.
        if(fc[i].isFacingPosition(pos)) {

            for (int k = 0; k < vs.length; k++) {
                normal(ns[k].x, ns[k].y, ns[k].z);
                vertex(vs[k].x, vs[k].y, vs[k].z);
            }
        }
    }
    endShape();
}



void drawNormals( Face[] fc ) {

    beginShape(LINES);
    // draw face normals
    for (int i = 0; i < fc.length; i++) {
        PVector v = fc[i].getCenter();
        PVector n = fc[i].getNormal();

        // scale the alpha of the stroke by the facing amount.
        // 0.0 = directly facing away
        // 1.0 = directly facing 
        // in truth this is the dot product normalized
        stroke(255, 0, 255, 255.0 * fc[i].getFacingAmount(pos));

        vertex(v.x, v.y, v.z);
        vertex(v.x + (n.x * normLength), v.y + (n.y * normLength), v.z + (n.z * normLength));
    }
    endShape();
}


void drawPoint(PVector p){
 
    translate(p.x, p.y, p.z);

    noStroke();
    ellipse(0,0,20,20);
    rotateX(HALF_PI);
    ellipse(0,0,20,20);
    rotateY(HALF_PI);
    ellipse(0,0,20,20);   
    
}


void mouseDragged() {
    rotX += (mouseX - pmouseX) * 0.01;
    rotY -= (mouseY - pmouseY) * 0.01;
}

