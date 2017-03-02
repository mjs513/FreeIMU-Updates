/*==============================================================================
 
 Copyright (c) 2010-2013 Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 https://github.com/SAIC-ATS/Algorithms.git
  
 ==============================================================================*/


int historyLength = 20;

float[] history = new float[historyLength]; // keep our history values

float[] historyX = new float[historyLength];
float[] historyY = new float[historyLength];

float averageAngle = 0;
float yamartinoAverageAngle = 0;

float currentAngle = 0;

void calculateMathematicalAverageOfHistory() {
    float sum = 0;
    float sq_sum = 0;
    for(int i = 0; i < history.length; ++i) {
       sum += history[i];
       sq_sum += history[i] * history[i];
    }
    
    averageAngle = sum / history.length;

}

void calculateYamartinoAverageOfHistory() {

  float sumX = 0;
  float sumY = 0;
  
  for (int i = 0; i < history.length; i++) {
    sumX += historyX[i];
    sumY += historyY[i];
  }

  float meanX = sumX / history.length;
  float meanY = sumY / history.length;

  yamartinoAverageAngle = myAtan2(sumY, sumX);
}

void addItemsToHistoryBuffers(float input) {
  addToHistory(history,input);
  addToHistory(historyX,cos(input));
  addToHistory(historyY,sin(input));
}

void addToHistory(float[] buffer, float input) {
  // delete the oldest value from the history
  // add one value to the history (the input)
  // take the average of the history and return it;

  // shift the values to the left in the array
  for (int i = buffer.length - 1; i >= 0; i--) {
    if (i == 0) {
      buffer[0] = input;
    } 
    else {
      buffer[i] = buffer[i-1];
    }
  }
}


float myAtan2(float y, float x) {
  float t = atan2(y, x);
  return t > 0 ? t : 2 * PI + t;
}

