#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "macros.h"             // мои макросы и битовые поля
#include "lcd/nokia1100_lcd_lib.h"	// Подключаем драйвер LCD-контроллера NOKIA1100
#include "menu/MicroMenu.h"          // микроменю
#include "sheduler/HAL.h"                // файл абстракции железа
#include "sheduler/EERTOS.h"             // диспетчер
//#include "button.h"             // мега кнопка
#include "kbd/kbd.h"                // мои кнопки

extern void InitControl();
extern uint8_t KeyCode(void);
extern void KeyScan();

/*** DUMY CODE ***/
enum ButtonValues
{
	BUTTON_NONE,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ENTER,
};

enum ButtonValues GetButtonPress(KeyCode)
//enum ButtonValues KeyCode()
{
	return BUTTON_NONE;
};
/*** END DUMY CODE ***/


/** Example menu item specific enter callback function, run when the associated menu item is entered. */
static void Level1Item1_Enter(void)
{
	puts("ENTER");
}

/** Example menu item specific select callback function, run when the associated menu item is selected. */
static void Level1Item1_Select(void)
{
	puts("SELECT");
	nlcd_PrintF(PSTR("select_menu"));
}

/** Generic function to write the text of a menu.
 *
 *  \param[in] Text   Text of the selected menu to write, in \ref MENU_ITEM_STORAGE memory space
 */
static void Generic_Write(const char* Text)
{
	if (Text)
		//puts_P(Text);
		nlcd_Print(Text);
}
//         Name      Next     Previous        Parent         Child     SelectFunc   EnterFunc  Text
MENU_ITEM(Menu_1,   Menu_2,     Menu_3,     NULL_MENU,      Menu_1_1,Generic_Write,   Level1Item1_Select,       "1");
MENU_ITEM(Menu_2,   Menu_3,     Menu_1,     NULL_MENU,      NULL_MENU,      NULL,   NULL,       "2");
MENU_ITEM(Menu_3,   Menu_1,     Menu_2,     NULL_MENU,      NULL_MENU,      NULL,   NULL,       "3");

MENU_ITEM(Menu_1_1, Menu_1_2,   Menu_1_2,   NULL_MENU,      NULL_MENU,      NULL,   NULL,       "1.1");
MENU_ITEM(Menu_1_2, Menu_1_1,   Menu_1_1,   NULL_MENU,      NULL_MENU,      NULL,   NULL,       "1.2");


// Прототипы задач ============================================================
void Reaction(void);

void DisplayHelloScreen();
void DisplayHelloScreenBlink();

void SwitchMenu();
void MenuInit();

// Global Variables

//RTOS Interrupt
ISR(RTOS_ISR){              // на втором таймере
    sei();
    TimerService();			// Прерывание ядра диспетчера
}
//..........................................................................


void Reaction(void){
    /*
    if (bt1) { bt1=0; LED_PORT |=1<<LED0; }
    if (bt2) { bt2=0; LED_PORT |=1<<LED1; }
    if (bt3) { bt3=0; LED_PORT |=1<<LED2; }
    if (bt4) { bt4=0; LED_PORT |=1<<LED3; }
    if (bt5) { bt5=0; LED_PORT |=1<<LED4; }

    if (bt_l)  { bt_l=0;  LED_PORT = 0;}
    if (bt_l2) { bt_l2=0; LED_PORT = 0xFF;}

    if (bt_al) { bt_al=0; LED_PORT |=1<<LED5;}

    SetTimerTask(Reaction, 100);
    //*/
}



//============================================================================
//Область задач
//============================================================================


//==============================================================================
int main(void){
    InitAll();		// Инициализируем периферию
    InitRTOS();		// Инициализируем ядро
    RunRTOS();		// Старт ядра.
    InitControl();

    nlcd_Init();
    _delay_ms(100);

    MenuInit();

 //   bt_scan();		// Запуск процедуры сканирования
//    Reaction();		// Запуск обработчика событий
//    DisplayHelloScreen();
    KeyScan();

    while(1){ 		// Главный цикл диспетчера

        if (KeyCode==0) 	nlcd_PrintF(PSTR("select"));
        GetButtonPress(KeyCode);

        wdt_reset();	// Сброс собачьего таймера
        TaskManager();	// Вызов диспетчера
    }

    return 0;
}

void MenuInit(){
	/* Set up the default menu text write callback, and navigate to an absolute menu item entry. */
	Menu_SetGenericWriteCallback(Generic_Write);
	Menu_Navigate(&Menu_1);
}

void SwitchMenu(){
		/* Example usage of Micromenu - here you can create your custom menu navigation system; you may wish to perform
		 * other tasks while detecting key presses, enter sleep mode while waiting for user input, etc.
		 */
		switch (GetButtonPress())
		{
			case BUTTON_UP:
				Menu_Navigate(MENU_PREVIOUS);
				break;
			case BUTTON_DOWN:
				Menu_Navigate(MENU_NEXT);
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
			default:
				break;
		}
}

char Text[] PROGMEM = "FLASH MEMORY TEST";


void DisplayHelloScreen(){

//	_delay_ms(100);

	nlcd_GotoXY(0,0);
	nlcd_PrintF(Text); // Выводим строку из программной памяти

	_delay_ms(4000);

	nlcd_GotoXY(0,0);

	nlcd_PrintF(PSTR(" УРА!!Оно пахает")); // Другой способ задания строк в программной памяти
	nlcd_PrintF(PSTR("----------------"));
	nlcd_PrintF(PSTR(" DigitalChip.ru "));
	nlcd_PrintF(PSTR("    present     "));
//	nlcd_PrintWide("NOKIA");
	nlcd_PrintF(PSTR(" NOKIA 1100 LCD "));
	nlcd_PrintF(PSTR("  demonstration "));
	nlcd_PrintF(PSTR("----------------"));
}

void DisplayHelloScreenBlink(){

		nlcd_GotoXY(4,7);
		nlcd_Print("It work!"); // Строка из ОЗУ (RAM)
		_delay_ms(1000);

		nlcd_GotoXY(4,7);
//		nlcd_PrintF(PSTR("        ")); // Строка из программной памяти (Flash)
//		nlcd_PrintWide(" 255 ");
		_delay_ms(1000);
}
