/*
  Part of the GUI for Processing library 
  	http://www.lagers.org.uk/g4p/index.html
	http://gui4processing.googlecode.com/svn/trunk/

  Copyright (c) 2012 Peter Lager

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

import g4p_controls.HotSpot.HSrect;
import g4p_controls.StyledString.TextLayoutHitInfo;
import g4p_controls.StyledString.TextLayoutInfo;

import java.awt.Graphics2D;
import java.awt.Shape;
import java.awt.font.TextAttribute;
import java.awt.font.TextHitInfo;
import java.awt.font.TextLayout;
import java.awt.geom.GeneralPath;
import java.util.LinkedList;

import processing.core.PApplet;
import processing.core.PGraphics;
import processing.core.PGraphicsJava2D;
import processing.event.MouseEvent;

/**
 * The text area component. <br>
 * 
 * This control allows the user to enter and edit multiple lines of text. The control
 * also allows default text, horizontal and vertical scrollbars.
 * 
 * Enables user text input at runtime. Text can be selected using the mouse
 * or keyboard shortcuts and then copied or cut to the clipboard. Text
 * can also be pasted in.
 *
 * @author Peter Lager
 *
 */
public class GTextArea extends GEditableTextControl {

	private static float pad = 6;

	protected boolean newline = false, backspace = false;

	/**
	 * Create a text area without scrollbars and a text wrap width to fit the control.
	 * 
	 * @param theApplet
	 * @param p0
	 * @param p1
	 * @param p2
	 * @param p3
	 */
	public GTextArea(PApplet theApplet, float p0, float p1, float p2, float p3) {
		this(theApplet, p0, p1, p2, p3, SCROLLBARS_NONE, Integer.MAX_VALUE);
	}

	/**
	 * Create a text field with the given scrollbar policy and a text wrap width to fit the control. <br>
	 * The scrollbar policy can be one of these <br>
	 * <ul>
	 * <li>SCROLLBARS_NONE</li>
	 * <li>SCROLLBARS_HORIZONTAL_ONLY</li>
	 * <li>SCROLLBARS_VERTICAL_ONLY</li>
	 * <li>SCROLLBARS_BOTH</li>
	 * </ul>
	 * If you want the scrollbar to auto hide then perform a logical or with 
	 * <ul>
	 * <li>SCROLLBARS_AUTOHIDE</li>
	 * </ul>
	 * e.g. SCROLLBARS_BOTH | SCROLLBARS_AUTOHIDE
	 * <br>
	 * @param theApplet
	 * @param p0
	 * @param p1
	 * @param p2
	 * @param p3
	 * @param sbPolicy
	 */
	public GTextArea(PApplet theApplet, float p0, float p1, float p2, float p3, int sbPolicy) {
		this(theApplet, p0, p1, p2, p3, sbPolicy, Integer.MAX_VALUE);
	}

