#include <avr/io.h>
#include "HAL.h"
//#include <util/delay.h>
//#include <avr/pgmspace.h>
//#include <avr/interrupt.h>
//#include <stdio.h>
//#include <avr/wdt.h>
#include "macros.h"             // мои макросы и битовые поля
//#include "lcd/nokia1100_lcd_lib.h"	// Подключаем драйвер LCD-контроллера NOKIA1100
//#include "menu/MicroMenu.h"          // микроменю
//#include "kbd/kbd.h"                // мои кнопки
//#include "onewire/delay.h"    // OW interface
//#include "onewire/onewire.h"
//#include "onewire/ds18x20.h"
#include "sheduler/dispatch.h"
#include "usart/usart.h"

//#define TASCII(numb) (numb+48)
//#define OW_BIT_OUT
// TODO: разобраться с инициализацией и переключением платформы
//#define StartFrom       0xF0 //для 1 мс 1КГц            //на большой частоте висит в убр регистре

//TODO:
//+кнопки
//+дисплей
//!+меню
//+диспетчер
//!+датчик температуры
//-датчик уровня
//!+юсарт для отладки
//-алгоритм работы горелки и насоса трубы
//-режимы
//-звуковое оповещение
/// куча проблемм из за отладки на другом МК onewire почти пашет
/// кнопки почти пашут , дисплей почти пашет
/// пора делать печатку , но не забыть про дисплей на семисегментниках и сдвиговых регистров
/// развести надо под оба варианта , 595 и 164
/// вводить режимы работы и работу с еепром, все остальное доделывать на железе
/// не забыть конечно про внешнюю логику и пиродатчик или фото резистор

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
    while(1) { 		// Главный цикл диспетчера
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
