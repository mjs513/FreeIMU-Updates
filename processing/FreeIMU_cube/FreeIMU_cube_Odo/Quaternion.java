
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
 * A 4 element unit quaternion represented by single precision floating point
 * x,y,z,w coordinates.
 * 
 */

public class Quaternion extends PApplet {
	/**
	 * The x coordinate, i.e., the x coordinate of the vector part of the
	 * Quaternion.
	 */
	public float x;

	/**
	 * The y coordinate, i.e., the y coordinate of the vector part of the
	 * Quaternion.
	 */
	public float y;

	/**
	 * The z coordinate, i.e., the z coordinate of the vector part of the
	 * Quaternion.
	 */
	public float z;

	/**
	 * The w coordinate which corresponds to the scalar part of the Quaternion.
	 */
	public float w;

	/**
	 * Constructs and initializes a Quaternion to (0.0,0.0,0.0,1.0), i.e., an
	 * identity rotation.
	 */
	public Quaternion () {
		this.x = 0;
		this.y = 0;
		this.z = 0;
		this.w = 1;
	}

	/**
	 * Default constructor for Quaternion(float x, float y, float z, float w,
	 * boolean normalize), with {@code normalize=true}.
	 * 
	 */
	public Quaternion (float x, float y, float z, float w) {
		this(x, y, z, w, true);
	}

	/**
	 * Constructs and initializes a Quaternion from the specified xyzw
	 * coordinates.
	 * 
	 * @param x
	 *          the x coordinate
	 * @param y
	 *          the y coordinate
	 * @param z
	 *          the z coordinate
	 * @param w
	 *          the w scalar component
	 * @param normalize
	 *          tells whether or not the constructed Quaternion should be
	 *          normalized.
	 */
	public Quaternion(float x, float y, float z, float w, boolean normalize) {
		if (normalize) {
			float mag = PApplet.sqrt(x * x + y * y + z * z + w * w);
			if (mag > 0.0f) {
				this.x = x / mag;
				this.y = y / mag;
				this.z = z / mag;
				this.w = w / mag;
			} else {
				this.x = 0f;
				this.y = 0f;
				this.z = 0f;
				this.w = 1f;
			}
		} else {
			this.x = x;
			this.y = y;
			this.z = z;
			this.w = w;
		}
	}

	/**
	 * Default constructor for Quaternion(float[] q, boolean normalize) with
	 * {@code normalize=true}.
	 * 
	 */
	public Quaternion(float[] q) {
		this(q, true);
	}

