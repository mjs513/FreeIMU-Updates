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

import java.io.File;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.Vector;
import java.util.logging.Logger;

import processing.core.PApplet;
import processing.core.PFont;
import processing.core.PVector;
import processing.event.KeyEvent;
import processing.event.MouseEvent;
import controlP5.ControlWindow.Pointer;

/**
 * <p>
 * controlP5 is a processing and java library for creating simple control GUIs. The ControlP5 class, the core of controlP5.
 * </p>
 * <p>
 * All addController-Methods are located inside the ControlP5Base class.
 * </p>
 * 
 * @see controlP5.ControlP5Base
 * @example use/ControlP5basics
 */
public class ControlP5 extends ControlP5Base {

	/**
	 * @exclude
	 */
	@ControlP5.Invisible public ControlWindow controlWindow;

	public final static CColor RETRO = new CColor(0xff00698c, 0xff003652, 0xff08a2cf, 0xffffffff, 0xffffffff);

	public final static CColor CP5BLUE = new CColor(0xff016c9e, 0xff02344d, 0xff00b4ea, 0xffffffff, 0xffffffff);

	public final static CColor RED = new CColor(0xffaa0000, 0xff660000, 0xffff0000, 0xffffffff, 0xffffffff);

	public final static CColor WHITE = new CColor(0x99ffffff, 0x55ffffff, 0xffffffff, 0xffffffff, 0xffffffff);

	/**
	 * @exclude
	 */
	@ControlP5.Invisible static CColor color = new CColor(CP5BLUE);

	/**
	 * @exclude
	 */
	@ControlP5.Invisible public PApplet papplet;

	/**
	 * @exclude
	 */
	@ControlP5.Invisible public static final String VERSION = "2.0.4";// "2.0.4";

	/**
	 * @exclude
	 */
	@ControlP5.Invisible public static boolean isApplet = false;

	public static final int standard58 = 0;

	public static final int standard56 = 1;

	public static final int synt24 = 2;

	public static final int grixel = 3;

	public final static int J2D = 1;

	public final static int P2D = 2;

	public final static int P3D = 3;

	static int renderer = J2D;

	/**
	 * use this static variable to turn DEBUG on or off.
	 */
	public static boolean DEBUG;

	/**
	 * @exclude
	 */
	@ControlP5.Invisible public static final Logger logger = Logger.getLogger(ControlP5.class.getName());

	private Map<String, ControllerInterface<?>> _myControllerMap;

	protected ControlBroadcaster _myControlBroadcaster;

	protected ControlWindow window;

	protected boolean isMoveable = false;

	protected boolean isAutoInitialization = false;

	protected boolean isGlobalControllersAlwaysVisible = true;

	protected boolean isTabEventsActive;

	protected boolean isUpdate;

	protected boolean isControlFont;

	protected ControlFont controlFont;

	static protected final PFont BitFontStandard56 = new BitFont(CP.decodeBase64(BitFont.standard56base64));

	static protected final PFont BitFontStandard58 = new BitFont(CP.decodeBase64(BitFont.standard58base64));

	protected ControlFont defaultFont = new ControlFont(BitFontStandard58);

	protected ControlFont defaultFontForText = new ControlFont(BitFontStandard56);

	/**
	 * from version 0.7.2 onwards shortcuts are disabled by default. shortcuts can be enabled using controlP5.enableShortcuts();
	 * 
	 * @see #enableShortcuts()
	 */
	protected boolean isShortcuts = false;

	@Deprecated public boolean blockDraw;

	protected Tooltip _myTooltip;

	protected boolean isAnnotation;

	boolean isAndroid = false;

	/**
	 * Create a new instance of controlP5.
	 * 
	 * @param theParent PApplet
	 */
	public ControlP5(final PApplet theParent) {
		papplet = theParent;
		init();
	}

	
	public ControlP5(final PApplet theParent, PFont thePFont) {
		papplet = theParent;
		init();
		setFont(thePFont);
	}
	
	
	public ControlP5(final PApplet theParent, ControlFont theControlFont) {
		papplet = theParent;
		init();
		setFont(theControlFont);
	}

	protected void init() {
		renderer = (papplet.g.getClass().getCanonicalName().indexOf("Java2D") > -1) ? J2D : P3D;
		Class<?> check = papplet.getClass();
		while (check != null) {
			check = check.getSuperclass();
			if (check != null) {
				if (check.toString().toLowerCase().indexOf("android.app.") > -1) {
					isAndroid = true;
					break;
				}
			}
		}

		isTabEventsActive = false;

		_myControlBroadcaster = new ControlBroadcaster(this);

		// } else {
		// defaultFont = new ControlFont(papplet.createFont("", 10));
		//
		// defaultFontForText = new ControlFont(papplet.createFont("", 10));
		// }

		controlFont = defaultFont;

		controlWindow = new ControlWindow(this, papplet);

		papplet.registerMethod("pre", this);

		papplet.registerMethod("dispose", this);

		_myControllerMap = new TreeMap<String, ControllerInterface<?>>();

		setFont(controlFont);

		_myTooltip = new Tooltip(this);

		super.init(this);

		if (welcome++ < 1) {
			welcome();
		}

		mapKeyFor(new ControlKey() {

			public void keyEvent() {
				saveProperties();
			}
		}, PApplet.ALT, PApplet.SHIFT, 's');

		mapKeyFor(new ControlKey() {

			public void keyEvent() {
				loadProperties();
			}
		}, PApplet.ALT, PApplet.SHIFT, 'l');

		mapKeyFor(new ControlKey() {

			public void keyEvent() {
				if (controlWindow.isVisible) {
					hide();
				} else {
					show();
				}
			}
		}, PApplet.ALT, PApplet.SHIFT, 'h');

		disableShortcuts();
		
		setFont(controlFont);
		
	}

