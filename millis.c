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
*                                         GND  4|    |5  PB0 (D X) PWM
*                                               +----+
*/

#include "main.h"

#include <avr/io.h>

#include "millis.h"
#include <avr/wdt.h>

volatile  uint16_t  delayInterval;

ISR(WDT_vect)
{
	// Отсчёт интервала в 32 миллисекунд
	// Завершение -> delayInterval = 0; 
	if (delayInterval)
	{
		delayInterval--;
	}

	WDTCR |= (1<<WDTIE); // разрешаем прерывания по ватчдогу. Иначе будет резет.
}


void Timer_Init()
{
	cli();
	
	wdt_reset();

    /* Start timed sequence */
	WDTCR |= (1<<WDCE) | (1<<WDE);
	
	/* Set new prescaler(time-out) value = 4K cycles (~32 ms) */
	WDTCR = (1<<WDE) | (1<<WDP0) | (1 << WDTIE);
	
	delayInterval = 0;
	
	sei();
}

// Задержка на заданное количество миллисекунд(примерно)
// Реальная задержка примерно равна (int)(intervalValue / 32) * 32 (обычно меньше)
void delayUpTo1sec(uint16_t intervalValue)
{
	SetTimerDelay(intervalValue / 32);

	while (!IsTimerEnded());
}

