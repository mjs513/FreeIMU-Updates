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

import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

import processing.core.PApplet;
import processing.core.PVector;
import processing.event.Event;

/**
 * The ControlP5Base supports the ControlP5 class and implements all adder methods to add controllers to controlP5.
 */
public class ControlP5Base implements ControlP5Constants {

	private ControlP5 cp5;

	ControllerProperties _myProperties;

	private ControllerAutomator _myAutomator;

	protected Map<Object, ArrayList<ControllerInterface<?>>> _myObjectToControllerMap = new HashMap<Object, ArrayList<ControllerInterface<?>>>();

	protected Map<String, FieldChangedListener> _myFieldChangedListenerMap = new HashMap<String, FieldChangedListener>();

	protected Map<KeyCode, List<ControlKey>> keymap = new HashMap<KeyCode, List<ControlKey>>();

	protected ControllerGroup<?> currentGroupPointer;

	protected boolean isCurrentGroupPointerClosed = true;

	protected int autoDirection = HORIZONTAL;

	public Tab getDefaultTab() {
		return (Tab) cp5.controlWindow.getTabs().get(1);
	}

	protected void init(ControlP5 theControlP5) {
		cp5 = theControlP5;
		_myProperties = new ControllerProperties(cp5);
		_myAutomator = new ControllerAutomator(cp5);
		currentGroupPointer = cp5.controlWindow.getTab("default");
	}

	public ControllerLayout getLayout() {
		return new ControllerLayout(cp5);
	}

	public Tab addTab(String theName) {
		for (int i = 0; i < cp5.getWindow().getTabs().size(); i++) {
			if (cp5.getWindow().getTabs().get(i).getName().equals(theName)) {
				return (Tab) cp5.getWindow().getTabs().get(i);
			}
		}
		Tab myTab = new Tab(cp5, cp5.getWindow(), theName);
		cp5.getWindow().getTabs().add(myTab);
		return myTab;
	}

	public Button addButton(final Object theObject, String theIndex, final String theName, final float theValue, final int theX, final int theY, final int theW, final int theH) {
		Button myController = new Button(cp5, (ControllerGroup<?>) cp5.controlWindow.getTabs().get(1), theName, theValue, theX, theY, theW, theH);
		cp5.register(theObject, theIndex, myController);
		myController.registerProperty("value");
		myController.getProperty("value").disable();
		return myController;
	}

	public Bang addBang(final String theName) {
		return addBang(null, "", theName);
	}

	public Bang addBang(final Object theObject, String theIndex, final String theName, final int theX, final int theY, final int theWidth, final int theHeight) {
		Bang myController = new Bang(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, theX, theY, theWidth, theHeight);
		cp5.register(theObject, theIndex, myController);
		myController.registerProperty("value");
		myController.getProperty("value").disable();
		return myController;
	}

	public Toggle addToggle(final Object theObject, String theIndex, final String theName, final boolean theDefaultValue, final float theX, final float theY, final int theWidth, final int theHeight) {
		Toggle myController = new Toggle(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, (theDefaultValue == true) ? 1f : 0f, theX, theY, theWidth, theHeight);
		cp5.register(theObject, theIndex, myController);
		myController.registerProperty("value");
		return myController;
	}

	public Tooltip addTooltip() {
		return null;
	}

	/**
	 * Matrix is a 2-D matrix controller using toggle controllers in a rows and a columns setup. useful for software drum machines.
	 */

	public Matrix addMatrix(final Object theObject, final String theIndex, final String theName, final int theCellX, final int theCellY, final int theX, final int theY, final int theWidth,
			final int theHeight) {
		Matrix myController = new Matrix(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, theCellX, theCellY, theX, theY, theWidth, theHeight);
		cp5.register(theObject, theIndex, myController);
		myController.registerProperty("cells").registerProperty("interval");
		return myController;
	}

	public Matrix addMatrix(final String theName) {
		return addMatrix(theName, 10, 10, 0, 0, 100, 100);
	}

	public Matrix addMatrix(final String theName, final int theCellX, final int theCellY, final int theX, final int theY, final int theWidth, final int theHeight) {
		return addMatrix(null, "", theName, theCellX, theCellY, theX, theY, theWidth, theHeight);
	}

	public Slider2D addSlider2D(final String theName) {
		return addSlider2D(null, "", theName, 0, 99, 0, 99, 0, 0, 0, 0, 99, 99);
	}

	/**
	 * Adds a 2D slider to controlP5. A 2D slider is a 2D area with 1 cursor returning its xy coordinates.
	 */
	public Slider2D addSlider2D(Object theObject, final String theIndex, final String theName, float theMinX, float theMaxX, float theMinY, float theMaxY, float theDefaultValueX,
			float theDefaultValueY, int theX, int theY, int theW, int theH) {
		Slider2D myController = new Slider2D(cp5, (ControllerGroup<?>) cp5.controlWindow.getTabs().get(1), theName, theX, theY, theW, theH);
		cp5.register(theObject, theIndex, myController);
		myController.setMinX(theMinX);
		myController.setMaxX(theMaxX);
		myController.setMinY(theMinY);
		myController.setMaxY(theMaxY);
		myController.setArrayValue(new float[] { theDefaultValueX, theDefaultValueY });
		myController.updateValue();
		myController.registerProperty("arrayValue").registerProperty("minX").registerProperty("maxX").registerProperty("minY").registerProperty("maxY");
		return myController;
	}

	/**
	 * Adds a slider to controlP5. by default it will be added to the default tab of the main window. Sliders can be arranged vertically and
	 * horizontally depending on their width and height. The look of a sliders control can either be a bar or a handle. you can add
	 * tickmarks to a slider or use the default free-control setting. A slider can be controller by mouse click, drag or mouse-wheel.
	 */
	public Slider addSlider(Object theObject, final String theIndex, final String theName, float theMin, float theMax, float theDefaultValue, int theX, int theY, int theW, int theH) {
		Slider myController = new Slider(cp5, (ControllerGroup<?>) cp5.controlWindow.getTabs().get(1), theName, theMin, theMax, theDefaultValue, theX, theY, theW, theH);
		cp5.register(theObject, theIndex, myController);
		myController.registerProperty("value").registerProperty("min").registerProperty("max");
		return myController;
	}

