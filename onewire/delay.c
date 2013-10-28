#include "delay.h"
#include <avr/io.h>

#ifdef RUN_TASKS 
	extern void RunTasks(void);
#endif

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

void timerDelayInit(void)
{
#if (T_PRESCALER==8)
	TCCR0 = (1<<CS01); // prescaler = 8
#elif (T_PRESCALER==64)
	TCCR0 = (1<<CS01) | (1<<CS00); // prescaler = 64
#elif (T_PRESCALER==256)
	TCCR0 = (1<<CS02); // prescaler = 256
#else 
	#warning T_PRESCALER not defined
#endif

	
}

void timerDelayMs(unsigned long int ms)
{
	while(ms--){
		TCNT0 = 0;
		while(TCNT0 < TIKS_1MS) {
			#ifdef RUN_TASKS 
				RunTasks();
			#endif
		}
	}
}
