/*
   The PeasyCam Processing library, which provides an easy-peasy
   camera for 3D sketching.
  
   Copyright 2008 Jonathan Feinberg

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */
package peasy;

import peasy.org.apache.commons.math.geometry.Rotation;
import peasy.org.apache.commons.math.geometry.Vector3D;

public class InterpolationUtil {

	// Thanks to Michael Kaufmann <mail@michael-kaufmann.ch> for improvements to this function.
	static public Rotation slerp(final Rotation a, final Rotation b, final double t) {
		final double a0 = a.getQ0(), a1 = a.getQ1(), a2 = a.getQ2(), a3 = a.getQ3();
		double b0 = b.getQ0(), b1 = b.getQ1(), b2 = b.getQ2(), b3 = b.getQ3();

		double cosTheta = a0 * b0 + a1 * b1 + a2 * b2 + a3 * b3;
		if (cosTheta < 0) {
			b0 = -b0;
			b1 = -b1;
			b2 = -b2;
			b3 = -b3;
			cosTheta = -cosTheta;
		}

		final double theta = Math.acos(cosTheta);
		final double sinTheta = Math.sqrt(1.0 - cosTheta * cosTheta);

		double w1, w2;
		if (sinTheta > 0.001) {
			w1 = Math.sin((1.0 - t) * theta) / sinTheta;
			w2 = Math.sin(t * theta) / sinTheta;
		} else {
			w1 = 1.0 - t;
			w2 = t;
		}
		return new Rotation(w1 * a0 + w2 * b0, w1 * a1 + w2 * b1, w1 * a2 + w2 * b2, w1
				* a3 + w2 * b3, true);
	}

	static public double smooth(final double a, final double b, final double t) {
		final double smooth = (t * t * (3 - 2 * t));
		return (b * smooth) + (a * (1 - smooth));

	}

	static public Vector3D smooth(final Vector3D a, final Vector3D b, final double t) {
		return new Vector3D(smooth(a.getX(), b.getX(), t), smooth(a.getY(), b.getY(), t),
				smooth(a.getZ(), b.getZ(), t));
	}

	static public double linear(final double a, final double b, final double t) {
		return a + (b - a) * t;
	}

	static public Vector3D linear(final Vector3D a, final Vector3D b, final double t) {
		return new Vector3D(linear(a.getX(), b.getX(), t), linear(a.getY(), b.getY(), t),
				linear(a.getZ(), b.getZ(), t));
	}
}
