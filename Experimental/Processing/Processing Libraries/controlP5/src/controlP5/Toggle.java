
package controlP5;

/**
 * controlP5 is a processing gui library.
 *
 *  2006-2012 by Andreas Schlegel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA
 *
 * @author 		Andreas Schlegel (http://www.sojamo.de)
 * @modified	12/23/2012
 * @version		2.0.4
 *
 */

import processing.core.PApplet;
import processing.core.PVector;

/**
 * a toggle can have two states, true and false, where true has the value 1 and false is 0.
 * 
 * @example controllers/ControlP5toggle
 * @nosuperclasses Controller Controller
 */
public class Toggle extends Controller<Toggle> {

	protected int cnt;

	protected boolean isOn = false;

	protected float internalValue = -1;

	public static int autoWidth = 39;

	public static int autoHeight = 19;

	protected PVector autoSpacing = new PVector(10, 20, 0);


	/**
	 * Convenience constructor to extend Toggle.
	 * 
	 * @example use/ControlP5extendController
	 * @param theControlP5
	 * @param theName
	 */
	public Toggle(ControlP5 theControlP5, String theName) {
		this(theControlP5, theControlP5.getDefaultTab(), theName, 0, 0, 0, autoWidth, autoHeight);
		theControlP5.register(theControlP5.papplet, theName, this);
	}


	/**
	 * 
	 * @param theControlP5 ControlP5
	 * @param theParent Tab
	 * @param theName String
	 * @param theValue float
	 * @param theX float
	 * @param theY float
	 * @param theWidth int
	 * @param theHeight int
	 */
	public Toggle(ControlP5 theControlP5, Tab theParent, String theName, float theValue, float theX, float theY, int theWidth, int theHeight) {
		super(theControlP5, theParent, theName, theX, theY, theWidth, theHeight);
		_myValue = theValue;
		_myCaptionLabel.align(LEFT, BOTTOM_OUTSIDE).setPadding(0, Label.paddingY);
	}


	/**
	 * 
	 * @param theApplet PApplet
	 */
	@ControlP5.Invisible public void draw(PApplet theApplet) {
		theApplet.pushMatrix();
		theApplet.translate(position.x, position.y);
		_myControllerView.display(theApplet, this);
		theApplet.popMatrix();
	}


	protected void onEnter() {
		isActive = true;
	}


	protected void onLeave() {
		isActive = false;
	}


	/**
	 * {@inheritDoc}
	 */
	@ControlP5.Invisible public void mousePressed() {
		setState(!isOn);
		isActive = false;
	}


	/**
	 * {@inheritDoc}
	 */
	@Override public Toggle setValue(float theValue) {
		if (theValue == 0) {
			setState(false);
		}
		else {
			setState(true);
		}
		return this;
	}


	/**
	 * @param theValue
	 */
	public Toggle setValue(boolean theValue) {
		setValue((theValue == true) ? 1 : 0);
		return this;
	}


	/**
	 * {@inheritDoc}
	 */
	@Override public Toggle update() {
		return setValue(_myValue);
	}


	/**
	 * sets the state of the toggle, this can be true or false.
	 * 
	 * @param theFlag boolean
	 */
	public Toggle setState(boolean theFlag) {
		isOn = theFlag;
		_myValue = (isOn == false) ? 0 : 1;
		broadcast(FLOAT);
		return this;
	}


	/**
	 * 
	 * @return
	 */
	public boolean getState() {
		return isOn;
	}


	protected void deactivate() {
		isOn = false;
		_myValue = (isOn == false) ? 0 : 1;
	}


	protected void activate() {
		isOn = true;
		_myValue = (isOn == false) ? 0 : 1;
	}


	/**
	 * switch the state of a toggle.
	 */
	public Toggle toggle() {
		if (isOn) {
			setState(false);
		}
		else {
			setState(true);
		}
		return this;
	}


	/**
	 * set the visual mode of a Toggle. use setMode(ControlP5.DEFAULT) or setMode(ControlP5.SWITCH)
	 * 
	 * @param theMode
	 */
	public Toggle setMode(int theMode) {
		updateDisplayMode(theMode);
		return this;
	}


	/**
	 * by default a toggle returns 0 (for off) and 1 (for on). the internal value variable can be
	 * used to store an additional value for a toggle event.
	 * 
	 * @param theInternalValue
	 */
	@ControlP5.Invisible public void setInternalValue(float theInternalValue) {
		internalValue = theInternalValue;
	}


	@ControlP5.Invisible public float internalValue() {
		return internalValue;
	}


	/**
	 * {@inheritDoc}
	 */
	@Override public Toggle linebreak() {
		cp5.linebreak(this, true, autoWidth, autoHeight, autoSpacing);
		return this;
	}


	/**
	 * {@inheritDoc}
	 */
	@Override @ControlP5.Invisible public Toggle updateDisplayMode(int theState) {
		_myDisplayMode = theState;
		switch (theState) {
		case (DEFAULT):
			_myControllerView = new ToggleView();
			break;
		case (IMAGE):
			_myControllerView = new ToggleImageView();
			break;
		case (SWITCH):
			_myControllerView = new ToggleSwitchView();
			break;
		case (CUSTOM):
		default:
			break;
		}
		return this;
	}


	class ToggleView implements ControllerView<Toggle> {

		public void display(PApplet theApplet, Toggle theController) {
			if (isActive) {
				theApplet.fill(isOn ? color.getActive() : color.getForeground());
			}
			else {
				theApplet.fill(isOn ? color.getActive() : color.getBackground());
			}
			theApplet.rect(0, 0, width, height);
			if (isLabelVisible) {
				_myCaptionLabel.draw(theApplet, 0, 0, theController);
			}
		}
	}

	class ToggleImageView implements ControllerView<Toggle> {

		public void display(PApplet theApplet, Toggle theController) {
			if (isActive) {
				theApplet.image((availableImages[ACTIVE] == true) ? images[ACTIVE] : images[DEFAULT], 0, 0);
			}
			else {
				if (isOn) {
					theApplet.image((availableImages[ACTIVE] == true) ? images[ACTIVE] : images[DEFAULT], 0, 0);
				}
				else {
					theApplet.image(images[DEFAULT], 0, 0);
				}
			}
			theApplet.rect(0, 0, width, height);
		}
	}

	class ToggleSwitchView implements ControllerView<Toggle> {

		public void display(PApplet theApplet, Toggle theController) {
			theApplet.fill(color.getBackground());
			theApplet.rect(0, 0, width, height);
			theApplet.fill(color.getActive());
			if (isOn) {
				theApplet.rect(0, 0, width / 2, height);
			}
			else {
				theApplet.rect((width % 2 == 0 ? 0 : 1) + width / 2, 0, width / 2, height);
			}
			if (isLabelVisible) {
				_myCaptionLabel.draw(theApplet, 0, 0, theController);
			}
		}
	}
}
