package saito.objloader;

/*
 * Alias .obj loader for processing
 * programmed by Tatsuya SAITO / UCLA Design | Media Arts 
 * Created on 2005/04/17
 *
 * 
 *  
 */

import java.util.ArrayList;

/**
 * @author tatsuyas
 * 
 */
public class Group {
	public String groupName;
	public ArrayList<Segment> segments;

	public Group(String groupName) {
		segments = new ArrayList<Segment>();
		this.groupName = groupName;
	}

	public String getName() {
		return groupName;
	}
}