	/**
	 * Create a text field with the given scrollbar policy with a user specified text wrap length <br>
	 * 
	 * @param theApplet
	 * @param p0
	 * @param p1
	 * @param p2
	 * @param p3
	 * @param sbPolicy
	 * @param wrapWidth
	 */
	public GTextArea(PApplet theApplet, float p0, float p1, float p2, float p3, int sbPolicy, int wrapWidth) {
		super(theApplet, p0, p1, p2, p3, sbPolicy);
		children = new LinkedList<GAbstractControl>();
		tx = ty = pad;
		tw = width - 2 * pad - ((scrollbarPolicy & SCROLLBAR_VERTICAL) != 0 ? 18 : 0);
		th = height - 2 * pad - ((scrollbarPolicy & SCROLLBAR_HORIZONTAL) != 0 ? 18 : 0);
		// The text wrap width is based on the width of the text display area unless
		// some other value is specified.
		this.wrapWidth = (wrapWidth == Integer.MAX_VALUE) ? (int)tw : wrapWidth;
		// Clip zone
		gpTextDisplayArea = new GeneralPath();
		gpTextDisplayArea.moveTo( 0,  0);
		gpTextDisplayArea.lineTo( 0, th);
		gpTextDisplayArea.lineTo(tw, th);
		gpTextDisplayArea.lineTo(tw,  0);
		gpTextDisplayArea.closePath();

		// The image buffer is just for the typing area
		buffer = (PGraphicsJava2D) winApp.createGraphics((int)width, (int)height, PApplet.JAVA2D);
		buffer.rectMode(PApplet.CORNER);
		buffer.g2.setFont(localFont);
		hotspots = new HotSpot[]{
				new HSrect(1, tx, ty, tw, th),			// typing area
				new HSrect(9, 0, 0, width, height)		// control surface
		};
		G4P.pushStyle();
		G4P.showMessages = false;
		z = Z_STICKY;
		G4P.control_mode = GControlMode.CORNER;
		if((scrollbarPolicy & SCROLLBAR_HORIZONTAL) != 0){
			hsb = new GScrollbar(theApplet, 0, 0, tw, 16);
			addControl(hsb, tx, ty + th + 2, 0);
			hsb.addEventHandler(this, "hsbEventHandler");
			hsb.setAutoHide(autoHide);
		}
		if((scrollbarPolicy & SCROLLBAR_VERTICAL) != 0){
			vsb = new GScrollbar(theApplet, 0, 0, th, 16);
			addControl(vsb, tx + tw + 18, ty, PI/2);
			vsb.addEventHandler(this, "vsbEventHandler");
			vsb.setAutoHide(autoHide);
		}
		G4P.popStyle();
		setText("", (int)tw);
		//		z = Z_STICKY;
		createEventHandler(G4P.sketchApplet, "handleTextEvents", 
				new Class<?>[]{ GEditableTextControl.class, GEvent.class }, 
				new String[]{ "textcontrol", "event" } 
				);
		registeredMethods = PRE_METHOD | DRAW_METHOD | MOUSE_METHOD | KEY_METHOD;
		G4P.addControl(this);
	}

	/**
	 * Set the default text for this control. If provided this text will be 
	 * displayed in italic whenever it is empty. It will also be wrapped to the control size.
	 * @param dtext
	 */
	public void setDefaultText(String dtext){
		if(dtext == null || dtext.length() == 0)
			defaultText = null;
		else {
			defaultText = new StyledString(dtext, wrapWidth);
			defaultText.addAttribute(G4P.POSTURE, G4P.POSTURE_OBLIQUE);
		}
		bufferInvalid = true;
	}

	/**
	 * Set the text to be used. The wrap width is determined by the current
	 * text wrapwidth or if there is no text then the text width 
	 * of the control.
	 * 
	 * @param text
	 */
	public void setText(String text){
		if(stext == null)
			setText(text, wrapWidth);
		else
			setText(text, stext.getWrapWidth());
		bufferInvalid = true;
	}

	/**
	 * Set the text to display and adjust any scrollbars
	 * @param text text to display
	 * @param wrapWidth the wrap width
	 */
	public void setText(String text, int wrapWidth){
//		this.wrapWidth = wrapWidth;
		setStyledText(new StyledString(text, wrapWidth));
		bufferInvalid = true;
	}

	/**
	 * Set the text to display and adjust any scrollbars
	 * @param text lines of text to display
	 */
	public void setText(String[] lines){
		if(lines != null){
			setText(PApplet.join(lines, "\n"));
			bufferInvalid = true;
		}
	}

	/**
	 * Set the text to display and adjust any scrollbars
	 * @param text lines of text to display
	 * @param wrapWidth the wrap width
	 */
	public void setText(String[] lines, int wrapWidth){
		if(lines != null){
			setText(PApplet.join(lines, "\n"), wrapWidth);
			bufferInvalid = true;
		}
	}

	/**
	 * Get the text as a String array. (splitting on line breaks).
	 * 
	 * @return the associated plain text as a String array split on line breaks
	 */
	public String[] getTextAsArray(){
		return stext.getPlainTextAsArray();
	}

	/**
	 * Adds the text attribute to a range of characters on a particular line. If charEnd
	 * is past the EOL then the attribute will be applied to the end-of-line.
	 * 
	 * @param attr the text attribute to add
	 * @param value value of the text attribute
	 * @param lineNo the line number (starts at 0)
	 * @param charStart the position of the first character to apply the attribute
	 * @param charEnd the position after the last character to apply the attribute
	 */
	public void addStyle(TextAttribute attr, Object value, int lineNo, int charStart, int charEnd){
		if(stext != null){
			stext.addAttribute(attr, value, lineNo, charStart, charEnd);
			bufferInvalid = true;
		}
	}