	static int welcome = 0;

	private void welcome() {
		System.out.println("ControlP5 " + VERSION + " " + "infos, comments, questions at http://www.sojamo.de/libraries/controlP5");
	}

	/**
	 * By default event originating from tabs are disabled, use setTabEventsActive(true) to receive controlEvents when tabs are clicked.
	 * 
	 * @param theFlag
	 */
	public void setTabEventsActive(boolean theFlag) {
		isTabEventsActive = theFlag;
	}

	/**
	 * autoInitialization can be very handy when it comes to initializing values, e.g. you load a set of controllers, then the values that
	 * are attached to the controllers will be reset to its saved state. to turn of auto intialization, call setAutoInitialization(false)
	 * right after initializing controlP5 and before creating any controller.
	 * 
	 * @param theFlag boolean
	 */
	public void setAutoInitialization(boolean theFlag) {
		isAutoInitialization = theFlag;
	}

	/**
	 * by default controlP5 draws any controller on top of any drawing done in the draw() function (this doesnt apply to P3D where
	 * controlP5.draw() has to be called manually in the sketch's draw() function ). to turn off the auto drawing of controlP5, use
	 * controlP5.setAutoDraw(false). now you can call controlP5.draw() any time whenever controllers should be drawn into the sketch.
	 * 
	 * @param theFlag boolean
	 */
	public void setAutoDraw(boolean theFlag) {
		if (isAutoDraw() && theFlag == false) {
			controlWindow.papplet().unregisterMethod("draw", controlWindow);
		}
		if (isAutoDraw() == false && theFlag == true) {
			controlWindow.papplet().registerMethod("draw", controlWindow);
		}
		controlWindow.isAutoDraw = theFlag;
	}

	/**
	 * check if the autoDraw function for the main window is enabled(true) or disabled(false).
	 * 
	 * @return boolean
	 */
	public boolean isAutoDraw() {
		return controlWindow.isAutoDraw;
	}

	/**
	 * 
	 * @see controlP5.ControlBroadcaster
	 */
	public ControlBroadcaster getControlBroadcaster() {
		return _myControlBroadcaster;
	}

	/**
	 * @see controlP5.ControlListener
	 */
	public ControlP5 addListener(ControlListener... theListeners) {
		getControlBroadcaster().addListener(theListeners);
		return this;
	}

	/**
	 * @see controlP5.ControlListener
	 */
	public ControlP5 removeListener(ControlListener... theListeners) {
		getControlBroadcaster().removeListener(theListeners);
		return this;
	}

	/**
	 * @see controlP5.ControlListener
	 */
	public ControlP5 removeListener(ControlListener theListener) {
		getControlBroadcaster().removeListener(theListener);
		return this;
	}

	/**
	 * @see controlP5.ControlListener
	 */
	public ControlListener getListener(int theIndex) {
		return getControlBroadcaster().getListener(theIndex);
	}

	/**
	 * @see controlP5.CallbackEvent
	 * @see controlP5.CallbackListener
	 */
	public ControlP5 addCallback(CallbackListener... theListeners) {
		getControlBroadcaster().addCallback(theListeners);
		return this;
	}

	/**
	 * @see controlP5.CallbackEvent
	 * @see controlP5.CallbackListener
	 */
	public ControlP5 addCallback(CallbackListener theListener) {
		getControlBroadcaster().addCallback(theListener);
		return this;
	}

	/**
	 * @see controlP5.CallbackEvent
	 * @see controlP5.CallbackListener
	 */
	public ControlP5 addCallback(CallbackListener theListener, Controller<?>... theControllers) {
		getControlBroadcaster().addCallback(theListener, theControllers);
		return this;
	}

	/**
	 * @see controlP5.CallbackEvent
	 * @see controlP5.CallbackListener
	 */
	public ControlP5 removeCallback(CallbackListener... theListeners) {
		getControlBroadcaster().removeCallback(theListeners);
		return this;
	}

	/**
	 * @see controlP5.CallbackEvent
	 * @see controlP5.CallbackListener
	 */
	public ControlP5 removeCallback(Controller<?>... theControllers) {
		getControlBroadcaster().removeCallback(theControllers);
		return this;
	}

	/**
	 * @see controlP5.CallbackEvent
	 * @see controlP5.CallbackListener
	 */
	public ControlP5 removeCallback(Controller<?> theController) {
		getControlBroadcaster().removeCallback(theController);
		return this;
	}

