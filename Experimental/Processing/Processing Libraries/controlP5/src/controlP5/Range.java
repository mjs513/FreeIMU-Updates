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

import java.util.ArrayList;
import java.util.logging.Level;

import controlP5.ControlP5.Invisible;

import processing.core.PApplet;
import processing.core.PVector;

/**
 * A range slider works just like a slider but can be adjusted on both ends.
 * 
 * @see Slider
 * @example controllers/ControlP5range
 * @nosuperclasses Controller Controller
 */
public class Range extends Controller<Range> {

	/*
	 * TODO if range value is int, value labels do initialize as floats. first click makes them
	 * display as ints without decimal point
	 */
	protected static final int HORIZONTAL = 0;

	protected static final int VERTICAL = 1;

	protected int _myDirection;

	protected float _myValuePosition;

	protected boolean isDragging;

	protected boolean isDraggable = true;

	protected boolean isFirstClick;

	protected Label _myHighValueLabel;

	protected float _myValueRange;

	protected boolean isMinHandle;

	protected boolean isMaxHandle;

	protected boolean isMoveHandle;

	protected float distanceHandle;

	protected int handleSize = 10;

	protected int minHandle = 0;

	protected int maxHandle = 0;

	protected int mr = 0;

	protected final ArrayList<TickMark> _myTickMarks = new ArrayList<TickMark>();

	protected boolean isShowTickMarks;

	protected boolean isSnapToTickMarks;

	public static int autoWidth = 99;

	public static int autoHeight = 9;

	public static PVector autoSpacing = new PVector(0, 5, 0);

	public int alignValueLabel = CENTER;

	protected int _myColorTickMark = 0xffffffff;

	private int mode = -1;

	/**
	 * Convenience constructor to extend Range.
	 * 
	 * @example use/ControlP5extendController
	 * @param theControlP5
	 * @param theName
	 */
	public Range(ControlP5 theControlP5, String theName) {
		this(theControlP5, theControlP5.getDefaultTab(), theName, 0, 100, 0, 100, 0, 0, 99, 9);
		theControlP5.register(theControlP5.papplet, theName, this);
	}

	/**
	 * 
	 * @param theControlP5 ControlP5
	 * @param theParent ControllerGroup
	 * @param theName String
	 * @param theMin float
	 * @param theMax float
	 * @param theDefaultValue float
	 * @param theX int
	 * @param theY int
	 * @param theWidth int
	 * @param theHeight int
	 */
	@ControlP5.Invisible public Range(ControlP5 theControlP5, ControllerGroup<?> theParent, String theName, float theMin, float theMax, float theDefaultMinValue, float theDefaultMaxValue, int theX, int theY, int theWidth, int theHeight) {
		super(theControlP5, theParent, theName, theX, theY, theWidth, theHeight);

		_myArrayValue = new float[] { theDefaultMinValue, theDefaultMaxValue };

		_myMin = theMin;
		_myMax = theMax;
		_myValueRange = _myMax - _myMin;

		minHandle = (int) PApplet.map(theDefaultMinValue, _myMin, _myMax, handleSize, getWidth() - handleSize);
		maxHandle = (int) PApplet.map(theDefaultMaxValue, _myMin, _myMax, handleSize, getWidth() - handleSize);
		mr = maxHandle - minHandle;

		_myCaptionLabel = new Label(cp5, theName).setColor(color.getCaptionLabel()).align(RIGHT_OUTSIDE, CENTER);
		_myValueLabel = new Label(cp5, "" + adjustValue(_myMin)).setColor(color.getValueLabel()).set("" + adjustValue(theDefaultMinValue)).align(LEFT, CENTER);
		_myHighValueLabel = new Label(cp5, adjustValue(_myMax)).setColor(color.getValueLabel()).set("" + adjustValue(theDefaultMaxValue)).align(RIGHT, CENTER);

		_myValue = theDefaultMinValue;

		_myDirection = HORIZONTAL;

		update();

	}

	@Override public Range setColorValueLabel(int theColor) {
		_myValueLabel.setColor(theColor);
		_myHighValueLabel.setColor(theColor);
		return this;
	}

