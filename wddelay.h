/*
 * wddelay.h
 *
 * Created: 25.12.2020 14:14:06
 *  Author: Admin
 */ 
// ATMEL ATTINY13 - (Fuse E:FF, H:FF, L:6A)
/*
*                                               +-\/-+
*                              AinX (D X) PB5  1|    |8  Vcc
*      Power supply relay      AinX (D X) PB3  2|    |7  PB2 (D X) AinX - Current sensor pin
*      Start relay             AinX (D X) PB4  3|    |6  PB1 (D X) PWM	- Tone port
*                                         GND  4|    |5  PB0 (D X) PWM
*                                               +----+
*/


#ifndef WDDELAY_H_
#define WDDELAY_H_

#include <avr/interrupt.h>


// Max interval - 255 * 32ms = 8160 ms
extern volatile  uint8_t  msCounter;

// Возвращает значение счётчика
inline int8_t Wdt_GetCurrentMsCount()
{
    return msCounter;
}

// Проверяет, не завершился ли указанный интервал
inline uint8_t Wdt_IsTimerEnded(uint8_t startMsCount, uint16_t timeInterval)
{
    return ((msCounter - startMsCount) >= timeInterval / 32);
}

// Инициализирует таймер watchdog указанными в флаге параметрами
void Wdt_InterruptInit(uint8_t flagsAndTimeout);

// Инициализация на прерывание через 32 мс.
inline void Wdt_Timer_Enable_32ms()
{
	// 32 ms

	Wdt_InterruptInit((1 << WDTIE) | (0 << WDP3) | (0 << WDP2) | (0 << WDP1) | (1 << WDP0));
}

// Полное отключение watchdog.
inline void Wdt_Timer_Disable()
{
	Wdt_InterruptInit((0 << WDTIE) | (0 << WDE) | (0 << WDP3) | (0 << WDP2) | (0 << WDP1) | (1 << WDP0));
}

#endif /* WDDELAY_H_ */