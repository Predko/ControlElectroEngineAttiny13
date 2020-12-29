/*
 * main.cpp
 * 
 * 1.2 MHz, built in resonator
 * 
 *
 * Created: 25.12.2020 13:17:20
 *  Author: Predko Victor
 */ 
// ATMEL ATTINY13
//                            
//                                              +-\/-+
//                             AinX (D X) PB5  1|    |8  Vcc
//     Power supply relay      AinX (D X) PB3  2|    |7  PB2 (D X) AinX - Current sensor pin
//     Start relay             AinX (D X) PB4  3|    |6  PB1 (D X) PWM	- Tone port
//                                        GND  4|    |5  PB0 (D X) PWM	- millis timer
//                                              +----+

#include "main.h"
#include <avr/io.h>

#include "ModifiedMovingAverage.h"
#include "AnalogIO_1_2Mhz.h"
#include "millis.h"
#include "soundsignals.h"

#define START_RELAY _BV(PB4)
#define ALARM_RELAY _BV(PB3)

// Время измерения в мс
#define measurementTime 100

// 66 mV / 1 A   - 30 A sensor
// 100 mV / 1 A  - 20 A sensor
// 185 mV / 1 A  - 5 A sensor

// 30A sensor
#define K_Current 66L

#define operatingCurrent 8  // (Ampere)

#define K0 (int)((K_Current * 1024 * 1.414) / 5000.0)

#define startSensorValue (int)((512.0 + (operatingCurrent + operatingCurrent / 2) * K0))

#define maxStartTime 6000	// milliseconds

int main(void)
{
	Setup();
	
	Loop();
}

void Setup()
{
	Timer_Init();
	
	pins_init();

	PORTB |= START_RELAY;
	PORTB |= ALARM_RELAY;

	int sensorValue = getMaxCurrentSensorValue();
	
	MMA_SetFirstValue(sensorValue);
	
	// Ready signal for 500 ms
	ReadySound(500);
}

void Loop()
{
	while (1)
	{
		int sensorValue = getMaxCurrentSensorValue();

		if ( sensorValue > startSensorValue)
		{
			AlarmSound(1);

			if (StartEngine(maxStartTime) == false)
			{

				PORTB &= ~ALARM_RELAY; // LOW

				{
					AlarmSound(2);

					delayUpTo1sec(2000);

					AlarmSound(2);

					delayUpTo1sec(2000);

					AlarmSound(2);
				}

				PORTB |= ALARM_RELAY; // HIGH
			}
		}
	
		delayUpTo1sec(1000);
	}
}

bool StartEngine(int startDuration)
{
	PORTB &= ~START_RELAY; // LOW

	SetTimerDelay(startDuration);

	do
	{
		int sensorValue = getMaxCurrentSensorValue();

		if ( sensorValue < startSensorValue)
		{
			PORTB |= START_RELAY; // HIGH

			return true;
		}
	}
	while(!IsTimerEnded());
	
	PORTB |= START_RELAY; // HIGH

	return false;
}

int getMaxCurrentSensorValue()
{
	int sensorValue;             // value read from the sensor
	int sensorMax = 0;
	
	SetTimerDelay(measurementTime);

	while(!IsTimerEnded())
	{
		sensorValue = MMA_CalcNew(adc_read());
		
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
	adc_setup();
	
	// Output mode
	DDRB |= START_RELAY;
	DDRB |= ALARM_RELAY;
}
