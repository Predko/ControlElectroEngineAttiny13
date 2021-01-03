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

#include "wddelay.h"
#include <avr/wdt.h>
#include <avr/io.h>

#define NumberOfCyclesPerSecond  31

// 
volatile uint8_t  msCounter;

ISR(WDT_vect)
{
    msCounter++;	
}

// Инициализирует таймер watchdog указанными в флаге параметрами
void Wdt_InterruptInit(uint8_t flagsAndTimeout)
{
    wdt_reset();

    uint8_t sregs = SREG;

    cli();

    // Start timed sequence. Got four cycles to set the new values from here
    WDTCR |= (1 << WDCE) | (1 << WDE);
    
    // Set new prescaler(time-out) value
    WDTCR = flagsAndTimeout;	// 

    SREG = sregs;

    msCounter = 0;

    sei();
}