	public Slider addSlider(String theName, float theMin, float theMax, float theDefaultValue, int theX, int theY, int theW, int theH) {
		return addSlider(null, "", theName, theMin, theMax, theDefaultValue, theX, theY, theW, theH);
	}

	public Slider addSlider(final String theName, final float theMin, final float theMax, final int theX, final int theY, final int theWidth, final int theHeight) {
		return addSlider(null, "", theName, theMin, theMax, theMin, theX, theY, theWidth, theHeight);
	}

	public Slider addSlider(Object theObject, final String theIndex, final String theName, float theMin, float theMax, int theX, int theY, int theW, int theH) {
		return addSlider(theObject, theIndex, theName, theMin, theMax, theMin, theX, theY, theW, theH);
	}

	public Range addRange(final String theName) {
		return addRange(theName, 0, 100, 0, 100, 0, 0, 99, 9);
	}

	/**
	 * A range controller, a slider that allows control on both ends of the slider.
	 */
	public Range addRange(Object theObject, final String theIndex, String theName, float theMin, float theMax, float theDefaultMinValue, float theDefaultMaxValue, int theX, int theY, int theW,
			int theH) {
		Range myController = new Range(cp5, (ControllerGroup<?>) cp5.controlWindow.getTabs().get(1), theName, theMin, theMax, theDefaultMinValue, theDefaultMaxValue, theX, theY, theW, theH);
		cp5.register(theObject, theIndex, myController);
		myController.registerProperty("lowValue").registerProperty("highValue");
		return myController;
	}

	public Range addRange(String theName, float theMin, float theMax, float theDefaultMinValue, float theDefaultMaxValue, int theX, int theY, int theW, int theH) {
		return addRange(null, "", theName, theMin, theMax, theDefaultMinValue, theDefaultMaxValue, theX, theY, theW, theH);
	}

	public Range addRange(final String theName, final float theMin, final float theMax, final int theX, final int theY, final int theWidth, final int theHeight) {
		return addRange(null, "", theName, theMin, theMax, theMin, theMax, theX, theY, theWidth, theHeight);
	}

	public Range addRange(final Object theObject, final String theIndex, final String theName, final float theMin, final float theMax, final int theX, final int theY, final int theWidth,
			final int theHeight) {
		return addRange(theObject, theIndex, theName, theMin, theMax, theMin, theMax, theX, theY, theWidth, theHeight);
	}

	public Numberbox addNumberbox(final Object theObject, final String theIndex, final String theName, final float theDefaultValue, final int theX, final int theY, final int theWidth,
			final int theHeight) {
		Numberbox myController = new Numberbox(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, theDefaultValue, theX, theY, theWidth, theHeight);
		cp5.register(theObject, theIndex, myController);
		myController.registerProperty("value");
		return myController;
	}

	public Numberbox addNumberbox(final String theName, final int theX, final int theY, final int theWidth, final int theHeight) {
		return addNumberbox(null, "", theName, Float.NaN, theX, theY, theWidth, theHeight);
	}

	public Numberbox addNumberbox(final Object theObject, final String theIndex, final String theName, final int theX, final int theY, final int theWidth, final int theHeight) {
		return addNumberbox(theObject, theIndex, theName, Float.NaN, theX, theY, theWidth, theHeight);
	}

	public Numberbox addNumberbox(final String theName, final float theDefaultValue, final int theX, final int theY, final int theWidth, final int theHeight) {
		return addNumberbox(null, "", theName, theDefaultValue, theX, theY, theWidth, theHeight);
	}

	/**
	 * Knobs can use limited and endless revolutions, custom angles and starting points. There are 2 control areas for a knob, an area
	 * closer to the edge allows 'click-and-adjust' control, a click and drag action at the inside allows to gradually change the value of a
	 * know when dragged. A knob can be controller by mouse click, drag or mouse-wheel.
	 */
	public Knob addKnob(final Object theObject, final String theIndex, final String theName, final float theMin, final float theMax, final float theDefaultValue, final int theX, final int theY,
			final int theDiameter) {
		Knob myController = new Knob(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, theMin, theMax, theDefaultValue, theX, theY, theDiameter);
		cp5.register(theObject, theIndex, myController);
		myController.registerProperty("value");
		return myController;
	}

	public Knob addKnob(final String theName, final float theMin, final float theMax, final int theX, final int theY, final int theDiameter) {
		return addKnob(null, "", theName, theMin, theMax, theMin, theX, theY, theDiameter);
	}

	public Knob addKnob(final Object theObject, final String theIndex, final String theName, final float theMin, final float theMax, final int theX, final int theY, final int theDiameter) {
		return addKnob(theObject, theIndex, theName, theMin, theMax, theX, theY, theDiameter);
	}

	public Knob addKnob(final String theName, final float theMin, final float theMax, final float theDefaultValue, final int theX, final int theY, final int theDiameter) {
		return addKnob(null, "", theName, theMin, theMax, theDefaultValue, theX, theY, theDiameter);
	}

	/**
	 * Multilist is a tree like menu.
	 */
	public MultiList addMultiList(final Object theObject, final String theIndex, final String theName, final int theX, final int theY, final int theWidth, final int theHeight) {
		MultiList myController = new MultiList(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, theX, theY, theWidth, theHeight);
		cp5.register(theObject, theIndex, myController);
		myController.registerProperty("value");
		return myController;
	}

	public MultiList addMultiList(final String theName, final int theX, final int theY, final int theWidth, final int theHeight) {
		return addMultiList(null, "", theName, theX, theY, theWidth, theHeight);
	}

