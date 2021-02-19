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

// Программа проверки работы системы считывания датчика тока.
// Так же сохраняет в EEPROM значения тока, потребляемого двигателем в момент запуска.

#include "main.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "AnalogIO_1_2Mhz.h"
#include "wddelay.h"
#include "soundsignals.h"
#include "tone.h"


const uint8_t START_RELAY = _BV(PB4);	// Включение - низкий уровень (0), выключение - высокий (1)
const uint8_t SUPPLY_RELAY = _BV(PB3);	// Включение - низкий уровень (0), выключение - высокий (1)

// Время измерения в 32 мсекундных интервалах (примерно три полных цикла тока сети 50Гц)
const uint16_t  measurementTime = 64 / 32;

// 66 mV / 1 A   - 30 A sensor
// 100 mV / 1 A  - 20 A sensor
// 185 mV / 1 A  - 5 A sensor

// 30A sensor
#define K_Current 66L

#define operatingCurrent 14  // (Ampere)

#define KStartCurrent 1.5	// Коэффициент превышения тока для начала процедуры запуска.

// (K_Current * 256 * 1.414 * KStartCurrent) / 5000.0)
#define K0 (float)((K_Current * 256 * 1.414 * KStartCurrent) / 5000.0)	// для 66l = 7.167

const uint8_t *startSensorValueEepromPtr = (uint8_t *)0;
// Значение сенсора тока, выше которого начинается процедура запуска двигателя.
#define startSensorValue (uint8_t)(128.0 + (operatingCurrent * K0))	// 228

const uint16_t *maxStartTimeEepromPtr = (uint16_t *)1;

// Максимальное время старта (миллисекунд))
uint16_t maxStartTime = 3200 / 32;

const uint8_t *MaximumStartAttemptsEepromPtr = (uint8_t *)3;
// Максимальное количество неудачных попыток запуска.
uint8_t MaximumStartAttempts = 3;

int main(void)
{
	Setup();

	ReadySound(500);

	_delay_ms(5000);

	Loop();
}

void Setup()
{
	OutputPinsInit();

	Adc_Setup();
	
	Wdt_Timer_Enable();
}

const uint16_t testDuration = 224 / 32;

void Loop()
{
	uint8_t *CurrentEepromPtr = (uint8_t *)3;
	
	eeprom_write_byte(++CurrentEepromPtr, startSensorValue);
	eeprom_write_byte(++CurrentEepromPtr, getMaxCurrentSensorValue());

	AlarmSound(1);

	PowerOn();

uint16_t startTime = Wdt_GetCurrentMsCount();

	// CapacitorOn();

	// Записываем 50 значений тока за ~4.8 секунд.
	for (int i = 0; i < 50; ++i)
	{
		eeprom_write_byte(++CurrentEepromPtr, getMaxCurrentSensorValue());

		// Ждём окончания периода измерения = 96 мс.
		while (!Wdt_IsTimerEnded(startTime, testDuration));
	}

	// for (uint8_t i = 0; i < 50; ++i)
	// {

	// 	eeprom_write_byte(++CurrentEepromPtr, (uint8_t)0);

	// }

	// *CurrentEepromPtr = (uint8_t *)5;

	// uint8_t lastSensorValue = 0; 
	// uint8_t sensorValue; 
	// uint8_t i = 0;

	// while (i != 50)
	// {
	// 	sensorValue = getMaxCurrentSensorValue();

	// 	uint8_t delta;

	// 	if (sensorValue > lastSensorValue)
	// 	{
	// 		delta = sensorValue - lastSensorValue;
	// 	}
	// 	else
	// 	{
	// 		delta = lastSensorValue - sensorValue;
	// 	}
		
	// 	if (delta > 1)
	// 	{
	// 		eeprom_write_byte(++CurrentEepromPtr, sensorValue);

	// 		lastSensorValue = sensorValue;

	// 		++i;
	// 	}
	// }
	
	// CapacitorOff();

	// Измерения завершены.
	AlarmSound (2);

	PowerOff();

	while(1);
}

uint8_t getMaxCurrentSensorValue()
{
int8_t sensorValue;             // value read from the sensor
int8_t sensorMax = 0;

	uint16_t startTime = Wdt_GetCurrentMsCount();

	do
	{
		sensorValue = Adc_Read();
		
		if (sensorValue > sensorMax)
		{
			sensorMax = sensorValue;
		}
	}
	while(!Wdt_IsTimerEnded(startTime, measurementTime));

	return sensorMax;
}

void OutputPinsInit()
{
	// Output mode
	DDRB |= START_RELAY;
	DDRB |= SUPPLY_RELAY;
}
