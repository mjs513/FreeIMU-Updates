
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

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * Used to convert Annotations into individual controllers this method of creating controllers is
 * derived from cp5magic by Karsten Schmidt http://hg.postspectacular.com/cp5magic/wiki/Home
 */

class ControllerAutomator {

	static Map<Set<Class<?>>, Class<? extends Controller<?>>> mapping = new HashMap<Set<Class<?>>, Class<? extends Controller<?>>>();

	static {
		mapping.put(makeKey(boolean.class), Toggle.class);
		mapping.put(makeKey(int.class), Slider.class);
		mapping.put(makeKey(float.class), Slider.class);
		mapping.put(makeKey(String.class), Textfield.class);
	}

	static Map<String, Class<? extends ControllerInterface<?>>> types = new HashMap<String, Class<? extends ControllerInterface<?>>>();

	static {
		types.put("slider", Slider.class);
		types.put("knob", Knob.class);
		types.put("numberbox", Numberbox.class);
		types.put("toggle", Toggle.class);
		types.put("bang", Bang.class);
		types.put("toggle", Toggle.class);
		types.put("textfield", Textfield.class);
		types.put("label", Textlabel.class);
		types.put("textlabel", Textlabel.class);
		types.put("list", ListBox.class);
		types.put("dropdown", DropdownList.class);
	}


	static Set<Class<?>> makeKey(Class<?>... cs) {
		Set<Class<?>> set = new HashSet<Class<?>>();
		for (Class<?> c : cs) {
			set.add(c);
		}
		return set;
	}


	private ControlP5 cp5;


	ControllerAutomator(ControlP5 c) {
		cp5 = c;
	}


	private Object[] getParameters(Class<?>[] cs, String v) {

		if (cs[0] == int.class) {
			return new Object[] { Integer.parseInt(v) };
		}
		else if (cs[0] == float.class) {
			return new Object[] { Float.parseFloat(v) };
		}
		else if (cs[0] == String.class) {
			return new Object[] { v };
		}
		else if (cs[0] == boolean.class) {
			return new Object[] { Boolean.parseBoolean(v) };
		}
		return new Object[0];
	}