	public Textlabel addTextlabel(final String theName) {
		return addTextlabel(theName, "", 0, 0);
	}

	public Textlabel addTextlabel(final Object theObject, final String theIndex, final String theName, final String theText, final int theX, final int theY) {
		Textlabel myController = new Textlabel(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, theText, theX, theY);
		cp5.register(theObject, theIndex, myController);
		myController.registerProperty("value").registerProperty("stringValue");
		return myController;
	}

	public Textlabel addTextlabel(final String theName, final String theText, final int theX, final int theY) {
		return addTextlabel(null, "", theName, theText, theX, theY);
	}

	public Textlabel addTextlabel(final Object theObject, final String theIndex, final String theName, final String theText) {
		return addTextlabel(theObject, theIndex, theName, theText, 0, 0);
	}

	public Textlabel addTextlabel(final String theName, final String theText) {
		return addTextlabel(null, "", theName, theText, 0, 0);
	}

	public Textarea addTextarea(final String theName) {
		return addTextarea(theName, "", 0, 0, 199, 99);
	}

	/**
	 * A Textarea is a label without any controller functionality and can be used to leave notes, headlines, etc when extending the
	 * dedicated area of the Textrea, a scrollbar is added on the right.
	 */
	public Textarea addTextarea(final String theName, final String theText, final int theX, final int theY, final int theW, final int theH) {
		Textarea myController = new Textarea(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, theText, theX, theY, theW, theH);
		cp5.register(null, "", myController);
		myController.registerProperty("text");
		return myController;
	}

	public Textfield addTextfield(final String theIndex) {
		return addTextfield(theIndex, 0, 0, 199, 19);
	}

	/**
	 * A Textfield allows single line text input. If text goes beyond the edges of a Textfield box, the text will automatically scroll. Use
	 * Arrow keys to navigate back and forth.
	 */
	public Textfield addTextfield(final Object theObject, final String theIndex, final String theName, final int theX, final int theY, final int theW, final int theH) {
		Textfield myController = new Textfield(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, "", theX, theY, theW, theH);
		cp5.register(theObject, theIndex, myController);
		myController.registerProperty("text");
		return myController;
	}

	public Textfield addTextfield(final String theName, final int theX, final int theY, final int theW, final int theH) {
		return addTextfield(null, "", theName, theX, theY, theW, theH);
	}

	public Textfield addTextfield(final Object theObject, final String theIndex, final String theName) {
		return addTextfield(theObject, theIndex, theName, 0, 0, 99, 19);
	}

	// TODO
	// addTextarea theObject

	public Accordion addAccordion(String theName) {
		Accordion myController = new Accordion(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, 0, 0, 200);
		cp5.register(null, "", myController);
		return myController;
	}

	public Accordion addAccordion(String theName, int theX, int theY, int theWidth) {
		Accordion myController = new Accordion(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, theX, theY, theWidth);
		cp5.register(null, "", myController);
		return myController;
	}

	/**
	 * Use radio buttons for multiple choice options.
	 */
	public RadioButton addRadioButton(final String theName) {
		return addRadioButton(theName, 0, 0);
	}

	public RadioButton addRadioButton(final String theName, final int theX, final int theY) {
		RadioButton myController = new RadioButton(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, theX, theY);
		cp5.register(null, "", myController);
		myController.registerProperty("arrayValue");
		return myController;
	}

	/**
	 * Use radio buttons for multiple choice options.
	 */
	public RadioButton addRadio(final String theName) {
		return addRadioButton(theName, 0, 0);
	}

	public RadioButton addRadio(final String theName, final int theX, final int theY) {
		RadioButton myController = new RadioButton(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, theX, theY);
		cp5.register(null, "", myController);
		myController.registerProperty("arrayValue");
		return myController;
	}

	// TODO
	// addRadioButton theObject

	/**
	 * Use a checkbox for single choice options
	 */
	public CheckBox addCheckBox(final String theName) {
		return addCheckBox(theName, 0, 0);
	}

	public CheckBox addCheckBox(final String theName, final int theX, final int theY) {
		CheckBox myController = new CheckBox(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, theX, theY);
		cp5.register(null, "", myController);
		myController.registerProperty("arrayValue");
		return myController;
	}

	// TODO
	// addCheckBox theObject

	public ListBox addListBox(final String theName) {
		return addListBox(theName, 0, 0, 99, 199);
	}

	/**
	 * A list box is a list of items a user can choose from. When items exceed the dedicated area of a list box, a scrollbar is added to the
	 * right of the box. the Box can be navigated using mouse click, drag and the mouse-wheel.
	 */
	public ListBox addListBox(final String theName, final int theX, final int theY, final int theW, final int theH) {
		ListBox myController = new ListBox(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, theX, theY, theW, theH);
		cp5.register(null, "", myController);
		myController.registerProperty("listBoxItems").registerProperty("value");
		return myController;
	}

	// TODO
	// addListBox theObject

	public DropdownList addDropdownList(final String theName) {
		return addDropdownList(theName, 0, 0, 99, 99);
	}

	public DropdownList addDropdownList(final String theName, final int theX, final int theY, final int theW, final int theH) {
		DropdownList myController = new DropdownList(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, theX, theY, theW, theH);
		cp5.register(null, "", myController);
		myController.registerProperty("listBoxItems").registerProperty("value");
		return myController;
	}

	// TODO
	// addDropdownList theObject

	public ColorPicker addColorPicker(final String theName) {
		return addColorPicker(theName, 0, 0, 255, 10);
	}

	/**
	 * adds a simple RGBA colorpicker.
	 */
	public ColorPicker addColorPicker(final String theName, final int theX, final int theY, final int theW, final int theH) {
		ColorPicker myController = new ColorPicker(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, theX, theY, theW, theH);
		cp5.register(null, "", myController);
		myController.registerProperty("arrayValue");
		return myController;
	}

