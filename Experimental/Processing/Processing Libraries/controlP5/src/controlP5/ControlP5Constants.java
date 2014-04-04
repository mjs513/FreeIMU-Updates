
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
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Array;
import java.net.URLEncoder;
import java.text.CharacterIterator;
import java.text.StringCharacterIterator;

import processing.core.PApplet;

/**
 * Constant variables used with ControlP5 are stored here.
 */
public interface ControlP5Constants {

	public final static String eventMethod = "controlEvent";

	public final static boolean VERBOSE = false;

	public final static float PI = (float) Math.PI;

	public final static float TWO_PI = PI * 2;

	public final static float HALF_PI = PI / 2;

	public final static int INVALID = -1;

	public final static int METHOD = 0;

	public final static int FIELD = 1;

	public final static int EVENT = 2;

	public final static int INTEGER = 1;

	public final static int FLOAT = 2;

	public final static int BOOLEAN = 3;

	public final static int STRING = 4;

	public final static int ARRAY = 5;

	public final static int BITFONT = 100;

	public final static Class<?>[] acceptClassList = { int.class, float.class, boolean.class, String.class };

	public final static Class<?> controlEventClass = ControlEvent.class;

	public final static int UP = PApplet.UP; // KeyEvent.VK_UP;

	public final static int DOWN = PApplet.DOWN; //KeyEvent.VK_DOWN;

	public final static int LEFT = PApplet.LEFT; //KeyEvent.VK_LEFT;

	public final static int RIGHT = PApplet.RIGHT; //KeyEvent.VK_RIGHT;

	public final static int SHIFT = PApplet.SHIFT; //KeyEvent.VK_SHIFT;

	public final static int DELETE = PApplet.DELETE; //KeyEvent.VK_DELETE;

	public final static int BACKSPACE = PApplet.BACKSPACE; //KeyEvent.VK_BACK_SPACE;

	public final static int ENTER = PApplet.ENTER; //KeyEvent.VK_ENTER;

	public final static int ESCAPE = PApplet.ESC; //KeyEvent.VK_ESCAPE;

	public final static int ALT = PApplet.ALT; //KeyEvent.VK_ALT;

	public final static int CONTROL = PApplet.CONTROL;//KeyEvent.VK_CONTROL;

	public final static int COMMANDKEY = 157; // Event.VK_META;

	public final static int TAB = PApplet.TAB; //KeyEvent.VK_TAB;

	public final static char INCREASE = PApplet.UP;

	public final static char DECREASE = PApplet.DOWN;

	public final static char SWITCH_FORE = PApplet.LEFT;

	public final static char SWITCH_BACK = PApplet.RIGHT;

	public final static char SAVE = 'S';

	public final static char RESET = 'R';

	public final static char PRINT = ' ';

	public final static char HIDE = 'H';

	public final static char LOAD = 'L';

	public final static char MENU = 'M';

	public final static char KEYCONTROL = 'K';

	public final static int TOP = 101; // PApplet.TOP

	public final static int BOTTOM = 102; // PApplet.BOTTOM

	public final static int CENTER = 3; // PApplet.CENTER

	public final static int BASELINE = 0; // PApplet.BASELINE

	public static final int HORIZONTAL = 0;

	public static final int VERTICAL = 1;

	public static final int DEFAULT = 0;

	public static final int OVER = 1;

	public static final int ACTIVE = 2;

	public static final int HIGHLIGHT = 3;

	public static final int IMAGE = 1;

	public static final int SPRITE = 2;

	public static final int CUSTOM = 3;

	public static final int SWITCH = 100;

	public static final int MOVE = 0;

	public static final int RELEASE = 1;

	public static final int PRESSED = 2;

	public static final int LINE = 1;

	public static final int ELLIPSE = 2;

	public static final int ARC = 3;

	public static final int INACTIVE = 0;

	public static final int WAIT = 1;

	public static final int TRANSITION_WAIT_FADEIN = 2;

	public static final int FADEIN = 3;

	public static final int IDLE = 4;

	public static final int FADEOUT = 5;

	public static final int DONE = 6;

	public static final int SINGLE_COLUMN = 0;

	public static final int SINGLE_ROW = 1;

	public static final int MULTIPLES = 2;

	public static final int ACTION_PRESSED = 1;

	public static final int ACTION_ENTER = 2;

	public static final int ACTION_LEAVE = 4;

	public static final int ACTION_RELEASED = 8;

	public static final int ACTION_RELEASEDOUTSIDE = 16;

	public static final int ACTION_BROADCAST = 32;

	public static final int LEFT_OUTSIDE = 10;

	public static final int RIGHT_OUTSIDE = 11;

	public static final int TOP_OUTSIDE = 12;

	public static final int BOTTOM_OUTSIDE = 13;

	public static final int CAPTIONLABEL = 0;

	public static final int VALUELABEL = 1;

	public static final int SINGLE = 0;

	@Deprecated public static final int ALL = 1;

	public static final int MULTI = 1;
	
}
