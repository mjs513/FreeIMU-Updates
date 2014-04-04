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

import peasy.org.apache.commons.math.geometry.CardanEulerSingularityException;
import peasy.org.apache.commons.math.geometry.Rotation;
import peasy.org.apache.commons.math.geometry.RotationOrder;
import peasy.org.apache.commons.math.geometry.Vector3D;
import processing.core.PApplet;
import processing.core.PConstants;
import processing.core.PGraphics;
import processing.core.PMatrix3D;
import processing.event.KeyEvent;
import processing.event.MouseEvent;

/**
 * 
 * @author Jonathan Feinberg
 */
public class PeasyCam {
	private static final Vector3D LOOK = Vector3D.plusK;
	private static final Vector3D UP = Vector3D.plusJ;

	private static enum Constraint {
		YAW, PITCH, ROLL, SUPPRESS_ROLL
	}

	private final PApplet p;

	private final double startDistance;
	private final Vector3D startCenter;

	private boolean resetOnDoubleClick = true;
	private double minimumDistance = 1;
	private double maximumDistance = Double.MAX_VALUE;

	private final DampedAction rotateX, rotateY, rotateZ, dampedZoom, dampedPanX,
			dampedPanY;

	private double distance;
	private Vector3D center;
	private Rotation rotation;

	private Constraint dragConstraint = null;
	private Constraint permaConstraint = null;

	private final InterpolationManager rotationInterps = new InterpolationManager();
	private final InterpolationManager centerInterps = new InterpolationManager();
	private final InterpolationManager distanceInterps = new InterpolationManager();

	private final PeasyDragHandler panHandler /* ha ha ha */= new PeasyDragHandler() {
		public void handleDrag(final double dx, final double dy) {
			dampedPanX.impulse(dx / 8.);
			dampedPanY.impulse(dy / 8.);
		}
	};
	private PeasyDragHandler centerDragHandler = panHandler;

	private final PeasyDragHandler rotateHandler = new PeasyDragHandler() {
		public void handleDrag(final double dx, final double dy) {
			mouseRotate(dx, dy);
		}
	};
	private PeasyDragHandler leftDragHandler = rotateHandler;

	private final PeasyDragHandler zoomHandler = new PeasyDragHandler() {
		public void handleDrag(final double dx, final double dy) {
			dampedZoom.impulse(dy / 10.0);
		}
	};
	private PeasyDragHandler rightDraghandler = zoomHandler;

	private final PeasyWheelHandler zoomWheelHandler = new PeasyWheelHandler() {
		public void handleWheel(final int delta) {
			dampedZoom.impulse(wheelScale * delta);
		}
	};
	private PeasyWheelHandler wheelHandler = zoomWheelHandler;
	private double wheelScale = 1.0;

	private final PeasyEventListener peasyEventListener = new PeasyEventListener();
	private boolean isActive = false;

	private final PMatrix3D originalMatrix; // for HUD restore

	public final String VERSION = "200";

	public PeasyCam(final PApplet parent, final double distance) {
		this(parent, 0, 0, 0, distance);
	}

	public PeasyCam(final PApplet parent, final double lookAtX, final double lookAtY,
			final double lookAtZ, final double distance) {
		this.p = parent;
		this.startCenter = this.center = new Vector3D(lookAtX, lookAtY, lookAtZ);
		this.startDistance = this.distance = distance;
		this.rotation = new Rotation();
		this.originalMatrix = parent.getMatrix((PMatrix3D)null);

		feed();

		rotateX = new DampedAction(this) {
			@Override
			protected void behave(final double velocity) {
				rotation = rotation.applyTo(new Rotation(Vector3D.plusI, velocity));
			}
		};

		rotateY = new DampedAction(this) {
			@Override
			protected void behave(final double velocity) {
				rotation = rotation.applyTo(new Rotation(Vector3D.plusJ, velocity));
			}
		};

		rotateZ = new DampedAction(this) {
			@Override
			protected void behave(final double velocity) {
				rotation = rotation.applyTo(new Rotation(Vector3D.plusK, velocity));
			}
		};

		dampedZoom = new DampedAction(this) {
			@Override
			protected void behave(final double velocity) {
				mouseZoom(velocity);
			}
		};

		dampedPanX = new DampedAction(this) {
			@Override
			protected void behave(final double velocity) {
				mousePan(velocity, 0);
			}
		};

		dampedPanY = new DampedAction(this) {
			@Override
			protected void behave(final double velocity) {
				mousePan(0, velocity);
			}
		};

		setActive(true);
		System.err.println("PeasyCam v" + VERSION);
	}

