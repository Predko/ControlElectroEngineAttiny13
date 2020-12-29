/*
 * AnalogIO_1_2Mhz.cpp
 * 
 * 1.2 MHz, built in resonator
 * PB2
 *
 * Created: 25.12.2020 13:17:20
 *  Author: Predko Victor
 */ 

#include <avr/io.h>

#include "AnalogIO_1_2Mhz.h"

void adc_setup (void)
{
	// Set the ADC input to PB2/ADC1
	ADMUX |= PortB2_10Bit;

	// Set the prescaler to clock_CPU/8 & enable ADC
	ADCSRA |= EnableConversion150Khz;
}

int adc_read (void)
{
	// Start the conversion
	ADCSRA |= (1 << ADSC);

	// Wait for it to finish
	while (ADCSRA & (1 << ADSC));

	uint8_t low = ADCL;
	uint8_t high = ADCH;

	return (high << 8) | low;
}
