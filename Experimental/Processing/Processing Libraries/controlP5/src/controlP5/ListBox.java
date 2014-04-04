
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
import java.util.Arrays;
import java.util.List;

import processing.core.PApplet;
import processing.event.KeyEvent;

/**
 * A ListBox is a list of vertically aligned items which can be scrolled if required.
 * 
 * @see controlP5.DropdownList
 * @example controllers/ControlP5listBox
 */
public class ListBox extends ControlGroup<ListBox> implements ControlListener {

	protected int _myItemHeight = 13;

	protected int maxButtons = 0;

	protected int _myOriginalBackgroundHeight = 0;

	protected Slider _myScrollbar;

	protected String _myName;

	protected float _myScrollValue = 0;

	protected boolean isScrollbarVisible = true;

	protected int _myScrollbarWidth = 6;

	protected int _myHeight;

	protected List<ListBoxItem> items;

	protected List<Button> buttons;

	protected int spacing = 1;

	protected boolean isMultipleChoice = false;

	protected boolean pulldown;

	protected int itemOffset = 0;

	protected boolean isToUpperCase = true;

	protected boolean bulkadding;


	/**
	 * Convenience constructor to extend ListBox.
	 * 
	 * @example use/ControlP5extendController
	 * @param theControlP5
	 * @param theName
	 */
	public ListBox(ControlP5 theControlP5, String theName) {
		this(theControlP5, theControlP5.getDefaultTab(), theName, 0, 0, 99, 199);
		theControlP5.register(theControlP5.papplet, theName, this);
	}


	protected ListBox(ControlP5 theControlP5, ControllerGroup<?> theGroup, String theName, int theX, int theY, int theW, int theH) {
		super(theControlP5, theGroup, theName, theX, theY, theW, 9);

		items = new ArrayList<ListBoxItem>();

		buttons = new ArrayList<Button>();

		_myWidth = theW;

		_myName = theName;

		// workaround fix see code.goode.com/p/controlp5 issue 7
		_myBackgroundHeight = theH < 10 ? 10 : theH;

		_myScrollbar = new Slider(cp5, _myParent, theName + "Scroller", 0, 1, 1, _myWidth - _myScrollbarWidth, 0, _myScrollbarWidth, _myBackgroundHeight);
		_myScrollbar.setBroadcast(false);
		_myScrollbar.setSliderMode(Slider.FLEXIBLE);
		_myScrollbar.setMoveable(false);
		_myScrollbar.setLabelVisible(false);
		_myScrollbar.setParent(this);
		_myScrollbar.addListener(this);
		_myScrollbar.setVisible(false);
		_myScrollbar.hide();
		_myScrollbar.updateDisplayMode(DEFAULT);
		add(_myScrollbar);
		setHeight(_myBackgroundHeight);
	}


	public ListBox hideScrollbar() {
		isScrollbarVisible = false;
		_myScrollbar.hide();
		return this;
	}


	public ListBox showScrollbar() {
		isScrollbarVisible = true;
		if ((items.size()) * _myItemHeight > _myBackgroundHeight && isScrollbarVisible) {
			_myScrollbar.show();
		}
		return this;
	}


	public boolean isScrollbarEnabled() {
		return isScrollbarVisible;
	}


	public ListBox setScrollbarVisible(boolean theValue) {
		if (theValue) {
			return showScrollbar();
		}
		return hideScrollbar();
	}


	public boolean isScrollable() {
		return _myScrollbar.isVisible();
	}


	/**
	 * scroll the scrollList remotely. values must range between 0 and 1.
	 */
	public ListBox scroll(float theValue) {
		if ((items.size()) * _myItemHeight > _myBackgroundHeight) {
			_myScrollbar.setValue(PApplet.abs(1 - PApplet.min(PApplet.max(0, theValue), 1)));
		}
		return this;
	}


	@ControlP5.Invisible public float getScrollPosition() {
		return _myScrollbar.getValue();
	}


	@ControlP5.Invisible public void scrolled(int theStep) {
		float step = 1.0f / items.size();
		scroll((1 - getScrollPosition()) + (theStep * step));
	}


	protected void scroll() {
		itemOffset = 0;
		if (buttons.size() < items.size() && isScrollbarVisible) {
			_myScrollbar.show();
			itemOffset = (int) Math.abs(_myScrollValue * (items.size() - buttons.size()));
		}
		else {
			_myScrollbar.hide();
		}
		if (!bulkadding) {
			for (int i = 0; i < buttons.size(); i++) {
				ListBoxItem item = items.get(itemOffset + i);
				Button b = buttons.get(i);
				b.getCaptionLabel().toUpperCase(isToUpperCase);
				b.setColor(item.getColor());
				b.getCaptionLabel().set(item.getText());
				b._myValue = item.getValue();
			}
		}
	}


