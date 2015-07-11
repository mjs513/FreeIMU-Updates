package saito.objloader;

import processing.core.*;

public class BoundingBox implements PConstants {

	PApplet parent;
	
	private PVector min = new PVector(MAX_FLOAT,MAX_FLOAT,MAX_FLOAT);
	private PVector max = new PVector(MIN_FLOAT,MIN_FLOAT,MIN_FLOAT);
	private PVector center = new PVector(0,0,0);
	private PVector whd = new PVector(0,0,0);	
	

	public BoundingBox(PApplet parent, OBJModel model) {

		this.parent = parent;

		model.debug.println("OBJBoundingBox - \tGetting the Bounding Box");

		int numberOfVerts = model.getVertexCount();

		if (numberOfVerts == 0) {
			model.debug.println("OBJBoundingBox - \tThe model has no verts. Have you loaded it yet?");
		} else {

			PVector v;

			for (int i = 0; i < numberOfVerts; i++) {

				v = model.getVertex(i);

				if(v.x < min.x) {min.x = v.x;}
				if(v.x > max.x) {max.x = v.x;}
				if(v.y < min.y) {min.y = v.y;}
				if(v.y > max.y) {max.y = v.y;}
				if(v.z < min.z) {min.z = v.z;}
				if(v.z > max.z) {max.z = v.z;}					
				
			}

			whd.x = (float) Math.sqrt((max.x - min.x) * (max.x - min.x));
			whd.y = (float) Math.sqrt((max.y - min.y) * (max.y - min.y));
			whd.z = (float) Math.sqrt((max.z - min.z) * (max.z - min.z));

			center.add(min);
			center.add(max);
			center.div(2);
			
		}
	}

	public void draw() {

		parent.rectMode(CORNERS);

		parent.pushMatrix();
		parent.translate(center.x,center.y,center.z);

		parent.box(whd.x, whd.y, whd.z);
		parent.popMatrix();

	}

	public PVector getMin() {
		return min;
	}

	public PVector getMax() {
		return max;
	}	
	
	public PVector getCenter() {
		return center;
	}

	public PVector getWHD() {
		return whd;
	}

}
