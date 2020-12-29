/*
 * millis.h
 *
 * Created: 25.12.2020 14:14:06
 *  Author: Admin
 */ 
// ATMEL ATTINY13 - (Fuse E:FF, H:FF, L:6A)
//
//                  +-\/-+
// AinX (D X) PB5  1|    |8  Vcc
// AinX (D X) PB3  2|    |7  PB2 (D X) AinX - Current sensor pin
// AinX (D X) PB4  3|    |6  PB1 (D X) PWM	- Tone port
//            GND  4|    |5  PB0 (D X) PWM	- millis timer
//                  +----+


#ifndef MILLIS_H_
#define MILLIS_H_

#include <avr/interrupt.h>

extern volatile  uint16_t  delayInterval;

void delayUpTo1sec(uint16_t intervalValue);

inline int8_t IsTimerEnded()
{
	return (delayInterval == 0);
}


inline void SetTimerDelay(uint16_t intervalValue)
{
    cli();

    delayInterval = intervalValue;

    sei();
}

void Timer_Init();



#endif /* MILLIS_H_ */