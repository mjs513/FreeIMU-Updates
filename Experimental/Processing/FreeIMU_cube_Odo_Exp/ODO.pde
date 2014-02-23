void position(){
  
  float g0 = 9.81;

  motionDetect_transition = motionDetect - motionDetect_old;
  
  if(motionDetect < 1){
    statex = statex + dyn_acc[0];
    statey = statey + dyn_acc[1];
    statez = statez + dyn_acc[2];
    state_cnt = state_cnt + 1;
  }
  
  if(motionDetect_transition == 1){
    statex_avg = statex / state_cnt;
    statey_avg = statey / state_cnt;
    statez_avg = statez / state_cnt;
    state_cnt = 0;
    statex = 0;
    statey = 0;
    statez = 0;
  } 
  
  motionDetect_old = motionDetect;
   
  accelerationx[1] = (dyn_acc[0]-statex_avg)*g0;
  accelerationy[1] = (dyn_acc[1]-statey_avg)*g0;
  accelerationz[1] = (dyn_acc[2]-statex_avg)*g0;
  
  dts = dt;
  
  //first ntegration:
  velocityx[1] = velocityx[0] + ((accelerationx[0]+accelerationx[1])/2)*dts;
  velocityy[1] = velocityy[0] + ((accelerationy[0]+accelerationy[1])/2)*dts;
  velocityz[1] = velocityz[0] + ((accelerationz[0]+accelerationz[1])/2)*dts;
  
  //if no motion is detected set velocity to zero to reduce errors propagating to position
  if(motionDetect < 1) {
    velocityx[1] = 0.0;
    velocityy[1] = 0.0;
    velocityz[1] = 0.0;
  }

  //second  integration:
  positionX[1]= positionX[0] + ((velocityx[0]+velocityx[1])/2)*dts;
  positionY[1]= positionY[0] + ((velocityy[0]+velocityy[1])/2)*dts;
  positionZ[1]= positionZ[0] + ((velocityz[1]+velocityz[1])/2)*dts;
  
  accelerationx[0] = accelerationx[1];   //The current acceleration value must be sent 
                                         //to the previous acceleration 
  accelerationy[0] = accelerationy[1];   //variable in order to introduce the new 
                                         //acceleration value.
  accelerationz[0] = accelerationz[1];                   
    
  velocityx[0] = velocityx[1];           //Same done for the velocity variable
  velocityy[0] = velocityy[1];
  velocityz[0] = velocityz[1];
  
  positionX[0]= positionX[1];
  positionY[0]= positionY[1];
  positionZ[0]= positionZ[1];
  
  
  
}

  
