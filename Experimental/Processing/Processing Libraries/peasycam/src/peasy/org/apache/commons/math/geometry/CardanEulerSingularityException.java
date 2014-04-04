/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package peasy.org.apache.commons.math.geometry;

import peasy.org.apache.commons.math.MathException;

/**
 * This class represents exceptions thrown while extractiong Cardan or Euler
 * angles from a rotation.
 * 
 * @version $Revision: 620312 $ $Date: 2008-02-10 12:28:59 -0700 (Sun, 10 Feb
 *          2008) $
 * @since 1.2
 */
public class CardanEulerSingularityException extends MathException {

	/**
	 * Simple constructor. build an exception with a default message.
	 * 
	 * @param isCardan
	 *            if true, the rotation is related to Cardan angles, if false it
	 *            is related to EulerAngles
	 */
	public CardanEulerSingularityException(final boolean isCardan) {
		super(isCardan ? "Cardan angles singularity" : "Euler angles singularity",
				new Object[0]);
	}

	/** Serializable version identifier */
	private static final long serialVersionUID = -1360952845582206770L;

}