	public Println addConsole(Textarea theTextarea) {
		return new Println(theTextarea);
	}

	public FrameRate addFrameRate() {
		FrameRate myController = new FrameRate(cp5, (Tab) cp5.controlWindow.getTabs().get(1), "-", 0, 4);
		cp5.register(null, "", myController);
		return myController;
	}

	// TODO
	// addColorPicker theObject

	public Chart addChart(String theName) {
		return addChart(theName, 0, 0, 200, 100);
	}

	/**
	 * adds chart support to display float array based data.
	 */
	public Chart addChart(String theName, int theX, int theY, int theW, int theH) {
		Chart myController = new Chart(cp5, (Tab) cp5.controlWindow.getTabs().get(1), theName, theX, theY, theW, theH);
		cp5.register(null, "", myController);
		return myController;
	}

	/**
	 * A controller group can be used to group controllers for a better organization of single controllers.
	 */
	public Group addGroup(Object theObject, final String theIndex, String theName, int theX, int theY, int theW) {
		Group myController = new Group(cp5, (ControllerGroup<?>) cp5.controlWindow.getTabs().get(1), theName, theX, theY, theW, 9);
		cp5.register(theObject, theIndex, myController);
		return myController;
	}

	public Group addGroup(String theName, int theX, int theY, int theW) {
		return addGroup(null, "", theName, theX, theY, theW);
	}

	public Group addGroup(String theName) {
		return addGroup(theName, 0, 0);
	}

	public Group addGroup(String theName, int theX, int theY) {
		return addGroup(null, "", theName, theX, theY, 99);
	}

	public Textlabel getTextlabel(String theText, int theX, int theY) {
		return new Textlabel(cp5, theText, theX, theY);
	}

	public Textlabel getTextlabel() {
		return getTextlabel("", 0, 0);
	}

	protected void setCurrentPointer(ControllerGroup<?> theGroup) {
		currentGroupPointer = theGroup;
		isCurrentGroupPointerClosed = false;
	}

	protected void releaseCurrentPointer(ControllerGroup<?> theGroup) {
		if (isCurrentGroupPointerClosed == false) {
			currentGroupPointer = theGroup;
			isCurrentGroupPointerClosed = true;
		} else {
			ControlP5.logger().warning("use .end() first before using .begin() again.");
		}
	}

	public void setAutoAddDirection(int theDirection) {
		if (theDirection == HORIZONTAL) {
			autoDirection = HORIZONTAL;
			return;
		}
		autoDirection = VERTICAL;
	}

	public void setAutoSpacing() {
		Controller.autoSpacing.x = 10;
		Controller.autoSpacing.y = 10;
		Controller.autoSpacing.z = 0;
	}

	public void setAutoSpacing(float theX) {
		Controller.autoSpacing.x = theX;
	}

	public void setAutoSpacing(float theX, float theY) {
		Controller.autoSpacing.x = theX;
		Controller.autoSpacing.y = theY;
	}

	public void setAutoSpacing(float theX, float theY, float theZ) {
		Controller.autoSpacing.x = theX;
		Controller.autoSpacing.y = theY;
		Controller.autoSpacing.z = theZ;
	}

	@SuppressWarnings("static-access") protected void linebreak(Controller<?> theController, boolean theFlag, int theW, int theH, PVector theSpacing) {
		if (currentGroupPointer.autoPosition.x + theController.autoSpacing.x + theW > cp5.papplet.width) {
			currentGroupPointer.autoPosition.y += currentGroupPointer.tempAutoPositionHeight;
			currentGroupPointer.autoPosition.x = currentGroupPointer.autoPositionOffsetX;
			currentGroupPointer.tempAutoPositionHeight = 0;
			theController.position.x = currentGroupPointer.autoPosition.x;
			theController.position.y = currentGroupPointer.autoPosition.y;
			theFlag = false;
		}

		if (theFlag == true) {
			currentGroupPointer.autoPosition.y += currentGroupPointer.tempAutoPositionHeight;
			currentGroupPointer.autoPosition.x = currentGroupPointer.autoPositionOffsetX;
			currentGroupPointer.tempAutoPositionHeight = 0;
		} else {
			if (theController instanceof Slider) {
				currentGroupPointer.autoPosition.x += theController.getCaptionLabel().getWidth();
			}
			currentGroupPointer.autoPosition.x += theController.autoSpacing.x + theW;
			if ((theH + theSpacing.y) > currentGroupPointer.tempAutoPositionHeight) {
				currentGroupPointer.tempAutoPositionHeight = theH + theSpacing.y;
			}
		}
	}

	/**
	 * Adds a default slider with a default width of 100 and height of 10. the default value range is from 0-100.
	 */
	public Slider addSlider(String theName) {
		return addSlider(theName, 0, 100);
	}

	public Slider addSlider(Object theObject, final String theIndex, String theName) {
		return addSlider(theObject, theIndex, theName, 0, 100);
	}

	public Slider addSlider(String theName, float theMin, float theMax) {
		return addSlider(null, "", theName, theMin, theMax);
	}

	public Slider addSlider(Object theObject, final String theIndex, String theName, float theMin, float theMax) {
		int x = (int) currentGroupPointer.autoPosition.x;
		int y = (int) currentGroupPointer.autoPosition.y;
		Slider s = addSlider(theObject, theIndex, theName, theMin, theMax, theMin, x, y, Slider.autoWidth, Slider.autoHeight);
		linebreak(s, false, Slider.autoWidth, Slider.autoHeight, Slider.autoSpacing);
		s.moveTo(currentGroupPointer);
		if (autoDirection == VERTICAL) {
			s.linebreak();
		}
		return s;
	}

	/**
	 * Adds a default Button, the default value is 1, width and height are set to the default values of Button.autoWidth and
	 * Button.autoHeight
	 */
	public Button addButton(String theName) {
		return addButton(null, "", theName, 1);
	}