	/**
	 * Clears all text attribute from a range of characters on a particular line. 
	 * If charEnd is past the EOL then the attributes will be cleared to the 
	 * end-of-line.
	 * 
	 * @param lineNo the line number (starts at 0)
	 * @param charStart the position of the first character to apply the attribute
	 * @param charEnd the position after the last character to apply the attribute
	 */
	public void clearStyles(int lineNo, int charStart, int charEnd){
		if(stext != null){
			stext.clearAttributes(lineNo, charStart, charEnd);
			bufferInvalid = true;
		}
	}

	/**
	 * Set the styled text to be displayed.
	 * 
	 */
	public void setStyledText(StyledString st){
		stext = st;
		if(stext.getWrapWidth() == Integer.MAX_VALUE)
			stext.setWrapWidth(wrapWidth);
		else
			wrapWidth = stext.getWrapWidth();
		stext.getLines(buffer.g2);
		if(stext.getNbrLines() > 0){
			endTLHI.tli = stext.getLines(buffer.g2).getFirst();
			endTLHI.thi = endTLHI.tli.layout.getNextLeftHit(1);	
			startTLHI.copyFrom(endTLHI);
			calculateCaretPos(endTLHI);
			keepCursorInView = true;
		}
		ptx = pty = 0;
		float sTextHeight;
		// If needed update the vertical scrollbar
		if(vsb != null){
			sTextHeight = stext.getTextAreaHeight();
			if(sTextHeight < th)
				vsb.setValue(0.0f, 1.0f);
			else 
				vsb.setValue(0, th/sTextHeight);
		}
		// If needed update the horizontal scrollbar
		if(hsb != null){
			if(stext.getMaxLineLength() < tw)
				hsb.setValue(0,1);
			else
				hsb.setValue(0, tw/stext.getMaxLineLength());
		}
		bufferInvalid = true;				
	}

	/**
	 * Add text to the end of the current text. This is useful for a logging' type activity.
	 * 
	 * @param extraText the text to append
	 */
	public void appendText(String extraText){
		if(extraText == null || extraText.equals(""))
			return;
		if(stext.insertCharacters(stext.length(), extraText, true) == 0)
			return;

		LinkedList<TextLayoutInfo> lines = stext.getLines(buffer.g2);
		endTLHI.tli = lines.getLast();
		endTLHI.thi = endTLHI.tli.layout.getNextRightHit(endTLHI.tli.nbrChars - 1);
		startTLHI.copyFrom(endTLHI);
		calculateCaretPos(endTLHI);
		if(vsb != null){
			float vfiller = Math.min(1, th/stext.getTextAreaHeight());
			vsb.setValue(1 - vfiller, vfiller);
			keepCursorInView = true;
		}
		// If needed update the horizontal scrollbar
		if(hsb != null){
			float hvalue = lines.getLast().layout.getVisibleAdvance();
			float hlinelength = stext.getMaxLineLength();
			float hfiller = Math.min(1, tw/hlinelength);
			if(caretX < tw)
				hsb.setValue(0,hfiller);
			else 
				hsb.setValue(hvalue/hlinelength, hfiller);
			keepCursorInView = true;
		}
		bufferInvalid = true;
	}

