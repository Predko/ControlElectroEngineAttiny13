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


#ifndef MAIN_H
#define MAIN_H

// 1.2 MHz (default) built in resonator
#define F_CPU 1200000UL

#include <avr/io.h>
#include <stdbool.h>

extern const uint8_t START_RELAY;	// Включение - низкий уровень (0), выключение - высокий (1)
extern const uint8_t SUPPLY_RELAY;	// Включение - низкий уровень (0), выключение - высокий (1)

uint8_t getMaxCurrentSensorValue();

bool StartEngine(uint16_t startDuration);

void OutputPinsInit();

void Loop();

void Setup();

void LoadInitialValuesFromEeprom();

// Включаем питание двигателя. Управляется высоким уровнем.
inline void PowerOn()
{
    PORTB |= SUPPLY_RELAY;
}

// Включаем питание двигателя.
inline void PowerOff()
{
    PORTB &= ~SUPPLY_RELAY;
}

// Подключаем пусковой конденсатор. Управляется высоким уровнем.
inline void CapacitorOn()
{
    PORTB |= START_RELAY;
}

// Подключаем пусковой конденсатор.
inline void CapacitorOff()
{
    PORTB &= ~START_RELAY;
}

#endif /* MAIN_H_ */