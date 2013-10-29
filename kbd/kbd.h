#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../macros.h"          // "../" -������������ ������� , "./" - ������� �������


#define KEY_DDR     DDRB
#define KEY_PORT    PORTB
#define KEY_PIN     PINB
#define KBD_PIN_1	PB0    //PINB0..PINB2 for atmega328
#define KBD_PIN_2	PB1     //�� ��� 328 ����� �� ����� �.�. DDRB0..DDRB2
#define KBD_PIN_3	PB2
//#define KEY_MASK    (_BV(PB2)|_BV(PB1)|_BV(PB0))
#define KEY_MASK    (_BV(KBD_PIN_3)|_BV(KBD_PIN_2)|_BV(KBD_PIN_1))
#define KEY_CODE    (~(KEY_PIN)&(KEY_MASK))

/*//��� ���� � ��������� �����
#define	BUTTON_NONE 0
#define	BUTTON_UP 3
#define	BUTTON_DOWN 4
#define	BUTTON_LEFT 1
#define	BUTTON_RIGHT 2
#define	BUTTON_ENTER 5
#define BUTTON_FIRE 6
#define BUTTON_VAPOR 7
//*/

void InitControl();
void KeyState();
void KeyScan();
volatile static uint8_t k=0;
uint8_t KeyCurrentCode;