	public void setActive(final boolean active) {
		if (active == isActive) {
			return;
		}
		isActive = active;
		if (isActive) {
			p.registerMethod("mouseEvent", peasyEventListener);
			p.registerMethod("keyEvent", peasyEventListener);
		} else {
			p.unregisterMethod("mouseEvent", peasyEventListener);
			p.unregisterMethod("keyEvent", peasyEventListener);
		}
	}

	public boolean isActive() {
		return isActive;
	}

	/**
	 * <p>
	 * Turn on or off default mouse-handling behavior:
	 * 
	 * <p>
	 * <table>
	 * <tr>
	 * <td><b>left-drag</b></td>
	 * <td>rotate camera around look-at point</td>
	 * <tr>
	 * <tr>
	 * <td><b>center-drag</b></td>
	 * <td>pan camera (change look-at point)</td>
	 * <tr>
	 * <tr>
	 * <td><b>right-drag</b></td>
	 * <td>zoom</td>
	 * <tr>
	 * <tr>
	 * <td><b>wheel</b></td>
	 * <td>zoom</td>
	 * <tr>
	 * </table>
	 * 
	 * @param isMouseControlled
	 * @deprecated use {@link #setActive(boolean)}
	 */
	@Deprecated
	public void setMouseControlled(final boolean isMouseControlled) {
		setActive(isMouseControlled);
	}

	public double getWheelScale() {
		return wheelScale;
	}

	public void setWheelScale(final double wheelScale) {
		this.wheelScale = wheelScale;
	}

	public PeasyDragHandler getPanDragHandler() {
		return panHandler;
	}

	public PeasyDragHandler getRotateDragHandler() {
		return rotateHandler;
	}

	public PeasyDragHandler getZoomDragHandler() {
		return zoomHandler;
	}

	public PeasyWheelHandler getZoomWheelHandler() {
		return zoomWheelHandler;
	}

	public void setLeftDragHandler(final PeasyDragHandler handler) {
		leftDragHandler = handler;
	}

	public void setCenterDragHandler(final PeasyDragHandler handler) {
		centerDragHandler = handler;
	}

	public void setRightDragHandler(final PeasyDragHandler handler) {
		rightDraghandler = handler;
	}

	public PeasyWheelHandler getWheelHandler() {
		return wheelHandler;
	}

	public void setWheelHandler(final PeasyWheelHandler wheelHandler) {
		this.wheelHandler = wheelHandler;
	}

	public String version() {
		return VERSION;
	}

	protected class PeasyEventListener {
		public void keyEvent(final KeyEvent e) {
			if (e.getAction() == KeyEvent.RELEASE && e.isShiftDown())
				dragConstraint = null;
		}

		public void mouseEvent(final MouseEvent e) {
			switch (e.getAction()) {
			case MouseEvent.WHEEL:
				wheelHandler.handleWheel((int)e.getCount());
				break;
			case MouseEvent.RELEASE:
				dragConstraint = null;
				break;
			case MouseEvent.CLICK:
				if (resetOnDoubleClick && 2 == (int)e.getCount()) {
					reset();
				}
				break;
			case MouseEvent.DRAG:
				final double dx = p.mouseX - p.pmouseX;
				final double dy = p.mouseY - p.pmouseY;

				if (e.isShiftDown()) {
					if (dragConstraint == null && Math.abs(dx - dy) > 1) {
						dragConstraint = Math.abs(dx) > Math.abs(dy) ? Constraint.YAW
								: Constraint.PITCH;
					}
				} else if (permaConstraint != null) {
					dragConstraint = permaConstraint;
				} else {
					dragConstraint = null;
				}

				final int b = p.mouseButton;
				if (centerDragHandler != null
						&& (b == PConstants.CENTER || (b == PConstants.LEFT && e
								.isMetaDown()))) {
					centerDragHandler.handleDrag(dx, dy);
				} else if (leftDragHandler != null && b == PConstants.LEFT) {
					leftDragHandler.handleDrag(dx, dy);
				} else if (rightDraghandler != null && b == PConstants.RIGHT) {
					rightDraghandler.handleDrag(dx, dy);
				}
				break;
			}
		}
	}

