//============================================================
void EstimatedAltitude() {
  
  gravityCompensateDynAcc();
  dyn_acc_q.x = dyn_acc[0];
  dyn_acc_q.y = dyn_acc[1];  
  dyn_acc_q.z = dyn_acc[2];
  dyn_acc_q.w = 0;
  q1.x = q[1]; q1.y = q[2]; q1.z = q[3]; q1.w = q[0];
  multQ = Quaternion.multiply(q1, dyn_acc_q);
  //conQ = Quaternion.conjugate(q1);
  conQ.x = -q1.x; conQ.y = -q1.y; conQ.z = -q1.z; conQ.w = q1.w;
  dyn_acc_q_earth = Quaternion.multiply(multQ, conQ);
  fused_alt = altitudeFilter.update(dyn_acc_q_earth.z, altitude, dt);
  
}

//=============================================================
void gravityCompensateDynAcc() {
  float[] g = new float[3];
  
  // get expected direction of gravity in the sensor frame
  g[0] = 2 * (q[1] * q[3] - q[0] * q[2]);
  g[1] = 2 * (q[0] * q[1] + q[2] * q[3]);
  g[2] = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
  
  // compensate accelerometer readings with the expected direction of gravity
  dyn_acc[0] = acc[0] - g[0];
  dyn_acc[1] = acc[1] - g[1];
  dyn_acc[2] = acc[2] - g[2];
}
 
////////////////////////////////////////////////////////////
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
    
    float dt1 = dt;

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