	public Button addButton(Object theObject, final String theIndex, String theName) {
		return addButton(theObject, theIndex, theName, 1);
	}

	public Button addButton(String theName, float theValue) {
		return addButton(null, "", theName, theValue);
	}

	public Button addButton(Object theObject, final String theIndex, String theName, float theValue) {
		int x = (int) currentGroupPointer.autoPosition.x;
		int y = (int) currentGroupPointer.autoPosition.y;
		Button b = addButton(theObject, theIndex, theName, theValue, x, y, Button.autoWidth, Button.autoHeight);
		linebreak(b, false, Button.autoWidth, Button.autoHeight, Button.autoSpacing);
		b.moveTo(currentGroupPointer);
		return b;
	}

	public Bang addBang(Object theObject, final String theIndex, String theName) {
		int x = (int) currentGroupPointer.autoPosition.x;
		int y = (int) currentGroupPointer.autoPosition.y;
		Bang b = addBang(theObject, theIndex, theName, x, y, Bang.autoWidth, Bang.autoHeight);
		linebreak(b, false, Bang.autoWidth, Bang.autoHeight, Bang.autoSpacing);
		b.moveTo(currentGroupPointer);
		return b;
	}

	/**
	 * Adds a default Toggle, the default value is false, width and height are set to the default values of Toggle.autoWidth and
	 * Toggle.autoHeight
	 */
	public Toggle addToggle(String theName) {
		return addToggle(null, "", theName);
	}

	public Toggle addToggle(String theName, boolean theValue) {
		return addToggle(null, "", theName, theValue);
	}

	public Toggle addToggle(Object theObject, final String theIndex, String theName) {
		return addToggle(theObject, theIndex, theName, false);
	}

	public Toggle addToggle(Object theObject, final String theIndex, String theName, boolean theValue) {
		Toggle t = addToggle(theObject, theIndex, theName, theValue, currentGroupPointer.autoPosition.x, currentGroupPointer.autoPosition.y, Toggle.autoWidth, Toggle.autoHeight);
		linebreak(t, false, Toggle.autoWidth, Toggle.autoHeight, t.autoSpacing);
		t.moveTo(currentGroupPointer);
		return t;
	}

	public Numberbox addNumberbox(Object theObject, final String theIndex, String theName) {
		int x = (int) currentGroupPointer.autoPosition.x;
		int y = (int) currentGroupPointer.autoPosition.y;
		Numberbox n = addNumberbox(theObject, theIndex, theName, x, y, Numberbox.autoWidth, Numberbox.autoHeight);
		linebreak(n, false, Numberbox.autoWidth, Numberbox.autoHeight, n.autoSpacing);
		n.moveTo(currentGroupPointer);
		return n;
	}

	public Numberbox addNumberbox(String theName) {
		return addNumberbox(null, "", theName);
	}

	public Knob addKnob(Object theObject, final String theIndex, String theName, int theMin, int theMax) {
		Knob n = addKnob(theObject, theIndex, theName, theMin, theMax, theMin, (int) currentGroupPointer.autoPosition.x, (int) currentGroupPointer.autoPosition.y, Knob.autoWidth);
		linebreak(n, false, Knob.autoWidth, Knob.autoHeight, n.autoSpacing);
		n.moveTo(currentGroupPointer);
		return n;
	}

	public Knob addKnob(String theName) {
		return addKnob(theName, 0, 100);

	}

	public Knob addKnob(Object theObject, final String theIndex, String theName) {
		return addKnob(theObject, theIndex, theName, 0, 100);
	}

	public Knob addKnob(String theName, int theMin, int theMax) {
		return addKnob(null, "", theName, theMin, theMax);
	}

	/**
	 * 
	 */
	public ControlWindow addControlWindow(String theName) {
		// TODO re-implement ControlWindow
		ControlP5.logger().warning("ControlWindow has been disabled currently, please have a look at the changlog.txt file inside the src folder.");
		return null;
		// return addControlWindow(theName, 20, 20, 300, 400);
	}

	/**
	 * Adds Controllers by Object reference, currently supports Slider, Bang, Button, Knob, Numberbox, Toggle, Textlabel, Textfield, Range,
	 * Slider2D. For internal use rather than on application level.
	 */
	public <C> C addController(final Object theObject, final String theIndex, final String theName, final Class<C> theClass, int theX, int theY) {
		Controller<?> c = null;
		if (theClass.equals(Slider.class)) {
			c = addSlider(theObject, theIndex, theName, 0, 100, 0, 0, 0, 99, 9);
		} else if (theClass.equals(Bang.class)) {
			c = addBang(theObject, theIndex, theName, 0, 0, 19, 19);
		} else if (theClass.equals(Button.class)) {
			c = addButton(theObject, theIndex, theName, 0, 0, 0, 49, 19);
		} else if (theClass.equals(Knob.class)) {
			c = addKnob(theObject, theIndex, theName, 0, 100, 0, 0, 0, 49);
		} else if (theClass.equals(Numberbox.class)) {
			c = addNumberbox(theObject, theIndex, theName, 0, 0, 0, 99, 19);
		} else if (theClass.equals(Toggle.class)) {
			c = addToggle(theObject, theIndex, theName, false, 0, 0, 49, 19);
		} else if (theClass.equals(Textfield.class)) {
			c = addTextfield(theObject, theIndex, theName, 0, 0, 99, 19);
		} else if (theClass.equals(Range.class)) {
			c = addRange(theObject, theIndex, theName, 0, 100, 0, 100, 0, 0, 99, 9);
		} else if (theClass.equals(Slider2D.class)) {
			c = addSlider2D(theObject, theIndex, theName, 0, 100, 0, 100, 0, 0, 0, 0, 99, 99);
		} else if (theClass.equals(Textlabel.class)) {
			c = addTextlabel(theName, "<empty>");
		}
		// TODO MultiList, Matrix
		c.setPosition(theX, theY);
		return (C) c;
	}

