package g4p_controls;

import java.util.HashMap;

import processing.core.PApplet;
import processing.core.PImage;

/**
 * This class is used to load bitmap files and create images. <br>
 * 
 * Although there maybe multiple requests for a particular bitmap file only
 * one PImage is created for each file.
 * 
 * @author Peter Lager
 *
 */
public class ImageManager {

	private static HashMap<String, PImage> textures = new HashMap<String, PImage>();
	
	/**
	 * Load a single bitmap file return a reference to the PImage created.
	 * 
	 * @param app
	 * @param filename
	 * @return null if the file does not exist else the PImage object
	 */
	public static PImage loadImage(PApplet app, String filename){
		if(textures.containsKey(filename)){
			return textures.get(filename);
		}
		PImage image = app.loadImage(filename);
		if(image != null){
			textures.put(filename, image);
		}
		else
			PApplet.println("Unable to load image from file '" + filename+"'");
		return image;
	}
	
	/**
	 * Load images from multiple files
	 * @param app
	 * @param filename an array of filenames
	 * @return an array of images
	 */
	public static PImage[] loadImage(PApplet app, String[] filename){
		PImage[] images = new PImage[filename.length];
		for(int i = 0; i < images.length; i++)
			images[i] = loadImage(app, filename[i]);
		return images;
	}
	
	
	
	/**
	 * Make multiple images from a given image. This method creates
	 * a 2D array (size [nCols, nRows] ) of PImage objects.
	 * 
	 * @param app
	 * @param img the tiled image
	 * @param nCols number of tiles across
	 * @param nRows number of tiles down
	 * @return a 2D array of images (tiles)
	 */
	public static PImage[][] makeTiles2D(PApplet app, PImage img, int nCols, int nRows){
		PImage[][] imageTiles = new PImage[nCols][nRows];
		int tileW = img.width / nCols;
		int tileH = img.height / nRows;
		for(int y = 0; y < nRows; y++){
			for(int x = 0; x < nCols; x++){
				imageTiles[x][y] = app.createImage(tileW, tileH, PApplet.ARGB);
				imageTiles[x][y].copy(img, x * tileW, y * tileH, tileW, tileH, 0, 0, tileW, tileH);
			}
		}
		return imageTiles;
	}
	
	/**
	 * Make multiple images from a given image. This method creates
	 * a 1D array of PImage objects. The order is left-right and top-down.
	 * 
	 * @param app
	 * @param img the tiled image
	 * @param nCols number of tiles across
	 * @param nRows number of tiles down
	 * @return a 1D array of images (tiles)
	 */
	public static PImage[] makeTiles1D(PApplet app, PImage img, int nCols, int nRows){
		PImage[] imageTiles = new PImage[nCols * nRows];
		int tileW = img.width / nCols;
		int tileH = img.height / nRows;
		int tileNo = 0;
		for(int y = 0; y < nRows; y++){
			for(int x = 0; x < nCols; x++){
				imageTiles[tileNo] = app.createImage(tileW, tileH, PApplet.ARGB);
				imageTiles[tileNo].copy(img, x * tileW, y * tileH, tileW, tileH, 0, 0, tileW, tileH);
				tileNo++;
			}
		}
		return imageTiles;
	}
	
	
	
}
