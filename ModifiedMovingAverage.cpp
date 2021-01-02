#include "ModifiedMovingAverage.h"
#include <avr/io.h>

uint8_t MMA_CalcNew(uint8_t newValue)
{
  // lastValue =  (newValue + lastValue * (8 - 1)) / 8;  8 - Measurement interval
  
  lastValue =  (newValue + ((lastValue << 3) - lastValue)) >> 3;

  return lastValue;
}
