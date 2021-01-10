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
#define  LowSound  128            // (150000 / 1000)
#define  HighSound 16            // (150000 / 9000)

#define  IntervalDuration 10   // ms
#define  IntervalLength   16    // (uint16_t)((HighSound - LowSound) / NumberOfIntervals)
*/

void AlarmSound(uint8_t numberOfSignals)
{
  ToneInit();

  for (int i = 0; i < numberOfSignals; i++)
  {
    for (int j = LowSound; j > HighSound; j--)
    {
      Tone(j);
      _delay_ms(IntervalDuration);
    }

    for (int j = HighSound; j < LowSound; j++)
    {
      Tone(j);
      _delay_ms(IntervalDuration);
    }
  }

  NoTone(); // Выключаем звук
}


void ReadySound()
{
  ToneInit();

  Tone(HighSound);

  _delay_ms(1000);  // ReadySoundDuration

  NoTone(); // Выключаем звук
}