#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
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
#include "sheduler/dispatch.h"
// TODO: разобраться с инициализацией и переключением платформы
#define StartFrom       0xF0 //для 1 мс 1КГц            //на большой частоте висит в убр регистре

//TODO:
//-кнопки
//-дисплей
//-меню
//-диспетчер
//-датчик температуры
//-датчик уровня
//-юсарт для отладки
//-алгоритм работы горелки и насоса трубы
//-режимы
//-звуковое оповещение


extern void InitControl();
extern uint8_t KeyCode(void);
extern void KeyScan();
uint8_t KeyCurrentCode;
void PositionMenuesLevel1();
void PositionMenuesLevel2();
uint16_t current_temp;
//int j=0;
/*

void USART0_write(unsigned char data)
{
	while ( !( UCSRA & (1<<UDRE)) ) ;
	UDR = data;
}

FILE usart_str = FDEV_SETUP_STREAM(USART0_write, NULL, _FDEV_SETUP_WRITE); // для функции printf

void print_address(unsigned char* address) {
	printf("%.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X", address[0],address[1],address[2],address[3],address[4],address[5],address[6],address[7]);
}

unsigned char	nDevices;	// количество сенсоров
unsigned char	owDevicesIDs[MAXDEVICES][8];	// Их ID

unsigned char search_ow_devices(void) // поиск всех устройств на шине
{
	unsigned char	i;
   	unsigned char	id[OW_ROMCODE_SIZE];
   	unsigned char	diff, sensors_count;

	sensors_count = 0;

	for( diff = OW_SEARCH_FIRST; diff != OW_LAST_DEVICE && sensors_count < MAXDEVICES ; )
    {
		OW_FindROM( &diff, &id[0] );

      	if( diff == OW_PRESENCE_ERR ) break;

      	if( diff == OW_DATA_ERR )	break;

      	for (i=0;i<OW_ROMCODE_SIZE;i++)
         	owDevicesIDs[sensors_count][i] = id[i];

		sensors_count++;
    }
	return sensors_count;

}
//*/
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
MENU_ITEM(Menu_1,   Menu_2,     Menu_3,     NULL_MENU,      Menu_1_1,PositionMenuesLevel1,Level1Item1_Enter,       "1");
MENU_ITEM(Menu_2,   Menu_3,     Menu_1,     NULL_MENU,      NULL_MENU,PositionMenuesLevel1,   NULL,       "2");
MENU_ITEM(Menu_3,   Menu_1,     Menu_2,     NULL_MENU,      NULL_MENU,PositionMenuesLevel1,   NULL,       "3");

MENU_ITEM(Menu_1_1, Menu_1_2,   Menu_1_2,   Menu_1,      NULL_MENU,PositionMenuesLevel2,   NULL,       "1.1");
MENU_ITEM(Menu_1_2, Menu_1_1,   Menu_1_1,   Menu_1,      NULL_MENU,PositionMenuesLevel2,   NULL,       "1.2");

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

