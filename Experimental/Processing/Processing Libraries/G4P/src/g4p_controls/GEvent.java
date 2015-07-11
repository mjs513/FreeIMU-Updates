/*
  Part of the GUI for Processing library 
  	http://www.lagers.org.uk/g4p/index.html
	http://gui4processing.googlecode.com/svn/trunk/

  Copyright (c) 2008-12 Peter Lager

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
 */

package g4p_controls;

/**
 * Enumeration of events that can be fired by G4P. <br>
 * 
 * GTextField and GTextArea events <br>
 * 	CHANGED 			Text has changed <br>
 *	SELECTION_CHANGED 	Text selection has changed <br>
 *	ENTERED			 	Enter/return key typed <br>
 *	LOST_FOCUS			TextField/Area lost focus <br>
 *	GETS_FOCUS			TextField/Area got focus <br>
 *
 * GPanel events <br>
 *	COLLAPSED  			Control was collapsed <br>
 *	EXPANDED 			Control was expanded <br>
 *	DRAGGED 			Control is being dragged <br>
 *
 * Button control events (PRESSED and RELEASED are not fired by default)
 *	CLICKED  			Mouse button was clicked <br>
 *	PRESSED  			Mouse button was pressed <br>
 *	RELEASED  			Mouse button was released <br>
 *
 * Slider control events events  <br>
 *	VALUE_CHANGING		Value is changing <br>
 *	VALUE_STEADY		Value has reached a steady state <br>
 *	DRAGGING			The mouse is being dragged over a component <br>
 *
 * 	GCheckbox & GOption events <br>
 *	SELECTED			( "Option selected <br>
 *	DESELECTED			( "Option de-selected <br>
 *
 * @author Peter Lager
 *
 */
public enum GEvent {
	// GTextField and GTextArea events
	CHANGED 			( "Text has changed" ),
	SELECTION_CHANGED 	( "Text selection has changed" ),
	ENTERED			 	( "Enter/return key typed" ),
	LOST_FOCUS			( "TextField/Area lost focus" ),
	GETS_FOCUS			( "TextField/Area got focus" ),
	
	
	// GPanel events 
	COLLAPSED  			( "Control was collapsed" ),
	EXPANDED 			( "Control was expanded" ),
	DRAGGED 			( "Control is being dragged" ),

	// Button control events (PRESSED and RELEASED are not fired by default)
	CLICKED  			( "Mouse button was clicked" ),
	PRESSED  			( "Mouse button was pressed" ),
	RELEASED  			( "Mouse button was released" ),

	// Slider control events events 
	VALUE_CHANGING		( "Value is changing" ),
	VALUE_STEADY		( "Value has reached a steady state" ),
	DRAGGING			( "The mouse is being dragged over a component "),
	
	/// GCheckbox & GOption events
	SELECTED			( "Option selected" ),
	DESELECTED			( "Option de-selected" );

	
	private String description;
	
	private GEvent(String desc ){
		description = desc;
	}
	
	public String toString(){
		return description;
	}
}
