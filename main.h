/*
 * main.cpp
 * 
 * Attiny13
 * 1.2 MHz, built in resonator
 * 
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


#ifndef MAIN_H
#define MAIN_H

// 1.2 MHz (default) built in resonator
#define F_CPU 1200000UL

#define ReadySoundDuration 500

#include <avr/io.h>
#include <stdbool.h>


int getMaxCurrentSensorValue();

bool StartEngine(int startDuration);

int getMaxCurrentSensorValue();

void pins_init();

void adc1_InputSetup (void);

int CurrentSensorRead (void);

void Loop();

void Setup();



#endif /* MAIN_H_ */