	/**
	 * TODO
	 * 
	 * @exclude
	 * @param theObject
	 */
	public void addControlsFor(Object theObject) {

	}

	public Tab getTab(String theName) {
		for (int i = 0; i < controlWindow.getTabs().size(); i++) {
			if (((Tab) controlWindow.getTabs().get(i)).getName().equals(theName)) {
				return (Tab) controlWindow.getTabs().get(i);
			}
		}
		Tab myTab = addTab(theName);
		return myTab;
	}

	public Tab getTab(ControlWindow theWindow, String theName) {
		for (int i = 0; i < theWindow.getTabs().size(); i++) {
			if (((Tab) theWindow.getTabs().get(i)).getName().equals(theName)) {
				return (Tab) theWindow.getTabs().get(i);
			}
		}
		Tab myTab = theWindow.add(new Tab(this, theWindow, theName));
		return myTab;
	}

	/**
	 * registers a Controller with ControlP5, a Controller should/must be registered with a unique name. If not, accessing Controllers by
	 * name is not guaranteed. the rule here is last come last serve, existing Controllers with the same name will be overridden.
	 * 
	 * @param theController ControllerInterface
	 * @return ControlP5
	 */
	public ControlP5 register(Object theObject, String theIndex, ControllerInterface<?> theController) {
		String address = "";
		if (theObject == papplet) {
			address = (theController.getName().startsWith("/")) ? "" : "/";
			address += theController.getName();
		} else {
			address = (((theIndex.length() == 0) || theIndex.startsWith("/")) ? "" : "/");
			address += theIndex;
			address += (theController.getName().startsWith("/") ? "" : "/");
			address += theController.getName();
		}
		theController.setAddress(address);
		if (checkName(theController.getAddress())) {
			/*
			 * in case a controller with the same name already exists, will be deleted
			 */
			remove(theController.getAddress());
		}
		/* add the controller to the controller map */
		_myControllerMap.put(theController.getAddress(), theController);

		/* update the properties' controller address */
		List<ControllerProperty> ps = getProperties().get(theController);
		if (ps != null) {
			for (ControllerProperty p : ps) {
				p.setAddress(theController.getAddress());
			}
		}
		/* initialize the controller */
		
		theController.init();

		/*
		 * handle controller plugs and map controllers to its reference objects if applicable.
		 */
		if (theController instanceof Controller<?>) {
			if (theObject == null) {
				theObject = papplet;
			}
			if (!theObject.equals(papplet)) {
				((Controller<?>) ((Controller<?>) theController).unplugFrom(papplet)).plugTo(theObject);
			}

			if (!_myObjectToControllerMap.containsKey(theObject)) {
				_myObjectToControllerMap.put(theObject, new ArrayList<ControllerInterface<?>>());
			}
			_myObjectToControllerMap.get(theObject).add(theController);
		} else {
			/*
			 * if theController is of type ControllerGroup, map accordingly here.
			 */
		}
		return this;
	}

	/**
	 * Returns a List of all controllers currently registered.
	 * 
	 * @return List<ControllerInterface<?>>
	 */
	public List<ControllerInterface<?>> getAll() {
		return new ArrayList<ControllerInterface<?>>(_myControllerMap.values());
	}

	/**
	 * Returns a list of controllers or groups of a particular type. The following example will return a list of registered Bangs only:<br />
	 * <code><pre>
	 * List<Bang> list = controlP5.getAll(Bang.class);
	 * println(list);
	 * for(Bang b:list) {
	 *   b.setColorForeground(color(255,255,0));
	 * }
	 * </pre></code> Here the foreground color of all Bangs is changed to yellow.
	 * 
	 * @param <T>
	 * @param theClass A class that extends ControllerInterface, which applies to all Controllers and ControllerGroups
	 * @return List<T>
	 */
	@SuppressWarnings("unchecked") public <T> List<T> getAll(Class<T> theClass) {
		ArrayList<T> l = new ArrayList<T>();
		for (ControllerInterface ci : _myControllerMap.values()) {
			if (ci.getClass() == theClass || ci.getClass().getSuperclass() == theClass) {
				l.add((T) ci);
			}
		}
		return l;
	}

	protected void deactivateControllers() {
		for (Textfield t : getAll(Textfield.class)) {
			t.setFocus(false);
		}
	}

	private String checkAddress(String theName) {
		if (!theName.startsWith("/")) {
			return "/" + theName;
		}
		return theName;
	}

	/**
	 * @excude
	 */
	public void printControllerMap() {
		List<String> strs = new ArrayList<String>();
		System.out.println("============================================");
		for (Iterator it = _myControllerMap.entrySet().iterator(); it.hasNext();) {
			Map.Entry entry = (Map.Entry) it.next();
			Object key = entry.getKey();
			Object value = entry.getValue();
			strs.add(key + " = " + value);
		}
		Collections.sort(strs);
		for (String s : strs) {
			System.out.println(s);
		}
		System.out.println("============================================");
	}

