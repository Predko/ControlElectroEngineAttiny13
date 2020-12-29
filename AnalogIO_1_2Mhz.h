/*
 * AnalogIO_1_2Mhz.h
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
//                                        GND  4|    |5  PB0 (D X) PWM	- millis timer
//                                              +----+


#ifndef ANALOGIO_1_2MHZ_H
#define ANALOGIO_1_2MHZ_H

// Analog input pin that current sensor is attached to PB2
#define PortB2_10Bit (1 << MUX0) | (1 << ADLAR)
	
#define EnableConversion150Khz (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN)

void adc_setup (void);

int adc_read (void);

#endif /* ANALOGIO_1_2MHZ_H_ */