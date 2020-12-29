/*
* The millis function is designed for Attiny13.
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
*                                         GND  4|    |5  PB0 (D X) PWM	- millis timer
*                                               +----+
*/

#include "main.h"

#include <avr/io.h>

#include "millis.h"
#include <avr/interrupt.h>

volatile  uint16_t  delayInterval;

int8_t fiveTickCounter = 4;

ISR(TIM0_OVF_vect)
{
	if (--fiveTickCounter < 0)
	{
		// every 256 * 5 clock cycles, for F_CPU = 1200000 Hz => 0.001 sec.

		fiveTickCounter = 4;

		// Отсчёт интервала в 65535 миллисекунд
		// Завершение -> delayInterval = 0; 
		if (delayInterval)
		{
			delayInterval--;
		}
	}
}


void Timer_Init()
{
	//Setup timer interrupt
	TCCR0B |= _BV(CS00);
	TCCR0A |= _BV(WGM00)|_BV(WGM01);
	TIMSK0 |= 2;
	TCNT0=0; 

	delayInterval = 0;
	
	fiveTickCounter = 4;
	
	sei();
}

void delayUpTo1sec(uint16_t intervalValue)
{
	SetTimerDelay(intervalValue);

	while (!IsTimerEnded());
}