	/**
	 * If the buffer is invalid then redraw it.
	 */
	protected void updateBuffer(){
		if(bufferInvalid) {
			Graphics2D g2d = buffer.g2;
			// Get the latest lines of text
			LinkedList<TextLayoutInfo> lines = stext.getLines(g2d);
			if(lines.isEmpty() && defaultText != null)
				lines = defaultText.getLines(g2d);

			bufferInvalid = false;

			TextLayoutHitInfo startSelTLHI = null, endSelTLHI = null;
			buffer.beginDraw();
			// Whole control surface if opaque
			if(opaque)
				buffer.background(palette[6]);
			else
				buffer.background(buffer.color(255,0));

			// Now move to top left corner of text display area
			buffer.translate(tx,ty); 

			// Typing area surface
			buffer.noStroke();
			buffer.fill(palette[7]);
			buffer.rect(-1,-1,tw+2,th+2);

			g2d.setClip(gpTextDisplayArea);
			buffer.translate(-ptx, -pty);
			// Translate in preparation for display selection and text
			if(hasSelection()){
				if(endTLHI.compareTo(startTLHI) == -1){
					startSelTLHI = endTLHI;
					endSelTLHI = startTLHI;
				}
				else {
					startSelTLHI = startTLHI;
					endSelTLHI = endTLHI;
				}
			}	

			// Display selection and text
			for(TextLayoutInfo lineInfo : lines){
				TextLayout layout = lineInfo.layout;
				buffer.translate(0, layout.getAscent());
				// Draw selection if any
				if(hasSelection() && lineInfo.compareTo(startSelTLHI.tli) >= 0 && lineInfo.compareTo(endSelTLHI.tli) <= 0 ){				
					int ss = 0;
					ss = (lineInfo.compareTo(startSelTLHI.tli) == 0) ? startSelTLHI.thi.getInsertionIndex()  : 0;
					int ee = endSelTLHI.thi.getInsertionIndex();
					ee = (lineInfo.compareTo(endSelTLHI.tli) == 0) ? endSelTLHI.thi.getInsertionIndex() : lineInfo.nbrChars-1;
					g2d.setColor(jpalette[14]);
					Shape selShape = layout.getLogicalHighlightShape(ss, ee);
					g2d.fill(selShape);
				}
				// display text
				g2d.setColor(jpalette[2]);
				lineInfo.layout.draw(g2d, 0, 0);
				buffer.translate(0, layout.getDescent() + layout.getLeading());
			}
			g2d.setClip(null);
			buffer.endDraw();
		}
	}

	public void pre(){
		if(keepCursorInView){
			boolean horzScroll = false, vertScroll = false;
			float max_ptx = caretX - tw + 2;
			float max_pty = caretY - th + 2 * stext.getMaxLineHeight();

			if(endTLHI != null){
				if(ptx > caretX){ 						// LEFT?
					ptx -= HORZ_SCROLL_RATE;
					if(ptx < 0) ptx = 0;
					horzScroll = true;
				}
				else if(ptx < max_ptx){ 				// RIGHT?
					ptx += HORZ_SCROLL_RATE;
					if(ptx > max_ptx) ptx = max_ptx;
					horzScroll = true;
				}
				if(pty > caretY){						// UP?
					pty -= VERT_SCROLL_RATE;
					if(pty < 0) pty = 0;
					vertScroll = true;
				}
				else if(pty < max_pty){					// DOWN?
					pty += VERT_SCROLL_RATE;
					vertScroll = true;
				}
				if(horzScroll && hsb != null)
					hsb.setValue(ptx / (stext.getMaxLineLength() + 4));
				if(vertScroll && vsb != null)
					vsb.setValue(pty / (stext.getTextAreaHeight() + 1.5f * stext.getMaxLineHeight()));
			}
			// If we have scrolled invalidate the buffer otherwise forget it
			if(horzScroll || vertScroll)
				bufferInvalid = true;
			else
				keepCursorInView = false;
		}
	}

	public void draw(){
		if(!visible) return;

		// Update buffer if invalid
		updateBuffer();
		winApp.pushStyle();

		winApp.pushMatrix();
		// Perform the rotation
		winApp.translate(cx, cy);
		winApp.rotate(rotAngle);

		winApp.pushMatrix();
		// Move matrix to line up with top-left corner
		winApp.translate(-halfWidth, -halfHeight);
		// Draw buffer
		winApp.imageMode(PApplet.CORNER);
		if(alphaLevel < 255)
			winApp.tint(TINT_FOR_ALPHA, alphaLevel);
		winApp.image(buffer, 0, 0);

		// Draw caret if text display area
		if(focusIsWith == this && showCaret && endTLHI != null){
			float[] cinfo = endTLHI.tli.layout.getCaretInfo(endTLHI.thi);
			float x_left =  - ptx + cinfo[0];
			float y_top = - pty + endTLHI.tli.yPosInPara; 
			float y_bot = y_top - cinfo[3] + cinfo[5];
			if(x_left >= 0 && x_left <= tw && y_top >= 0 && y_bot <= th){
				winApp.strokeWeight(1.9f);
				winApp.stroke(palette[15]);
				winApp.line(tx+x_left, ty+Math.max(0, y_top), tx+x_left, ty+Math.min(th, y_bot));
			}
		}

		winApp.popMatrix();
		// Draw scrollbars
		if(children != null){
			for(GAbstractControl c : children)
				c.draw();
		}
		winApp.popMatrix();
		winApp.popStyle();
	}