	/**
	 * Use with caution, only for internal use.
	 * 
	 * @exclude
	 */
	@ControlP5.Invisible public <C> C addGroup(final Object theObject, final String theIndex, final String theName, final Class<C> theClass, int theX, int theY, int theW, int theH) {
		ControlGroup<?> c = null;
		if (theClass.equals(DropdownList.class)) {
			c = addDropdownList(theName, theX, theY, theW, theH);
		} else if (theClass.equals(ListBox.class)) {
			c = addListBox(theName, theX, theY, theW, theH);
		} else if (theClass.equals(ListBox.class)) {
			c = addListBox(theName, theX, theY, theW, theH);
		} else if (theClass.equals(RadioButton.class)) {
			c = addRadioButton(theName, theX, theY);
		} else if (theClass.equals(CheckBox.class)) {
			c = addCheckBox(theName, theX, theY);
		} else if (theClass.equals(ControlGroup.class)) {
			c = addGroup(theName, theX, theY);
		} else if (theClass.equals(Group.class)) {
			c = addGroup(theName, theX, theY);
		}
		c.setPosition(theX, theY);
		c.setWidth(theW);
		c.setHeight(theH);
		return (C) c;
	}

	public <C> C addController(String theName, Class<C> theClass, int theX, int theY) {
		return addController(null, "", theName, theClass, theX, theY);
	}

	public ControlP5Base addControllersFor(PApplet theApplet) {
		addControllersFor("", theApplet);
		return this;
	}

	/**
	 * Adds controllers for a specific object using annotations.
	 * <p>
	 * Uses a forward slash delimited address, for example:
	 * </p>
	 * <p>
	 * lets say the theAddressSpace parameter is set to "hello", and the Object (second parameter) contains an annotated field "x",
	 * addControllersFor("hello", o); will add a controller for field x with address /hello/x This address can be used with
	 * getController("/hello/x") to access the controller of that particular Object and field.
	 * </p>
	 */
	public ControlP5Base addControllersFor(final String theAddressSpace, Object t) {
		_myAutomator.addControllersFor(theAddressSpace, t);
		return this;
	}

	public Controller<?> getController(String theName, Object theObject) {
		if (_myObjectToControllerMap.containsKey(theObject)) {
			ArrayList<ControllerInterface<?>> cs = _myObjectToControllerMap.get(theObject);
			for (ControllerInterface<?> c : cs) {
				if (c.getName().equals(theName)) {
					return (Controller<?>) c;
				}
			}
		}
		return null;
	}

	@SuppressWarnings("unchecked") public Object getObjectForController(ControllerInterface theController) {
		for (Iterator it = _myObjectToControllerMap.entrySet().iterator(); it.hasNext();) {
			Map.Entry entry = (Map.Entry) it.next();
			Object key = entry.getKey();
			ArrayList<ControllerInterface> value = (ArrayList<ControllerInterface>) entry.getValue();
			for (ControllerInterface c : value) {
				if (c.equals(theController)) {
					return key;
				}
			}
		}
		return null;
	}

	public ControlP5Base setPosition(int theX, int theY, Object o) {
		if (o != null && _myObjectToControllerMap.containsKey(o)) {
			ArrayList<ControllerInterface<?>> cs = _myObjectToControllerMap.get(o);
			for (ControllerInterface<?> c : cs) {
				int x = (int) c.getPosition().x + theX;
				int y = (int) c.getPosition().y + theY;
				c.setPosition(x, y);
			}
		}
		return this;
	}

	public ControlP5Base hide(Object theObject) {
		if (theObject != null && _myObjectToControllerMap.containsKey(theObject)) {
			ArrayList<ControllerInterface<?>> cs = _myObjectToControllerMap.get(theObject);
			for (ControllerInterface<?> c : cs) {
				c.hide();
			}
		}
		return this;
	}

	public ControlP5Base show(Object theObject) {
		if (theObject != null && _myObjectToControllerMap.containsKey(theObject)) {
			ArrayList<ControllerInterface<?>> cs = _myObjectToControllerMap.get(theObject);
			for (ControllerInterface<?> c : cs) {
				c.show();
			}
		}
		return this;
	}

	public ControlP5Base remove(Object theObject) {
		if (theObject != null && _myObjectToControllerMap.containsKey(theObject)) {
			ArrayList<ControllerInterface<?>> cs = _myObjectToControllerMap.get(theObject);
			for (ControllerInterface<?> c : cs) {
				c.remove();
			}
		}
		return this;
	}

	public ControlP5Base setColor(CColor theColor, Object theObject) {
		if (_myObjectToControllerMap.containsKey(theObject)) {
			ArrayList<ControllerInterface<?>> cs = _myObjectToControllerMap.get(theObject);
			for (ControllerInterface<?> c : cs) {
				c.setColor(theColor);
			}
		}
		return this;
	}

	public ControlP5Base listenTo(String theFieldName, Object theObject) {
		String key = theObject.hashCode() + "" + theFieldName.hashCode();
		FieldChangedListener value = new FieldChangedListener(cp5);
		value.listenTo(theObject, theFieldName);
		_myFieldChangedListenerMap.put(key, value);
		return this;
	}

	public ControlP5Base stopListeningTo(String theFieldName, Object theObject) {
		String key = theObject.hashCode() + "" + theFieldName.hashCode();
		_myFieldChangedListenerMap.remove(key);
		return this;
	}

	public ControlP5Base moveTo(ControllerGroup<?> theController, Object theObject) {
		if (_myObjectToControllerMap.containsKey(theObject)) {
			ArrayList<ControllerInterface<?>> cs = _myObjectToControllerMap.get(theObject);
			for (ControllerInterface<?> c : cs) {
				c.moveTo(theController);
			}
		}
		return this;
	}

