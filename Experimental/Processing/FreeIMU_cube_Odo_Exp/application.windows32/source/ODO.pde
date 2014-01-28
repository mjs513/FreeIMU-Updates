
/**************************************************************************
//This function allows movement end detection. If a certain number of 
//acceleration samples are  equal to zero we can assume movement has stopped.
//Accumulated Error generated in the velocity calculations is eliminated by 
//resetting the velocity variables. This stops position increment and greatly
//eliminates position error.
//***************************************************************************/  

void movement_end_check()   
{

  if (accelerationx[1] == 0)   //we count the number of acceleration samples that equals zero 
{ countx++;}
  else { countx = 0;}

  if (countx >= 25)             //if this number exceeds 25, we can assume that velocity is zero
   {    velocityx[1] = 0;
        velocityx[0] = 0;
    }
   
   if (accelerationy[1] == 0)   //we do the same for the Y axis
   { county++;}
   else {  county = 0;}
  
  if (county >= 25)
   { 
     velocityy[1] = 0;
     velocityy[0] = 0;
    } 
}

//***********************************************************************
//***********************************************************************
//This function transforms acceleration to a proportional position by 
//integrating the acceleration data twice. It also adjusts sensibility by   
//multiplying the "positionX" and "positionY" variables. 
//This integration algorithm carries error, which is compensated in the 
//"movenemt end check" subroutine. Faster sampling frequency implies less 
//error but requires more memory. Keep in  mind that the same process is 
//applied to the X and Y axis. 
//******************************************************************************/  
 
void position()  
{
  int count2=0;
    
  do{
     //ADC GetAllAxis();
     gravity_compensate();
     
     accelerationx[0] = accelerationx[0] + Sample_X; //filtering routine for noise attenuation
     accelerationy[0] = accelerationy[0] + Sample_Y; //64 samples are averaged. The resulting 
     //average represents the acceleration of 
     //an instant
      count2++;                                       

    } while (count2 != 0x40);                   // 64 sums of the acceleration sample
  

  accelerationx[0]= accelerationx[0]/64;       // division by 64
  accelerationy[0]= accelerationy[0]/64;
  
  accelerationx[0] = accelerationx[0] - (int)sstatex;  //eliminating zero reference 
                                                       //offset of the acceleration data
  accelerationy[0] = accelerationy[0] - (int)sstatey;  //to obtain positive and negative 
                                                       //acceleration
  
  if ((accelerationx[0] <=0.010)&&(accelerationx[0] >= -0.010))  //Discrimination window applied 
          {accelerationx[0] = 0;}                        // to the X axis acceleration 
                                                         //variable
               
  if ((accelerationy[0] <=0.010) && (accelerationy[0] >= -0.010)) 
                {accelerationy[0] = 0;} 
  
  //first X integration:
  velocityx[0]= velocityx[0]+ accelerationx[0]+ ((accelerationx[1] - accelerationx[0] )/2);  

  //second X integration:
  positionX[0]= positionX[0] + velocityx[0] + ((velocityx[1] - velocityx[0])/2);

  //first Y integration:       
  velocityy[0] = velocityy[0] + accelerationy[0] + ((accelerationy[1] -accelerationy[0])/2);

  //second Y integration:       
  positionY[0] = positionY[0] + velocityy[0] + ((velocityy[1] - velocityy[0])/2);  

  accelerationx[1] = accelerationx[0];   //The current acceleration value must be sent 
                                         //to the previous acceleration 
  accelerationy[1] = accelerationy[0];   //variable in order to introduce the new 
                                         //acceleration value.
    
  velocityx[1] = velocityx[0];         //Same done for the velocity variable
  velocityy[1] = velocityy[0];
  
  /* positionX[1] = positionX[1]*262144;       //The idea behind this shifting (multiplication) 
                                         //is a sensibility adjustment. <<18
  positionY[1] = positionY[1]*262144;      //Some applications require adjustments to a 
                                        //particular situation 
                                        //i.e. mouse application 
  */
  
  //data_transfer();
  
  /*  
  positionX[1] = positionX[1]/262144;      //once the variables are sent them must return to 
  positionY[1] = positionY[1]/262144;      //their original state
  */
  //text("px:  " + positionX[0] + "\n" + "py:  " + positionY[0], 200, 200);
  
  movement_end_check();
  
  /*  
  positionX[0] = positionX[1];          //actual position data must be sent to the  
  positionY[0] = positionY[1];     //previous position
  */  

  direction = 0;                        // data variable to direction variable reset


}    

//*****************************************************************************/

// compensate the accelerometer readings from gravity. 
// @param q the quaternion representing the orientation of a 9DOM MARG sensor array
// @param acc the readings coming from an accelerometer expressed in g
//
// Code snippet from Fabio Versano - blog on same topic
//
// @return a 3d vector representing dynamic acceleration expressed in g
void gravity_compensate(){
  float [] g = {0.0, 0.0, 0.0};
  
  // get expected direction of gravity
  g[0] = 2 * (q[1] * q[3] - q[0] * q[2]);
  g[1] = 2 * (q[0] * q[1] + q[2] * q[3]);
  g[2] = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
  
  // compensate accelerometer readings with the expected direction of gravity
  //return [acc[0] - g[0], acc[1] - g[1], acc[2] - g[2]]
  Sample_X = acc[0] - g[0];
  Sample_Y = acc[1] - g[1];

 }