	public PGraphics getSnapshot(){
		updateBuffer();
		PGraphicsJava2D snap = (PGraphicsJava2D) winApp.createGraphics(buffer.width, buffer.height, PApplet.JAVA2D);
		snap.beginDraw();
		snap.image(buffer,0,0);
		if(hsb != null){
			snap.pushMatrix();
			snap.translate(hsb.getX(), hsb.getY());
			snap.image(hsb.getBuffer(), 0, 0);
			snap.popMatrix();
		}
		if(vsb != null){
			snap.pushMatrix();
			snap.translate(vsb.getX(), vsb.getY());
			snap.rotate(PApplet.PI/2);
			snap.image(vsb.getBuffer(), 0, 0);
			snap.popMatrix();
		}
		snap.endDraw();
		return snap;
	}

	protected void keyPressedProcess(int keyCode, char keyChar, boolean shiftDown, boolean ctrlDown){
		boolean validKeyCombo = true;

		switch(keyCode){
		case LEFT:
			moveCaretLeft(endTLHI);
			break;
		case RIGHT:
			moveCaretRight(endTLHI);
			break;
		case UP:
			moveCaretUp(endTLHI);
			break;
		case DOWN:
			moveCaretDown(endTLHI);
			break;
		case GConstants.HOME:
			if(ctrlDown)		// move to start of text
				moveCaretStartOfText(endTLHI);
			else 	// Move to start of line
				moveCaretStartOfLine(endTLHI);
			break;
		case GConstants.END:
			if(ctrlDown)		// move to end of text
				moveCaretEndOfText(endTLHI);
			else 	// Move to end of line
				moveCaretEndOfLine(endTLHI);
			break;
		case 'A':
			if(ctrlDown){
				moveCaretStartOfText(startTLHI);
				moveCaretEndOfText(endTLHI);
				// Make shift down so that the start caret position is not
				// moved to match end caret position.
				shiftDown = true;
			}
			break;
		case 'C':
			if(ctrlDown)
				GClip.copy(getSelectedText());
			validKeyCombo = false;
			break;
		case 'V':
			if(ctrlDown){
				String p = GClip.paste();
				if(p.length() > 0){
					// delete selection and add 
					if(hasSelection())
						stext.deleteCharacters(pos, nbr);
					stext.insertCharacters(pos, p);
					adjust = p.length();
					textChanged = true;
				}
			}
			break;
		default:
			validKeyCombo = false;
		}

		if(validKeyCombo){
			calculateCaretPos(endTLHI);
			//****************************************************************
			// If we have moved  to the end of a paragraph marker
			if(caretX > stext.getWrapWidth()){
				switch(keyCode){
				case LEFT:
				case UP:
				case DOWN:
				case END:
					moveCaretLeft(endTLHI);
					validKeyCombo = true;
					break;
				case RIGHT:
					if(!moveCaretRight(endTLHI))
						moveCaretLeft(endTLHI);
					validKeyCombo = true;
				}
				// Calculate new caret position
				// calculateCaretPos(startTLHI); 
				calculateCaretPos(endTLHI);
			}
			//****************************************************************

			calculateCaretPos(endTLHI);	

			if(!shiftDown)
				startTLHI.copyFrom(endTLHI);
			bufferInvalid = true;
		}
	}

	protected void keyTypedProcess(int keyCode, char keyChar, boolean shiftDown, boolean ctrlDown){
		int ascii = (int)keyChar;
		newline = false;
		backspace = false;

		if(ascii >= 32 && ascii < 127){
			if(hasSelection())
				stext.deleteCharacters(pos, nbr);
			stext.insertCharacters(pos, "" + keyChar);
			adjust = 1; textChanged = true;
		}
		else if(keyChar == BACKSPACE){
			if(hasSelection()){
				stext.deleteCharacters(pos, nbr);
				adjust = 0; textChanged = true;				
			}
			else if(stext.deleteCharacters(pos - 1, 1)){
				adjust = -1; textChanged = true; backspace = true;
			}
		}
		else if(keyChar == DELETE){
			if(hasSelection()){
				stext.deleteCharacters(pos, nbr);
				adjust = 0; textChanged = true;				
			}
			else if(stext.deleteCharacters(pos, 1)){
				adjust = 0; textChanged = true;
			}
		}
		else if(keyChar == ENTER || keyChar == RETURN) {
			if(stext.insertEOL(pos)){
				adjust = 1; textChanged = true;
				newline = true;
			}
		}
		else if(keyChar == TAB){
			// If possible move to next text control
			if(tabManager != null){
				boolean result = (shiftDown) ? tabManager.prevControl(this) : tabManager.nextControl(this);
				if(result){
					startTLHI.copyFrom(endTLHI);
					return;
				}
			}
		}
		// If we have emptied the text then recreate a one character string (space)
		if(stext.length() == 0){
			stext.insertCharacters(0, " ");
			adjust++; textChanged = true;
		}
	}

