#ifndef EERTOSHAL_H
#define EERTOSHAL_H
#include "HAL.h"

#define STATUS_REG 			SREG
#define Interrupt_Flag		SREG_I
#define Disable_Interrupt	cli();
#define Enable_Interrupt	sei();

#define ATMEGA328       //for more memory

//RTOS Config
#ifndef ATMEGA328
#define RTOS_ISR  	 		TIMER2_COMP_vect
#endif
#ifdef ATMEGA328
#define   RTOS_ISR  			TIMER2_COMPA_vect
#endif // ATMEGA328

#define	TaskQueueSize		8
#define MainTimerQueueSize	8



extern void RunRTOS (void);

#endif
