#include "EERTOSHAL.h"
//#include <avr/io.h>
//#include <avr/interrupt.h>

//RTOS Запуск системного таймера
#ifndef ATMEGA328
inline void RunRTOS (void)
{
TCCR2 = 1<<WGM21|4<<CS20; 				// Freq = CK/64 - Установить режим и предделитель
										// Автосброс после достижения регистра сравнения
TCNT2 = 0;								// Установить начальное значение счётчиков
OCR2  = LO(TimerDivider); 				// Установить значение в регистр сравн ения
TIMSK |= 1<<OCIE2;						// Разрешаем прерывание RTOS - запуск ОС

sei();
}
#endif
#ifdef ATMEGA328
inline void RunRTOS (void)
{
TCCR2A = 1<<WGM21|4<<CS20; 				// Freq = CK/64 - Установить режим и предделитель
										// Автосброс после достижения регистра сравнения
TCNT2 = 0;								// Установить начальное значение счётчиков
OCR2A  = LO(TimerDivider); 				// Установить значение в регистр сравнения
TIMSK2 |= 1<<OCIE2A;						// Разрешаем прерывание RTOS - запуск ОС

sei();
}
#endif // ATMEGA328
