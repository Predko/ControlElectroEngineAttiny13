/*
 * AnalogIO_1_2Mhz.cpp
 * 
 * 1.2 MHz, built in resonator
 * PB2
 *
 * Created: 25.12.2020 13:17:20
 *  Author: Predko Victor
 */ 
// ATMEL ATTINY13
//                            
//                                              +-\/-+
//                             AinX (D X) PB5  1|    |8  Vcc
//     Power supply relay      AinX (D X) PB3  2|    |7  PB2 (D X) AinX - Current sensor pin
//     Start relay             AinX (D X) PB4  3|    |6  PB1 (D X) PWM	- Tone port
//                                        GND  4|    |5  PB0 (D X) PWM
//                                              +----+

#include <avr/io.h>

#include "AnalogIO_1_2Mhz.h"


void Adc_Setup (void)
{
	// Set the ADC input to PB2/ADC1, left adjust result
	ADMUX = (1 << MUX0) & ~(1 << REFS0);

	// Set the prescaler to clock_CPU/8 = 150 kHz & enable ADC
	ADCSRA = (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
}

uint16_t Adc_Read (void)
{
	// Start the conversion
	ADCSRA |= (1 << ADSC);

	// Wait for it to finish
	while (ADCSRA & (1 << ADSC));

	uint8_t l = ADCL;
	uint8_t h = ADCH;

	return (h << 8) | l;
}


void Pwm_Setup (uint8_t pinBit)
{
	//DDRB   |= (1 << PB0);    //Ножка PB0 как выход(ШИМ)

	// TCCR0A = (1 << COM0A1) | (1 << WGM00);  //выбор ШИМ  с фазовой коррекцией
	// TCCR0B = 0x05; //Предделитель - 1024 и старт таймера
	// TCNT0 = 0;     // Сброс счетчика прерываний(0-255)
	// OCR0A=0;       //Выбор Значения ШИМ
	
	// TIMSK0=0x04;//Разрешить выполнение прерываний по совпадению в OCR0A	 		
	// sei();// Разрешить глобальные прерывания
    
    // Set Timer 0 prescaler to clock/8.
    // At 9.6 MHz this is 1.2 MHz.
    // See ATtiny13 datasheet, Table 11.9.
    
	DDRB |= pinBit;
 
    // Set to 'Fast PWM' mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
 
    // Clear OC0A output on compare match, upwards counting. (PB0)
    TCCR0A |= (1 << COM0A1);

	TCCR0B = (1 << CS01);

	// Начальное значение счётчика
	TCNT0 = 0;

	OCR0A = 0;

}
 
void Pwm_Write (uint8_t val)
{
    OCR0A = val;
}