	/**
	 * Constructs and initializes a Quaternion from the array of length 4.
	 * 
	 * @param q
	 *          the array of length 4 containing xyzw in order
	 */
	public Quaternion(float[] q, boolean normalize) {
		if (normalize) {
			float mag = PApplet.sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3]
					* q[3]);
			if (mag > 0.0f) {
				this.x = q[0] / mag;
				this.y = q[1] / mag;
				this.z = q[2] / mag;
				this.w = q[3] / mag;
			} else {
				this.x = 0;
				this.y = 0;
				this.z = 0;
				this.w = 1;
			}
		} else {
			this.x = q[0];
			this.y = q[1];
			this.z = q[2];
			this.w = q[3];
		}
	}

	/**
	 * Copy constructor.
	 * 
	 * @param q1
	 *          the Quaternion containing the initialization x y z w data
	 */
	public Quaternion(Quaternion q1) {
		set(q1);
	}

	/**
	 * Copy constructor. If {@code normalize} is {@code true} this Quaternion is
	 * {@link #normalize()}.
	 * 
	 * @param q1
	 *          the Quaternion containing the initialization x y z w data
	 */
	public Quaternion(Quaternion q1, boolean normalize) {
		set(q1, normalize);
	}

	/**
	 * Convenience function that simply calls {@code set(q1, true);}
	 * 
	 * @see #set(Quaternion, boolean)
	 */
	public void set(Quaternion q1) {
		set(q1, true);
	}

	/**
	 * Set this Quaternion from quaternion {@code q1}. If {@code normalize} is
	 * {@code true} this Quaternion is {@link #normalize()}.
	 */
	public void set(Quaternion q1, boolean normalize) {
		this.x = q1.x;
		this.y = q1.y;
		this.z = q1.z;
		this.w = q1.w;
		if (normalize)
			this.normalize();
	}

	/**
	 * Constructs and initializes a Quaternion from the specified rotation
	 * {@link #axis() axis} (non null) and {@link #angle() angle} (in radians).
	 * 
	 * @param axis
	 *          the PVector representing the axis
	 * @param angle
	 *          the angle in radians
	 * 
	 * @see #fromAxisAngle(PVector, float)
	 */
	public Quaternion(PVector axis, float angle) {
		fromAxisAngle(axis, angle);
	}

	/**
	 * Constructs a Quaternion that will rotate from the {@code from} direction to
	 * the {@code to} direction.
	 * 
	 * @param from
	 *          the first PVector
	 * @param to
	 *          the second PVector
	 * 
	 * @see #fromTo(PVector, PVector)
	 */
	public Quaternion(PVector from, PVector to) {
		fromTo(from, to);
	}

	/**
	 * Sets the value of this Quaternion to the conjugate of itself.
	 */
	public final void conjugate() {
		this.x = -this.x;
		this.y = -this.y;
		this.z = -this.z;
	}

	/**
	 * Sets the value of this Quaternion to the conjugate of Quaternion q1.
	 * 
	 * @param q1
	 *          the source vector
	 */
	public final void conjugate(Quaternion q1) {
		this.x = -q1.x;
		this.y = -q1.y;
		this.z = -q1.z;
		this.w = q1.w;
	}

	/**
	 * Negates all the coefficients of the Quaternion.
	 */
	public final void negate() {
		this.x = -this.x;
		this.y = -this.y;
		this.z = -this.z;
		this.w = -this.w;
	}

	/**
	 * Returns the "dot" product of this Quaternion and {@code b}:
	 * <p>
	 * {@code this.x * b.x + this.y * b.y + this.z * b.z + this.w * b.w}
	 * 
	 * @param b
	 *          the Quaternion
	 */
	public final float dotProduct(Quaternion b) {
		return this.x * b.x + this.y * b.y + this.z * b.z + this.w * b.w;
	}

	/**
	 * Returns the "dot" product of {@code a} and {@code b}:
	 * <p>
	 * {@code a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w}
	 * 
	 * @param a
	 *          the first Quaternion
	 * @param b
	 *          the second Quaternion
	 */
	public final static float dotProduct(Quaternion a, Quaternion b) {
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	/**
	 * Sets the value of this Quaternion to the Quaternion product of itself and
	 * {@code q1}, (i.e., {@code this = this * q1}).
	 * 
	 * @param q1
	 *          the other Quaternion
	 */
	public final void multiply(Quaternion q1) {
		float x, y, w;

		w = this.w * q1.w - this.x * q1.x - this.y * q1.y - this.z * q1.z;
		x = this.w * q1.x + q1.w * this.x + this.y * q1.z - this.z * q1.y;
		y = this.w * q1.y + q1.w * this.y - this.x * q1.z + this.z * q1.x;
		this.z = this.w * q1.z + q1.w * this.z + this.x * q1.y - this.y * q1.x;
		this.w = w;
		this.x = x;
		this.y = y;
	}

	/**
	 * Returns the Quaternion which is product of quaternions {@code q1} and
	 * {@code q2}.
	 * 
	 * @param q1
	 *          the first Quaternion
	 * @param q2
	 *          the second Quaternion
	 */
	public final static Quaternion multiply(Quaternion q1, Quaternion q2) {
		float x, y, z, w;
		w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
		x = q1.w * q2.x + q2.w * q1.x + q1.y * q2.z - q1.z * q2.y;
		y = q1.w * q2.y + q2.w * q1.y - q1.x * q2.z + q1.z * q2.x;
		z = q1.w * q2.z + q2.w * q1.z + q1.x * q2.y - q1.y * q2.x;
		return new Quaternion(x, y, z, w);
	}

	/**
	 * Returns the image of {@code v} by the rotation of this vector. Same as
	 * {@code this.rotate(v).}
	 * 
	 * @param v
	 *          the PVector
	 * 
	 * @see #rotate(PVector)
	 * @see #inverseRotate(PVector)
	 */
	public final PVector multiply(PVector v) {
		return this.rotate(v);
	}

	/**
	 * Returns the image of {@code v} by the rotation {@code q1}. Same as {@code
	 * q1.rotate(v).}
	 * 
	 * @param q1
	 *          the Quaternion
	 * 
	 * @param v
	 *          the PVector
	 * 
	 * @see #rotate(PVector)
	 * @see #inverseRotate(PVector)
	 */
	public static final PVector multiply(Quaternion q1, PVector v) {
		return q1.rotate(v);
	}

	/**
	 * Multiplies this Quaternion by the inverse of Quaternion {@code q1} and
	 * places the value into this Quaternion (i.e., {@code this = this * q^-1}).
	 * The value of the argument Quaternion is preserved.
	 * 
	 * @param q1
	 *          the other Quaternion
	 */
	public final void multiplyInverse(Quaternion q1) {
		Quaternion tempQuat = new Quaternion(q1);
		tempQuat.invert();
		this.multiply(tempQuat);
	}

	/**
	 * Returns the product of Quaternion {@code q1} by the inverse of Quaternion
	 * {@code q2} (i.e., {@code q1 * q2^-1}). The value of both argument
	 * quaternions is preserved.
	 * 
	 * @param q1
	 *          the first Quaternion
	 * @param q2
	 *          the second Quaternion
	 */
	public static final Quaternion multiplyInverse(Quaternion q1, Quaternion q2) {
		Quaternion tempQuat = new Quaternion(q2);
		tempQuat.invert();
		return Quaternion.multiply(q1, tempQuat);
	}

	/**
	 * Returns the inverse Quaternion (inverse rotation).
	 * <p>
	 * The result has a negated {@link #axis()} direction and the same
	 * {@link #angle()}.
	 * <p>
	 * A composition of a Quaternion and its {@link #inverse()} results in an
	 * identity function. Use {@link #invert()} to actually modify the Quaternion.
	 * 
	 * @see #invert()
	 */
	public final Quaternion inverse() {
		Quaternion tempQuat = new Quaternion(this);
		tempQuat.invert();
		return tempQuat;
	}

	/**
	 * Sets the value of this Quaternion to the inverse of itself.
	 * 
	 * @see #inverse()
	 */
	public final void invert() {
		float sqNorm = squaredNorm(this);
		this.w /= sqNorm;
		this.x /= -sqNorm;
		this.y /= -sqNorm;
		this.z /= -sqNorm;
	}

	/**
	 * Sets the value of this Quaternion to the Quaternion inverse of {@code q1}.
	 * 
	 * @param q1
	 *          the Quaternion to be inverted
	 */
	public final void invert(Quaternion q1) {
		float sqNorm = squaredNorm(q1);
		this.w = q1.w / sqNorm;
		this.x = -q1.x / sqNorm;
		this.y = -q1.y / sqNorm;
		this.z = -q1.z / sqNorm;
	}

	/**
	 * Normalizes the value of this Quaternion in place and return its {@code
	 * norm}.
	 */
	public final float normalize() {
		float norm = PApplet.sqrt(this.x * this.x + this.y * this.y + this.z
				* this.z + this.w * this.w);
		if (norm > 0.0f) {
			this.x /= norm;
			this.y /= norm;
			this.z /= norm;
			this.w /= norm;
		} else {
			this.x = (float) 0.0;
			this.y = (float) 0.0;
			this.z = (float) 0.0;
			this.w = (float) 1.0;
		}
		return norm;
	}

	/**
	 * Returns the image of {@code v} by the Quaternion rotation.
	 * 
	 * @param v
	 *          the PVector
	 */
	public final PVector rotate(PVector v) {
		float q00 = 2.0f * x * x;
		float q11 = 2.0f * y * y;
		float q22 = 2.0f * z * z;

		float q01 = 2.0f * x * y;
		float q02 = 2.0f * x * z;
		float q03 = 2.0f * x * w;

		float q12 = 2.0f * y * z;
		float q13 = 2.0f * y * w;

		float q23 = 2.0f * z * w;

		return new PVector((1.0f - q11 - q22) * v.x + (q01 - q23) * v.y
				+ (q02 + q13) * v.z, (q01 + q23) * v.x + (1.0f - q22 - q00) * v.y
				+ (q12 - q03) * v.z, (q02 - q13) * v.x + (q12 + q03) * v.y
				+ (1.0f - q11 - q00) * v.z);
	}

	/**
	 * Returns the image of {@code v} by the Quaternion {@link #inverse()}
	 * rotation.
	 * <p>
	 * {@link #rotate(PVector)} performs an inverse transformation.
	 * 
	 * @param v
	 *          the PVector
	 */
	public final PVector inverseRotate(PVector v) {
		Quaternion tempQuat = new Quaternion(x, y, z, w);
		tempQuat.invert();
		return tempQuat.rotate(v);
	}

	/**
	 * Sets the Quaternion as a rotation of {@link #axis() axis} and
	 * {@link #angle() angle} (in radians).
	 * <p>
	 * The {@code axis} does not need to be normalized. A null {@code axis} will
	 * result in an identity Quaternion.
	 * 
	 * @param axis
	 *          the PVector representing the axis
	 * @param angle
	 *          the angle in radians
	 */
	public void fromAxisAngle(PVector axis, float angle) {
		float norm = axis.mag();
		if (norm < 1E-8f) {
			// Null rotation
			this.x = 0.0f;
			this.y = 0.0f;
			this.z = 0.0f;
			this.w = 1.0f;
		} else {
			float sin_half_angle = PApplet.sin(angle / 2.0f);
			this.x = sin_half_angle * axis.x / norm;
			this.y = sin_half_angle * axis.y / norm;
			this.z = sin_half_angle * axis.z / norm;
			this.w = PApplet.cos(angle / 2.0f);
		}
	}

	/**
	 * Same as {@link #fromEulerAngles(PVector)}.
	 */
	public void fromTaitBryan(PVector angles) {
		fromEulerAngles(angles);
	}

	/**
	 * Same as {@link #fromEulerAngles(float, float, float)}.
	 */
	public void fromTaitBryan(float roll, float pitch, float yaw) {
		fromEulerAngles(roll, pitch, yaw);
	}

	/**
	 * Convenience function that simply calls {@code fromEulerAngles(angles.x,
	 * angles.y, angles.z)}.
	 * 
	 * @see #fromEulerAngles(float, float, float)
	 * @see #eulerAngles()
	 */
	public void fromEulerAngles(PVector angles) {
		fromEulerAngles(angles.x, angles.y, angles.z);
	}

	/**
	 * Converts Euler rotation angles {@code roll}, {@code pitch} and {@code yaw},
	 * respectively defined to the x, y and z axes, to this Quaternion. In the
	 * convention used here these angles represent a composition of extrinsic
	 * rotations (rotations about the reference frame axes), which is also known
	 * as {@link #taitBryanAngles()} (See
	 * http://en.wikipedia.org/wiki/Euler_angles and
	 * http://en.wikipedia.org/wiki/Tait-Bryan_angles). {@link #eulerAngles()}
	 * performs the inverse operation.
	 * <p>
	 * Each rotation angle is converted to an axis-angle pair, with the axis
	 * corresponding to one of the Euclidean axes. The axis-angle pairs are
	 * converted to quaternions and multiplied together. The order of the
	 * rotations is: y->z->x which follows the convention found here:
	 * http://www.euclideanspace.com/maths/geometry/rotations/euler/index.htm.
	 * 
	 * @see #eulerAngles()
	 */
	public void fromEulerAngles(float roll, float pitch, float yaw) {
		Quaternion qx = new Quaternion(new PVector(1, 0, 0), roll);
		Quaternion qy = new Quaternion(new PVector(0, 1, 0), pitch);
		Quaternion qz = new Quaternion(new PVector(0, 0, 1), yaw);
		set(qy);
		multiply(qz);
		multiply(qx);
	}

	/**
	 * Same as {@link #eulerAngles()}.
	 */
	public PVector taitBryanAngles() {
		return eulerAngles();
	}

	/**
	 * Converts this Quaternion to Euler rotation angles {@code roll}, {@code
	 * pitch} and {@code yaw} in radians.
	 * {@link #fromEulerAngles(float, float, float)} performs the inverse
	 * operation. The code was adapted from:
	 * http://www.euclideanspace.com/maths/geometry
	 * /rotations/conversions/quaternionToEuler/index.htm.
	 * <p>
	 * <b>Attention:</b> This method assumes that this Quaternion is normalized.
	 * 
	 * @return the PVector holding the roll (x coordinate of the vector), pitch (y
	 *         coordinate of the vector) and yaw angles (z coordinate of the
	 *         vector). <b>Note:</b> The order of the rotations that would produce
	 *         this Quaternion (i.e., as with {@code fromEulerAngles(roll, pitch,
	 *         yaw)}) is: y->z->x.
	 * 
	 * @see #fromEulerAngles(float, float, float)
	 */
	public PVector eulerAngles() {
		float roll, pitch, yaw;
		float test = x * y + z * w;
		if (test > 0.499) { // singularity at north pole
			pitch = 2 * PApplet.atan2(x, w);
			yaw = PApplet.PI / 2;
			roll = 0;
			return new PVector(roll, pitch, yaw);
		}
		if (test < -0.499) { // singularity at south pole
			pitch = -2 * PApplet.atan2(x, w);
			yaw = -PApplet.PI / 2;
			roll = 0;
			return new PVector(roll, pitch, yaw);
		}
		float sqx = x * x;
		float sqy = y * y;
		float sqz = z * z;
		pitch = PApplet.atan2(2 * y * w - 2 * x * z, 1 - 2 * sqy - 2 * sqz);
		yaw = PApplet.asin(2 * test);
		roll = PApplet.atan2(2 * x * w - 2 * y * z, 1 - 2 * sqx - 2 * sqz);
		return new PVector(roll, pitch, yaw);
	}

	/**
	 * public PVector eulerAngles() { //This quaternion does not need to be
	 * normalized. See:
	 * //http://www.euclideanspace.com/maths/geometry/rotations/conversions
	 * /quaternionToEuler/index.htm float roll, pitch, yaw; float sqw = w*w; float
	 * sqx = x*x; float sqy = y*y; float sqz = z*z; float unit = sqx + sqy + sqz +
	 * sqw; // if normalised is one, otherwise is correction factor float test =
	 * x*y + z*w; if (test > 0.499*unit) { // singularity at north pole pitch = 2
	 * * PApplet.atan2(x,w); yaw = PApplet.PI/2; roll = 0; return new
	 * PVector(roll, pitch, yaw); } if (test < -0.499*unit) { // singularity at
	 * south pole pitch = -2 * PApplet.atan2(x,w); yaw = - PApplet.PI/2; roll = 0;
	 * return new PVector(roll, pitch, yaw); } pitch = PApplet.atan2(2*y*w-2*x*z ,
	 * sqx - sqy - sqz + sqw); yaw = PApplet.asin(2*test/unit); roll =
	 * PApplet.atan2(2*x*w-2*y*z , -sqx + sqy - sqz + sqw); return new
	 * PVector(roll, pitch, yaw); } //
	 */

	/**
	 * Sets the Quaternion as a rotation from the {@code from} direction to the
	 * {@code to} direction.
	 * <p>
	 * <b>Attention:</b> this rotation is not uniquely defined. The selected axis
	 * is usually orthogonal to {@code from} and {@code to}, minimizing the
	 * rotation angle. This method is robust and can handle small or almost
	 * identical vectors.
	 * 
	 * @see #fromAxisAngle(PVector, float)
	 */
	public void fromTo(PVector from, PVector to) {
		float fromSqNorm = MathUtils.squaredNorm(from);
		float toSqNorm = MathUtils.squaredNorm(to);
		// Identity Quaternion when one vector is null
		if ((fromSqNorm < 1E-10f) || (toSqNorm < 1E-10f)) {
			this.x = this.y = this.z = 0.0f;
			this.w = 1.0f;
		} else {

			PVector axis = from.cross(to);

			float axisSqNorm = MathUtils.squaredNorm(axis);

			// Aligned vectors, pick any axis, not aligned with from or to
			if (axisSqNorm < 1E-10f)
				axis = MathUtils.orthogonalVector(from);

			float angle = PApplet.asin(PApplet.sqrt(axisSqNorm
					/ (fromSqNorm * toSqNorm)));

			if (from.dot(to) < 0.0)
				angle = PI - angle;

			fromAxisAngle(axis, angle);
		}
	}

	/**
	 * Set the Quaternion from a (supposedly correct) 3x3 rotation matrix.
	 * <p>
	 * The matrix is expressed in European format: its three columns are the
	 * images by the rotation of the three vectors of an orthogonal basis.
	 * <p>
	 * {@link #fromRotatedBasis(PVector, PVector, PVector)} sets a Quaternion from
	 * the three axis of a rotated frame. It actually fills the three columns of a
	 * matrix with these rotated basis vectors and calls this method.
	 * 
	 * @param m
	 *          the 3*3 matrix of float values
	 */
	public final void fromRotationMatrix(float m[][]) {
		// Compute one plus the trace of the matrix
		float onePlusTrace = 1.0f + m[0][0] + m[1][1] + m[2][2];

		if (onePlusTrace > 1E-5f) {
			// Direct computation
			float s = PApplet.sqrt(onePlusTrace) * 2.0f;
			this.x = (m[2][1] - m[1][2]) / s;
			this.y = (m[0][2] - m[2][0]) / s;
			this.z = (m[1][0] - m[0][1]) / s;
			this.w = 0.25f * s;
		} else {
			// Computation depends on major diagonal term
			if ((m[0][0] > m[1][1]) & (m[0][0] > m[2][2])) {
				float s = PApplet.sqrt(1.0f + m[0][0] - m[1][1] - m[2][2]) * 2.0f;
				this.x = 0.25f * s;
				this.y = (m[0][1] + m[1][0]) / s;
				this.z = (m[0][2] + m[2][0]) / s;
				this.w = (m[1][2] - m[2][1]) / s;
			} else if (m[1][1] > m[2][2]) {
				float s = PApplet.sqrt(1.0f + m[1][1] - m[0][0] - m[2][2]) * 2.0f;
				this.x = (m[0][1] + m[1][0]) / s;
				this.y = 0.25f * s;
				this.z = (m[1][2] + m[2][1]) / s;
				this.w = (m[0][2] - m[2][0]) / s;
			} else {
				float s = PApplet.sqrt(1.0f + m[2][2] - m[0][0] - m[1][1]) * 2.0f;
				this.x = (m[0][2] + m[2][0]) / s;
				this.y = (m[1][2] + m[2][1]) / s;
				this.z = 0.25f * s;
				this.w = (m[0][1] - m[1][0]) / s;
			}
		}
		normalize();
	}

	/**
	 * Set the Quaternion from a (supposedly correct) 3x3 rotation matrix given in
	 * the upper left 3x3 sub-matrix of the PMatrix3D.
	 * 
	 * @see #fromRotationMatrix(float[][])
	 */
	public final void fromMatrix(PMatrix3D pM) {
		fromRotationMatrix(MathUtils.get3x3UpperLeftMatrixFromPMatrix3D(pM));
	}

	/**
	 * Sets the Quaternion from the three rotated vectors of an orthogonal basis.
	 * <p>
	 * The three vectors do not have to be normalized but must be orthogonal and
	 * direct (i,e., {@code X^Y=k*Z, with k>0}).
	 * 
	 * @param X
	 *          the first PVector
	 * @param Y
	 *          the second PVector
	 * @param Z
	 *          the third PVector
	 * 
	 * @see #fromRotationMatrix(float[][])
	 * @see #Quaternion(PVector, PVector)
	 * 
	 */
	public final void fromRotatedBasis(PVector X, PVector Y, PVector Z) {
		float m[][] = new float[3][3];
		float normX = X.mag();
		float normY = Y.mag();
		float normZ = Z.mag();

		for (int i = 0; i < 3; ++i) {
			m[i][0] = (X.array())[i] / normX;
			m[i][1] = (Y.array())[i] / normY;
			m[i][2] = (Z.array())[i] / normZ;
		}

		fromRotationMatrix(m);
	}

	/**
	 * Returns the normalized axis direction of the rotation represented by the
	 * Quaternion.
	 * <p>
	 * The result is null for an identity Quaternion.
	 * 
	 * @see #angle()
	 */
	public final PVector axis() {
		PVector res = new PVector(this.x, this.y, this.z);
		float sinus = res.mag();
		if (sinus > 1E-8f)
			res.div(sinus);
		if (PApplet.acos(this.w) <= HALF_PI)
			return res;
		else {
			res.x = -res.x;
			res.y = -res.y;
			res.z = -res.z;
			return res;
		}
	}

	/**
	 * Returns the {@code angle} (in radians) of the rotation represented by the
	 * Quaternion.
	 * <p>
	 * This value is always in the range {@code [0-pi]}. Larger rotational angles
	 * are obtained by inverting the {@link #axis()} direction.
	 * 
	 * @see #axis()
	 */
	public final float angle() {
		float angle = 2.0f * PApplet.acos(this.w);
		return (angle <= PI) ? angle : 2.0f * PI - angle;
	}

	/**
	 * Returns the 3x3 rotation matrix associated with the Quaternion.
	 * <p>
	 * <b>Attention:</b> The method returns the European mathematical
	 * representation of the rotation matrix.
	 * 
	 * @see #inverseRotationMatrix()
	 * 
	 */
	public final float[][] rotationMatrix() {
		return MathUtils.get3x3UpperLeftMatrixFromPMatrix3D(matrix());
	}

	/**
	 * Returns the PMatrix3D (processing matrix) which represents the rotation
	 * matrix associated with the Quaternion.
	 * 
	 * @see #rotationMatrix()
	 */
	public final PMatrix3D matrix() {

		float q00 = 2.0f * this.x * this.x;
		float q11 = 2.0f * this.y * this.y;
		float q22 = 2.0f * this.z * this.z;

		float q01 = 2.0f * this.x * this.y;
		float q02 = 2.0f * this.x * this.z;
		float q03 = 2.0f * this.x * this.w;

		float q12 = 2.0f * this.y * this.z;
		float q13 = 2.0f * this.y * this.w;

		float q23 = 2.0f * this.z * this.w;

		float m00 = 1.0f - q11 - q22;
		float m01 = q01 - q23;
		float m02 = q02 + q13;

		float m10 = q01 + q23;
		float m11 = 1.0f - q22 - q00;
		float m12 = q12 - q03;

		float m20 = q02 - q13;
		float m21 = q12 + q03;
		float m22 = 1.0f - q11 - q00;

		float m30 = 0.0f;
		float m31 = 0.0f;
		float m32 = 0.0f;

		float m03 = 0.0f;
		float m13 = 0.0f;
		float m23 = 0.0f;
		float m33 = 1.0f;

		return new PMatrix3D(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22,
				m23, m30, m31, m32, m33);
	}

	/**
	 * Returns the associated inverse rotation processing PMatrix3D. This is
	 * simply {@link #matrix()} of the {@link #inverse()}.
	 * <p>
	 * <b>Attention:</b> The result is only valid until the next call to
	 * {@link #inverseMatrix()}. Use it immediately (as in {@code
	 * applyMatrix(q.inverseMatrix())}).
	 */
	public final PMatrix3D inverseMatrix() {
		Quaternion tempQuat = new Quaternion(x, y, z, w);
		tempQuat.invert();
		return tempQuat.matrix();
	}

	/**
	 * Returns the 3x3 inverse rotation matrix associated with the Quaternion.
	 * <p>
	 * <b>Attention:</b> This is the classical mathematical rotation matrix.
	 */
	public final float[][] inverseRotationMatrix() {
		return MathUtils.get3x3UpperLeftMatrixFromPMatrix3D(inverseMatrix());
	}

	/**
	 * Returns the logarithm of the Quaternion.
	 * 
	 * @see #exp()
	 */
	public final Quaternion log() {
		// Warning: this method should not normalize the Quaternion
		float len = PApplet.sqrt(this.x * this.x + this.y * this.y + this.z
				* this.z);

		if (len < 1E-6f)
			return new Quaternion(this.x, this.y, this.z, 0.0f, false);
		else {
			float coef = PApplet.acos(this.w) / len;
			return new Quaternion(this.x * coef, this.y * coef, this.z * coef, 0.0f,
					false);
		}
	}

	/**
	 * Returns the exponential of the Quaternion.
	 * 
	 * @see #log()
	 */
	public final Quaternion exp() {
		float theta = PApplet.sqrt(this.x * this.x + this.y * this.y + this.z
				* this.z);

		if (theta < 1E-6f)
			return new Quaternion(this.x, this.y, this.z, PApplet.cos(theta));
		else {
			float coef = PApplet.sin(theta) / theta;
			return new Quaternion(this.x * coef, this.y * coef, this.z * coef,
					PApplet.cos(theta));
		}
	}

	/**
	 * Returns a random unit Quaternion.
	 * <p>
	 * You can create a randomly directed unit vector using:
	 * <p>
	 * {@code PVector randomDir = new PVector(1.0f, 0.0f, 0.0f);} <br>
	 * {@code randomDir = Quaternion.multiply(Quaternion.randomQuaternion(),
	 * randomDir);}
	 */
	public final static Quaternion randomQuaternion() {
		float seed = (float) Math.random();
		float r1 = PApplet.sqrt(1.0f - seed);
		float r2 = PApplet.sqrt(seed);
		float t1 = 2.0f * PI * (float) Math.random();
		float t2 = 2.0f * PI * (float) Math.random();

		return new Quaternion(PApplet.sin(t1) * r1, PApplet.cos(t1) * r1, PApplet
				.sin(t2)
				* r2, PApplet.cos(t2) * r2);
	}

	/**
	 * Wrapper function that simply calls {@code slerp(a, b, t, true)}.
	 * <p>
	 * See {@link #slerp(Quaternion, Quaternion, float, boolean)} for details.
	 */
	public static final Quaternion slerp(Quaternion a, Quaternion b, float t) {
		return Quaternion.slerp(a, b, t, true);
	}

	/**
	 * Returns the slerp interpolation of quaternions {@code a} and {@code b}, at
	 * time {@code t}.
	 * <p>
	 * {@code t} should range in {@code [0,1]}. Result is a when {@code t=0 } and
	 * {@code b} when {@code t=1}.
	 * <p>
	 * When {@code allowFlip} is true (default) the slerp interpolation will
	 * always use the "shortest path" between the quaternions' orientations, by
	 * "flipping" the source Quaternion if needed (see {@link #negate()}).
	 * 
	 * @param a
	 *          the first Quaternion
	 * @param b
	 *          the second Quaternion
	 * @param t
	 *          the t interpolation parameter
	 * @param allowFlip
	 *          tells whether or not the interpolation allows axis flip
	 */
	public static final Quaternion slerp(Quaternion a, Quaternion b, float t,
			boolean allowFlip) {
		// Warning: this method should not normalize the Quaternion
		float cosAngle = Quaternion.dotProduct(a, b);

		float c1, c2;
		// Linear interpolation for close orientations
		if ((1.0 - PApplet.abs(cosAngle)) < 0.01) {
			c1 = 1.0f - t;
			c2 = t;
		} else {
			// Spherical interpolation
			float angle = PApplet.acos(PApplet.abs(cosAngle));
			float sinAngle = PApplet.sin(angle);
			c1 = PApplet.sin(angle * (1.0f - t)) / sinAngle;
			c2 = PApplet.sin(angle * t) / sinAngle;
		}

		// Use the shortest path
		if (allowFlip && (cosAngle < 0.0))
			c1 = -c1;

		return new Quaternion(c1 * a.x + c2 * b.x, c1 * a.y + c2 * b.y, c1 * a.z
				+ c2 * b.z, c1 * a.w + c2 * b.w, false);
	}

	/**
	 * Returns the slerp interpolation of the two quaternions {@code a} and
	 * {@code b}, at time {@code t}, using tangents {@code tgA} and {@code tgB}.
	 * <p>
	 * The resulting Quaternion is "between" {@code a} and {@code b} (result is
	 * {@code a} when {@code t=0} and {@code b} for {@code t=1}).
	 * <p>
	 * Use {@link #squadTangent(Quaternion, Quaternion, Quaternion)} to define the
	 * Quaternion tangents {@code tgA} and {@code tgB}.
	 * 
	 * @param a
	 *          the first Quaternion
	 * @param tgA
	 *          the first tangent Quaternion
	 * @param tgB
	 *          the second tangent Quaternion
	 * @param b
	 *          the second Quaternion
	 * @param t
	 *          the t interpolation parameter
	 */
	public static final Quaternion squad(Quaternion a, Quaternion tgA,
			Quaternion tgB, Quaternion b, float t) {
		Quaternion ab = Quaternion.slerp(a, b, t);
		Quaternion tg = Quaternion.slerp(tgA, tgB, t, false);
		return Quaternion.slerp(ab, tg, 2.0f * t * (1.0f - t), false);
	}

	/**
	 * Simply returns {@code log(a. inverse() * b)}.
	 * <p>
	 * Useful for {@link #squadTangent(Quaternion, Quaternion, Quaternion)}.
	 * 
	 * @param a
	 *          the first Quaternion
	 * @param b
	 *          the second Quaternion
	 */
	public static final Quaternion lnDif(Quaternion a, Quaternion b) {
		Quaternion dif = a.inverse();
		dif.multiply(b);

		dif.normalize();
		return dif.log();
	}

	/**
	 * Returns a tangent Quaternion for {@code center}, defined by {@code before}
	 * and {@code after} quaternions.
	 * 
	 * @param before
	 *          the first Quaternion
	 * @param center
	 *          the second Quaternion
	 * @param after
	 *          the third Quaternion
	 */
	public static final Quaternion squadTangent(Quaternion before,
			Quaternion center, Quaternion after) {
		Quaternion l1 = Quaternion.lnDif(center, before);
		Quaternion l2 = Quaternion.lnDif(center, after);
		Quaternion e = new Quaternion();

		e.x = -0.25f * (l1.x + l2.x);
		e.y = -0.25f * (l1.y + l2.y);
		e.z = -0.25f * (l1.z + l2.z);
		e.w = -0.25f * (l1.w + l2.w);

		return Quaternion.multiply(center, e.exp());
	}

	/**
	 * Utility function that returns the squared norm of the Quaternion.
	 */
	public static float squaredNorm(Quaternion q) {
		return (q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
	}
}