	@Override public Range setColorCaptionLabel(int theColor) {
		_myCaptionLabel.setColor(theColor);
		return this;
	}

	public Range setHighValueLabel(final String theLabel) {
		_myHighValueLabel.set(theLabel);
		return this;
	}

	public Range setLowValueLabel(final String theLabel) {
		_myValueLabel.set(theLabel);
		return this;
	}

	@ControlP5.Invisible public Range setSliderMode(int theMode) {
		return this;
	}

	public Range setHandleSize(int theSize) {
		handleSize = theSize;
		setLowValue(_myArrayValue[0], false);
		setHighValue(_myArrayValue[1], false);
		mr = maxHandle - minHandle;
		return this;
	}

	@ControlP5.Invisible public Range updateInternalEvents(PApplet theApplet) {
		if (isVisible) {
			int c = _myControlWindow.mouseX - _myControlWindow.pmouseX;
			if (c == 0) {
				return this;
			}
			if (isMousePressed && !cp5.isAltDown()) {
				switch (mode) {
				case (LEFT):
					minHandle = PApplet.max(handleSize, PApplet.min(maxHandle, minHandle + c));
					break;
				case (RIGHT):
					maxHandle = PApplet.max(minHandle, PApplet.min(getWidth() - handleSize, maxHandle + c));
					break;
				case (CENTER):
					minHandle = PApplet.max(handleSize, PApplet.min(getWidth() - mr - handleSize, minHandle + c));
					maxHandle = PApplet.max(minHandle, PApplet.min(getWidth() - handleSize, minHandle + mr));
					break;
				}
				update();
			}
		}
		return this;
	}

	@Override @Invisible public void mousePressed() {

		final float posX = _myParent.getAbsolutePosition().x + position.x;
		final float posY = _myParent.getAbsolutePosition().y + position.y;

		if (_myControlWindow.mouseY < posY || _myControlWindow.mouseY > posY + getHeight()) {
			mode = -1;
			isMinHandle = isMaxHandle = false;
			return;
		}

		int x0 = (int) (posX + minHandle);
		int x1 = (int) (posX + maxHandle);

		if (_myControlWindow.mouseX >= x0 - handleSize && _myControlWindow.mouseX < x0) {
			mode = LEFT;
			isMinHandle = true;
		} else if (_myControlWindow.mouseX >= x1 && _myControlWindow.mouseX < x1 + handleSize) {
			mode = RIGHT;
			isMaxHandle = true;
		} else if (_myControlWindow.mouseX > x0 && _myControlWindow.mouseX < x1 && isDraggable) {
			mode = CENTER;
		}
	}

	/**
	 * set the value of the range-slider. to set the low and high value, use setLowValue and
	 * setHighValue or setRangeValues
	 * 
	 * @see #setLowValue(float)
	 * @see #setHighValue(float)
	 * @see #setRangeValues(float, float)
	 * 
	 * @param theValue float
	 * @return Range
	 */
	@Override @ControlP5.Invisible public Range setValue(float theValue) {
		_myValue = theValue;
		broadcast(ARRAY);
		return this;
	}

	/**
	 * @exclude
	 */
	@Override @ControlP5.Invisible public Range update() {
		_myArrayValue[0] = PApplet.map(minHandle, handleSize, getWidth() - handleSize, _myMin, _myMax);
		_myArrayValue[1] = PApplet.map(maxHandle, handleSize, getWidth() - handleSize, _myMin, _myMax);
		mr = maxHandle - minHandle;
		_myHighValueLabel.set(adjustValue(_myArrayValue[1]));
		_myValueLabel.set(adjustValue(_myArrayValue[0]));
		return setValue(_myValue);
	}

	@ControlP5.Invisible public Range setDraggable(boolean theFlag) {
		isDraggable = theFlag;
		isDragging = (theFlag == false) ? false : isDragging;
		return this;
	}

	public float[] getArrayValue() {
		return _myArrayValue;
	}

	@Override public Range setArrayValue(float[] theArray) {
		setLowValue(theArray[0], false);
		setHighValue(theArray[1], false);
		return update();
	}