	/**
	 * analyzes an object and adds fields with ControlElement annotations to controlP5.
	 * 
	 * @param theAddressSpace
	 * @param ts
	 */
	void addControllersFor(final String theAddressSpace, final Object t) {

		if (t instanceof List<?>) {
			return;
		}

		Class<?> c = t.getClass();
		Field[] fs = c.getDeclaredFields();
		Method[] ms = c.getDeclaredMethods();

		for (Method m : ms) {
			if (m.isAnnotationPresent(ControlElement.class)) {

				ControlElement ce = m.getAnnotation(ControlElement.class);

				Map<String, String> params = new HashMap<String, String>();

				Class<? extends ControllerInterface<?>> type = null;

				for (String s : ce.properties()) {
					String[] a = s.split("=");
					if (a[0].startsWith("type")) {
						type = types.get(a[1].toLowerCase());
					}
					else {
						params.put("set" + capitalize(a[0]), a[1]);
					}
				}

				if (type == null) {
					type = mapping.get(makeKey(m.getParameterTypes()));
				}
				if (type != null) {

					ControllerInterface<?> cntr = null;

					if (params.containsKey("setItems")) {
						if (type.equals(ListBox.class)) {
							cntr = cp5.addListBox(m.getName(), ce.x(), ce.y(), 100, 100);
							((ListBox) cntr).addItems(params.get("setItems").split(","));
						}
						else if (type.equals(DropdownList.class)) {
							cntr = cp5.addDropdownList(m.getName(), ce.x(), ce.y(), 100, 100);
							((DropdownList) cntr).addItems(params.get("setItems").split(","));
						}
						try {
							final Method method = t.getClass().getDeclaredMethod(m.getName(), int.class);

							cntr.addListener(new ControlListener() {

								public void controlEvent(ControlEvent ev) {
									try {
										method.setAccessible(true);
										method.invoke(t, new Object[] { (int) ev.getValue() });
									} catch (Exception e) {
										ControlP5.logger.severe(e.toString());
									}
								}
							});

						} catch (Exception e1) {
							System.out.println(e1);
						}
					}
					else {
						cntr = cp5.addController(t, theAddressSpace, m.getName(), type, ce.x(), ce.y());
					}

					if (ce.label().length() > 0) {
						cntr.setCaptionLabel(ce.label());
					}

					for (Iterator<String> i = params.keySet().iterator(); i.hasNext();) {
						String k = (String) i.next();
						String v = (String) params.get(k);
						for (Method method : cntr.getClass().getMethods()) {
							if (method.getName().equals(k)) {
								try {
									Object[] os = getParameters(method.getParameterTypes(), v);
									method.setAccessible(true);
									method.invoke(cntr, os);
								} catch (Exception e) {
									ControlP5.logger.severe(e.toString());
								}
							}
						}
					}
				}
			}
		}

		for (Field f : fs) {
			if (f.isAnnotationPresent(ControlElement.class)) {

				ControlElement ce = f.getAnnotation(ControlElement.class);

				Map<String, String> params = new HashMap<String, String>();

				Class<? extends ControllerInterface<?>> type = null;

				for (String s : ce.properties()) {
					String[] a = s.split("=");
					if (a[0].startsWith("type")) {
						type = types.get(a[1].toLowerCase());
					}
					else {
						params.put("set" + capitalize(a[0]), a[1]);
					}
				}

				ControllerInterface<?> cntr = null;

				f.setAccessible(true);

				if (f.getType() == float.class || f.getType() == int.class) {
					if (type == Knob.class) {
						cntr = cp5.addKnob(t, theAddressSpace, f.getName());
					}
					else if (type == Numberbox.class) {
						cntr = cp5.addNumberbox(t, theAddressSpace, f.getName());
					}
					else {
						cntr = cp5.addSlider(t, theAddressSpace, f.getName());
					}
					try {
						if (f.getType() == float.class) {
							cntr.setValue(f.getFloat(t));
						}
						else {
							cntr.setValue(f.getInt(t));
						}
					} catch (Exception e) {
						ControlP5.logger.severe(e.toString());
					}
				}
				else if (f.getType() == String.class) {
					if (type == Textlabel.class) {
						String s = "";
						try {
							s = "" + f.get(t);
							if (f.get(t) == null) {
								s = ce.label();
							}
						} catch (Exception e) {
						}
						cntr = cp5.addTextlabel(t, theAddressSpace, f.getName(), s);
					}
					else {
						cntr = cp5.addTextfield(t, theAddressSpace, f.getName());
					}
				}
				else if (f.getType() == boolean.class) {
					cntr = cp5.addToggle(t, theAddressSpace, f.getName());
					try {
						cntr.setValue(f.getBoolean(t) ? 1 : 0);
					} catch (Exception e) {
						ControlP5.logger.severe(e.toString());
					}
				}

				if (cntr != null) {

					if (ce.label().length() > 0) {
						cntr.setCaptionLabel(ce.label());
					}
					cntr.setPosition(ce.x(), ce.y());

					for (Iterator<String> i = params.keySet().iterator(); i.hasNext();) {
						String k = (String) i.next();
						String v = (String) params.get(k);
						for (Method method : cntr.getClass().getMethods()) {
							if (method.getName().equals(k)) {
								try {
									Object[] os = getParameters(method.getParameterTypes(), v);
									method.setAccessible(true);
									method.invoke(cntr, os);
								} catch (Exception e) {
									ControlP5.logger.severe(e.toString());
								}
							}
						}
					}
				}
			}
		}
	}


	/**
	 * capitalizes a string.
	 * 
	 * @param theString
	 * @return String
	 */
	static String capitalize(String theString) {
		final StringBuilder result = new StringBuilder(theString.length());
		String[] words = theString.split("\\s");
		for (int i = 0, l = words.length; i < l; ++i) {
			if (i > 0)
				result.append(" ");
			result.append(Character.toUpperCase(words[i].charAt(0))).append(words[i].substring(1));
		}
		return result.toString();
	}

}
