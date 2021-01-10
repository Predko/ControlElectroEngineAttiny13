#ifndef ModifiedMovingAvarage_H
#define ModifiedMovingAvarage_H

#include <avr/io.h>

/*
 * Modified Moving Average
 */

// Последннее значение.
extern uint16_t lastValue;

inline void MMA_SetFirstValue(uint16_t fv)
{
	lastValue = fv;
}

// Вычисляет новое значение
uint16_t MMA_CalcNew(uint16_t newValue);

inline uint16_t MMA_Get()
{
	return lastValue;
}

#endif

