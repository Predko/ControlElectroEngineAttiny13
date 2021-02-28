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
//	Вариант 1.
//                            
//                                              	 +-\/-+
//	   					(PCINT5/RESET/ADC0/dW) PB5  1|    |8  Vcc
//	Power supply relay -    (PCINT3/CLKI/ADC3) PB3  2|    |7  PB2 (SCK/ADC1/T0/PCINT2) 				- Current sensor pin
//	Start relay	       -    	 (PCINT4/ADC2) PB4  3|    |6  PB1 PWM (MISO/AIN1/OC0B/INT0/PCINT1) 	- Tone port
//                                  		   GND  4|    |5  PB0 PWM (MOSI/AIN0/OC0A/PCINT0)		- Reserved
//                                             		 +----+
//	Вариант 2.
//                            
//                                              	 +-\/-+
//	   					(PCINT5/RESET/ADC0/dW) PB5  1|    |8  Vcc
//	Power supply relay -    (PCINT3/CLKI/ADC3) PB3  2|    |7  PB2 (SCK/ADC1/T0/PCINT2) 				- Current sensor pin
//	Reserved	       -    	 (PCINT4/ADC2) PB4  3|    |6  PB1 PWM (MISO/AIN1/OC0B/INT0/PCINT1) 	- Tone port
//                                  		   GND  4|    |5  PB0 PWM (MOSI/AIN0/OC0A/PCINT0)		- Start relay
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



// Время измерения в 32 мсекундных интервалах (примерно три полных цикла тока сети 50Гц)
const uint16_t  measurementTime = 64 / 32;

// 66 mV / 1 A   - 30 A sensor
// 100 mV / 1 A  - 20 A sensor
// 185 mV / 1 A  - 5 A sensor

// 30A sensor
#define K_Current 66L

#define operatingCurrent 10  // (Ampere)

#define KStartCurrent 2.0	// Коэффициент превышения тока для начала процедуры запуска.

// (K_Current * 256 * 1.414 * KStartCurrent) / 5000.0)
#define K0 (float)((K_Current * 256 * 1.414 * KStartCurrent) / 5000.0)	// для 66l = 7.167

const uint8_t *startSensorValueEepromPtr = (uint8_t *)0;
// Значение сенсора тока, выше которого начинается процедура запуска двигателя.
#define startSensorValue (uint8_t)(128.0 + (operatingCurrent * K0))	// 228

const uint16_t *maxStartTimeEepromPtr = (uint16_t *)1;

// Максимальное время старта (миллисекунд))
uint16_t maxStartTime = 4800 / 32;

const uint8_t *MaximumStartAttemptsEepromPtr = (uint8_t *)3;

// Максимальное количество неудачных попыток запуска.
uint8_t MaximumStartAttempts = 3;

#define SIZE_ARRAY_CURRENTS 30

uint8_t CurrentValues[SIZE_ARRAY_CURRENTS];

void TestCurrent()
{
   	ReadySound();

	_delay_ms(8000);

	ReadySound();

	PowerOn();

	_delay_ms(200);
	uint8_t oldCurrentValue = getMaxCurrentSensorValue();

	uint8_t *eepAdr = (uint8_t *)0;

	eeprom_write_byte(eepAdr++, oldCurrentValue);

	_delay_ms(200);
	
	oldCurrentValue = getMaxCurrentSensorValue();

	eeprom_write_byte(eepAdr++, oldCurrentValue);

	uint16_t startTime = Wdt_GetCurrentMsCount();
	
	eeprom_write_word((uint16_t *)eepAdr, startTime);
	eepAdr += 2;

	uint8_t i = 0;

	while (!Wdt_IsTimerEnded(startTime, 5120 / 32))
	{
		eeprom_write_byte(eepAdr++, getMaxCurrentSensorValue());

		_delay_ms(300);
		i++;

		if (i == 5)
		{
			CapacitorOn();

			
		}
	}
	
	CapacitorOff();

	eeprom_write_byte(eepAdr++, 0);
	eeprom_write_byte(eepAdr++, i);
	eeprom_write_byte(eepAdr++, 0);

	while (!Wdt_IsTimerEnded(startTime, 10240 / 32))
	{
		eeprom_write_byte(eepAdr++, getMaxCurrentSensorValue());

		_delay_ms(300);
		i++;
	}
	
	PowerOff();

	eeprom_write_word((uint16_t *)eepAdr, 0);
	eepAdr += 2;

	eeprom_write_byte(eepAdr++, i);
	eeprom_write_word((uint16_t *)eepAdr, Wdt_GetCurrentMsCount());

	AlarmSound(2);

	while(1)
	{
	}
}


int main(void)
{
	Setup();

	TestCurrent();
}

void Setup()
{
	OutputPinsInit();

	Adc_Setup();
	
	Wdt_Timer_Enable();
}

uint8_t getMaxCurrentSensorValue()
{
uint8_t sensorValue;             // value read from the sensor
uint8_t sensorMax = 0;

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
