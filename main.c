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
#include "onewire/delay.h"    // OW interface
#include "onewire/onewire.h"
#include "onewire/ds18x20.h"
#include "sheduler/dispatch.h"          //as
#include "usart/usart.h"
#include "burner/burner.h"

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
//-eeprom
//-все связать
//-отладить
/// куча проблемм из за отладки на другом МК onewire пашет
/// кнопки пашут , дисплей пашет
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
extern void nlcd_Init();
//extern void InitBurner();

int main(void)
{
    timerDelayInit();
    USART_init();
    usartDebug();
    InitScheduler();
    InitControl();
    nlcd_Init();
    MenuInit();
    InitBurner();
///*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//    nlcd_PrintF(PSTR("HELLO"));
    AddTask(KeyScan,50);
    AddTask(SwitchMenu,100);
    sei();
    while(1) {
        DispatchTask();
    }

    return 0;
}


