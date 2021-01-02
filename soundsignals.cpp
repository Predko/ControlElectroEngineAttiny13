#include "soundsignals.h"
#include "tone.h"
#include "main.h"
#include <util/delay.h>

/*
* The sound function.
* NO WARRANTEE OR GUARANTEES!
* Written by Victor Predko 
* You are free to use, redistribute and modify
* 
*  ATMEL ATTINY13
*                             
*                                               +-\/-+
*                              AinX (D X) PB5  1|    |8  Vcc
*      Power supply relay      AinX (D X) PB3  2|    |7  PB2 (D X) AinX - Current sensor pin
*      Start relay             AinX (D X) PB4  3|    |6  PB1 (D X) PWM	- Tone port
*                                         GND  4|    |5  PB0 (D X) PWM
*                                               +----+
*/


/* 
#define  NumberOfIntervals 8
#define  LowSound  9            // (9375 / 1000)
#define  HighSound 1            // (9375 / 9000)

#define  IntervalDuration 100   // ms
#define  IntervalLength   1     // (uint16_t)((HighSound - LowSound) / NumberOfIntervals)
*/

void AlarmSound(uint8_t numberOfSignals)
{
  for (int i = 0; i < numberOfSignals; i++)
  {
    for (int j = 0; j < NumberOfIntervals; j++)
    {
      Tone(LowSound - j * IntervalLength);
      _delay_ms(IntervalDuration);
    }

    for (int j = 0; j < NumberOfIntervals; j++)
    {
      Tone(HighSound + j * IntervalLength);
      _delay_ms(IntervalDuration);
    }
  }

  NoTone(); // Выключаем звук
}


void ReadySound()
{
  Tone(HighSound);

  _delay_ms(ReadySoundDuration);

  NoTone(); // Выключаем звук
}