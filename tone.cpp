// ATMEL ATTINY13
//                            
/*
*                                               +-\/-+
*                              AinX (D X) PB5  1|    |8  Vcc
*      Power supply relay      AinX (D X) PB3  2|    |7  PB2 (D X) AinX - Current sensor pin
*      Start relay             AinX (D X) PB4  3|    |6  PB1 (D X) PWM	- Tone port
*                                         GND  4|    |5  PB0 (D X) PWM
*                                               +----+
*/

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
	PORTB &= ~SPEAKER_PORT;
	
	// режим CTC
	// максимальная частота таймера 150000Гц при F_CPU = 1200000

	TCCR0A |= (1 << WGM01); // set timer mode to CTC
	TCCR0A |= (1 << COM0B0); // connect PWM pin to Channel B of Timer0

	sei();
}


void Tone(uint8_t Tonefrequency)
{
	// Частота звука = 1200000 / (2 * (8) * (1 + коэффициент) = 150000 / (1 + коэффициент)
	// коэффициент = 150000 / (частота) - 1, для частот в районе 1000+ = 9375 / (частота)

	TCCR0B |= (0 << CS02) | (1 << CS01) | (0 << CS00);
	OCR0A = Tonefrequency; // set the OCRnx
	
}

