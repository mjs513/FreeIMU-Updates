/**
 * Copyright (c) 2008 Andrew Rapp. All rights reserved.
 *  
 * This file is part of XBee-API.
 *  
 * XBee-API is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *  
 * XBee-API is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with XBee-API.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.rapplogic.xbee.api.zigbee;

import java.util.EnumSet;
import java.util.HashMap;
import java.util.Map;

import com.rapplogic.xbee.api.AtCommandResponse;

// TODO create for wpan
public enum AssociationStatus {
	SUCCESS (0, "Successful completion - Coordinator started or Router/End Device found and joined with a parent."),
	NO_PAN (0x21, "Scan found no PANs"),
	NO_VALID_PAN (0x22, "Scan found no valid PANs based on current SC and ID settings"),
	NJ_EXPIRED (0x23, "Valid Coordinator or Routers found, but they are not allowing joining (NJ expired)"),
	NJ_FAILED (0x27, "Node Joining attempt failed (typically due to incompatible security settings)"),
	COORDINATOR_START_FAILED (0x2a, "Coordinator Start attempt failed"),
	SCANNING_FOR_PARENT (0xff, "Scanning for a Parent"),
	EXISTING_COORDINATOR_CHECK (0x2b, "Checking for an existing coordinator");
	
	private static final Map<Integer,AssociationStatus> lookup = new HashMap<Integer,AssociationStatus>();
	
	static {
		for(AssociationStatus s : EnumSet.allOf(AssociationStatus.class)) {
			lookup.put(s.getValue(), s);
		}
	}
	
	public static AssociationStatus get(int value) { 
		return lookup.get(value); 
	}
	
	public static AssociationStatus get(AtCommandResponse response) { 
		return AssociationStatus.get(response.getValue()[0]); 
	}
	
    private final int value;
    private final String description;
    
    AssociationStatus(int value, String description) {
        this.value = value;
        this.description = description;
        
    }

	public int getValue() {
		return value;
	}

	public String getDescription() {
		return description;
	}
}