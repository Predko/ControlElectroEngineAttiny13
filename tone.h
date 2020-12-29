// ATMEL ATTINY13
//                            
//                                              +-\/-+
//                             AinX (D X) PB5  1|    |8  Vcc
//     Power supply relay      AinX (D X) PB3  2|    |7  PB2 (D X) AinX - Current sensor pin
//     Start relay             AinX (D X) PB4  3|    |6  PB1 (D X) PWM	- Tone port
//                                        GND  4|    |5  PB0 (D X) PWM	- millis timer
//                                              +----+

#ifndef Tone_h
#define Tone_h

void Tone(uint8_t Tonefrequency);

inline void NoTone()
{
	TCCR0B = 0;// выкл. таймер
}

#endif
