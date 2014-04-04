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

/**
 * Based on a damned clever and aesthetic idea by David Bollinger.
 * 
 * http://www.davebollinger.com/works/p5/catmouse/CatMouse.pde.txt
 * 
 * @author jdf
 * 
 */
abstract public class DampedAction {
	private final PeasyCam p;
	private double velocity;
	private final double damping;

	public DampedAction(final PeasyCam p) {
		this(p, 0.16);
	}

	public DampedAction(final PeasyCam p, final double friction) {
		this.p = p;
		this.velocity = 0;
		this.damping = 1.0 - friction;
		p.getApplet().registerMethod("draw", this);
	}

	public void impulse(final double impulse) {
		velocity += impulse;
	}

	public void draw() {
		if (velocity == 0) {
			return;
		}
		behave(velocity);
		p.feed();
		velocity *= damping;
		if (Math.abs(velocity) < .001) {
			velocity = 0;
		}
	}

	public void stop() {
		velocity = 0;
	}

	abstract protected void behave(final double velocity);
}
