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
// Алгоритм запуска двигателя:
// 	- Подача предупреждающего сигнала. Пауза 1 сек.
// 	- Включение питания.
//	- Пока ток в пределах рабочего - ничего не делаем. если превышает - следующий шаг.
//	- Проверяем время с начала процедуры запуска. 
// 		- если время в пределах диапазона 
//			- инкрементация счётчика попыток старта.
//			- проверка счётчика.
//				- если счётчик превышен - Подаём сигнал аварии, прекращаем работу.
//			- иначе
//				- переходим к следующему пункту.
// 	- Включение пускового конденсатора.
// 	- В течении времени запуска контролируем тока 
//		- если ток ниже CapacitorOffSensorCurrent(= 23.44А) => выключаем конденсатор.
//		- если время запуска вышло и ток не снизился 
//			- отключаем питание и подаём сигнал.
//			- делаем паузу и включаем питание опять.
//			- переписываем время начала процедуры запуска.
//  - Пауза 0.5 секунд(минимум 300 мс), на стабилизацию рабочего тока после отключения конденсатора
//	


#include "main.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "AnalogIO_1_2Mhz.h"
#include "wddelay.h"
#include "soundsignals.h"
#include "tone.h"

// Время измерения в 32-х миллисекундных интервалах.
const uint16_t  measurementTime = 64 / 32;

// 66 mV / 1 A   - 30 A sensor
// 100 mV / 1 A  - 20 A sensor
// 185 mV / 1 A  - 5 A sensor

// 30A sensor
#define K_Current 66L

#define operatingCurrent 10  // (Ampere)

#define KStartCurrent 2.0	// Коэффициент превышения тока для начала процедуры запуска.

// (K_Current * 256 * 1.414 * KStartCurrent) / 5000.0)
#define K0 ((K_Current * 256 * 1.414 * KStartCurrent) / 5000.0)	// = 9,5563776

// Значение сенсора тока, выше которого начинается процедура запуска двигателя.
uint8_t startSensorValue = (uint8_t)(128.0 + operatingCurrent * K0);	// = 223 = 20 А
const uint8_t *startSensorValueEepromPtr = (uint8_t *)0;

#define CapacitorOffSensorCurrent 240	// 23.44 А

// Максимальное время старта в 32-х миллисекундных интервалах.
uint16_t maxStartTime = 3200 / 32;	// = 100 * 32 ms.
const uint16_t *maxStartTimeEepromPtr = (uint16_t *)1;

// Максимальное количество неудачных попыток запуска.
uint8_t MaximumStartAttempts = 3;
const uint8_t *MaximumStartAttemptsEepromPtr = (uint8_t *)3;

// суммарное время для попыток старта
#define MaxTimeForStartAttempts 20000 / 32

int main(void)
{
	//LoadInitialValuesFromEeprom();

	Setup();

	Loop();
}

// Чтение предопределённых данных из EEPROM:
// - стартовое значение сенсора тока,
// - максимальное время старта,
// - максимальное количество попыток старта.
void LoadInitialValuesFromEeprom()
{
	startSensorValue = eeprom_read_byte(startSensorValueEepromPtr);

	maxStartTime = eeprom_read_word(maxStartTimeEepromPtr);

	MaximumStartAttempts = eeprom_read_byte(MaximumStartAttemptsEepromPtr);
}

void Setup()
{
	OutputPinsInit();
	
	CapacitorOff();	// Выключаем пусковой конденсатор.
	PowerOff(); // Питание выключаем

	Adc_Setup();
	
	Wdt_Timer_Enable();
	
	// Ready signal.
	ReadySound();
}

void Loop()
{
uint8_t startAttemptsCounter = 0;

uint16_t beginTime = Wdt_GetCurrentMsCount();

	PowerOn();

	do
	{
		// Пока значения тока рабочие - ничего не делаем.
		while (getMaxCurrentSensorValue() < startSensorValue);

		// Контролируем число попыток старта двигателя за указанный промежуток времени.
		if (!Wdt_IsTimerEnded(beginTime, MaxTimeForStartAttempts))
		{
			startAttemptsCounter++;

			if (startAttemptsCounter > MaximumStartAttempts)
			{
				// Двигатель не запустился за указанный промежуток времени.
				// Число попыток старта исчерпано.
				PowerOff();

				EndSound();

				// Стоп до перезапуска системы(отключение и включение питания)
				while(1);
			}
		}
		else
		{
			// Запоминаем время первого запуска.
			beginTime = Wdt_GetCurrentMsCount();

			startAttemptsCounter = 1;
		}

		// Ток высокий - старт двигателя.
		if (StartEngine (maxStartTime) == 0)
		{
			// Не удалось запустить двигатель, выключаем питание. 
			PowerOff();

			// И подаём сигнал и пауза на 4 секунд.
			{
				AlarmSound (2);

				_delay_ms (4000);
			}

			// Изменяем начало отсчёта времени запуска.
			beginTime = Wdt_GetCurrentMsCount();

			// Повторяем запуск.
			PowerOn();
		}
		else
		{
			// Двигатель запущен.
			_delay_ms(500); // Для стабилизации тока.
		}
		
	}
	while(1);
}

int8_t StartEngine(uint16_t startDuration)
{
uint16_t startTime = Wdt_GetCurrentMsCount();

	// Подключаем пусковой конденсатор на время запуска.
	CapacitorOn();

	while(!Wdt_IsTimerEnded(startTime, startDuration))
	{
		// Проверяем, запустился ли двигатель(ток снизился)
		if (getMaxCurrentSensorValue() < CapacitorOffSensorCurrent)
		{
			return 1;
		}
	}

	CapacitorOff();

	return 0;
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
