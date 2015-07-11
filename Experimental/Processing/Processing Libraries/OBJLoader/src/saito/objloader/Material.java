package saito.objloader;

/*
 * Alias .obj loader for processing
 * programmed by Tatsuya SAITO / UCLA Design | Media Arts 
 * Created on 2005/04/17
 *
 * 
 *  
 */

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;

import javax.media.opengl.GL;
import processing.core.PImage;

/**
 * @author tatsuyas
 * 
 */
public class Material {
	// this should be turned back on for OPENGL support
	// public PImage map_Ka;
	public PImage map_Kd;
	// private Texture map_Kd_Gl;
	public float[] Ka;
	public float[] Kd;
	public float[] Ks;
	public float d;

	public String mtlName;

	/**
	 * Constructs a default Material object.
	 */
	public Material() {
		Ka = new float[4];
		Kd = new float[4];
		Ks = new float[4];

		for (int i = 0; i < Ka.length; i++) {
			if (i == 3) {
				Ka[i] = 1f;
				Kd[i] = 1f;
				Ks[i] = 1f;
			} else {
				Ka[i] = 0.5f;
				Kd[i] = 0.5f;
				Ks[i] = 0.5f;
			}
		}

		d = 1.0f;

		mtlName = "default";
	}

	/**
	 * Sets up the OPENGL material as a series of float and int buffers
	 * 
	 * @param gl
	 * @param debug
	 */
	public void setupGL(GL gl, Debug debug) {
		// make magic here that turns a PImage into an OPENGL texture
		if (map_Kd != null) {
			map_Kd.loadPixels();

			IntBuffer glPixels = setupIntBuffer(map_Kd.pixels);

			gl.glGenTextures(1, tex, 0);
			gl.glBindTexture(GL.GL_TEXTURE_2D, tex[0]);
			gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_NEAREST);
			gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_NEAREST);
			gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_WRAP_S, GL.GL_REPEAT);
			gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_WRAP_T, GL.GL_REPEAT);
			gl.glTexImage2D(GL.GL_TEXTURE_2D, 0, GL.GL_RGBA, map_Kd.width, map_Kd.height, 0, GL.GL_BGRA, GL.GL_UNSIGNED_BYTE, glPixels);
		}
	}

	public void beginDrawGL(GL gl, boolean useMaterial, boolean useTexture) {
		if (useMaterial) {
			gl.glMaterialfv(GL.GL_FRONT, GL.GL_AMBIENT, Ka, 0);

			if (Kd[3] == 1)
				gl.glMaterialfv(GL.GL_FRONT, GL.GL_AMBIENT_AND_DIFFUSE, Kd, 0);
			else {
				gl.glMaterialfv(GL.GL_FRONT, GL.GL_AMBIENT_AND_DIFFUSE, Kd, 0);
				gl.glMaterialfv(GL.GL_FRONT, GL.GL_SPECULAR, Ks, 0);
			}

			if (map_Kd != null && useTexture) {
				gl.glBindTexture(GL.GL_TEXTURE_2D, tex[0]);
				gl.glEnable(GL.GL_TEXTURE_2D);
			}
		}
	}

	public void endDrawGL(GL gl, boolean useMaterial, boolean useTexture) {
		if (useMaterial)
			if (map_Kd != null && useTexture)
				gl.glDisable(GL.GL_TEXTURE_2D);
	}

	private IntBuffer setupIntBuffer(int[] i) {
		IntBuffer fb = ByteBuffer.allocateDirect(4 * i.length).order(ByteOrder.nativeOrder()).asIntBuffer();
		fb.put(i);
		fb.rewind();

		return fb;
	}

	protected int[] tex = { 0 };
}
