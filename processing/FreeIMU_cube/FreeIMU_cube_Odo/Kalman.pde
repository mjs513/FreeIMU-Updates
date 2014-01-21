public class MyKalman {
//private float Q = 0.001;
//private float R = 0.1;
private float Q = 0.0000005;
private float R = 0.01;
private float P = 1, X = 0, K;
 
private void measurementUpdate(){
K = (P + Q) / (P + Q + R);
P = R * (P + Q) / (R + P + Q);
}
 
public float update(float measurement){
measurementUpdate();
float result = X + (measurement - X) * K;
X = result;
 
return result;
}
} 