	@Override public Range setMin(float theValue) {
		_myMin = theValue;
		_myValueRange = _myMax - _myMin;
		return setLowValue(_myArrayValue[0]);
	}

	@Override public Range setMax(float theValue) {
		_myMax = theValue;
		_myValueRange = _myMax - _myMin;
		return setHighValue(_myArrayValue[1]);
	}

	public float getLowValue() {
		return _myArrayValue[0];
	}

	public float getHighValue() {
		return _myArrayValue[1];
	}

	@Override public Range setWidth(int theValue) {
		width = theValue;
		return this;
	}

	@Override public Range setHeight(int theValue) {
		height = theValue;
		return this;
	}

	@Override @ControlP5.Invisible public void mouseReleased() {
		isDragging = isMinHandle = isMaxHandle = isMoveHandle = false;
		mode = -1;
	}

	@Override @ControlP5.Invisible public void mouseReleasedOutside() {
		mouseReleased();
	}

	@Override @ControlP5.Invisible public void onLeave() {
		isMinHandle = false;
		isMaxHandle = false;
	}

	protected void setTickMarks() {
		System.out.println("Range Tickmarks not yet supported");
	}

	public Range setColorTickMark(int theColor) {
		_myColorTickMark = theColor;
		return this;
	}

	public Range showTickMarks(boolean theFlag) {
		isShowTickMarks = theFlag;
		return this;
	}

	public Range snapToTickMarks(boolean theFlag) {
		isSnapToTickMarks = theFlag;
		System.out.println("Range Tickmarks not yet supported");
		return this;
	}

	@ControlP5.Invisible public TickMark getTickMark() {
		System.out.println("Range Tickmarks not yet supported");
		return null;
	}

	public ArrayList<TickMark> getTickMarks() {
		return _myTickMarks;
	}

	public Range setNumberOfTickMarks(int theNumber) {
		System.out.println("Range Tickmarks not yet supported");
		_myTickMarks.clear();
		if (theNumber > 0) {
			for (int i = 0; i < theNumber; i++) {
				_myTickMarks.add(new TickMark(this));
			}
			showTickMarks(true);
			snapToTickMarks(true);
		} else {
			showTickMarks(false);
			snapToTickMarks(false);
		}
		_myUnit = (_myMax - _myMin) / ((width > height) ? width - 1 : height - 1);
		setLowValue(_myArrayValue[0], false);
		setHighValue(_myArrayValue[1], false);
		return update();
	}

	public Range setRange(float theMinValue, float theMaxValue) {
		setMin(theMinValue);
		setMax(theMaxValue);
		return this;
	}

	public Range setRangeValues(float theLowValue, float theHighValue) {
		return setArrayValue(new float[] { theLowValue, theHighValue });
	}

	private Range setLowValue(float theValue, boolean isUpdate) {
		_myArrayValue[0] = PApplet.max(_myMin, snapValue(theValue));
		minHandle = (int) PApplet.map(_myArrayValue[0], _myMin, _myMax, handleSize, getWidth() - handleSize);
		return (isUpdate) ? update() : this;
	}

	public Range setLowValue(float theValue) {
		return setLowValue(theValue, true);
	}

	private Range setHighValue(float theValue, boolean isUpdate) {
		_myArrayValue[1] = PApplet.min(_myMax, snapValue(theValue));
		maxHandle = (int) PApplet.map(_myArrayValue[1], _myMin, _myMax, handleSize, getWidth() - handleSize);
		return (isUpdate) ? update() : this;
	}

	public Range setHighValue(float theValue) {
		return setHighValue(theValue, true);
	}

	protected float snapValue(float theValue) {
		if (isMousePressed) {
			return theValue;
		}
		if (isSnapToTickMarks) {
			_myValuePosition = ((theValue - _myMin) / _myUnit);
			float n = PApplet.round(PApplet.map(_myValuePosition, 0, (_myDirection == HORIZONTAL) ? getWidth() : getHeight(), 0, _myTickMarks.size() - 1));
			theValue = PApplet.map(n, 0, _myTickMarks.size() - 1, _myMin, _myMax);
		}
		return theValue;
	}

