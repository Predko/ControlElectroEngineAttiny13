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
const uint8_t  measurementTime = 32;

// 66 mV / 1 A   - 30 A sensor
// 100 mV / 1 A  - 20 A sensor
// 185 mV / 1 A  - 5 A sensor

// 30A sensor
#define K_Current 66L

#define operatingCurrent 14  // (Ampere)

#define KStartCurrent 1.5	// Коэффициент превышения тока для начала процедуры запуска.

// (K_Current * 1024 * 1.414 * KStartCurrent) / 5000.0)
#define K0 (int)((K_Current * 1024 * 1.414 * KStartCurrent) / 5000.0)

const uint16_t *startSensorValueEepromPtr = (uint16_t *)0;
// Значение сенсора тока, выше которого начинается процедура запуска двигателя.
uint16_t startSensorValue = (uint8_t)(512.0 + operatingCurrent * K0);

inline uint8_t CalcCurrent(uint16_t sensorValue)
{
	return (sensorValue - 512) >> 1; // Значение тока умноженное на 9.56(примерно 10)
}

const uint16_t *maxStartTimeEepromPtr = (uint16_t *)2;
// Максимальное время старта (миллисекунд))
uint16_t maxStartTime = 3000;

const uint8_t *MaximumStartAttemptsEepromPtr = (uint8_t *)4;
// Максимальное количество неудачных попыток запуска.
uint8_t MaximumStartAttempts = 3;

#define SIZE_ARRAY_CURRENTS 40

uint8_t CurrentValues[SIZE_ARRAY_CURRENTS];


int main(void)
{
	//LoadInitialValuesFromEeprom();

	Setup();

	DDRB &= ~(1 << PB4);	// Output
	PORTB |= (1 << PB4);	// PullUp

	DDRB |= (1 << PB3);
	PORTB |= (1 << PB3);

	// при нажатой кнопке блокируется работа программы
	while ((PINB & (1 << PB4)) == 0);

	uint8_t currentValue;
	uint8_t oldCurrentValue = CalcCurrent(getMaxCurrentSensorValue());

	uint8_t i = SIZE_ARRAY_CURRENTS;
	uint8_t *eepAdr = (uint8_t *)5;

	eeprom_write_word((uint16_t *)55, 0x0000);

	eeprom_write_byte(eepAdr++, oldCurrentValue);
	eeprom_write_byte(eepAdr++, 0x00);

	eeprom_write_word((uint16_t *)eepAdr, Wdt_GetCurrentMsCount()); eepAdr += 2;

	while (i)
	{
		// Exit when the button is pressed
		if ((PINB & (1 << PB4)) == 0)
		{
			break;
		}

		currentValue = CalcCurrent(getMaxCurrentSensorValue());

		// // Ожидаем подачи питания на двигатель
		// if (i == 20 && currentValue < 530)
		// {
		// 	continue;
		// }

		uint8_t delta = currentValue - oldCurrentValue;

		if (delta > 1)	// 0.5 a
		{
			CurrentValues[SIZE_ARRAY_CURRENTS - i] = currentValue;
			
			oldCurrentValue = currentValue;
			i--;
		}

		// _delay_ms(500);
	}

	// Saving begins
	PORTB &= ~(1 << PB3);

	for (uint8_t k = 0; k != SIZE_ARRAY_CURRENTS; k++)
	{
		eeprom_write_byte(eepAdr++, CurrentValues[k]);	
	}

	eeprom_write_byte(eepAdr++, 0x00);

	eeprom_write_word((uint16_t *)eepAdr, Wdt_GetCurrentMsCount()); eepAdr += 2;

	_delay_ms(2000);

	eeprom_write_byte(eepAdr++, CalcCurrent(getMaxCurrentSensorValue()));
	_delay_ms(500);
	eeprom_write_byte(eepAdr++, CalcCurrent(getMaxCurrentSensorValue()));
	_delay_ms(500);
	eeprom_write_byte(eepAdr++, CalcCurrent(getMaxCurrentSensorValue()));


	eeprom_write_word((uint16_t *)55, 0x1234);
	// Save finished
	PORTB |= (1 << PB3);

	while(1);

	
	//Loop();
}

void LoadInitialValuesFromEeprom()
{
	startSensorValue = eeprom_read_word(startSensorValueEepromPtr);

	maxStartTime = eeprom_read_word(maxStartTimeEepromPtr);

	MaximumStartAttempts = eeprom_read_byte(MaximumStartAttemptsEepromPtr);
}

void Setup()
{
	pins_init();

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
		uint16_t sensorValue = getMaxCurrentSensorValue();

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
		uint16_t sensorValue = getMaxCurrentSensorValue();

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

uint16_t getMaxCurrentSensorValue()
{
int16_t sensorValue;             // value read from the sensor
int16_t sensorMax = 0;

	uint16_t startTime = Wdt_GetCurrentMsCount();

	while(!Wdt_IsTimerEnded(startTime, measurementTime))
	{
		sensorValue = Adc_Read();
		
		if (sensorValue > sensorMax)
		{
			/*record the maximum sensor value*/
			sensorMax = sensorValue;
		}
	}

	return sensorMax;
}

void pins_init()
{
	// Output mode
	DDRB |= START_RELAY;
	DDRB |= SUPPLY_RELAY;
}
