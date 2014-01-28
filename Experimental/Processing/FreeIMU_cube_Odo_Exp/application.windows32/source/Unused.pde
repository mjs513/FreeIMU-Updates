/*
//****************************************************************************
//The purpose of the calibration routine is to obtain the value of the 
//reference threshold. It consists on a 1024 samples average in no-movement 
//condition.
//*****************************************************************************

void Calibrate() 
{
  int count1;
  count1 = 0;
  do {  
  //ADC_GetAllAxis();
  gravity_compensate();
  sstatex = sstatex + Sample_X;             // Accumulate Samples
  sstatey = sstatey + Sample_Y;
  println(Sample_X);
  count1++;
  } while(count1 != 0x0400);                   // 1024 times

  sstatex=sstatex / 1024;                      // division between l024
  sstatey=sstatey / 1024;
  
}

*/

/* 
//**************************************************************************/
//**************************************************************************/ 
//This function obtains magnitude and direction 
//In   this   particular   protocol   direction   and   magnitude   are   sent   in   separate   variables. 
//Management can be done in many other different ways.
//**************************************************************************/  

/* void data_transfer() 
{
 
 float positionXbkp; 
 float positionYbkp;
 int delay;
 float [] posx_seg = new float [4];
 float [] posy_seg = new float [4]; 
 
 
  if (positionX[1] >= 0) {                          //This line compares the sign of the X direction data 
    direction = (direction | 0x10);                 //if its positive the most significant byte 
    posx_seg[0]= positionX[1] & 0x000000FF;         //is set to 1 else it is set to 8
    posx_seg[1]= (positionX[1]>>8) & 0x000000FF;    //the data is also managed in the 
                                                    // subsequent lines in order to 
    posx_seg[2]= (positionX[1]>>16) & 0x000000FF;   // be sent. The 32 bit variable must be 
    posx_seg[3]= (positionX[1]>>24) & 0x000000FF;   // split into 4 different 8 bit 
                                                    // variables in order to be sent via   
                                                    // the 8 bit SCI frame

      }

    else {direction = (direction | 0x80);
             positionXbkp = positionX[1]-1;
             positionXbkp = positionXbkp & 0xFFFFFFFF;
             posx_seg[0] = positionXbkp & 0x000000FF;
             posx_seg[1] = (positionXbkp >> 8) & 0x000000FF;
             posx_seg[2] = (positionXbkp >> 16) & 0x000000FF;
             posx_seg[3] = (positionXbkp >> 24) & 0x000000FF;
             } 
  
  if (positionY[1] >=0 ) {                            //  Same management than in the previous case 
             direction= (direction | 0x08);        // but with the Y data.
             posy_seg[0]= positionY[1] & 0x000000FF;
             posy_seg[1]= (positionY[1]>>8) & 0x000000FF;
             posy_seg[2]= (positionY[1]>>16) & 0x000000FF;
             posy_seg[3]= (positionY[1]>>24) & 0x000000FF;
             }
  
  else {direction= (direction | 0x01);
             positionYbkp = positionY[1]-1;
             positionYbkp = positionYbkp & 0xFFFFFFFF;
             posy_seg[0] = positionYbkp & 0x000000FF;
             posy_seg[1] = (positionYbkp >> 8) & 0x000000FF;
             posy_seg[2] = (positionYbkp >> 16) & 0x000000FF;
             posy_seg[3] = (positionYbkp >> 24) & 0x000000FF;
             } 

  delay =0x0100;
  
  Sensor_Data[0] = 0x03;
  Sensor_Data[1] = direction;
  Sensor_Data[2] = posx_seg[3];
  Sensor_Data[3] = posy_seg[3];
  Sensor_Data[4] = 0x01;
  Sensor_Data[7] = 0x01;
  Sensor_Data[6] = 9999;
  
  //while (--delay);                                   
  
  //SCITxMsg(Sensor_Data);         //  Data transferring function   
  //while (SCIC2 & 0x08);

} */


//************************************************************************
//************************************************************************
//This function returns data format to its original state. When obtaining 
//the magnitude and  direction of the position, an inverse two's complement 
//is made. This function makes the two's complement in order to return the 
//data to it original state.
//It is important to notice that the sensibility adjustment is greatly 
//impacted here, the amount of "ones" inserted in the mask must be equivalent
//to the "ones" lost in the shifting made in the previous function upon the 
//sensibility modification. 
//************************************************************************

/* void data_reintegration() 
{
  if (direction >= 10) 
   {
    positionX[1]= positionX[1] | 0xFFFFC000; }   // l8 "ones" inserted. Same size as the 
                  //amount of shifts 
    
    direction = direction & 0x01;
    if (direction == 1)   
      {  positionY[1]= positionY[1] | 0xFFFFC000;  }
 } */

