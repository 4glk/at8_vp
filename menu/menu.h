//#include "../kbd/kbd.h"
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include "../HAL.h"
//#include "../usart/usart.h"
//#include "../lcd/nokia1100_lcd_lib.h"
#include "MicroMenu.h"
#include "MenuConfig.h"
//#include "../onewire/OWfunc.h"


static void Level1Item1_Enter(void);
static void Level1Item1_Select(void);
static void Generic_Write(const char* Text);
void PositionMenuesLevel1();
void PositionMenuesLevel2();
void MenuInit();
void SwitchMenu();
extern uint8_t KeyCurrentCode;
extern void USART0_write(unsigned char data);
extern void USART_Print(const char *USART_String);
//extern void nlcd_PrintF();
//extern void nlcd_GotoXY(char x,char y);
//extern void nlcd_PrintWide();
extern void AddTask (void (*taskfunc)(void), uint16_t taskdelay);
extern void usartPrintOnewire();
extern void nlcd_PrintF();
extern void nlcd_GotoXY(char x,char y);
extern void usartDebug();
extern void temperatureRead();
extern void temperatureStartConvert();
extern void burnerStart();
extern void pumpStart();

//uint8_t current_temp=115;

#pragma once

/*** DUMY CODE ***/
enum ButtonValues {
    BUTTON_NONE, //0
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_ENTER, //5
    BUTTON_FIRE,
    BUTTON_PUMP,        // восьмой никогда не будет , если только из функции
    BUTTON_SCROLL_UP=9,
    BUTTON_SCROLL_DOWN,


};
//#pragma once
/*
enum ButtonValues GetButtonPress()
//enum ButtonValues KeyCode()
{
    return BUTTON_NONE;
};
/ *** END DUMY CODE *** /
//*/