	/**
	 * removes a controller by instance.
	 * 
	 * TODO Fix this. this only removes the reference to a controller from the controller map but not its children, fatal for controller
	 * groups!
	 * 
	 * @param theController ControllerInterface
	 */
	protected void remove(ControllerInterface<?> theController) {
		_myControllerMap.remove(theController.getAddress());
	}

	/**
	 * removes a controlP5 element such as a controller, group, or tab by name.
	 * 
	 * @param theString String
	 */
	public void remove(String theName) {
		String address = checkAddress(theName);

		if (getController(address) != null) {
			getController(address).remove();
		}

		if (getGroup(address) != null) {
			getGroup(address).remove();
		}

		for (int i = 0; i < controlWindow.getTabs().size(); i++) {
			if (controlWindow.getTabs().get(i).getAddress().equals(address)) {
				controlWindow.getTabs().get(i).remove();
			}
		}
		_myControllerMap.remove(address);
	}

	public ControllerInterface<?> get(String theName) {
		String address = checkAddress(theName);
		if (_myControllerMap.containsKey(address)) {
			return _myControllerMap.get(address);
		}
		return null;
	}

	public <C> C get(Class<C> theClass, String theName) {
		for (ControllerInterface<?> ci : _myControllerMap.values()) {
			if (ci.getClass() == theClass || ci.getClass().getSuperclass() == theClass) {
				return (C) get(theName);
			}
		}
		return null;
	}

	/**
	 * @exclude
	 * @see controlP5.ControlP5#getAll(Class)
	 * @return List<ControllerInterface>
	 */
	@ControlP5.Invisible public List<ControllerInterface<?>> getList() {
		LinkedList<ControllerInterface<?>> l = new LinkedList<ControllerInterface<?>>();
		l.addAll(controlWindow.getTabs().get());
		l.addAll(getAll());
		return l;
	}

	public float getValue(String theIndex) {
		Controller c = getController(theIndex);
		if (c != null) {
			return c.getValue();
		}
		return Float.NaN;
	}

	public Controller<?> getController(String theName) {
		String address = checkAddress(theName);
		if (_myControllerMap.containsKey(address)) {
			if (_myControllerMap.get(address) instanceof Controller<?>) {
				return (Controller<?>) _myControllerMap.get(address);
			}
		}
		return null;
	}

	public ControllerGroup<?> getGroup(String theGroupName) {
		String address = checkAddress(theGroupName);
		if (_myControllerMap.containsKey(address)) {
			if (_myControllerMap.get(address) instanceof ControllerGroup<?>) {
				return (ControllerGroup<?>) _myControllerMap.get(address);
			}
		}
		return null;
	}

	private boolean checkName(String theName) {
		if (_myControllerMap.containsKey(checkAddress(theName))) {
			ControlP5.logger().warning("Controller with name \"" + theName + "\" already exists. overwriting reference of existing controller.");
			return true;
		}
		return false;
	}

	public void moveControllersForObject(Object theObject, ControllerGroup<?> theGroup) {
		if (_myObjectToControllerMap.containsKey(theObject)) {
			ArrayList<ControllerInterface<?>> cs = _myObjectToControllerMap.get(theObject);
			for (ControllerInterface<?> c : cs) {
				((Controller<?>) c).moveTo(theGroup);
			}
		}
	}

	public void move(Object theObject, ControllerGroup<?> theGroup) {
		moveControllersForObject(theObject, theGroup);
	}

	protected void clear() {
		controlWindow.clear();
		_myControllerMap.clear();
	}

	/**
	 * @exclude
	 */
	@ControlP5.Invisible public void pre() {
		Iterator<FieldChangedListener> itr = _myFieldChangedListenerMap.values().iterator();
		while (itr.hasNext()) {
			itr.next().update();
		}
	}

	/**
	 * call draw() from your program when autoDraw is disabled.
	 * 
	 * @exclude
	 */
	@ControlP5.Invisible public void draw() {
		if(!isAutoDraw()) {
			controlWindow.draw();
		}
	}

	/**
	 * convenience method to access the main window (ControlWindow class).
	 */
	public ControlWindow getWindow() {
		return getWindow(papplet);
	}

	public void mouseEvent(MouseEvent theMouseEvent) {
		getWindow().mouseEvent(theMouseEvent);
	}

	public void keyEvent(KeyEvent theKeyEvent) {
		getWindow().keyEvent(theKeyEvent);
	}

	/**
	 * convenience method to access the pointer of the main control window.
	 */
	public Pointer getPointer() {
		return getWindow(papplet).getPointer();
	}

	/**
	 * convenience method to check if the mouse (or pointer) is hovering over any controller. only applies to the main window. To receive
	 * the mouseover information for a ControlWindow use getWindow(nameOfWindow).isMouseOver();
	 */
	public boolean isMouseOver() {
		return getWindow(papplet).isMouseOver();
	}

