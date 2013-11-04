#include <avr/io.h>
#include "HAL.h"
//#include <util/delay.h>
//#include <avr/pgmspace.h>
//#include <avr/interrupt.h>
//#include <stdio.h>
//#include <avr/wdt.h>
#include "macros.h"             // ��� ������� � ������� ����
#include "lcd/nokia1100_lcd_lib.h"	// ���������� ������� LCD-����������� NOKIA1100
#include "menu/menu.h"          // ���������
//#include "menu/MicroMenu.h"
//#include "menu/MenuConfig.h"
#include "kbd/kbd.h"                // ��� ������
//#include "onewire/delay.h"    // OW interface
//#include "onewire/onewire.h"
//#include "onewire/ds18x20.h"
#include "sheduler/dispatch.h"          //aasdfasf
#include "usart/usart.h"

//#define TASCII(numb) (numb+48)
//#define OW_BIT_OUT
// TODO: ����������� � �������������� � ������������� ���������
//#define StartFrom       0xF0 //��� 1 �� 1���            //�� ������� ������� ����� � ��� ��������

//TODO:
//+������
//+�������
//!+����
//+���������
//!+������ �����������
//-������ ������
//!+����� ��� �������
//-�������� ������ ������� � ������ �����
//-������
//-�������� ����������
/// ���� �������� �� �� ������� �� ������ �� onewire ����� �����
/// ������ ����� ����� , ������� ����� �����
/// ���� ������ ������� , �� �� ������ ��� ������� �� ���������������� � ��������� ���������
/// �������� ���� ��� ��� �������� , 595 � 164
/// ������� ������ ������ � ������ � ������, ��� ��������� ���������� �� ������
/// �� ������ ������� ��� ������� ������ � ���������� ��� ���� ��������

//==============================================================================


extern void USART0_write(unsigned char data);
extern void USART_init();
extern void InitScheduler (void);
extern void MenuInit();
extern void InitControl();
extern void usartDebug();
extern void timerDelayInit();
//extern void nlcd_Init();

int main(void)
{
    USART_init();
    InitScheduler();
    InitControl();
 //   nlcd_Init();
    MenuInit();
    timerDelayInit();
//    forMenuInit();
    usartDebug();
    AddTask(KeyScan,50);
    AddTask(SwitchMenu,100);
    sei();
    while(1) {
        DispatchTask();
    }

    return 0;
}

