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

#include "main.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "tone.h"

#define SPEAKER PB1
#define SPEAKER_PORT (1 << SPEAKER)

void ToneInit()
{
	cli();
	
	DDRB |= SPEAKER_PORT;	// Configuring PortB1 to Output
	PORTB |= SPEAKER_PORT;	// Устанавливаем высокий уровень - (второй вывод подключён к источнику питания)
	
	// режим CTC
	// максимальная частота таймера 150000Гц при F_CPU = 1200000

	TCCR0A = (1 << WGM01); // set timer mode to CTC
	TCCR0A |= (1 << COM0B0); // connect PWM pin to Channel B of Timer0

	sei();
}


void Tone(uint8_t Tonefrequency)
{
	// Частота звука = 1200000 / (2 * (8) * (1 + коэффициент) = 150000 / (1 + коэффициент)

	TCCR0B |= (0 << CS02) | (1 << CS01) | (0 << CS00);
	OCR0A = Tonefrequency; // set the OCRnx
	
}