	/**
	 * convenience method to check if the mouse (or pointer) is hovering over a specific controller. only applies to the main window. To
	 * receive the mouseover information for a ControlWindow use getWindow(nameOfWindow).isMouseOver(ControllerInterface<?>);
	 */
	public boolean isMouseOver(ControllerInterface<?> theController) {
		return getWindow(papplet).isMouseOver(theController);
	}

	/**
	 * convenience method to check if the mouse (or pointer) is hovering over a specific controller. only applies to the main window. To
	 * receive the mouseover information for a ControlWindow use getWindow(nameOfWindow).getMouseOverList();
	 */
	public List<ControllerInterface<?>> getMouseOverList() {
		return getWindow(papplet).getMouseOverList();
	}

	public ControlWindow getWindow(PApplet theApplet) {
		if (theApplet.equals(papplet)) {
			return controlWindow;
		}
		// TODO !!! check for another window in case theApplet is of type
		// PAppletWindow.
		return controlWindow;
	}

	/**
	 * adds a Canvas to the default sketch window.
	 * 
	 * @see controlP5.Canvas
	 */
	public ControlP5 addCanvas(Canvas theCanvas) {
		getWindow().addCanvas(theCanvas);
		return this;
	}

	public ControlP5 removeCanvas(Canvas theCanvas) {
		getWindow().removeCanvas(theCanvas);
		return this;
	}

	public ControlP5 setColor(CColor theColor) {
		setColorBackground(theColor.getBackground());
		setColorForeground(theColor.getForeground());
		setColorActive(theColor.getActive());
		setColorCaptionLabel(theColor.getCaptionLabel());
		setColorValueLabel(theColor.getValueLabel());
		return this;
	}

	public static CColor getColor() {
		return color;
	}

	/**
	 * sets the active state color of tabs and controllers, this cascades down to all known controllers.
	 */
	public ControlP5 setColorActive(int theColor) {
		color.setActive(theColor);
		controlWindow.setColorActive(theColor);
		return this;
	}

	/**
	 * sets the foreground color of tabs and controllers, this cascades down to all known controllers.
	 */
	public ControlP5 setColorForeground(int theColor) {
		color.setForeground(theColor);
		controlWindow.setColorForeground(theColor);
		return this;
	}

	/**
	 * sets the background color of tabs and controllers, this cascades down to all known controllers.
	 */
	public ControlP5 setColorBackground(int theColor) {
		color.setBackground(theColor);
		controlWindow.setColorBackground(theColor);
		return this;
	}

	/**
	 * sets the label color of tabs and controllers, this cascades down to all known controllers.
	 */
	public ControlP5 setColorCaptionLabel(int theColor) {
		color.setCaptionLabel(theColor);
		controlWindow.setColorLabel(theColor);
		return this;
	}

	/**
	 * sets the value color of controllers, this cascades down to all known controllers.
	 */
	public ControlP5 setColorValueLabel(int theColor) {
		color.setValueLabel(theColor);
		controlWindow.setColorValue(theColor);
		return this;
	}

	/**
	 * Enables/disables Controllers to be moved around when ALT-key is down and mouse is dragged. Other key events are still available like
	 * ALT-h to hide and show the controllers To disable all key events, use disableKeys()
	 */
	public ControlP5 setMoveable(boolean theFlag) {
		isMoveable = theFlag;
		return this;
	}

	/**
	 * Checks if controllers are generally moveable
	 * 
	 */
	public boolean isMoveable() {
		return isMoveable;
	}

	/**
	 * Saves the current values of controllers into a default properties file
	 * 
	 * @see controlP5.ControllerProperties
	 */
	public boolean saveProperties() {
		return _myProperties.save();
	}

	/**
	 * Saves the current values of controllers into a file, the filepath is given by parameter theFilePath.
	 * 
	 * @see controlP5.ControllerProperties
	 */
	public boolean saveProperties(String theFilePath) {
		return _myProperties.saveAs(theFilePath);
	}

	public boolean saveProperties(String theFilePath, String... theSets) {
		return _myProperties.saveAs(theFilePath, theSets);
	}

	/**
	 * Loads properties from a default properties file and changes values of controllers accordingly.
	 * 
	 * @see controlP5.ControllerProperties
	 * @return
	 */
	public boolean loadProperties() {
		return _myProperties.load();
	}

	/**
	 * Loads properties from a properties file and changes the values of controllers accordingly, the filepath is given by parameter
	 * theFilePath.
	 * 
	 * @param theFilePath
	 * @return
	 */
	public boolean loadProperties(String theFilePath) {
		theFilePath = checkPropertiesPath(theFilePath);
		File f = new File(theFilePath);
		if (f.exists()) {
			return _myProperties.load(theFilePath);
		}
		theFilePath = checkPropertiesPath(theFilePath + ".ser");
		f = new File(theFilePath);
		if (f.exists()) {
			return _myProperties.load(theFilePath);
		}
		logger.info("Properties File " + theFilePath + " does not exist.");
		return false;
	}

	String checkPropertiesPath(String theFilePath) {
		theFilePath = (theFilePath.startsWith("/") || theFilePath.startsWith(".")) ? theFilePath : papplet.sketchPath(theFilePath);
		return theFilePath;
	}

