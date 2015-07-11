/*
  Part of the GUI for Processing library 
  	http://www.lagers.org.uk/g4p/index.html
	http://gui4processing.googlecode.com/svn/trunk/

  Copyright (c) 2008-09 Peter Lager

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
 * CLASS FOR INTERNAL USE ONLY
 * 
 * @author Peter Lager
 *
 */
class GMessenger implements GConstants, GConstantsInternal {

	/**
	 * Display an error message message
	 * 
	 * @param id message ID number
	 * @param obj 
	 * @param info
	 */
	static void message(Integer id, Object[] info){
		// Display G4P messages if required
		if(G4P.showMessages){
			switch(id){
			case MISSING:
				missingEventHandler(info);
				break;
			case USER_COL_SCHEME:
				System.out.println("USER DEFINED colour schema active");
				break;
			}
		}
		// Display all runtime errors
		switch(id){
		case NONEXISTANT:
			nonexistantEventHandler(info);
			break;
		case EXCP_IN_HANDLER:
			eventHandlerFailed(info);
			break;
		}
	}
	
	/**
	 * Error message when an exception is create inside an event handler.
	 * @param handler
	 * @param info
	 */
	private static void eventHandlerFailed(Object[] info) {
		String className = info[0].getClass().getSimpleName();
		String methodName = (String) info[1];
		Exception e = (Exception) info[2];
		Throwable t = e.getCause();
		StackTraceElement[] calls = t.getStackTrace();
		StringBuilder output = new StringBuilder();
		output.append("################  EXCEPTION IN EVENT HANDLER  ################");
		output.append("\nAn error occured during execution of the eventhandler:");
		output.append("\nCLASS: "+className+"   METHOD: "+methodName);
		output.append("\n\tCaused by " + t.toString());
		for(Object line : calls)
			output.append("\n\t"+ line.toString());			
//		if(calls.length > 0)
//			output.append("\n\t"+ calls[0].toString());
		output.append("\n##############################################################\n");
		System.out.println(output);
	}

	//			GMessenger.message(, this, new Object[] {methodName, param_classes, param_names});

	/**
	 * Unable to find the default handler method.
	 * 
	 * info[0] event generator class
	 * info[1] event handling method name
	 * info[2] the parameter class names
	 * info[3] the parameter names (identifiers)
	 * 
	 * @param event_generator the object creating the events
	 * @param info method signature information.
	 */
	private static void missingEventHandler(Object[] info) {
		String className = info[0].getClass().getSimpleName();
		String methodName = (String) info[1];
		StringBuilder output = new StringBuilder();

		output.append("You might want to add a method to handle " + className + " events syntax is\n");
		output.append("public void " + methodName + "(");
		Class<?>[] param_classes = (Class[])(info[2]);
		String[] param_names = (String[])(info[3]);
		if(param_classes != null) {
			for(int i = 0; i < param_classes.length; i++){
				String pname = (param_classes[i]).getSimpleName();
				output.append(pname + " " + param_names[i]);
				if(i < param_classes.length - 1)
					output.append(", ");
			}
		}

		output.append(") { /* code */ }\n");
		System.out.println(output.toString());
	}

	/**
	 * Unable to find the user defined handler method.
	 * 
	 * info[0] event generator class
	 * info[1] event handling method name
	 * info[2] the parameter class names
	 * 
	 * 
	 * @param obj1 the object generating the method
	 * @param obj2 the method name
	 * @param obj3 parameter types (Class[])
	 */
	private static void nonexistantEventHandler(Object[] info) {
		String className = info[0].getClass().getSimpleName();
		String methodName = (String) info[1];
		String pname;
		StringBuilder output = new StringBuilder();

		output.append("The "+className+" class cannot find this method \n");
		output.append("\tpublic void " + methodName + "(");
		Class<?>[] param_names = (Class[])(info[2]);
		for(int i = 0; i < param_names.length; i++){
			pname = (param_names[i]).getSimpleName();
			output.append(pname + " " + pname.substring(1).toLowerCase());
				if(i < param_names.length - 1)
					output.append(", ");
		}
		output.append(") { /* code */ }\n");
		System.out.println(output.toString());
	}


}