//==============================================================================
int main(void)
{
    //timer for keyscan initiaization
//    TCCR2 |= (1<<CS22)|(0<<CS21)|(1<<CS20);   // устанавливаем прескалер - 1024(101) 256(100) 64(011) 8(010) 0(001) off(000)
//   TIFR = 1<<TOV2;   // очищаем флаг прерывания таймера Т0
//  TIMSK |= 1<<TOIE2;   // разрешаем прерывание по переполнению
//  TCNT2 = StartFrom;    // загружаем начальное зн. в счетный регистр
    //keyscan initialization
    InitControl();
    //lcd initialization
    nlcd_Init();
    _delay_ms(100); //delay for reset display
    // menu initialization
    MenuInit();

//    	stdout = &usart_str; // указываем, куда будет выводить printf

//	DDRB = 0b00000010; PORTB = 0b00000010;
//	DDRC = 0b00000000; PORTC = 0b00000000;
    DDRD = 0b00001010;
    PORTD = 0b00001000;

//	USART_init(); // включаем uart

    timerDelayInit();

//	nDevices = search_ow_devices(); // ищем все устройства

//	printf("---------- Found %d devices ----------", nDevices);

    /*
    	for (unsigned char i=0; i<nDevices; i++) // теперь сотируем устройства и запрашиваем данные
    	{
    		// узнать устройство можно по его груповому коду, который расположен в первом байте адресса
    		switch (owDevicesIDs[i][0])
    		{
    			case OW_DS18B20_FAMILY_CODE: { // если найден термодатчик DS18B20
    				printf("\r"); print_address(owDevicesIDs[i]); // печатаем знак переноса строки, затем - адрес
    				printf(" - Thermometer DS18B20"); // печатаем тип устройства
    				DS18x20_StartMeasureAddressed(owDevicesIDs[i]); // запускаем измерение
    				timerDelayMs(800); // ждем минимум 750 мс, пока конвентируется температура
    				unsigned char	data[2]; // переменная для хранения старшего и младшего байта данных
    				DS18x20_ReadData(owDevicesIDs[i], data); // считываем данные
    				unsigned char	themperature[3]; // в этот массив будет записана температура
    				DS18x20_ConvertToThemperature(data, themperature); // преобразовываем температуру в человекопонятный вид
    				printf(": %d.%d C", themperature[1],themperature[2]);
    				current_temp=themperature[1];
    			} break;
    			case OW_DS18S20_FAMILY_CODE: { // если найден термодатчик DS18B20
    				printf("\r"); print_address(owDevicesIDs[i]); // печатаем знак переноса строки, затем - адрес
    				printf(" - Thermometer DS18S20"); // печатаем тип устройства
    			} break;

    			case OW_DS1990_FAMILY_CODE: { // если найден электронный ключ DS1990
    				printf("\r"); print_address(owDevicesIDs[i]); // печатаем знак переноса строки, затем - адрес
    				printf(" - Serial button DS1990"); // печатаем тип устройства
    			} break;
    			case OW_DS2430_FAMILY_CODE: { // если найдена EEPROM
    				printf("\r"); print_address(owDevicesIDs[i]); // печатаем знак переноса строки, затем - адрес
    				printf(" - EEPROM DS2430"); // печатаем тип устройства
    			} break;
    			case OW_DS2413_FAMILY_CODE: { // если найден ключ
    				printf("\r"); print_address(owDevicesIDs[i]); // печатаем знак переноса строки, затем - адрес
    				printf(" - Switch 2413"); // печатаем тип устройства
    			} break;
    		}

    	}

    //*/

//    DisplayHelloScreen();
//    KeyScan();
//    nlcd_PrintF(PSTR("HELLO!!!"));
    sei();
    while(1) { 		// Главный цикл диспетчера
//           if(!flags.KeyPressed&&flags.KeyReleased) nlcd_PrintF(PSTR("BUTTON"));
        //SwitchMenu();
        /*
                   if (KeyCurrentCode){
                   switch(KeyCurrentCode){
                   case 0:break;
                   case 1:nlcd_PrintF(PSTR("1"));break;
                   case 2:nlcd_PrintF(PSTR("2"));break;
                   case 3:nlcd_PrintF(PSTR("3"));break;
                   case 4:nlcd_PrintF(PSTR("4"));break;
                   case 5:nlcd_PrintF(PSTR("5"));break;
                   case 6:nlcd_PrintF(PSTR("6"));break;
                   case 7:nlcd_PrintF(PSTR("7"));break;
                   default:KeyCurrentCode=0;break;
                   }
                       KeyCurrentCode=0;
                   }
        //*/    nlcd_GotoXY(3,3);
        // nlcd_Print(current_temp);

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
//		switch (GetButtonPress())
        switch (KeyCurrentCode) {
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
        case BUTTON_SCROLL_UP:
            nlcd_PrintF(PSTR("SROLL_UP"));
            break;
        case BUTTON_SCROLL_DOWN:
            nlcd_PrintF(PSTR("SCROLL_DOWN"));
            break;
        case BUTTON_FIRE:
            nlcd_PrintF(PSTR("FIRE_ENABLE"));
            break;
        case BUTTON_PUMP:
            nlcd_PrintF(PSTR("PUMP_ENABLE"));
            break;
        default:
            break;
        }
        KeyCurrentCode=0;
    }
}

//char Text[] PROGMEM = "FLASH MEMORY TEST";

