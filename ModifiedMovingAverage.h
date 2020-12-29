#ifndef ModifiedMovingAvarage_H
#define ModifiedMovingAvarage_H

#include <avr/io.h>

/*
 * Modified Moving Average
 */

// Последннее значение.
uint8_t lastValue;

// Интервал измерений
#define SMOOTHING_INTERVAL 8

inline void MMA_SetFirstValue(uint8_t fv)
{
	lastValue = fv;
}

// Вычисляет новое значение
uint8_t MMA_CalcNew(uint8_t newValue);

inline uint8_t MMA_Get()
{
	return lastValue;
}

#endif