	private void mouseZoom(final double delta) {
		safeSetDistance(distance + delta * Math.log1p(distance));
	}

	private void mousePan(final double dxMouse, final double dyMouse) {
		final double panScale = Math.sqrt(distance * .005);
		pan(dragConstraint == Constraint.PITCH ? 0 : -dxMouse * panScale,
				dragConstraint == Constraint.YAW ? 0 : -dyMouse * panScale);
	}

	private void mouseRotate(final double dx, final double dy) {
		final Vector3D u = LOOK.scalarMultiply(100 + .6 * startDistance).negate();

		final int xSign = dx > 0 ? -1 : 1;
		final int ySign = dy < 0 ? -1 : 1;

		final double eccentricity = Math.abs((p.height / 2d) - p.mouseY)
				/ (p.height / 2d);
		final double rho = Math.abs((p.width / 2d) - p.mouseX) / (p.width / 2d);

		if (dragConstraint == null || dragConstraint == Constraint.YAW
				|| dragConstraint == Constraint.SUPPRESS_ROLL) {
			final double adx = Math.abs(dx) * (1 - eccentricity);
			final Vector3D vx = u.add(new Vector3D(adx, 0, 0));
			rotateY.impulse(Vector3D.angle(u, vx) * xSign);
		}
		if (dragConstraint == null || dragConstraint == Constraint.PITCH
				|| dragConstraint == Constraint.SUPPRESS_ROLL) {
			final double ady = Math.abs(dy) * (1 - rho);
			final Vector3D vy = u.add(new Vector3D(0, ady, 0));
			rotateX.impulse(Vector3D.angle(u, vy) * ySign);
		}
		if (dragConstraint == null || dragConstraint == Constraint.ROLL) {
			{
				final double adz = Math.abs(dy) * rho;
				final Vector3D vz = u.add(new Vector3D(0, adz, 0));
				rotateZ.impulse(Vector3D.angle(u, vz) * -ySign
						* (p.mouseX < p.width / 2 ? -1 : 1));
			}
			{
				final double adz = Math.abs(dx) * eccentricity;
				final Vector3D vz = u.add(new Vector3D(0, adz, 0));
				rotateZ.impulse(Vector3D.angle(u, vz) * xSign
						* (p.mouseY > p.height / 2 ? -1 : 1));
			}
		}
	}

	public double getDistance() {
		return distance;
	}

	public void setDistance(final double newDistance) {
		setDistance(newDistance, 300);
	}

	public void setDistance(final double newDistance, final long animationTimeMillis) {
		distanceInterps.startInterpolation(new DistanceInterp(newDistance,
				animationTimeMillis));
	}

	public float[] getLookAt() {
		return new float[] { (float)center.getX(), (float)center.getY(),
				(float)center.getZ() };
	}

	public void lookAt(final double x, final double y, final double z) {
		centerInterps.startInterpolation(new CenterInterp(new Vector3D(x, y, z), 300));
	}

	public void lookAt(final double x, final double y, final double z,
			final double distance) {
		lookAt(x, y, z);
		setDistance(distance);
	}

	public void lookAt(final double x, final double y, final double z,
			final long animationTimeMillis) {
		lookAt(x, y, z, distance, animationTimeMillis);
	}

	public void lookAt(final double x, final double y, final double z,
			final double distance, final long animationTimeMillis) {
		setState(new CameraState(rotation, new Vector3D(x, y, z), distance),
				animationTimeMillis);
	}

	private void safeSetDistance(final double distance) {
		this.distance = Math.min(maximumDistance, Math.max(minimumDistance, distance));
		feed();
	}

