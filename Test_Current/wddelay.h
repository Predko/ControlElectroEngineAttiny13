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


#ifndef WDDELAY_H_
#define WDDELAY_H_

#include <avr/interrupt.h>


extern volatile  uint16_t  msCounter;

// Возвращает значение счётчика 32-х миллисекундных интервалов.
inline uint16_t Wdt_GetCurrentMsCount()
{
    return msCounter;
}

// Проверяет, не прошло ли указанное время.
// Время указано в 32 миллисекундных интервалах.
inline uint16_t Wdt_IsTimerEnded(uint16_t startMsCount, uint16_t timeInterval)
{
    return ((msCounter - startMsCount) >= timeInterval);
}

// Инициализирует таймер watchdog указанными в флаге параметрами
void Wdt_InterruptInit(uint8_t flagsAndTimeout);

// Инициализация прерывания. 32 мс.
inline void Wdt_Timer_Enable()
{
	Wdt_InterruptInit((1 << WDTIE) | (0 << WDP3) | (0 << WDP2) | (0 << WDP1) | (1 << WDP0));
}

// Полное отключение watchdog.
inline void Wdt_Timer_Disable()
{
	Wdt_InterruptInit((0 << WDTIE) | (0 << WDE) | (0 << WDP3) | (0 << WDP2) | (0 << WDP1) | (1 << WDP0));
}

#endif /* WDDELAY_H_ */