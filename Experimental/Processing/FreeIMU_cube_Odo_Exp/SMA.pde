
import java.util.LinkedList;
import java.util.Queue;

//==============================================================
//SMA filter
// Use the next value and calculate the 
// moving average 
public void AddNewValue(float value){
  total -= data[p];
  data[p] = value;
  total += value;
  p = ++p % data.length;
  if(n < data.length) n++;
  average = total / n;
} 

//==============================================================
// Moving Average class from Rosetta Code
// 
// 

public class MovingAverage {
    private final Queue<Float> window = new LinkedList<Float>();
    private final int period;
    private float sum;
 
    public MovingAverage(int period) {
        assert period > 0 : "Period must be a positive integer";
        this.period = period;
    }
 
    public void newNum(float num) {
        sum += num;
        window.add(num);
        if (window.size() > period) {
            sum -= window.remove();
        }
    }
 
    public float getAvg() {
        if (window.isEmpty()) return 0; // technically the average is undefined
        return sum / window.size();
    }
}

public class EMA {
    private float alpha;
    private float oldValue;
    public EMA(float alpha) {
        this.alpha = alpha;
    }

    public float getAvg(float value){
        //if(oldValue == null) {
        //    oldValue = value;
        //    return value;
        //}
        float newValue = oldValue + alpha * (value - oldValue);
        oldValue = newValue;
        return newValue;
    }
}


