#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/wdt.h>
#include "macros.h"             // мои макросы и битовые поля
#include "lcd/nokia1100_lcd_lib.h"	// Подключаем драйвер LCD-контроллера NOKIA1100
#include "menu/MicroMenu.h"          // микроменю
#include "kbd/kbd.h"                // мои кнопки
#include "onewire/onewire.h"        // OW interface
#include <avr/wdt.h>
#include <util/delay.h>
#include "onewire/delay.h"
#include "onewire/onewire.h"
#include "onewire/ds18x20.h"
#include "onewire/delay.h"
#include "sheduler/dispatch.h"
#include "usart/usart.h"

#define TASCII(numb) (numb+48)
// TODO: разобраться с инициализацией и переключением платформы
//#define StartFrom       0xF0 //для 1 мс 1КГц            //на большой частоте висит в убр регистре

//TODO:
//+кнопки
//+дисплей
//!+меню
//+диспетчер
//-датчик температуры
//-датчик уровня
//-юсарт для отладки
//-алгоритм работы горелки и насоса трубы
//-режимы
//-звуковое оповещение


extern void InitControl();
extern uint8_t KeyCode(void);
extern void InitScheduler();
extern void KeyScan();
extern uint8_t KeyCurrentCode;
extern void USART0_write();
void PositionMenuesLevel1();
void PositionMenuesLevel2();
uint16_t current_temp=100;
extern void usartDebug();
extern void readTemperature();

//int j=0;

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

enum ButtonValues GetButtonPress()
//enum ButtonValues KeyCode()
{
    return BUTTON_NONE;
};
/*** END DUMY CODE ***/


/** Example menu item specific enter callback function, run when the associated menu item is entered. */
static void Level1Item1_Enter(void)
{
//	puts("ENTER");
    nlcd_PrintF(PSTR("ENTER_MENU_1_1"));
}

/** Example menu item specific select callback function, run when the associated menu item is selected. */
static void Level1Item1_Select(void)
{
//	puts("SELECT");
    nlcd_PrintF(PSTR("SELECT_MENU_1_1"));
    nlcd_GotoXY(0,3);
//	nlcd_SendByte(CMD_LCD_MODE,0xA7);
}

/** Generic function to write the text of a menu.
 *
 *  \param[in] Text   Text of the selected menu to write, in \ref MENU_ITEM_STORAGE memory space
 */
static void Generic_Write(const char* Text)
{
    if (Text)
        //puts_P(Text);
//		nlcd_Print(Text);
        nlcd_PrintF(Text);
}
//         Name      Next     Previous        Parent         Child     SelectFunc   EnterFunc  Text
MENU_ITEM(Menu_1,   Menu_2,     Menu_3,     NULL_MENU,      Menu_1_1,PositionMenuesLevel1,Level1Item1_Enter,       "1 FIRE");
MENU_ITEM(Menu_2,   Menu_3,     Menu_1,     NULL_MENU,      NULL_MENU,PositionMenuesLevel1,   NULL,       "2 VAPORIZER");
MENU_ITEM(Menu_3,   Menu_1,     Menu_2,     NULL_MENU,      NULL_MENU,PositionMenuesLevel1,   NULL,       "3 PUMP");

MENU_ITEM(Menu_1_1, Menu_1_2,   Menu_1_2,   Menu_1,      NULL_MENU,PositionMenuesLevel2,   NULL,       "1.1        ");
MENU_ITEM(Menu_1_2, Menu_1_1,   Menu_1_1,   Menu_1,      NULL_MENU,PositionMenuesLevel2,   NULL,       "1.2        ");

void PositionMenuesLevel1()
{
    nlcd_GotoXY(0,3);
}

void PositionMenuesLevel2()
{
    nlcd_GotoXY(0,4);
}
// Прототипы задач ============================================================


void SwitchMenu();
void MenuInit();
void usartPrint(){

}

//    FILE usart_str = FDEV_SETUP_STREAM(USART0_write, NULL, _FDEV_SETUP_WRITE); // для функции printf
//==============================================================================
int main(void)
{
    InitControl();
    nlcd_Init();
    _delay_ms(100); //delay for reset display
    MenuInit();
    InitScheduler();
//  DDRD = 0b00001010;
//    PORTD = 0b00001000;

	USART_init(); // включаем uart
    timerDelayInit();
//    DisplayHelloScreen();
//    KeyScan();
//    nlcd_PrintF(PSTR("HELLO!!!"));

//    stdout = &usart_str; // указываем, куда будет выводить printf
    AddTask(KeyScan,Idle,5,0,0xffff);
    AddTask(SwitchMenu,Idle,25,0,0xffff);
//    AddTask(usartPrint,Idle,250,0,0xffff);
//    void usartDebug();
 //    nlcd_Print(current_temp);
    usartDebug();
    sei();
        USART0_write('D');
//    printf("Debug start");
    while(1) { 		// Главный цикл диспетчера
        DispatchTask();
//        nlcd_Print(current_temp);
    }

    return 0;
}

void MenuInit()
{
    /* Set up the default menu text write callback, and navigate to an absolute menu item entry. */
        Menu_Navigate(&Menu_1);
    Menu_SetGenericWriteCallback(Generic_Write); //вот тут скорее всего меняется местами время запуска вывода текста
                                                    // и выполнения функции выполнения
}

void SwitchMenu()
{
    /* Example usage of Micromenu - here you can create your custom menu navigation system; you may wish to perform
     * other tasks while detecting key presses, enter sleep mode while waiting for user input, etc.
     */
    if (KeyCurrentCode) {
//    if (6) {
//		switch (GetButtonPress())
        USART0_write('K');
        switch (KeyCurrentCode) {
        case BUTTON_UP:
            Menu_Navigate(MENU_PREVIOUS);
            USART0_write('U');
            break;
        case BUTTON_DOWN:
            Menu_Navigate(MENU_NEXT);
//            printf("Down");
            USART0_write('D');
            break;
        case BUTTON_LEFT:
            Menu_Navigate(MENU_PARENT);
            break;
        case BUTTON_RIGHT:
            Menu_Navigate(MENU_CHILD);
            break;
        case BUTTON_ENTER:
            Menu_EnterCurrentItem();
            break;
        case BUTTON_SCROLL_UP:
            nlcd_PrintF(PSTR("SROLL_UP"));
            nlcd_PrintWide(current_temp);
            break;
        case BUTTON_SCROLL_DOWN:
            nlcd_PrintF(PSTR("SCROLL_DOWN"));
//            usartDebug();
//            printf("Debug USART");
            break;
        case BUTTON_FIRE:
            nlcd_PrintF(PSTR("FIRE_ENABLE"));
            USART0_write('F');
            break;
        case BUTTON_PUMP:
            nlcd_PrintF(PSTR("PUMP_ENABLE"));
            USART0_write('P');
            break;
        default:
            break;
        }
        KeyCurrentCode=0;
    }
}

//char Text[] PROGMEM = "FLASH MEMORY TEST";