	/**
	 * @exclude
	 * @param theFilePath
	 * @return
	 */
	@ControlP5.Invisible public boolean loadLayout(String theFilePath) {
		theFilePath = checkPropertiesPath(theFilePath);
		File f = new File(theFilePath);
		if (f.exists()) {
			getLayout().load(theFilePath);
			return true;
		}
		logger.info("Layout File " + theFilePath + " does not exist.");
		return false;
	}

	/**
	 * @exclude
	 * @param theFilePath
	 */
	public void saveLayout(String theFilePath) {
		getLayout().save(theFilePath);
	}

	/**
	 * Returns the current version of controlP5
	 * 
	 * @return String
	 */
	public String version() {
		return VERSION;
	}

	/**
	 * shows all controllers and tabs in your sketch.
	 * 
	 * @see controlP5.ControlP5#isVisible()
	 * @see controlP5.ControlP5#hide()
	 */

	public void show() {
		controlWindow.show();
	}

	public ControlP5 setBroadcast(boolean theValue) {
		_myControlBroadcaster.broadcast = theValue;
		return this;
	}

	/**
	 * returns true or false according to the current visibility flag.
	 * 
	 * @see controlP5.ControlP5#show()
	 * @see controlP5.ControlP5#hide()
	 */
	public boolean isVisible() {
		return controlWindow.isVisible();
	}

	/**
	 * hide all controllers and tabs inside your sketch window.
	 * 
	 * @see controlP5.ControlP5#show()
	 * @see controlP5.ControlP5#isVisible()
	 */
	public void hide() {
		controlWindow.hide();
	}

	/**
	 * forces all controllers to update.
	 * 
	 * @see controlP5.ControlP5#isUpdate()
	 * @see controlP5.ControlP5#setUpdate()
	 */
	public void update() {
		controlWindow.update();
	}

	/**
	 * checks if automatic updates are enabled. By default this is true.
	 * 
	 * @see controlP5.ControlP5#update()
	 * @see controlP5.ControlP5#setUpdate(boolean)
	 * @return
	 */
	public boolean isUpdate() {
		return isUpdate;
	}

	/**
	 * changes the update behavior according to parameter theFlag
	 * 
	 * @see controlP5.ControlP5#update()
	 * @see controlP5.ControlP5#isUpdate()
	 * @param theFlag
	 */
	public void setUpdate(boolean theFlag) {
		isUpdate = theFlag;
		controlWindow.setUpdate(theFlag);
	}

	@Deprecated public boolean setFont(int theBitFontIndex) {
		logger.warning("BitFont is now of type PFont, use setFont(PFont) instead.");
		return false;
	}

	public boolean setFont(ControlFont theControlFont) {
		controlFont = theControlFont;
		isControlFont = true;
		updateFont(controlFont);
		return isControlFont;
	}

	public boolean setFont(PFont thePFont, int theFontSize) {
		controlFont = new ControlFont(thePFont, theFontSize);
		isControlFont = true;
		updateFont(controlFont);
		return isControlFont;
	}

	public boolean setFont(PFont thePFont) {
		controlFont = new ControlFont(thePFont);
		isControlFont = true;
		updateFont(controlFont);
		return isControlFont;
	}

	protected void updateFont(ControlFont theControlFont) {
		controlWindow.updateFont(theControlFont);
	}

	public ControlFont getFont() {
		return controlFont;
	}

	/**
	 * disables shortcuts such as alt-h for hiding/showing controllers
	 * 
	 */
	public void disableShortcuts() {
		isShortcuts = false;
	}

	public boolean isShortcuts() {
		return isShortcuts;
	}

	/**
	 * enables shortcuts.
	 */
	public void enableShortcuts() {
		isShortcuts = true;
	}

	public Tooltip getTooltip() {
		return _myTooltip;
	}

	public void setTooltip(Tooltip theTooltip) {
		_myTooltip = theTooltip;
	}

	public void setMouseWheelRotation(int theRotation) {
		getWindow().setMouseWheelRotation(theRotation);
	}

	/**
	 * cp5.begin() and cp5.end() are mechanisms to auto-layout controllers, see the ControlP5beginEnd example.
	 */
	public ControllerGroup<?> begin() {
		// TODO replace controlWindow.tab("default") with
		// controlWindow.tabs().get(1);
		return begin(controlWindow.getTab("default"));
	}

	public ControllerGroup<?> begin(ControllerGroup<?> theGroup) {
		setCurrentPointer(theGroup);
		return theGroup;
	}

	public ControllerGroup<?> begin(int theX, int theY) {
		// TODO replace controlWindow.tab("default") with
		// controlWindow.tabs().get(1);
		return begin(controlWindow.getTab("default"), theX, theY);
	}

	public ControllerGroup<?> begin(ControllerGroup<?> theGroup, int theX, int theY) {
		setCurrentPointer(theGroup);
		theGroup.autoPosition.x = theX;
		theGroup.autoPosition.y = theY;
		theGroup.autoPositionOffsetX = theX;
		return theGroup;
	}

