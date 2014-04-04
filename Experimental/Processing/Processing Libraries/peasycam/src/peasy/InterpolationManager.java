/**
 * 
 */
package peasy;

import peasy.PeasyCam.AbstractInterp;

class InterpolationManager {
	private AbstractInterp currentInterpolator = null;

	protected synchronized void startInterpolation(final AbstractInterp interpolation) {
		cancelInterpolation();
		currentInterpolator = interpolation;
		currentInterpolator.start();
	}

	protected synchronized void cancelInterpolation() {
		if (currentInterpolator != null) {
			currentInterpolator.cancel();
			currentInterpolator = null;
		}
	}

}