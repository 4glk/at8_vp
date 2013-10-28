#ifndef HAL_H
#define HAL_H


//Clock Config
//#define F_CPU 8000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include "avrlibtypes.h"
#include "avrlibdefs.h"
#include <avr/pgmspace.h>
#include <avr/wdt.h>
//#include <util/delay.h>
#include <stdlib.h>

#define INLINE __attribute__((always_inline))

//System Timer Config
#define Prescaler	  		64
#define	TimerDivider  		(F_CPU/Prescaler/1000)		// 1 mS


//USART Config
#define baudrate 9600L
#define bauddivider ((F_CPU+baudrate*8)/(baudrate*16)-1)
#define HI(x) ((x)>>8)
#define LO(x) ((x)& 0xFF)


//PORT Defines
#define LED_PORT	PORTB
#define LED_DDR		DDRB

#define LED0		0
#define LED1 		1
#define LED2		2
#define	LED3		3
#define LED4		4
#define LED5		5
#define LED6		6
#define LED7		7

#define BTN_PORT	PORTD
#define BTN_DDR		DDRD
#define BTN_PIN		PIND
#define BTN			7

#define SEND(X) do{buffer_index = 1; UDR = X; UCSRB|=(1<<UDRIE); }while(0)

extern void InitAll(void);



#endif