	@ControlP5.Invisible public ListBox updateListBoxItems() {
		scroll();
		return this;
	}


	public ListBox toUpperCase(boolean theFlag) {
		_myLabel.toUpperCase(theFlag);
		isToUpperCase = theFlag;
		updateListBoxItems();
		return this;
	}


	public ListBox setItemHeight(int theHeight) {
		_myItemHeight = theHeight;
		for (int i = 0; i < buttons.size(); i++) {
			buttons.get(i).height = theHeight;
			buttons.get(i).position.y = (theHeight + spacing) * i;
		}
		setHeight(_myOriginalBackgroundHeight);
		return this;
	}


	@Override public ListBox setHeight(int theHeight) {
		_myOriginalBackgroundHeight = theHeight;

		// re-adjust the _myAdjustedListHeight variable based on height change.
		_myBackgroundHeight = (_myOriginalBackgroundHeight / (_myItemHeight + spacing)) * (_myItemHeight + spacing);
		maxButtons = _myBackgroundHeight / (_myItemHeight + spacing);

		int pn = buttons.size();
		int n = (int) (_myBackgroundHeight / (_myItemHeight + spacing));

		if (n < pn) {
			for (int i = buttons.size() - 1; i >= n; i--) {
				cp5.remove(cp5.getController(buttons.get(i).getName()));
				controllers.remove(buttons.get(i));
				buttons.remove(i);
			}
		}
		else if (pn < n) { // increase size of list
			int nn = Math.min(n, items.size());
			nn -= pn;
			addListButton(nn);
		}
		updateBackground();
		scroll();
		return this;
	}


	/**
	 * @exclude {@inheritDoc}
	 */
	@Override @ControlP5.Invisible public ListBox updateInternalEvents(PApplet theApplet) {
		boolean xx = cp5.getWindow().mouseX > getAbsolutePosition().x && cp5.getWindow().mouseX < getAbsolutePosition().x + _myWidth;
		// there is a 1px gap between bar and controllers, so -1 the top-y-position
		boolean yy = cp5.getWindow().mouseY > getAbsolutePosition().y - 1 && cp5.getWindow().mouseY < getAbsolutePosition().y + _myBackgroundHeight;
		isInsideGroup = isOpen() ? xx && yy : false;
		if ((isBarVisible ? isInside : false) || isInsideGroup) {
			cp5.getWindow().setMouseOverController(this);
		}
		return this;
	}


	protected void updateScroll() {
		_myScrollValue = _myScrollbar.getValue();
		_myScrollbar.setValue(_myScrollValue);
		if (buttons.size() < items.size() && isScrollbarVisible) {
			_myScrollbar.show();
		}
		updateBackground();
		scroll();
	}


	protected void updateBackground() {
		if (items.size() * (_myItemHeight + spacing) < _myOriginalBackgroundHeight) {
			_myBackgroundHeight = items.size() * (_myItemHeight + spacing);
		}
		if (buttons.size() < items.size()) {
			_myScrollbar.setHeight(_myBackgroundHeight - spacing);
			_myScrollbar.show();
		}
		else {
			_myScrollbar.hide();
		}
		updateButtonWidth();
	}


	protected void updateButtonWidth() {
		boolean b = (buttons.size() < items.size() && isScrollbarVisible);
		if (b) {
			for (int i = 1; i < controllers.size(); i++) {
				((Button) controllers.get(i)).width = _myWidth - _myScrollbarWidth - 1;
			}
		}
		else {
			for (int i = 1; i < controllers.size(); i++) {
				((Button) controllers.get(i)).width = _myWidth;
			}
		}
	}


	public ListBox setScrollbarWidth(int theWidth) {
		_myScrollbar.setWidth(theWidth);
		_myScrollbarWidth = theWidth;
		setWidth(getWidth());
		return this;
	}


	@Override public ListBox setWidth(int theWidth) {
		_myWidth = theWidth;
		updateButtonWidth();
		_myScrollbar.position.x = _myWidth - _myScrollbarWidth;
		return this;
	}


	public ListBox setSize(int theWidth, int theHeight) {
		setWidth(theWidth);
		setHeight(theHeight);
		return this;
	}


	protected ListBox addListButton(int theNum) {
		for (int i = 0; (i < theNum) && (buttons.size() < maxButtons); i++) {
			int index = buttons.size();
			Button b = new Button(cp5, (ControllerGroup<?>) this, _myName + "Button" + index, index, 0, index * (_myItemHeight + spacing), _myWidth, _myItemHeight);
			b.setMoveable(false);
			add(b);
			cp5.register(null, "", b);
			b.setBroadcast(false);
			b.addListener(this);
			buttons.add(b);
		}
		updateScroll();
		return this;
	}


