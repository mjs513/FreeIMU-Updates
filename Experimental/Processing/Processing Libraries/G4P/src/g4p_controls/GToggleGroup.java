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
 * Use this class to create a single selection collection of options. <br>
 * 
 * To use create an object of this class and then add GOption objects to it with
 * the addControl method. <br>
 * 
 * You cannot add GCheckbox objects because they are designed to work independently.
 * 
 * @author Peter Lager
 *
 */
public class GToggleGroup {

	private GToggleControl selected = null;
	private GToggleControl deselected = null;
	
	/**
	 * Create a toggle group object.
	 */
	public GToggleGroup(){	}

	/**
	 * Add a GOption object to this group.
	 * @param tc
	 */
	public void addControl(GToggleControl tc){
		tc.setToggleGroup(this);
	}
	
	/**
	 * Add a set of comma separated GOptions.
	 * @param controls
	 */
	public void addControls(GToggleControl... controls ){
		for(GToggleControl tc : controls)
			tc.setToggleGroup(this);			
	}
	
	/**
	 * Used internally to change selection
	 */
	void makeSelected(GToggleControl tc){
		deselected = selected;
		if(deselected != null)
			deselected.setSelected(false);
		selected = tc;
	}
}
