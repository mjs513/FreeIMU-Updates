
/**
 *                     ProScene (version 1.1.1)      
 *    Copyright (c) 2010-2012 by National University of Colombia
 *                 @author Jean Pierre Charalambos      
 *           http://www.disi.unal.edu.co/grupos/remixlab/
 *                           
 * This java package provides classes to ease the creation of interactive 3D
 * scenes in Processing.
 * 
 * This source file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * A copy of the GNU General Public License is available on the World Wide Web
 * at <http://www.gnu.org/copyleft/gpl.html>. You can also obtain it by
 * writing to the Free Software Foundation, 51 Franklin Street, Suite 500
 * Boston, MA 02110-1335, USA.
 */


import processing.core.*;

/**
 * Utility class that complements the PVector and PMatrix classes.
 */
public class MathUtils /**implements PConstants*/ {
	/**
	 * Utility function that simply projects {@code src} on the axis of direction
	 * {@code direction} that passes through the origin.
	 * <p>
	 * {@code direction} does not need to be normalized (but must be non null).
	 */
	public static PVector projectVectorOnAxis(PVector src, PVector direction) {
		float directionSquaredNorm = squaredNorm(direction);
		if (directionSquaredNorm < 1E-10f)
			throw new RuntimeException("Direction squared norm is nearly 0");

		float modulation = src.dot(direction) / directionSquaredNorm;
		return PVector.mult(direction, modulation);
	}

	/**
	 * Utility function that simply projects {@code src} on the plane whose normal
	 * is {@code normal} that passes through the origin.
	 * <p>
	 * {@code normal} does not need to be normalized (but must be non null).
	 */
	public static PVector projectVectorOnPlane(PVector src, PVector normal) {
		float normalSquaredNorm = squaredNorm(normal);
		if (normalSquaredNorm < 1E-10f)
			throw new RuntimeException("Normal squared norm is nearly 0");

		float modulation = src.dot(normal) / normalSquaredNorm;
		return PVector.sub(src, PVector.mult(normal, modulation));
	}

	/**
	 * Utility function that returns the squared norm of the PVector.
	 */
	public static float squaredNorm(PVector v) {
		return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
	}

	/**
	 * Utility function that returns a PVector orthogonal to {@code v}. Its
	 * {@code mag()} depends on the PVector, but is zero only for a {@code null}
	 * PVector. Note that the function that associates an {@code
	 * orthogonalVector()} to a PVector is not continuous.
	 */
	public static PVector orthogonalVector(PVector v) {
		// Find smallest component. Keep equal case for null values.
		if ((PApplet.abs(v.y) >= 0.9f * PApplet.abs(v.x))
				&& (PApplet.abs(v.z) >= 0.9f * PApplet.abs(v.x)))
			return new PVector(0.0f, -v.z, v.y);
		else if ((PApplet.abs(v.x) >= 0.9f * PApplet.abs(v.y))
				&& (PApplet.abs(v.z) >= 0.9f * PApplet.abs(v.y)))
			return new PVector(-v.z, 0.0f, v.x);
		else
			return new PVector(-v.y, v.x, 0.0f);
	}

	/**
	 * Utility function that returns the PMatrix3D representation of the 4x4
	 * {@code m} given in European format.
	 */
	public static final PMatrix3D fromMatrix(float[][] m) {
		return new PMatrix3D(m[0][0], m[0][1], m[0][2], m[0][3], m[1][0], m[1][1],
				m[1][2], m[1][3], m[2][0], m[2][1], m[2][2], m[2][3], m[3][0], m[3][1],
				m[3][2], m[3][3]);
	}

	/**
	 * Utility function that returns the PMatrix3D representation of the 4x4
	 * {@code matrix} given in OpenGL format.
	 */
	public static final PMatrix3D fromOpenGLMatrix(float[][] matrix) {
		return fromMatrix(transpose4x4Matrix(matrix));
	}

	/**
	 * Utility function that returns the PMatrix3D representation of the 16
	 * {@code array} given in European format.
	 */
	public static final PMatrix3D fromArray(float[] a) {
		return new PMatrix3D(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8],
				a[9], a[10], a[11], a[12], a[13], a[14], a[15]);
	}

	/**
	 * Utility function that returns the PMatrix3D representation of the 16
	 * {@code array} given in OpenGL format.
	 */
	public static final PMatrix3D fromOpenGLArray(float[] array) {
		return fromArray(from4x4MatrixToArray(transpose4x4Matrix(fromArrayTo4x4Matrix(array))));
	}

	/**
	 * Utility function that returns the [4][4]float matrix representation
	 * (European format) of the given PMatrix3D.
	 */
	public static final float[][] toMatrix(PMatrix3D pM) {
		float[] array = new float[16];
		pM.get(array);
		return fromArrayTo4x4Matrix(array);
	}

	/**
	 * Utility function that returns the [16]array representation (OpenGL format)
	 * of the given PMatrix3D.
	 */
	public static final float[] toOpenGLArray(PMatrix3D pM) {
		return from4x4MatrixToArray(toOpenGLMatrix(pM));
	}

	/**
	 * Utility function that returns the [4][4]float matrix representation (OpenGL
	 * format) of the given PMatrix3D.
	 */
	public static final float[][] toOpenGLMatrix(PMatrix3D pM) {
		return transpose4x4Matrix(toMatrix(pM));
	}

	/**
	 * Utility function that returns the transpose of the given 3X3 matrix.
	 */
	public static final float[][] transpose3x3Matrix(float[][] m) {
		float[][] matrix = new float[4][4];
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				matrix[i][j] = m[j][i];
		return matrix;
	}

	/**
	 * Utility function that returns the transpose of the given 4X4 matrix.
	 */
	public static final float[][] transpose4x4Matrix(float[][] m) {
		float[][] matrix = new float[4][4];
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				matrix[i][j] = m[j][i];
		return matrix;
	}

	/**
	 * Utility function that returns the [4][4] float matrix version of the given
	 * {@code m} array.
	 */
	public static final float[][] fromArrayTo4x4Matrix(float[] m) {
		// m should be of size [16]
		float[][] mat = new float[4][4];
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				mat[i][j] = m[i * 4 + j];
		return mat;
	}

	/**
	 * Utility function that returns the [16] float array version of the given
	 * {@code mat} matrix.
	 */
	public static final float[] from4x4MatrixToArray(float[][] mat) {
		float[] m = new float[16];
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				m[i * 4 + j] = mat[i][j];
		return m;
	}

	/**
	 * Utility function that returns the 3x3 upper left sub-matrix of the given
	 * PMatrix3D.
	 */
	public static final float[][] get3x3UpperLeftMatrixFromPMatrix3D(PMatrix3D pM) {
		float[][] m = new float[3][3];
		m[0][0] = pM.m00;
		m[0][1] = pM.m01;
		m[0][2] = pM.m02;
		m[1][0] = pM.m10;
		m[1][1] = pM.m11;
		m[1][2] = pM.m12;
		m[2][0] = pM.m20;
		m[2][1] = pM.m21;
		m[2][2] = pM.m22;
		return m;
	}	
}
