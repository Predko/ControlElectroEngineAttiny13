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



void Tone(uint8_t Tonefrequency)
{
	cli();
	
	DDRB |= (1 << PORTB1);	// Configuring PortB1 to Output
	
	// режим CTC
	// максимальная частота таймера 9375Гц при F_CPU = 1200000
	TCCR0A = (1<<COM0A0) | (1<<WGM01); 	// дергаем ногой по совпадению
	TCCR0B = (1<<CS01) | (1 << CS00); 	// делитель на 64

	sei();

	// Частота звука = 1200000 / (2 * (делитель) * (1 + коэффициент) = 9375 / (1 + коэффициент)
	// коэффициент = 9375 / (частота) - 1, для частот в районе 1000+ = 9375 / (частота)

	OCR0A = Tonefrequency; // загружаем коэффициент для заданной частоты
}