	protected boolean changeText(){
		if(!super.changeText())
			return false;
		// The following actions handle multi-line stuff
		// Do we have to move cursor to start of next line
		if(newline) {
			if(pos >= stext.length()){
				stext.insertCharacters(pos, " ");
				stext.getLines(buffer.g2);
			}
			moveCaretRight(endTLHI);
			calculateCaretPos(endTLHI);
		}
		if(backspace && pos > 0){
			char ch = stext.getPlainText().charAt(pos-1);
			if(ch == '\n'){
				moveCaretRight(endTLHI);
				calculateCaretPos(endTLHI);
			}
			if(pos >= stext.length()){
				stext.insertCharacters(pos, " ");
				stext.getLines(buffer.g2);
			}
		}
		// Finish off by ensuring no selection, invalidate buffer etc.
		startTLHI.copyFrom(endTLHI);
		return true;

	}

	/**
	 * Move caret to home position
	 * @return true if caret moved else false
	 */
	protected boolean moveCaretStartOfLine(TextLayoutHitInfo currPos){
		if(currPos.thi.getCharIndex() == 0)
			return false; // already at start of line
		currPos.thi = currPos.tli.layout.getNextLeftHit(1);
		return true;
	}

	protected boolean moveCaretEndOfLine(TextLayoutHitInfo currPos){
		if(currPos.thi.getCharIndex() == currPos.tli.nbrChars - 1)
			return false; // already at end of line
		currPos.thi = currPos.tli.layout.getNextRightHit(currPos.tli.nbrChars - 1);
		return true;
	}

	protected boolean moveCaretStartOfText(TextLayoutHitInfo currPos){
		if(currPos.tli.lineNo == 0 && currPos.thi.getCharIndex() == 0)
			return false; // already at start of text
		currPos.tli = stext.getTLIforLineNo(0);
		currPos.thi = currPos.tli.layout.getNextLeftHit(1);
		return true;
	}

	protected boolean moveCaretEndOfText(TextLayoutHitInfo currPos){
		if(currPos.tli.lineNo == stext.getNbrLines() - 1 && currPos.thi.getCharIndex() == currPos.tli.nbrChars - 1)
			return false; // already at end of text
		currPos.tli = stext.getTLIforLineNo(stext.getNbrLines() - 1);		
		currPos.thi = currPos.tli.layout.getNextRightHit(currPos.tli.nbrChars - 1);
		return true;
	}

	protected boolean moveCaretUp(TextLayoutHitInfo currPos){
		if(currPos.tli.lineNo == 0)
			return false;
		TextLayoutInfo ntli = stext.getTLIforLineNo(currPos.tli.lineNo - 1);	
		TextHitInfo nthi = ntli.layout.hitTestChar(caretX, 0);
		currPos.tli = ntli;
		currPos.thi = nthi;
		return true;
	}

	protected boolean moveCaretDown(TextLayoutHitInfo currPos){
		if(currPos.tli.lineNo == stext.getNbrLines() - 1)
			return false;
		TextLayoutInfo ntli = stext.getTLIforLineNo(currPos.tli.lineNo + 1);	
		TextHitInfo nthi = ntli.layout.hitTestChar(caretX, 0);
		currPos.tli = ntli;
		currPos.thi = nthi;
		return true;
	}