	public void beginItems() {
		bulkadding = true;
	}


	public void endItems() {
		bulkadding = false;
		scroll();
	}


	/**
	 * Adds an item to the ListBox.
	 * 
	 * @see controlP5.ListBox#removeItem(String,int)
	 * @param theName String
	 * @param theValue int
	 */
	public ListBoxItem addItem(String theName, int theValue) {
		ListBoxItem lbi = new ListBoxItem(this, theName, theValue);
		items.add(lbi);
		addListButton(1);
		return lbi;
	}


	/**
	 * adds a list of items from a string array. when iterating through the array, the index of each
	 * item will be used as value.
	 * 
	 * @param theItems
	 */
	public ListBox addItems(String[] theItems) {
		addItems(Arrays.asList(theItems), 0);
		return this;
	}


	public ListBox addItems(List<?> theItems) {
		addItems(theItems, 0);
		return this;
	}


	public ListBox addItems(List<?> theItems, int theOffset) {
		for (int i = 0; i < theItems.size(); i++) {
			addItem(theItems.get(i).toString(), i + theOffset);
		}
		return this;
	}


	/**
	 * Removes an item from the ListBox using the unique name of the item given when added to the
	 * list.
	 * 
	 * @see controlP5.ListBox#addItem(String,int)
	 * @param theItemName String
	 */
	public ListBox removeItem(String theItemName) {
		try {
			for (int i = items.size() - 1; i >= 0; i--) {
				if ((items.get(i)).name.equals(theItemName)) {
					items.remove(i);
				}
			}
			if ((buttons.size()) > items.size()) {
				String buttonName = ((Button) controllers.get(buttons.size())).getName();
				buttons.remove(cp5.getController(buttonName));
				controllers.remove(cp5.getController(buttonName));
				cp5.remove(buttonName);

			}
			updateScroll();
		} catch (Exception e) {
			ControlP5.logger().finer("ScrollList.removeItem exception:" + e);
		}
		return this;
	}


	/**
	 * returns a listBoxItem by index in the list of items.
	 * 
	 * @param theIndex
	 * @return
	 */
	public ListBoxItem getItem(int theIndex) {
		return items.get(theIndex);
	}


	/**
	 * TODO faulty returns a listBoxItem by name.
	 * 
	 * @param theItemName
	 * @return
	 */
	public ListBoxItem getItem(String theItemName) {
		for (int i = items.size() - 1; i >= 0; i--) {
			if ((items.get(i)).name.equals(theItemName)) {
				return items.get(i);
			}
		}
		return null;
	}


	/**
	 * returns a ListBoxItem based on its Button reference.
	 * 
	 * @param theButton
	 * @return
	 */
	public ListBoxItem getItem(Controller<?> theButton) {
		if (theButton instanceof Button) {
			int n = buttons.indexOf(theButton);
			if (n >= 0) {
				return items.get(n + itemOffset);
			}
		}
		return null;
	}


	/**
	 * @exclude {@inheritDoc}
	 */
	@ControlP5.Invisible public void controlEvent(ControlEvent theEvent) {
		if (theEvent.getController() instanceof Button) {
			try {
				_myValue = theEvent.getController().getValue();
				ControlEvent myEvent = new ControlEvent(this);
				if (pulldown) {
					close();
					setLabel(theEvent.getController().getLabel());
				}
				for (ControlListener cl : _myControlListener) {
					cl.controlEvent(myEvent);
				}
				cp5.getControlBroadcaster().broadcast(myEvent, ControlP5Constants.FLOAT);
				theEvent.getController().onLeave();
				theEvent.getController().setIsInside(false);
				theEvent.getController().setMouseOver(false);
			} catch (Exception e) {
				ControlP5.logger().warning("ListBox.controlEvent exception:" + e);
			}
		}
		else {
			_myScrollValue = -(1 - theEvent.getValue());
			scroll();
		}
	}


	@Override public ListBox setValue(float theValue) {
		for (int i = 1; i < controllers.size(); i++) {
			if (controllers.get(i).getValue() == theValue) {
				controllers.get(i).update();
			}
		}
		return this;
	}