	public void feed() {
		final Vector3D pos = rotation.applyTo(LOOK).scalarMultiply(distance).add(center);
		final Vector3D rup = rotation.applyTo(UP);
		p.camera((float)pos.getX(), (float)pos.getY(), (float)pos.getZ(), //
				(float)center.getX(), (float)center.getY(), (float)center.getZ(), //
				(float)rup.getX(), (float)rup.getY(), (float)rup.getZ());
	}

	static void apply(final PGraphics g, final Vector3D center, final Rotation rotation,
			final double distance) {
		final Vector3D pos = rotation.applyTo(LOOK).scalarMultiply(distance).add(center);
		final Vector3D rup = rotation.applyTo(UP);
		g.camera((float)pos.getX(), (float)pos.getY(), (float)pos.getZ(), //
				(float)center.getX(), (float)center.getY(), (float)center.getZ(), //
				(float)rup.getX(), (float)rup.getY(), (float)rup.getZ());
	}

	/**
	 * Where is the PeasyCam in world space?
	 * 
	 * @return float[]{x,y,z}
	 */
	public float[] getPosition() {
		final Vector3D pos = rotation.applyTo(LOOK).scalarMultiply(distance).add(center);
		return new float[] { (float)pos.getX(), (float)pos.getY(), (float)pos.getZ() };
	}

	public void reset() {
		reset(300);
	}

	public void reset(final long animationTimeInMillis) {
		setState(new CameraState(new Rotation(), startCenter, startDistance),
				animationTimeInMillis);
	}

	public void pan(final double dx, final double dy) {
		center = center.add(rotation.applyTo(new Vector3D(dx, dy, 0)));
		feed();
	}

	public void rotateX(final double angle) {
		rotation = rotation.applyTo(new Rotation(Vector3D.plusI, angle));
		feed();
	}

	public void rotateY(final double angle) {
		rotation = rotation.applyTo(new Rotation(Vector3D.plusJ, angle));
		feed();
	}

	public void rotateZ(final double angle) {
		rotation = rotation.applyTo(new Rotation(Vector3D.plusK, angle));
		feed();
	}

	PApplet getApplet() {
		return p;
	}

	public CameraState getState() {
		return new CameraState(rotation, center, distance);
	}

	/**
	 * Permit arbitrary rotation. (Default mode.)
	 */
	public void setFreeRotationMode() {
		permaConstraint = null;
	}

	/**
	 * Only permit yaw.
	 */
	public void setYawRotationMode() {
		permaConstraint = Constraint.YAW;
	}

	/**
	 * Only permit pitch.
	 */
	public void setPitchRotationMode() {
		permaConstraint = Constraint.PITCH;
	}

	/**
	 * Only permit roll.
	 */
	public void setRollRotationMode() {
		permaConstraint = Constraint.ROLL;
	}

	/**
	 * Only suppress roll.
	 */
	public void setSuppressRollRotationMode() {
		permaConstraint = Constraint.SUPPRESS_ROLL;
	}

	public void setMinimumDistance(final double minimumDistance) {
		this.minimumDistance = minimumDistance;
		safeSetDistance(distance);
	}

	public void setMaximumDistance(final double maximumDistance) {
		this.maximumDistance = maximumDistance;
		safeSetDistance(distance);
	}

	public void setResetOnDoubleClick(final boolean resetOnDoubleClick) {
		this.resetOnDoubleClick = resetOnDoubleClick;
	}

	public void setState(final CameraState state) {
		setState(state, 300);
	}

	public void setState(final CameraState state, final long animationTimeMillis) {
		if (animationTimeMillis > 0) {
			rotationInterps.startInterpolation(new RotationInterp(state.rotation,
					animationTimeMillis));
			centerInterps.startInterpolation(new CenterInterp(state.center,
					animationTimeMillis));
			distanceInterps.startInterpolation(new DistanceInterp(state.distance,
					animationTimeMillis));
		} else {
			this.rotation = state.rotation;
			this.center = state.center;
			this.distance = state.distance;
		}
		feed();
	}

