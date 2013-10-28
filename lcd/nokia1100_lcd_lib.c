//***************************************************************************
//  File........: nokia1100_lcd_lib.c
//  Author(s)...: Chiper
//  URL(s)......: http://digitalchip.ru/
//  Device(s)...: ATMega...
//  Compiler....: AVR-GCC
//  Description.: Драйвер LCD-контроллера от Nokia1100
//  Data........: 21.02.12
//  Version.....: 1.0
//***************************************************************************
//  Notice: Все управляющие контакты LCD-контроллера должны быть подключены к
//  одному и тому же порту на микроконтроллере
//***************************************************************************

#include "nokia1100_lcd_lib.h"
#include "nokia1100_lcd_fnt.h"	// Подключаем шрифт (будет размещен в программной памяти)

//******************************************************************************
// Инициализация контроллера
void nlcd_Init(void)
{
	// Инициализируем порт на вывод для работы с LCD-контроллером
	DDR_LCD |= (1<<SCLK_LCD_PIN)|(1<<SDA_LCD_PIN)|(1<<CS_LCD_PIN)|(1<<RST_LCD_PIN);

	CS_LCD_RESET;
	RST_LCD_RESET;

	_delay_ms(10);            // выжидем не менее 5мс для установки генератора(менее 5 мс может неработать)

	RST_LCD_SET;

	nlcd_SendByte(CMD_LCD_MODE,0x20); // Запись в регистр Vop
	nlcd_SendByte(CMD_LCD_MODE,0x90);
	nlcd_SendByte(CMD_LCD_MODE,0xA4); // all on/normal display
	nlcd_SendByte(CMD_LCD_MODE,0x2F); // Power control set(charge pump on/off)
	nlcd_SendByte(CMD_LCD_MODE,0x40); // set start row address = 0
	nlcd_SendByte(CMD_LCD_MODE,0xB0); // установить Y-адрес = 0
	nlcd_SendByte(CMD_LCD_MODE,0x10); // установить X-адрес, старшие 3 бита
	nlcd_SendByte(CMD_LCD_MODE,0x0);  // установить X-адрес, младшие 4 бита
	nlcd_SendByte(CMD_LCD_MODE,0xC8); // mirror Y axis (about X axis)
	//nlcd_SendByte(CMD_LCD_MODE,0xA1); // Инвертировать экран по горизонтали
	nlcd_SendByte(CMD_LCD_MODE,0xAC); // set initial row (R0) of the display
	nlcd_SendByte(CMD_LCD_MODE,0x07);
	//nlcd_SendByte(CMD_LCD_MODE,0xF9); //
	nlcd_SendByte(CMD_LCD_MODE,0xAF); // экран вкл/выкл

	nlcd_Clear(); // clear LCD

	nlcd_SendByte(CMD_LCD_MODE,0xA7); 	// инвертированный экран
	_delay_ms(500);                		// 1/2 Sec delay
	nlcd_SendByte(CMD_LCD_MODE,0xA6); 	// нормальный экран (не инвертированный)
	_delay_ms(1000);               		// 1 Sec delay
}

//******************************************************************************
// Очистка экрана
void nlcd_Clear(void)
{
	nlcd_SendByte(CMD_LCD_MODE,0x40); // Y = 0
	nlcd_SendByte(CMD_LCD_MODE,0xB0);
	nlcd_SendByte(CMD_LCD_MODE,0x10); // X = 0
	nlcd_SendByte(CMD_LCD_MODE,0x00);
	//nlcd_SendByte(CMD_LCD_MODE,0xAE); // disable display;

	for(unsigned int i=0;i<864;i++) nlcd_SendByte(DATA_LCD_MODE,0x00);

	//nlcd_SendByte(CMD_LCD_MODE,0x07);
	//nlcd_SendByte(CMD_LCD_MODE,0xAF); // enable display;
}


//******************************************************************************
// Передача байта (команды или данных) на LCD-контроллер
//  mode: CMD_LCD_MODE - передаем команду
//		  DATA_LCD_MODE - передаем данные
//  c: значение передаваемого байта
void nlcd_SendByte(char mode,unsigned char c)
{
    CS_LCD_RESET;
    SCLK_LCD_RESET;

    if(mode) SDA_LCD_SET;
	 else	 SDA_LCD_RESET;

    SCLK_LCD_SET;

    for(unsigned char i=0;i<8;i++)
    {
    	SCLK_LCD_RESET;

        if(c & 0x80) SDA_LCD_SET;
         else	     SDA_LCD_RESET;

        SCLK_LCD_SET;
        c <<= 1;
 //       _delay_ms(3);             // из версии без графики
//		_delay_us(NLCD_MIN_DELAY);	// *****!!!!! 34 - Минимальная задержка, при которой работает мой LCD-контроллер
		_delay_us(34);	// из версии с графикой
     }

    CS_LCD_SET;
}

