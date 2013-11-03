#include <avr/io.h>
#include "HAL.h"
//#include <util/delay.h>
//#include <avr/pgmspace.h>
//#include <avr/interrupt.h>
//#include <stdio.h>
//#include <avr/wdt.h>
#include "macros.h"             // ��� ������� � ������� ����
//#include "lcd/nokia1100_lcd_lib.h"	// ���������� ������� LCD-����������� NOKIA1100
//#include "menu/MicroMenu.h"          // ���������
//#include "kbd/kbd.h"                // ��� ������
//#include "onewire/delay.h"    // OW interface
//#include "onewire/onewire.h"
//#include "onewire/ds18x20.h"
#include "sheduler/dispatch.h"
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
void usartWrite();
void usartWrite1();
void usartStartMsg();

extern void USART0_write(unsigned char data);
extern void USART_init();
extern void InitScheduler (void);

int main(void)
{
    USART_init();
    InitScheduler();


//    usartWrite();
    AddTask(usartWrite,150);
    AddTask(usartWrite1,1500);
    AddTask(usartStartMsg,500);
    sei();
    while(1) { 		// ������� ���� ����������
        DispatchTask();
//        nlcd_Print(current_temp);
    }

    return 0;
}

void usartWrite(){
    USART0_write('D');
    AddTask(usartWrite,500);
}

void usartWrite1(){
    USART0_write('S');
    AddTask(usartWrite1,2500);
}

void usartStartMsg(){
    USART0_write('S');
    USART0_write('T');
    USART0_write('A');
    USART0_write('R');
    USART0_write('T');
}
