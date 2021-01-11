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
//	   					(PCINT5/RESET/ADC0/dW) PB5  1|    |8  Vcc
//	Power supply relay -    (PCINT3/CLKI/ADC3) PB3  2|    |7  PB2 (SCK/ADC1/T0/PCINT2) 				- Current sensor pin
//	Start relay        -    	 (PCINT4/ADC2) PB4  3|    |6  PB1 PWM (MISO/AIN1/OC0B/INT0/PCINT1) 	- Tone port
//                                  		   GND  4|    |5  PB0 PWM (MOSI/AIN0/OC0A/PCINT0)
//                                             		 +----+

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