	/* Properties */

	public ControllerProperties getProperties() {
		return _myProperties;
	}

	public void removeProperty(ControllerInterface<?> theController) {
		_myProperties.remove(theController);
	}

	/**
	 * prints a list of public methods of requested class into the console. You can specify patterns that will print methods found with only
	 * these particular patterns in their name.
	 * <p>
	 * printed Format: returnType methodName(parameter type)
	 */
	public static void printPublicMethodsFor(Class<?> theClass, String... thePattern) {
		Set<String> set = getPublicMethodsFor(theClass, true, thePattern);
		String str = "";

		str += "/**\n";
		str += "* ControlP5 " + theClass.getSimpleName() + "\n";
		str += "*\n";
		str += "*\n";
		str += "* find a list of public methods available for the " + theClass.getSimpleName() + " Controller\n";
		str += "* at the bottom of this sketch.\n";
		str += "*\n";
		str += "* by Andreas Schlegel, 2012\n";
		str += "* www.sojamo.de/libraries/controlp5\n";
		str += "*\n";
		str += "*/\n\n";
		str += "/*\n";
		str += "a list of all methods available for the " + theClass.getSimpleName() + " Controller\n";
		str += "use ControlP5.printPublicMethodsFor(" + theClass.getSimpleName() + ".class);\n";
		str += "to print the following list into the console.\n\n";
		str += "You can find further details about class " + theClass.getSimpleName() + " in the javadoc.\n\n";
		str += "Format:\n";
		str += "ClassName : returnType methodName(parameter type)\n\n\n";
		for (String s : set) {
			str += s + "\n";
		}
		str += "\n\n*/\n\n";
		System.out.println(str);
	}

	public static void printPublicMethodsFor(Class<?> theClass) {
		printPublicMethodsFor(theClass, "");
	}

	public static Set<String> getPublicMethodsFor(Class<?> theClass) {
		return getPublicMethodsFor(theClass, true, "");
	}

	public static Set<String> getPublicMethodsFor(Class<?> theClass, String... thePattern) {
		return getPublicMethodsFor(theClass, true, thePattern);
	}

	public static Set<String> getPublicMethodsFor(Class<?> theClass, boolean theFlag) {
		return getPublicMethodsFor(theClass, true, "");
	}

	public static Set<String> getPublicMethodsFor(Class<?> theClass, boolean isSuperclass, String... thePattern) {
		Set<String> s = new TreeSet<String>();

		Class<?> c = theClass;
		while (c != null) {
			for (Method method : c.getDeclaredMethods()) {
				if (!method.isAnnotationPresent(Deprecated.class) && !method.isAnnotationPresent(ControlP5.Invisible.class) && method.getModifiers() == Modifier.PUBLIC) {
					for (String p : thePattern) {
						if (p.length() > 0) {
							if (!method.getName().toLowerCase().contains(p.toLowerCase())) {
								continue;
							}
						}
						String params = "";
						for (Class<?> t : method.getParameterTypes()) {
							params += t.getSimpleName() + ", ";
						}
						if (params.length() > 0) {
							params = params.substring(0, params.length() - 2);
						}
						s.add(c.getCanonicalName() + " : " + method.getReturnType().getSimpleName().replace("Object", theClass.getSimpleName()) + " " + method.getName() + "(" + params + ") ");
					}
				}
			}

			if (isSuperclass) {
				c = c.getSuperclass();
			} else {
				c = null;
			}
		}
		return s;
	}

	public int getKeyCode() {
		return cp5.getWindow().keyCode;
	}

	public char getKey() {
		return cp5.getWindow().key;
	}

	private char[] fromIntToChar(int... theChar) {
		char[] n = new char[theChar.length];
		for (int i = 0; i < n.length; i++) {
			if (theChar[i] >= 'a' && theChar[i] <= 'z') {
				theChar[i] -= 32;
			}
			n[i] = (char) theChar[i];
		}
		return n;
	}

	public ControlP5 removeKeyFor(ControlKey theKey, int... theChar) {
		removeKeyFor(theKey, fromIntToChar(theChar));
		return cp5;
	}

	public ControlP5 mapKeyFor(ControlKey theKey, Object... os) {
		List<Integer> l = new ArrayList<Integer>();
		for (Object o : os) {
			if (o instanceof Integer) {
				l.add((int) (Integer) o);
			} else if (o instanceof Character) {
				char c = ((Character) o);
				if (c >= 'a' && c <= 'z') {
					c -= 32;
				}
				l.add((int) c);
			}
		}

		char[] n = new char[l.size()];
		for (int i = 0; i < l.size(); i++) {
			n[i] = (char) ((int) l.get(i));
		}

		KeyCode kc = new KeyCode(n);
		if (!keymap.containsKey(kc)) {
			keymap.put(kc, new ArrayList<ControlKey>());
		}
		keymap.get(kc).add(theKey);
		cp5.enableShortcuts();
		return cp5;
	}

	public ControlP5 removeKeyFor(ControlKey theKey, char... theChar) {
		List<ControlKey> l = keymap.get(new KeyCode(theChar));
		if (l != null) {
			l.remove(theKey);
		}
		return cp5;
	}

	public ControlP5 removeKeysFor(char... theChar) {
		keymap.remove(new KeyCode(theChar));
		return cp5;
	}

	public ControlP5 removeKeysFor(int... theChar) {
		removeKeysFor(fromIntToChar(theChar));
		return cp5;
	}

	protected int modifiers;

	public boolean isShiftDown() {
		return (modifiers & Event.SHIFT & (cp5.isShortcuts() ? -1 : 1)) != 0;
	}

	public boolean isControlDown() {
		return (modifiers & Event.CTRL & (cp5.isShortcuts() ? -1 : 1)) != 0;
	}

	public boolean isMetaDown() {
		return (modifiers & Event.META & (cp5.isShortcuts() ? -1 : 1)) != 0;
	}

