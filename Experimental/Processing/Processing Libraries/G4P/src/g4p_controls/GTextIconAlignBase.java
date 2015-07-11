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

import processing.core.PApplet;
import processing.core.PImage;

/**
 * Base class for controls with text and/or icon.<br>
 * 
 * This class forms the basis for any control that has text and/or an icon. <br>
 * Use the setIcon, setIconAlign, setText and setTextAlign to control 
 * horizontal and vertical alignment of the icon and text withing the control face.
 * 
 * @author Peter Lager
 *
 */
public abstract class GTextIconAlignBase extends GTextAlign {

	protected PImage[] bicon = null;
	protected int iconW = 0, iconH = 0;
	protected GAlign iconAlignH = GAlign.RIGHT, iconAlignV = GAlign.MIDDLE;
	protected int siX, siY;


	public GTextIconAlignBase(PApplet theApplet, float p0, float p1, float p2, float p3) {
		super(theApplet, p0, p1, p2, p3);
	}

	/**
	 * Set the text to be displayed and calculate the wrap length taking into
	 * account any icon set.
	 * 
	 * @param text
	 */
	public void setText(String text){
		if(text == null || text.length() == 0 )
			text = " ";
		if(iconW == 0)
			stext = new StyledString(text, (int) width - TPAD2);
		else
			stext = new StyledString(text, (int) width - iconW - TPAD4);
		bufferInvalid = true;
	}

	/**
	 * Set the icon to be used and the horizontal and/or vertical icon alignment. 
	 * Use the constants in GAlign e.g. <pre>GAlign.LEFT</pre> <br>
	 * 
	 * @param fname the filename of the icon
	 * @param nbrImages number of tiled images in the icon
	 * @param horz LEFT or RIGHT
	 * @param vert TOP, MIDDLE, BOTTOM
	 */
	public void setIcon(String fname, int nbrImages, GAlign horz, GAlign vert){
		PImage iconImage = ImageManager.loadImage(winApp, fname);
		setIcon(iconImage, nbrImages, horz, vert);
	}

	/**
	 * Set the icon to be used and the horizontal and/or vertical icon alignment. 
	 * Use the constants in GAlign e.g. <pre>GAlign.LEFT</pre> <br>
	 * 
	 * If you want to set just one of these then pass null in the other 
	 * 
	 * @param icon the icon
	 * @param nbrImages number of tiled images in the icon
	 * @param horz LEFT or RIGHT
	 * @param vert TOP, MIDDLE, BOTTOM
	 */
	public void setIcon(PImage icon, int nbrImages, GAlign horz, GAlign vert){
		if(icon != null){
			if(nbrImages == 3)
				bicon = ImageManager.makeTiles1D(winApp, icon, nbrImages, 1);
			else {
				bicon = new PImage[3];
				PImage[] temp = ImageManager.makeTiles1D(winApp, icon, nbrImages, 1);
				System.arraycopy(temp, 0, bicon, 0, temp.length);
				for(int i = temp.length; i < 3; i++){
						bicon[i] = bicon[i-1];
				}
			}

			// We have loaded the image so validate alignment
			if(horz != null && horz.isHorzAlign() && horz != GAlign.CENTER){ 
				iconAlignH = horz;
			}
			if(vert != null && vert.isVertAlign()){
				iconAlignV = vert;
			}
			iconW = bicon[0].width;
			iconH = bicon[0].height;
			stext.setWrapWidth((int) width - iconW - TPAD4);
			bufferInvalid = true;
		}
	}

	/**
	 * Change the alignment of an existing icon.
	 * @param horz horizontal alignment (see @see GAlign)
	 * @param vert vertical alignment (see @see GAlign)
	 */
	public void setIconAlign(GAlign horz, GAlign vert){
		if(iconW != 0){
			if(horz != null && horz.isHorzAlign() && horz != GAlign.CENTER){
				iconAlignH = horz;
			}
			if(vert != null && vert != null && vert.isVertAlign()){
				iconAlignV = vert;
			}
			bufferInvalid = true;
		}
	}

	/**
	 * Calculate various values based on alignment of text and icon
	 */
	protected void calcAlignment(){
		super.calcAlignment();	// calculate the text alignment
		if(iconW != 0){
			switch(iconAlignH){
			case LEFT:
				siX = TPAD;
				if(textAlignH != GAlign.RIGHT)
					stX += (iconW + TPAD2); // Image on left so adjust text start x position
				break;
			case RIGHT:
			default:
				siX = (int)width - iconW - TPAD2;
				if(textAlignH == GAlign.RIGHT)
					stX -= (iconW + TPAD2);
				break;
			}
			switch(iconAlignV){
			case TOP:
				siY = TPAD;
				break;
			case BOTTOM:
				siY =(int) height - iconH - TPAD2;
				break;
			case MIDDLE:
			default:
				siY = (int)(height - iconH)/2;
			}
		}
	}
	
	public String toString(){
		return tag;
	}
}
