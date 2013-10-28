#include "EERTOSHAL.h"
//#include <avr/io.h>
//#include <avr/interrupt.h>

//RTOS ������ ���������� �������
#ifndef ATMEGA328
inline void RunRTOS (void)
{
TCCR2 = 1<<WGM21|4<<CS20; 				// Freq = CK/64 - ���������� ����� � ������������
										// ��������� ����� ���������� �������� ���������
TCNT2 = 0;								// ���������� ��������� �������� ���������
OCR2  = LO(TimerDivider); 				// ���������� �������� � ������� ����� ����
TIMSK |= 1<<OCIE2;						// ��������� ���������� RTOS - ������ ��

sei();
}
#endif
#ifdef ATMEGA328
inline void RunRTOS (void)
{
TCCR2A = 1<<WGM21|4<<CS20; 				// Freq = CK/64 - ���������� ����� � ������������
										// ��������� ����� ���������� �������� ���������
TCNT2 = 0;								// ���������� ��������� �������� ���������
OCR2A  = LO(TimerDivider); 				// ���������� �������� � ������� ���������
TIMSK2 |= 1<<OCIE2A;						// ��������� ���������� RTOS - ������ ��

sei();
}
#endif // ATMEGA328
