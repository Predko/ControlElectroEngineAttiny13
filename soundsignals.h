#ifndef SOUNDSIGNAL_H
#define SOUNDSIGNAL_H

#include <avr/io.h>

#define  DelaySound 500         // ms 
#define  NumberOfIntervals 8
#define  LowSound  9            // (9375 / 1000)
#define  HighSound 1            // (9375 / 9000)

#define  IntervalDuration 100   //  ms
#define  IntervalLength   1     // (uint16_t)((HighSound - LowSound) / NumberOfIntervals)

void AlarmSound(uint8_t numberOfSignals);

void ReadySound(uint16_t duration);

#endif