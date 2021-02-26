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
//	Reserved	       -    	 (PCINT4/ADC2) PB4  3|    |6  PB1 PWM (MISO/AIN1/OC0B/INT0/PCINT1) 	- Tone port
//                                  		   GND  4|    |5  PB0 PWM (MOSI/AIN0/OC0A/PCINT0)		- Start relay
//                                             		 +----+

#ifndef SOUNDSIGNAL_H
#define SOUNDSIGNAL_H

#include <avr/io.h>

#define  DelaySound 500         // ms 
#define  NumberOfIntervals 8
#define  LowSound  128            // (150000 / 1000)
#define  HighSound 16            // (150000 / 9000)

#define  IntervalDuration 10   //  ms
#define  IntervalLength   1     // (uint16_t)((HighSound - LowSound) / NumberOfIntervals)

void AlarmSound(uint8_t numberOfSignals);

void ReadySound();

#endif