	public ControllerGroup<?> begin(ControlWindow theWindow) {
		return begin(theWindow.getTab("default"));
	}

	public ControllerGroup<?> begin(ControlWindow theWindow, int theX, int theY) {
		return begin(theWindow.getTab("default"), theX, theY);
	}

	public ControllerGroup<?> end(ControllerGroup<?> theGroup) {
		releaseCurrentPointer(theGroup);
		return theGroup;
	}

	/**
	 * cp5.begin() and cp5.end() are mechanisms to auto-layout controllers, see the ControlP5beginEnd example.
	 */
	public ControllerGroup<?> end() {
		return end(controlWindow.getTab("default"));
	}

	public void addPositionTo(int theX, int theY, List<ControllerInterface<?>> theControllers) {
		PVector v = new PVector(theX, theY);
		for (ControllerInterface<?> c : theControllers) {
			c.setPosition(PVector.add(c.getPosition(), v));
		}
	}

	public void addPositionTo(int theX, int theY, ControllerInterface<?>... theControllers) {
		addPositionTo(theX, theY, Arrays.asList(theControllers));
	}

	/**
	 * disposes and clears all controlP5 elements. When running in applet mode, opening new tabs or switching to another tab causes the
	 * applet to call dispose(). therefore dispose() is disabled when running ing applet mode. TODO implement better dispose handling for
	 * applets.
	 * 
	 * @exclude
	 */
	public void dispose() {
		if (!isApplet) {
			clear();
		}
	}

	/* add Objects with Annotation */

	public static Logger logger() {
		return logger;
	}

	@Retention(RetentionPolicy.RUNTIME) @interface Invisible {}

	@Retention(RetentionPolicy.RUNTIME) @interface Layout {}

	/**
	 * @exclude
	 * @deprecated
	 * @param theControlFont
	 * @return
	 */
	@Deprecated public boolean setControlFont(ControlFont theControlFont) {
		return setFont(theControlFont);
	}

	/**
	 * @exclude
	 * @deprecated
	 * @param thePFont
	 * @param theFontSize
	 * @return
	 */
	@Deprecated public boolean setControlFont(PFont thePFont, int theFontSize) {
		return setFont(thePFont, theFontSize);
	}

	/**
	 * @exclude
	 * @deprecated
	 * @param thePFont
	 * @return
	 */
	@Deprecated public boolean setControlFont(PFont thePFont) {
		return setFont(thePFont);
	}

	/**
	 * @exclude
	 * @deprecated
	 * @return
	 */
	@Deprecated public ControlFont getControlFont() {
		return getFont();
	}

	/**
	 * @deprecated
	 * @exclude
	 */
	@Deprecated public boolean save(String theFilePath) {
		ControlP5.logger().info("Saving ControlP5 settings in XML format has been removed, have a look at controlP5's properties instead.");
		return false;
	}

	/**
	 * @deprecated
	 * @exclude
	 */
	@Deprecated public boolean save() {
		ControlP5.logger().info("Saving ControlP5 settings in XML format has been removed, have a look at controlP5's properties instead.");
		return false;
	}

	/**
	 * @deprecated
	 * @exclude
	 */
	@Deprecated public boolean load(String theFileName) {
		ControlP5.logger().info("Loading ControlP5 from an XML file has been removed, have a look at controlP5's properties instead.");
		return false;
	}

	/**
	 * @deprecated
	 * @exclude
	 */
	@Deprecated public void trigger() {
		Iterator<?> iter = _myControllerMap.keySet().iterator();
		while (iter.hasNext()) {
			Object key = iter.next();
			if (_myControllerMap.get(key) instanceof Controller<?>) {
				((Controller<?>) _myControllerMap.get(key)).trigger();
			}
		}
	}

	/**
	 * @deprecated use disableShortcuts()
	 * @exclude
	 */
	@Deprecated public void disableKeys() {
		isShortcuts = false;
	}

	/**
	 * @deprecated use enableShortcuts()
	 * @exclude
	 */
	@Deprecated public void enableKeys() {
		isShortcuts = true;
	}

	/**
	 * lock ControlP5 to disable moving Controllers around. Other key events are still available like ALT-h to hide and show the controllers
	 * To disable all key events, use disableShortcuts() use setMoveable(false) instead
	 * 
	 * @deprecated
	 * @exclude
	 */
	@Deprecated public void lock() {
		isMoveable = false;
	}

	/**
	 * unlock ControlP5 to enable moving Controllers around. use setMoveable(true) instead
	 * 
	 * @see controlP5.ControlP5#setMoveable(boolean)
	 * @deprecated
	 * @exclude
	 */
	@Deprecated public void unlock() {
		isMoveable = true;
	}

	/**
	 * @deprecated
	 * @exclude
	 */
	@Deprecated protected Vector<ControlWindow> controlWindows() {
		return getControlWindows();
	}

	/**
	 * @deprecated
	 * @exclude
	 */
	@Deprecated public Controller<?> controller(String theName) {
		return getController(theName);
	}

	/**
	 * @deprecated
	 * @exclude
	 */
	@Deprecated public ControllerGroup<?> group(String theGroupName) {
		return getGroup(theGroupName);
	}