//******************************************************************************
// Вывод символа на LCD-контроллер в текущее место
//  c: код символа
void nlcd_Putc(unsigned char c)
{
	for ( unsigned char i = 0; i < 5; i++ )
	{
    	nlcd_SendByte(DATA_LCD_MODE,pgm_read_byte(&(nlcd_Font[c-32][i])));
    }

	nlcd_SendByte(DATA_LCD_MODE,0x00); // Зазор между символами по горизонтали в 1 пиксель
}

//******************************************************************************
// Вывод строки символов на LCD-контроллер в текущее место. Если строка выходит
// за экран в текущей строке, то остаток переносится на следующую строку.
//  message: указатель на строку символов. 0x00 - признак конца строки.
void nlcd_Print(char * message)
{
	while (*message) // Конец строки обозначен нулем
	{
    	nlcd_Putc(*message++);
    }
}

//******************************************************************************
// Вывод строки символов на LCD-контроллер в текущее место из программной памяти.
// Если строка выходит за экран в текущей строке, то остаток переносится на следующую строку.
//  message: указатель на строку символов в программной памяти. 0x00 - признак конца строки.
void nlcd_PrintF(unsigned char * message)
{
	unsigned char data;
	while (data=pgm_read_byte(message), data)
	{
    	nlcd_Putc(data);
		message++;
    }
}

#ifdef WIDE_CHAR


//******************************************************************************
// Вывод широкого символа на LCD-экран NOKIA 1100 в текущее место
//  c: код символа
void nlcd_PutcWide(unsigned char c)
{
	if (c>127) c=c-64; 	// Переносим символы кирилицы в кодировке CP1251 в начало второй
						// половины таблицы ASCII (начиная с кода 0x80)

	for ( unsigned char i = 0; i < 5; i++ )
	{
		unsigned char glyph = pgm_read_byte(&(nlcd_Font[c-32][i]));
	   	nlcd_SendByte(DATA_LCD_MODE,glyph);
	   	nlcd_SendByte(DATA_LCD_MODE,glyph);
    }

	nlcd_SendByte(DATA_LCD_MODE,0x00); // Зазор между символами по горизонтали в 1 пиксель
//	nlcd_SendByte(DATA_LCD_MODE,0x00); // Можно сделать две линии
}


//******************************************************************************
// Вывод строки символов двойной ширины на LCD-экран NOKIA 1100 в текущее место
// из оперативной памяти. Если строка выходит за экран в текущей строке, то остаток
// переносится на следующую строку.
//  message: указатель на строку символов в программной памяти. 0x00 - признак конца строки.
void nlcd_PrintWide(unsigned char * message)
{
	while (*message) nlcd_PutcWide(*message++);  // Конец строки обозначен нулем
}

#endif // WIDE_CHAR

//******************************************************************************
// Устанавливает курсор в необходимое положение. Отсчет начинается в верхнем
// левом углу. По горизонтали 16 знакомест, по вертикали - 8
//  x: 0..15
//  y: 0..7
void nlcd_GotoXY(char x,char y)
{
	x=x*6;	// Переходим от координаты в пикселях к координатам в знакоместах
	nlcd_SendByte(CMD_LCD_MODE,(0xB0|(y&0x0F)));      // установка адреса по Y: 0100 yyyy
    nlcd_SendByte(CMD_LCD_MODE,(0x00|(x&0x0F)));      // установка адреса по X: 0000 xxxx - биты (x3 x2 x1 x0)
    nlcd_SendByte(CMD_LCD_MODE,(0x10|((x>>4)&0x07))); // установка адреса по X: 0010 0xxx - биты (x6 x5 x4)

}

//******************************************************************************
// Устанавливаер режим инверсии всего экрана. Данные на экране не изменяются, только инвертируются
//  mode: INV_MODE_ON или INV_MODE_OFF
void nlcd_Inverse(unsigned char mode)
{
	if (mode) nlcd_SendByte(CMD_LCD_MODE,0xA6);
	 else nlcd_SendByte(CMD_LCD_MODE,0xA7);
}

