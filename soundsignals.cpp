//
// NO WARRANTEE OR GUARANTEES!
// Written by Victor Predko 
// You are free to use, redistribute and modify
// 
// 
// 1.2 MHz, built in resonator
// 
//
// Created: 25.12.2020 13:17:20
//  Author: Predko Victor
// 
// ATMEL ATTINY13
//                            
//                                              	 +-\/-+
//	   					      (PCINT5/RESET/ADC0/dW) PB5  1|    |8  Vcc
//	Power supply relay -  (PCINT3/CLKI/ADC3) PB3  2|    |7  PB2 (SCK/ADC1/T0/PCINT2) 				      - Current sensor pin
//	Reserved	         -    	 (PCINT4/ADC2) PB4  3|    |6  PB1 PWM (MISO/AIN1/OC0B/INT0/PCINT1) 	- Tone port
//                                  		     GND  4|    |5  PB0 PWM (MOSI/AIN0/OC0A/PCINT0)		    - Start relay
//                                             		 +----+

#include "soundsignals.h"
#include "tone.h"
#include "main.h"
#include <util/delay.h>

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