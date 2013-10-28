#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "nokia1100_lcd_lib.h"	// Подключаем драйвер LCD-контроллера NOKIA1100

char Text[] PROGMEM = "FLASH MEMORY TEST";

int main(void)
{
	nlcd_Init();
	_delay_ms(100);

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

	while(1)
	{

		nlcd_GotoXY(4,7);
		nlcd_Print("It паше!"); // Строка из ОЗУ (RAM)
		_delay_ms(1000);

		nlcd_GotoXY(4,7);
		nlcd_PrintF(PSTR("        ")); // Строка из программной памяти (Flash)
//		nlcd_PrintWide(" 255 ");
		_delay_ms(1000);
	}

}
