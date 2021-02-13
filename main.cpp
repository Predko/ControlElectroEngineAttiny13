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

#include "main.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "AnalogIO_1_2Mhz.h"
#include "wddelay.h"
#include "soundsignals.h"
#include "tone.h"

const uint8_t START_RELAY = _BV(PB4);	// Включение - низкий уровень (0), выключение - высокий (1)
const uint8_t  SUPPLY_RELAY = _BV(PB3);	// Включение - низкий уровень (0), выключение - высокий (1)

// Время измерения в мс
const uint8_t  measurementTime = 70;

// 66 mV / 1 A   - 30 A sensor
// 100 mV / 1 A  - 20 A sensor
// 185 mV / 1 A  - 5 A sensor

// 30A sensor
#define K_Current 66L

#define operatingCurrent 14  // (Ampere)

#define KStartCurrent 1.5	// Коэффициент превышения тока для начала процедуры запуска.

// (K_Current * 1024 * 1.414 * KStartCurrent) / 5000.0)
#define K0 (int)((K_Current * 1024 * 1.414 * KStartCurrent) / 5000.0)

const uint8_t *startSensorValueEepromPtr = (uint8_t *)0;
// Значение сенсора тока, выше которого начинается процедура запуска двигателя.
uint8_t startSensorValue = (uint8_t)(512.0 + operatingCurrent * K0);

const uint16_t *maxStartTimeEepromPtr = (uint16_t *)1;
// Максимальное время старта (миллисекунд))
uint16_t maxStartTime = 3000;

const uint8_t *MaximumStartAttemptsEepromPtr = (uint8_t *)3;
// Максимальное количество неудачных попыток запуска.
uint8_t MaximumStartAttempts = 3;

int main(void)
{
	//LoadInitialValuesFromEeprom();

	Setup();

	Loop();
}

void LoadInitialValuesFromEeprom()
{
	startSensorValue = eeprom_read_byte(startSensorValueEepromPtr);

	maxStartTime = eeprom_read_word(maxStartTimeEepromPtr);

	MaximumStartAttempts = eeprom_read_byte(MaximumStartAttemptsEepromPtr);
}

void Setup()
{
	OutputPinsInit();

	Adc_Setup();
	
	PORTB |= START_RELAY;	// Выключаем
	PORTB &= ~SUPPLY_RELAY; // Питание включаем
	
	Wdt_Timer_Enable();
	
	// Ready signal for 500 ms
	ReadySound(500);
}

void Loop()
{
uint8_t startAttemptsCounter = 0;

	PORTB &= ~(1 << PB0);
	
	do
	{
		uint8_t sensorValue = getMaxCurrentSensorValue();

		if ( sensorValue > startSensorValue )
		{
			AlarmSound(1);

			if (StartEngine (maxStartTime) == false)
			{
				startAttemptsCounter++;

				// Неудалось запустить двигатель, подаём сигнал и выключаем питание на 10 секунд. 
				PORTB |= SUPPLY_RELAY;

				{
					AlarmSound (2);

					_delay_ms (1000);

					AlarmSound (2);

					_delay_ms (1000);

					AlarmSound (2);

					_delay_ms (8000);
				}

				if (startAttemptsCounter >= MaximumStartAttempts)
				{
					// Блокируем дальнейшую работу, до рестарта системы пуска.
					while(1);
				}
				
				// Включаем питание.
				PORTB &= ~SUPPLY_RELAY;
			}
			else
			{
				startAttemptsCounter = 0;
			}
			
		}
	}
	while(1);
}

bool StartEngine(uint16_t startDuration)
{
uint16_t startTime = Wdt_GetCurrentMsCount();

	// Подключаем пусковой конденсатор.
	PORTB &= ~START_RELAY;

	do
	{
		uint8_t sensorValue = getMaxCurrentSensorValue();

		if ( sensorValue < startSensorValue)
		{
			// Отключаем пусковой конденсатор.
			PORTB |= START_RELAY;
			
			// Двигатель запущен.
			return true;
		}
	}
	while(!Wdt_IsTimerEnded(startTime, startDuration));
	
	// Отключаем пусковой конденсатор.
	PORTB |= START_RELAY;

	// Неудачный старт.
	return false;
}

uint8_t getMaxCurrentSensorValue()
{
int8_t sensorValue;             // value read from the sensor
int8_t sensorMax = 0;

	uint16_t startTime = Wdt_GetCurrentMsCount();

	while(!Wdt_IsTimerEnded(startTime, measurementTime))
	{
		sensorValue = Adc_Read();
		
		if (sensorValue > sensorMax)
		{
			sensorMax = sensorValue;
		}
	}

	return sensorMax;
}

void OutputPinsInit()
{
	// Output mode
	DDRB |= START_RELAY;
	DDRB |= SUPPLY_RELAY;
}