	public void setRotations(final double pitch, final double yaw, final double roll) {
		rotationInterps.cancelInterpolation();
		this.rotation = new Rotation(RotationOrder.XYZ, pitch, yaw, roll);
		feed();
	}

	/**
	 * Express the current camera rotation as an equivalent series
	 * of world rotations, in X, Y, Z order. This is useful when,
	 * for example, you wish to orient text towards the camera
	 * at all times, as in
	 * 
	 * <pre>float[] rotations = cam.getRotations(rotations);
	 *rotateX(rotations[0]);
	 *rotateY(rotations[1]);
	 *rotateZ(rotations[2]);
	 *text("Here I am!", 0, 0, 0);</pre>
	 */
	public float[] getRotations() {
		try {
			final double[] angles = rotation.getAngles(RotationOrder.XYZ);
			return new float[] { (float)angles[0], (float)angles[1], (float)angles[2] };
		} catch (final CardanEulerSingularityException e) {
		}
		try {
			final double[] angles = rotation.getAngles(RotationOrder.YXZ);
			return new float[] { (float)angles[1], (float)angles[0], (float)angles[2] };
		} catch (final CardanEulerSingularityException e) {
		}
		try {
			final double[] angles = rotation.getAngles(RotationOrder.ZXY);
			return new float[] { (float)angles[2], (float)angles[0], (float)angles[1] };
		} catch (final CardanEulerSingularityException e) {
		}
		return new float[] { 0, 0, 0 };
	}

	/**
	 * Thanks to A.W. Martin for the code to do HUD
	 */
	public void beginHUD() {
		p.pushMatrix();
		p.hint(PConstants.DISABLE_DEPTH_TEST);
		// Load the identity matrix.
		p.resetMatrix();
		// Apply the original Processing transformation matrix.
		p.applyMatrix(originalMatrix);
	}

	public void endHUD() {
		p.hint(PConstants.ENABLE_DEPTH_TEST);
		p.popMatrix();
	}

	abstract public class AbstractInterp {
		double startTime;
		final double timeInMillis;

		protected AbstractInterp(final long timeInMillis) {
			this.timeInMillis = timeInMillis;
		}

		void start() {
			startTime = p.millis();
			p.registerMethod("draw", this);
		}

		void cancel() {
			p.unregisterMethod("draw", this);
		}

		public void draw() {
			final double t = (p.millis() - startTime) / timeInMillis;
			if (t > .99) {
				cancel();
				setEndState();
			} else {
				interp(t);
			}
			feed();
		}

		protected abstract void interp(double t);

		protected abstract void setEndState();
	}

	class DistanceInterp extends AbstractInterp {
		private final double startDistance = distance;
		private final double endDistance;

		public DistanceInterp(final double endDistance, final long timeInMillis) {
			super(timeInMillis);
			this.endDistance = Math.min(maximumDistance,
					Math.max(minimumDistance, endDistance));
		}

		@Override
		protected void interp(final double t) {
			distance = InterpolationUtil.smooth(startDistance, endDistance, t);
		}

		@Override
		protected void setEndState() {
			distance = endDistance;
		}
	}

	class CenterInterp extends AbstractInterp {
		private final Vector3D startCenter = center;
		private final Vector3D endCenter;

		public CenterInterp(final Vector3D endCenter, final long timeInMillis) {
			super(timeInMillis);
			this.endCenter = endCenter;
		}

		@Override
		protected void interp(final double t) {
			center = InterpolationUtil.smooth(startCenter, endCenter, t);
		}

		@Override
		protected void setEndState() {
			center = endCenter;
		}
	}

	class RotationInterp extends AbstractInterp {
		final Rotation startRotation = rotation;
		final Rotation endRotation;

		public RotationInterp(final Rotation endRotation, final long timeInMillis) {
			super(timeInMillis);
			this.endRotation = endRotation;
		}

		@Override
		void start() {
			rotateX.stop();
			rotateY.stop();
			rotateZ.stop();
			super.start();
		}

		@Override
		protected void interp(final double t) {
			rotation = InterpolationUtil.slerp(startRotation, endRotation, t);
		}

		@Override
		protected void setEndState() {
			rotation = endRotation;
		}
	}
}
