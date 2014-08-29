public float MotionDetect() {
  
    /*###################################################################
    
       accelerationsquared euclidean norm analysis
    
    ################################################################### */
    accnorm = (acc[0]*acc[0]+acc[1]*acc[1]+acc[2]*acc[2]);
    //if((accnorm >=0.96) && (accnorm <= 0.99)){
    // 0.995
    if((accnorm >=0.94) && (accnorm <= 1.03)){  
        accnorm_test = 0;
    } else {
        accnorm_test = 1; }
    //take average of 5 to 10 points
    accnorm_avg.newNum(accnorm);
    accnorm_test_avg.newNum(accnorm_test);

    /* ####################################################################
    #
    #   squared norm analysis to determine suddenly changes signal
    #
    ##################################################################### */
    accnorm_var.newNum(sq(accnorm-accnorm_avg.getAvg()));
    if(accnorm_var.getAvg() < 0.0005) {
        accnorm_var_test = 0;
    }else {
        accnorm_var_test = 1; }

    /*####################################################################
    #
    #   angular rate analysis in order to disregard linear acceleration
    #
    ################################################################### */
    //0, 0.00215, 0.00215
    if ((gyro[0] >=-0.005) && (gyro[0] <= 0.005)) {
        omegax = 0;
    } else {
        omegax = 1; }
        
    if((gyro[1] >= -0.005) && (gyro[1] <= 0.005)) {
        omegay = 0;
    } else {
        omegay = 1; }
        
    if((gyro[2] >= -0.005) && (gyro[2] <= 0.005)) {
        omegaz = 0;
    } else {
        omegaz = 1; }
        
    if ((omegax+omegay+omegaz) > 0) {
        omega_test = 1;
    } else {
        omega_test = 0; }


    /* ####################################################################
    #
    # combined movement detector
    #
    #################################################################### */
    if ((accnorm_test_avg.getAvg() + omega_test + accnorm_var_test) > 0) {
        motionDetect = 1;
    } else {
        motionDetect = 0; }
    
    //####################################################################    
    
    motion_detect_ma.newNum(motionDetect);
    
    if(motion_detect_ma.getAvg() > 0.5) {
      return 1;
    } else {
      return 0; }
      
     //return omegaz;
}

