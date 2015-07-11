package g4p_controls;

import processing.core.PApplet;
import processing.core.PConstants;

public enum GControlMode implements PConstants{

	CORNER 		( "X Y W H coordinates", 		"CORNER",	PApplet.CORNER	),
	CORNERS 	( "X0 Y0 X1 Y1 coordinates", 	"CORNERS",	PApplet.CORNERS	),
	CENTER		( "X Y W H coordinates",		"CENTER",	PApplet.CENTER	);

	
	public final String description;
	public final String ps_name;
	public final int mode;
	
	private GControlMode(String desc, String name, int ctrl_mode ){
		description = desc;
		ps_name = name;
		mode = ctrl_mode;
	}
	
	public String toString(){
		return description;
	}
}
