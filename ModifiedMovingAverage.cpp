#include "ModifiedMovingAverage.h"
#include <avr/io.h>

// Последннее значение.
uint16_t lastValue;

uint16_t MMA_CalcNew(uint16_t newValue)
{
  // SMOOTHING_INTERVAL = 8
  // lastValue =  newValue + (lastValue * (SMOOTHING_INTERVAL - 1)) / SMOOTHING_INTERVAL;
  
  lastValue =  (newValue + ((lastValue << 3) - lastValue)) >> 3;

  return lastValue;
}