	/**
	 * Move caret left by one character. If necessary move to the end of the line above
	 * @return true if caret was moved else false
	 */
	protected boolean moveCaretLeft(TextLayoutHitInfo currPos){
		TextLayoutInfo ntli;
		TextHitInfo nthi = currPos.tli.layout.getNextLeftHit(currPos.thi);
		if(nthi == null){ 
			// Move the caret to the end of the previous line 
			if(currPos.tli.lineNo == 0)
				// Can't goto previous line because this is the first line
				return false;
			else {
				// Move to end of previous line
				ntli = stext.getTLIforLineNo(currPos.tli.lineNo - 1);
				nthi = ntli.layout.getNextRightHit(ntli.nbrChars-1);
				currPos.tli = ntli;
				currPos.thi = nthi;
			}
		}
		else {
			// Move the caret to the left of current position
			currPos.thi = nthi;
		}
		return true;
	}

	/**
	 * Move caret right by one character. If necessary move to the start of the next line 
	 * @return true if caret was moved else false
	 */
	protected boolean moveCaretRight(TextLayoutHitInfo currPos){
		TextLayoutInfo ntli;
		TextHitInfo nthi = currPos.tli.layout.getNextRightHit(currPos.thi);
		if(nthi == null){ 
			// Move the caret to the start of the next line the previous line 
			if(currPos.tli.lineNo >= stext.getNbrLines() - 1)
				// Can't goto next line because this is the last line
				return false;
			else {
				// Move to start of next line
				ntli = stext.getTLIforLineNo(currPos.tli.lineNo + 1);
				nthi = ntli.layout.getNextLeftHit(1);
				currPos.tli = ntli;
				currPos.thi = nthi;
			}
		}
		else {
			// Move the caret to the right of current position
			currPos.thi = nthi;
		}
		return true;
	}

	/**
	 * Move the insertion point (caret) to the specified line and character. If the position is invalid
	 * then the caret is not moved. The text will be scrolled so that the caret position is visible.
	 * 
	 * @param lineNo the line number (starts at 0)
	 * @param charNo the character position on the line (starts at 0)
	 */
	public void moveCaretTo(int lineNo, int charNo){
		try {
			TextLayoutHitInfo thli = stext.getTLHIforCharPosition(lineNo, charNo);
			if(thli != null){
				startTLHI.copyFrom(thli);
				endTLHI.copyFrom(thli);
				calculateCaretPos(thli);
				keepCursorInView = true;
				showCaret = true;
			}
		}
		catch(Exception e){}
	}
	/**
	 * Will respond to mouse events.
	 */
	public void mouseEvent(MouseEvent event){
		if(!visible  || !enabled || !available) return;

		calcTransformedOrigin(winApp.mouseX, winApp.mouseY);
		ox -= tx; oy -= ty; // Remove translation

		currSpot = whichHotSpot(ox, oy);

		if(currSpot == 1 || focusIsWith == this)
			cursorIsOver = this;
		else if(cursorIsOver == this)
			cursorIsOver = null;

		switch(event.getAction()){
		case MouseEvent.PRESS:
			if(currSpot == 1){
				if(focusIsWith != this && z >= focusObjectZ()){
					keepCursorInView = true;
					takeFocus();
				}
				dragging = false;
				if(stext == null || stext.length() == 0){
					stext = new StyledString(" ", wrapWidth);
					stext.getLines(buffer.g2);
				}
				endTLHI = stext.calculateFromXY(buffer.g2, ox + ptx, oy + pty);
				startTLHI = new TextLayoutHitInfo(endTLHI);
				calculateCaretPos(endTLHI);
				bufferInvalid = true;
			}
			else { // Not over this control so if we have focus loose it
				if(focusIsWith == this)
					loseFocus(null);
			}
			break;
		case MouseEvent.RELEASE:
			dragging = false;
			bufferInvalid = true;
			break;
		case MouseEvent.DRAG:
			if(focusIsWith == this){
				keepCursorInView = true;
				dragging = true;
				endTLHI = stext.calculateFromXY(buffer.g2, ox + ptx, oy + pty);
				calculateCaretPos(endTLHI);
				fireEvent(this, GEvent.SELECTION_CHANGED);
				bufferInvalid = true;
			}
			break;
		}
	}

	protected void calculateCaretPos(TextLayoutHitInfo tlhi){
		float temp[] = tlhi.tli.layout.getCaretInfo(tlhi.thi);
		caretX = temp[0];		
		caretY = tlhi.tli.yPosInPara;
	}



}
