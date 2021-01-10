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

#define ReadySoundDuration 3000

#include <avr/io.h>
#include <stdbool.h>

extern uint16_t *eep;

uint16_t getMaxCurrentSensorValue();

bool StartEngine(int startDuration);

void pins_init();

void Loop();

void Setup();

void LoadInitialValuesFromEeprom();

#endif /* MAIN_H_ */