	/**
	 * Adding key support. up and down arrows can be used to scroll listbox or dropdownList,up and
	 * down, use shift+up/down for faster scrolling, use alt+up/down to jump to the top or bottom.
	 * 
	 * @exclude {@inheritDoc}
	 */
	@ControlP5.Invisible public void keyEvent(final KeyEvent theEvent) {
		super.keyEvent(theEvent);
		float x = getAbsolutePosition().x;
		float y = getAbsolutePosition().y;
		boolean b = (getWindow().mouseX > x && getWindow().mouseX < (x + _myWidth) && getWindow().mouseY > (y - getBarHeight()) && getWindow().mouseY < y + _myOriginalBackgroundHeight);
		if (b && isOpen()) {
			float step = (1.0f / (float) items.size());
			if (cp5.isShiftDown()) {
				step *= 10;
			}
			else if (cp5.isAltDown()) {
				step = 1;
			}
			if (theEvent.getAction() == KeyEvent.PRESS) {
				switch (theEvent.getKeyCode()) {
				case (PApplet.UP):
					_myScrollbar.setValue(PApplet.constrain(_myScrollbar.getValue() + step, 0, 1));
					break;
				case (PApplet.DOWN):
					_myScrollbar.setValue(PApplet.constrain(_myScrollbar.getValue() - step, 0, 1));
					break;
				}
			}
		}
	}


	/**
	 * Enables a ListBox to act as a pulldown menu. Alternatively use class PulldownMenu instead.
	 */
	public ListBox actAsPulldownMenu(boolean theValue) {
		pulldown = theValue;
		if (pulldown) {
			close();
		}
		return this;
	}


	/**
	 * Removes all items from a list box
	 */
	public ListBox clear() {
		for (int i = items.size() - 1; i >= 0; i--) {
			items.remove(i);
		}
		items.clear();
		for (int i = buttons.size() - 1; i >= 0; i--) {
			String buttonName = ((Button) controllers.get(buttons.size())).getName();
			buttons.remove(cp5.getController(buttonName));
			controllers.remove(cp5.getController(buttonName));
			cp5.remove(buttonName);
		}
		updateScroll();
		_myBackgroundHeight = 0;
		return this;
	}


	/**
	 * {@inheritDoc}
	 */
	@Override public ListBox setColor(CColor theColor) {
		setColorActive(theColor.getActive());
		setColorForeground(theColor.getForeground());
		setColorBackground(theColor.getBackground());
		setColorLabel(theColor.getCaptionLabel());
		setColorValue(theColor.getValueLabel());
		return this;
	}


	/**
	 * {@inheritDoc}
	 */
	@Override public ListBox setColorActive(int theColor) {
		super.setColorActive(theColor);
		for (int i = 0; i < items.size(); i++) {
			(items.get(i)).getColor().setActive(theColor);
		}
		scroll();
		return this;
	}


	/**
	 * {@inheritDoc}
	 */
	@Override public ListBox setColorForeground(int theColor) {
		super.setColorForeground(theColor);
		for (int i = 0; i < items.size(); i++) {
			(items.get(i)).getColor().setForeground(theColor);
		}
		scroll();
		return this;
	}


	/**
	 * {@inheritDoc}
	 */
	@Override public ListBox setColorBackground(int theColor) {
		super.setColorBackground(theColor);
		for (int i = 0; i < items.size(); i++) {
			(items.get(i)).getColor().setBackground(theColor);
		}
		scroll();
		return this;
	}


	/**
	 * {@inheritDoc}
	 */
	@Override public ListBox setColorLabel(int theColor) {
		super.setColorLabel(theColor);
		for (int i = 0; i < items.size(); i++) {
			(items.get(i)).getColor().setCaptionLabel(theColor);
		}
		scroll();
		return this;
	}


	/**
	 * {@inheritDoc}
	 */
	@Override @ControlP5.Invisible public ListBox setColorValue(int theColor) {
		super.setColorValue(theColor);
		for (int i = 0; i < items.size(); i++) {
			(items.get(i)).getColor().setValueLabel(theColor);
		}
		scroll();
		return this;
	}


	public String[][] getListBoxItems() {
		String[][] l = new String[items.size()][3];
		for (int i = 0; i < l.length; i++) {
			l[i] = new String[] { items.get(i).name, items.get(i).text, Integer.toString(items.get(i).value) };
		}
		return l;
	}


	public ListBox setListBoxItems(String[][] l) {
		clear();
		for (String[] s : l) {
			addItem(s[0], new Integer(s[2]).intValue()).setText(s[1]);
		}
		return this;
	}


	/**
	 * @exclude
	 * @deprecated
	 */
	@Deprecated public ListBoxItem item(int theIndex) {
		return getItem(theIndex);
	}


	/**
	 * @exclude
	 * @deprecated
	 */
	@Deprecated public ListBoxItem item(String theItemName) {
		return getItem(theItemName);
	}


	/**
	 * @exclude
	 * @deprecated
	 */
	@Deprecated public ListBoxItem item(Controller<?> theButton) {
		return getItem(theButton);
	}


	/**
	 * @exclude
	 * @deprecated
	 * @return
	 */
	@Deprecated public boolean isScrollbarVisible() {
		return isScrollbarVisible;
	}
}