	/**
	 * @deprecated
	 * @exclude
	 */
	@Deprecated public ControlWindow window(String theWindowName) {
		return getWindow(theWindowName);
	}

	/**
	 * @deprecated
	 * @exclude
	 */
	@Deprecated public ControlWindow window() {
		return getWindow();
	}

	/**
	 * @deprecated
	 * @exclude
	 */
	@Deprecated public Tab tab(ControlWindow theWindow, String theName) {
		return getTab(theWindow, theName);
	}

	/**
	 * @deprecated
	 * @exclude
	 */
	@Deprecated public ControlWindow window(PApplet theApplet) {
		return getWindow(theApplet);
	}

	/**
	 * @exclude
	 * @deprecated
	 */
	@Deprecated public ControlBroadcaster controlbroadcaster() {
		return _myControlBroadcaster;
	}

	/**
	 * @exclude
	 * @deprecated
	 */
	@Deprecated public Tab tab(String theName) {
		return getTab(theName);
	}

	/**
	 * returns a list of registered Controllers. Controllers with duplicated reference names will be ignored, only the latest of such
	 * Controllers will be included in the list.
	 * 
	 * use getAll() instead
	 * 
	 * @see controlP5.ControlP5#getAll()
	 * @see controlP5.ControlP5#getAll(Class)
	 * @return ControllerInterface[]
	 */
	@Deprecated public ControllerInterface<?>[] getControllerList() {
		ControllerInterface<?>[] myControllerList = new ControllerInterface[_myControllerMap.size()];
		_myControllerMap.values().toArray(myControllerList);
		return myControllerList;
	}

	/**
	 * @exclude
	 * @deprecated
	 */
	@Deprecated public ControlP5 setColorLabel(int theColor) {
		return setColorCaptionLabel(theColor);
	}

	/**
	 * @exclude
	 * @deprecated
	 */
	@Deprecated public ControlP5 setColorValue(int theColor) {
		return setColorValueLabel(theColor);
	}

	/**
	 * @deprecated disables the mouse wheel as input for the main window, by default the mouse wheel is enabled.
	 */
	@Deprecated public ControlP5 disableMouseWheel() {
		getWindow().disableMouseWheel();
		return this;
	}

	/**
	 * @deprecated enables the mouse wheel as input for the main window, by default the mouse wheel is enabled.
	 */
	@Deprecated public ControlP5 enableMouseWheel() {
		getWindow().enableMouseWheel();
		return this;
	}

	/**
	 * @deprecated checks the status of the mouse wheel.
	 */
	@Deprecated public boolean isMouseWheel() {
		return getWindow().isMouseWheel();
	}

	/**
	 * @deprecated removes a controlWindow and all its contained controllers.
	 * 
	 * @param theWindow ControlWindow
	 */
	@Deprecated protected ControlP5 remove(ControlWindow theWindow) {
		theWindow.remove();
		return this;
	}

	/**
	 * @deprecated
	 * @param theWindowName
	 * @return
	 */
	@Deprecated public ControlWindow getWindow(String theWindowName) {
		return controlWindow;
	}

	/**
	 * @deprecated
	 * @return
	 */
	@Deprecated protected Vector<ControlWindow> getControlWindows() {
		Vector<ControlWindow> v = new Vector<ControlWindow>();
		v.add(controlWindow);
		return v;
	}

}

// new controllers
// http://www.cambridgeincolour.com/tutorials/photoshop-curves.htm
// http://images.google.com/images?q=synthmaker
// http://en.wikipedia.org/wiki/Pie_menu
//
// inspiration
// http://www.explodingart.com/arb/Andrew_R._Brown/Code/Code.html

// projects using controlP5
// http://www.danielsauter.com/showreel.php?id=59
// http://www.raintone.com/2009/03/fractalwavetables-v2/
// http://www.flickr.com/photos/jrosenk/3631041263/
// http://www.gtdv.org/
// http://0p0media.com/aurapixlab/
// http://www.introspector.be/index.php?/research/dook/
// http://createdigitalmotion.com/2009/11/29/processing-beats-keyframing-typestar-karaoke-machine-generates-kinetic-lyrics/
// http://www.yonaskolb.com/predray
// http://www.creativeapplications.net/processing/cop15-identity-processing/
// http://vimeo.com/9158064 + http://vimeo.com/9153342 processing-city,
// sandy-city
//
// http://vimeo.com/24358631 http://diplome.canalblog.com/
// http://www.yannickmathey.com/prototyp
// http://vimeo.com/24665893 http://jcnaour.com/#Kinect-Graffiti-
// http://www.onformative.com/work/zero-visuals/
// http://www.youtube.com/watch?v=DIPgRMtYqGQ

// TODO
// (1) file dialog:
// http://java.sun.com/j2se/1.5.0/docs/api/java/awt/FileDialog.html
// (2) add ControlIcon for custom icons with PImage

// gui addons inspiration.
// http://www.futureaudioworkshop.com/
//
// new color schemes http://ethanschoonover.com/solarized

