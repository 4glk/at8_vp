
//#define RUN_TASKS


//#include "delay.h"
#include <avr/io.h>
//#include "EERTOSHAL.h"
#ifdef RUN_TASKS
	extern void RunTasks(void);
#endif
#define ATMEGA8
#ifndef F_CPU
	#warning F_CPU not defined
#elif (F_CPU<2020000)
	#define T_PRESCALER 8
#elif (F_CPU<16200000)
	#define T_PRESCALER 64
#else
	#define T_PRESCALER 256
#endif

#define TIKS_1MS (F_CPU/T_PRESCALER/1000)

#ifndef ATMEGA328

#endif // ATMEGA328


#ifdef ATMEGA328

#endif // ATMEGA328





void timerDelayInit(void);
void timerDelayMs(unsigned long int ms);
