#include "HAL.h"

inline void InitAll(void)
{

//InitUSART
UBRRL = LO(bauddivider);
UBRRH = HI(bauddivider);
UCSRA = 0;
UCSRB = 1<<RXEN|1<<TXEN|0<<RXCIE|0<<TXCIE|0<<UDRIE;
UCSRC = 1<<URSEL|1<<UCSZ0|1<<UCSZ1;

//InitPort
LED_DDR = 0xFF;
LED_PORT = 0;

BTN_PORT |= 1<<BTN;
BTN_DDR = 0;

DDRD |=1<<5 | 1<<4;


//Init ADC


//Init PWM


//Init Interrupts


//Init Timers


}
