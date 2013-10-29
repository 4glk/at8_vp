#include "HAL.h"

inline void InitAll(void)
{

//InitUSART
//UBRR0L = LO(bauddivider);
//UBRR0H = HI(bauddivider);
//UCSR0A = 0;
//UCSR0B = 1<<RXEN0|1<<TXEN0|0<<RXCIE0|0<<TXCIE0|0<<UDRIE0;
//UCSR0C = 1<<UCSZ00|1<<UCSZ01;   //  1<<URSEL|

//InitPort
//LED_DDR = 0xFF;
//LED_PORT = 0;

<<<<<<< HEAD
//BTN_PORT |= 1<<BTN;//test branches
=======
//BTN_PORT |= 1<<BTN;
>>>>>>> a3bb881... stop the chaos scretches
//BTN_DDR = 0;

//DDRD |=1<<5 | 1<<4;


//Init ADC


//Init PWM


//Init Interrupts


//Init Timers


}
