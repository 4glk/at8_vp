#include <avr/io.h>
//#include <util/delay.h>
//#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "../onewire/onewire.h"

void usartPrintOnewire();
extern uint16_t current_temp;
void usartDebug();
