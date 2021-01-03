#ifndef SOUNDSIGNAL_H
#define SOUNDSIGNAL_H

#include <avr/io.h>

#define  DelaySound 500         // ms 
#define  NumberOfIntervals 8
#define  LowSound  128            // (150000 / 1000)
#define  HighSound 16            // (150000 / 9000)

#define  IntervalDuration 10   //  ms
#define  IntervalLength   1     // (uint16_t)((HighSound - LowSound) / NumberOfIntervals)

void AlarmSound(uint8_t numberOfSignals);

void ReadySound();

#endif