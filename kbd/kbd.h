#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../macros.h"          // "../" -родительский каталог , "./" - текущий каталог

#define KEY_MASK (~PINC&0b00000111)
#define KEY_DDR     DDRB
#define KEY_PORT    PORTB


void InitControl();
uint8_t KeyCode();
void KeyState();
void KeyScan();
//uint16_t i;
int KeyCurrentCode;
//extern void StateAutomate();

