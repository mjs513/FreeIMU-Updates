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

import java.io.Serializable;

import peasy.org.apache.commons.math.geometry.Rotation;
import peasy.org.apache.commons.math.geometry.Vector3D;
import processing.core.PApplet;
import processing.core.PGraphics;

public class CameraState implements Serializable {
	private static final long serialVersionUID = 1L;
	final Rotation rotation;
	final Vector3D center;
	final double distance;

	public CameraState(final Rotation rotation, final Vector3D center,
			final double distance) {
		this.rotation = rotation;
		this.center = center;
		this.distance = distance;
	}

	public void apply(final PApplet a) {
		if (a.recorder != null) {
			apply(a.recorder);
		}
		apply(a.g);
	}

	public void apply(final PGraphics g) {
		PeasyCam.apply(g, center, rotation, distance);
	}

}
