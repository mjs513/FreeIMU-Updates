// complementary filter from Palla Software
// 
final float Kp1 = 0.55f;                // PI observer velocity gain 
final float Kp2 = 1.0f;                 // PI observer position gain
final float Ki  = 0.001f;               // PI observer integral gain (bias cancellation)

public class AltitudeComplementary {
  
    //self.inited = 0
    public float AltErrorI = 0;
    //self.AccScale  = 0.0
    //public int startTime = millis();
    public float AltError = 0;
    public float InstAcc = 0.0;
    public float Delta = 0;
    public float EstVelocity = 0.0;
    public float EstAlt = altitude;
    //public float lastTime = millis();
    
  // Computes the complementary estimation of Altitude based from the Barometer and Accelerometer
  // @param dyn_acc_z the Z componente of the dynamic acceleration expressed in the Earth frame
  // @BaroAlt the Baromether altitude reading as coming from the sensor expressed in meters with decimals
  public float update(float dyn_acc_z, float BaroAlt, float dt ){
    //int currentTime = millis();
    
    //println(EstAlt);
    //println(dyn_acc_z);
    // Estimation Error
    AltError = BaroAlt - EstAlt;
    AltErrorI += AltError;
    
    AltErrorI = constrain(AltErrorI,-150.00,+150.00);

    InstAcc = dyn_acc_z * 9.80665; //+ ((self.AltErrorI / 1000) if self.AltErrorI != 0.0 else 0)
    if (AltErrorI != 0.0){
      //self.InstAcc += self.AltErrorI / 10000.0
      InstAcc = AltErrorI * Ki;
    }
    if(Float.isNaN(InstAcc)){
      InstAcc = 0;
    }
    
    float dt1 = dt/1000.;

    // Integrators
    Delta = InstAcc * dt1 + (Kp1 * dt1) * AltError;
    
    if (EstVelocity != 0.0) {
      EstAlt += (EstVelocity/5.0 + Delta) * (dt1 / 2) + (Kp2 * dt1) * AltError;
    } else {
      EstAlt = Delta * (dt1 / 2) + (Kp2 * dt1) * AltError;
    }
    
    EstVelocity += Delta * 10.0;
    
    //print (self.InstAcc, self.AltErrorI, dt, self.Delta)
    
    //lastTime = currentTime;
    
    return EstAlt;
  }
}    