	@Override @ControlP5.Invisible public Range updateDisplayMode(int theMode) {
		_myDisplayMode = theMode;
		switch (theMode) {
		case (DEFAULT):
			_myControllerView = new RangeView();
			break;
		case (SPRITE):
			_myControllerView = new RangeSpriteView();
			break;
		case (IMAGE):
			_myControllerView = new RangeImageView();
			break;
		case (CUSTOM):
		default:
			break;
		}
		return this;
	}

	class RangeSpriteView implements ControllerView<Range> {
		public void display(PApplet theApplet, Range theController) {
			ControlP5.logger().log(Level.INFO, "RangeSpriteDisplay not available.");
		}
	}

	class RangeView implements ControllerView<Range> {

		public void display(PApplet theApplet, Range theController) {

			int high = mode;

			final float posX = _myParent.getAbsolutePosition().x + position.x;
			int x0 = (int) (posX + minHandle);
			int x1 = (int) (posX + maxHandle);

			if (isInside() && high < 0) {
				if (_myControlWindow.mouseX >= x0 - handleSize && _myControlWindow.mouseX < x0) {
					high = LEFT;
				} else if (_myControlWindow.mouseX >= x1 && _myControlWindow.mouseX < x1 + handleSize) {
					high = RIGHT;
				} else if (_myControlWindow.mouseX > x0 && _myControlWindow.mouseX < x1 && isDraggable) {
					high = CENTER;
				}
			}

			theApplet.pushMatrix();

			theApplet.fill(color.getBackground());

			theApplet.noStroke();

			theApplet.rect(0, 0, width, height);

			theApplet.fill(high == CENTER ? color.getActive() : color.getForeground());

			if (isShowTickMarks) {
				int n = handleSize / 2;
				theApplet.rect(minHandle - n, 0, mr + handleSize, height);
				theApplet.fill((isMinHandle || high == LEFT) ? color.getActive() : color.getForeground());
				theApplet.triangle(minHandle - handleSize, 0, minHandle, 0, minHandle - n, height);
				theApplet.fill((isMaxHandle || high == RIGHT) ? color.getActive() : color.getForeground());
				theApplet.triangle(maxHandle, 0, maxHandle + handleSize, 0, maxHandle + n, height);
			} else {
				theApplet.rect(minHandle, 0, mr, height);
				theApplet.fill((isMinHandle || high == LEFT) ? color.getActive() : color.getForeground());
				theApplet.rect((minHandle - handleSize), 0, handleSize, height);
				theApplet.fill((isMaxHandle || high == RIGHT) ? color.getActive() : color.getForeground());
				theApplet.rect(maxHandle, 0, handleSize, height);

			}

			if (isLabelVisible) {
				_myCaptionLabel.draw(theApplet, 0, 0, theController);
				_myValueLabel.draw(theApplet, 0, 0, theController);
				_myHighValueLabel.draw(theApplet, 0, 0, theController);
			}

			theApplet.popMatrix();

			if (isShowTickMarks) {
				theApplet.pushMatrix();
				float x = (getWidth() - handleSize) / (getTickMarks().size() - 1);
				theApplet.translate(handleSize / 2, getHeight());
				theApplet.fill(_myColorTickMark);
				for (TickMark tm : getTickMarks()) {
					tm.draw(theApplet);
					theApplet.translate(x, 0);
				}
				theApplet.popMatrix();
			}
		}
	}

	class RangeImageView implements ControllerView<Range> {
		public void display(PApplet theApplet, Range theController) {
			ControlP5.logger().log(Level.INFO, "RangeImageDisplay not implemented.");
		}
	}

	@Override @ControlP5.Invisible public String toString() {
		return "type:\tRange\n" + super.toString();
	}

	@Deprecated public float lowValue() {
		return getLowValue();
	}

	@Deprecated public float highValue() {
		return getHighValue();
	}

	@Deprecated public float[] arrayValue() {
		return _myArrayValue;
	}

}
