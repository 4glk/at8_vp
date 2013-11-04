#include <avr/io.h>
#include "HAL.h"
//#include <util/delay.h>
//#include <avr/pgmspace.h>
//#include <avr/interrupt.h>
//#include <stdio.h>
//#include <avr/wdt.h>
#include "macros.h"             // мои макросы и битовые поля
#include "lcd/nokia1100_lcd_lib.h"	// Подключаем драйвер LCD-контроллера NOKIA1100
#include "menu/menu.h"          // микроменю
//#include "menu/MicroMenu.h"
//#include "menu/MenuConfig.h"
#include "kbd/kbd.h"                // мои кнопки
//#include "onewire/delay.h"    // OW interface
//#include "onewire/onewire.h"
//#include "onewire/ds18x20.h"
#include "sheduler/dispatch.h"          //aasdfasf
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