	public boolean isAltDown() {
		return (modifiers & Event.ALT & (cp5.isShortcuts() ? -1 : 1)) != 0;
	}

	static class KeyCode {

		final char[] chars;

		KeyCode(char... theChars) {
			chars = theChars;
			Arrays.sort(chars);
		}

		public String toString() {
			String s = "";
			for (char c : chars) {
				s += c + "(" + ((int) c) + ") ";
			}
			return s;
		}

		public int size() {
			return chars.length;
		}

		public char[] getChars() {
			return chars;
		}

		public char get(int theIndex) {
			if (theIndex >= 0 && theIndex < size()) {
				return chars[theIndex];
			}
			return 0;
		}

		public boolean equals(Object obj) {
			if (!(obj instanceof KeyCode)) {
				return false;
			}

			KeyCode k = (KeyCode) obj;

			if (k.size() != size()) {
				return false;
			}

			for (int i = 0; i < size(); i++) {
				if (get(i) != k.get(i)) {
					return false;
				}
			}
			return true;
		}

		boolean contains(char n) {
			for (char c : chars) {
				if (n == c) {
					return true;
				}
			}
			return false;
		}

		public int hashCode() {
			int hashCode = 0;
			int n = 1;
			for (char c : chars) {
				hashCode += c + Math.pow(c, n++);
			}
			return hashCode;
		}
	}

	@Deprecated public Tab addTab(PApplet theWindow, String theName) {
		return addTab(cp5.controlWindow, theName);
	}

	@Deprecated public Tab addTab(ControlWindow theWindow, String theName) {
		for (int i = 0; i < theWindow.getTabs().size(); i++) {
			if (theWindow.getTabs().get(i).getName().equals(theName)) {
				return (Tab) theWindow.getTabs().get(i);
			}
		}
		Tab myTab = new Tab(cp5, theWindow, theName);
		theWindow.getTabs().add(myTab);
		return myTab;
	}

	@Deprecated public ControlWindow addControlWindow(final String theName, final int theX, final int theY, final int theWidth, final int theHeight, String theRenderer, int theFrameRate) {

		// for (int i = 0; i < cp5.controlWindowList.size(); i++) {
		// if (((ControlWindow) cp5.controlWindowList.get(i)).name().equals(theWindowName)) {
		// ControlP5.logger().warning("ControlWindow with name " + theWindowName + " already exists. overwriting now.");
		// }
		// }
		// PAppletWindow myPAppletWindow = new PAppletWindow(cp5, theWindowName, theX, theY, theWidth, theHeight, theRenderer,
		// theFrameRate);
		// myPAppletWindow.setParent(cp5);
		// myPAppletWindow.setMode(PAppletWindow.ECONOMIC);
		// ControlWindow myControlWindow = new ControlWindow(cp5, myPAppletWindow);
		// cp5.controlWindowList.add(myControlWindow);
		// return myControlWindow;
		return addControlWindow(theName);
	}

	@Deprecated public ControlWindow addControlWindow(final String theWindowName, final int theWidth, final int theHeight) {
		return addControlWindow(theWindowName, 100, 100, theWidth, theHeight, "", 30);
	}

	@Deprecated public ControlWindow addControlWindow(final String theWindowName, final int theX, final int theY, final int theWidth, final int theHeight) {
		return addControlWindow(theWindowName, theX, theY, theWidth, theHeight, "", 30);
	}

	@Deprecated public ControlWindow addControlWindow(final String theWindowName, final int theX, final int theY, final int theWidth, final int theHeight, final int theFrameRate) {
		return addControlWindow(theWindowName, theX, theY, theWidth, theHeight, "", theFrameRate);
	}

	@Deprecated public Slider2D addSlider2D(String theName, int theX, int theY, int theW, int theH) {
		return addSlider2D(null, "", theName, 0, theW, 0, theH, 0, 0, theX, theY, theW, theH);
	}

	@Deprecated public Slider2D addSlider2D(Object theObject, final String theIndex, final String theName, int theX, int theY, int theW, int theH) {
		return addSlider2D(theObject, theIndex, theName, 0, theW, 0, theH, 0, 0, theX, theY, theW, theH);
	}

	@Deprecated public Slider2D addSlider2D(String theName, float theMinX, float theMaxX, float theMinY, float theMaxY, float theDefaultValueX, float theDefaultValueY, int theX, int theY, int theW,
			int theH) {
		return addSlider2D(null, "", theName, theMinX, theMaxX, theMinY, theMaxY, theDefaultValueX, theDefaultValueY, theX, theY, theW, theH);
	}

	@Deprecated public Button addButton(final String theName, final float theValue, final int theX, final int theY, final int theW, final int theH) {
		return addButton(null, "", theName, theValue, theX, theY, theW, theH);
	}

	@Deprecated public Bang addBang(final String theName, final int theX, final int theY) {
		return addBang(null, "", theName, theX, theY, 20, 20);
	}

	@Deprecated public Bang addBang(final String theName, final int theX, final int theY, final int theWidth, final int theHeight) {
		return addBang(null, "", theName, theX, theY, theWidth, theHeight);
	}

	@Deprecated public Toggle addToggle(final String theName, final boolean theDefaultValue, final float theX, final float theY, final int theWidth, final int theHeight) {
		return addToggle(null, "", theName, theDefaultValue, theX, theY, theWidth, theHeight);
	}

	@Deprecated public Toggle addToggle(final String theName, final float theX, final float theY, final int theWidth, final int theHeight) {
		return addToggle(null, "", theName, false, theX, theY, theWidth, theHeight);
	}

	@Deprecated public Toggle addToggle(final Object theObject, final String theIndex, final String theName, final float theX, final float theY, final int theWidth, final int theHeight) {
		return addToggle(theObject, theIndex, theName, false, theX, theY, theWidth, theHeight);
	}

}
