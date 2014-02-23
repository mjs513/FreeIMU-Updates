//Drawing points is much much faster than drawing spheres
void drawPoints(float screen_X, float screen_Y, float screen_Z, boolean highlight) {
  if (highlight) {
    stroke(255); // colour most recent points white
  } else {
    stroke(map(screen_Z, 0, 50, 240, 50), 99, 99); //otherwise map point colour to elevation
  }
  point(screen_X, screen_Y, screen_Z);
}

/* -- drawing spheres required too much processor gumption and 
 //slowed everything down so that pan and zoom were not satisfying
 void drawSpheres(float screen_X, float screen_Y, float screen_Z) {
 //fill(255, 0, 0);
 //fill(map(screen_Z, 80, 140, 250, 60), 100, 100);
 fill(map((200-screen_Z), 0, 150, 230, 359), 99, 99);
 
 pushMatrix();
 translate(screen_X, screen_Y, screen_Z);
 sphere(1);
 popMatrix();
 }
 */
