/*
https://github.com/sebnil/Moving-Avarage-Filter--Arduino-Library-
*/
#include "MovingAvarageFilter.h"

MovingAvarageFilter::MovingAvarageFilter(unsigned int newDataPointsCount) {
  k = 0; //initialize so that we start to write at index 0
  if (newDataPointsCount < MAX_DATA_POINTS)
	dataPointsCount = newDataPointsCount;
  else
	dataPointsCount = MAX_DATA_POINTS;
  
  for (i=0; i<dataPointsCount; i++) {
    values[i] = 0; // fill the array with 0's
  }
}

float MovingAvarageFilter::process(float in) {
  out = 0;

  values[k] = in;
  k = (k+1) % dataPointsCount;

  for (i=0; i<dataPointsCount; i++) {
    out += values[i];
  }

  return out/dataPointsCount;
}

