#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "nokia1100_lcd_lib.h"	// ���������� ������� LCD-����������� NOKIA1100

char Text[] PROGMEM = "FLASH MEMORY TEST";

int main(void)
{
	nlcd_Init();
	_delay_ms(100);

	nlcd_GotoXY(0,0);
	nlcd_PrintF(Text); // ������� ������ �� ����������� ������

	_delay_ms(4000);

	nlcd_GotoXY(0,0);

	nlcd_PrintF(PSTR(" ���!!��� ������")); // ������ ������ ������� ����� � ����������� ������
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
		nlcd_Print("It ����!"); // ������ �� ��� (RAM)
		_delay_ms(1000);

		nlcd_GotoXY(4,7);
		nlcd_PrintF(PSTR("        ")); // ������ �� ����������� ������ (Flash)
//		nlcd_PrintWide(" 255 ");
		_delay_ms(1000);
	}

}
