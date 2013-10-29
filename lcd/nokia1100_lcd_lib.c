//***************************************************************************
//  File........: nokia1100_lcd_lib.c
//  Author(s)...: Chiper
//  URL(s)......: http://digitalchip.ru/
//  Device(s)...: ATMega...
//  Compiler....: AVR-GCC
//  Description.: Драйвер LCD-контроллера от Nokia1100 с графическими функциями
//  Data........: 28.03.12
//  Version.....: 2.1
//***************************************************************************
//  Notice: Все управляющие контакты LCD-контроллера должны быть подключены к
//  одному и тому же порту на микроконтроллере
//***************************************************************************

#include "nokia1100_lcd_lib.h"
#include "nokia1100_lcd_font.h"	// Подключаем шрифт (будет размещен в программной памяти)
#include <stdlib.h>

//******************************************************************************
// Макросы, определения, служебные переменные

// Макросы для работы с битами
#define ClearBit(reg, bit)       reg &= (~(1<<(bit)))
#define SetBit(reg, bit)         reg |= (1<<(bit))
#define InvBit(reg, bit)         reg ^= 1<<bit

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


	nlcd_SendByte(CMD_LCD_MODE,0xE2); // *** SOFTWARE RESET

	nlcd_SendByte(CMD_LCD_MODE,0x3A); // *** Use internal oscillator
	nlcd_SendByte(CMD_LCD_MODE,0xEF); // *** FRAME FREQUENCY:
	nlcd_SendByte(CMD_LCD_MODE,0x04); // *** 80Hz
	nlcd_SendByte(CMD_LCD_MODE,0xD0); // *** 1:65 divider

	nlcd_SendByte(CMD_LCD_MODE,0x20); // Запись в регистр Vop
	nlcd_SendByte(CMD_LCD_MODE,0x90);

	nlcd_SendByte(CMD_LCD_MODE,0xA4); // all on/normal display

	nlcd_SendByte(CMD_LCD_MODE,0x2F); // Power control set(charge pump on/off)
									  // Определяет контрастность

	nlcd_SendByte(CMD_LCD_MODE,0x40); // set start row address = 0
	nlcd_SendByte(CMD_LCD_MODE,0xB0); // установить Y-адрес = 0
	nlcd_SendByte(CMD_LCD_MODE,0x10); // установить X-адрес, старшие 3 бита
	nlcd_SendByte(CMD_LCD_MODE,0x0);  // установить X-адрес, младшие 4 бита

	//nlcd_SendByte(CMD_LCD_MODE,0xC8); // mirror Y axis (about X axis)
	//nlcd_SendByte(CMD_LCD_MODE,0xA1); // Инвертировать экран по горизонтали

	nlcd_SendByte(CMD_LCD_MODE,0xAC); // set initial row (R0) of the display
	nlcd_SendByte(CMD_LCD_MODE,0x07);

	//nlcd_SendByte(CMD_LCD_MODE,0xF9); //

	nlcd_SendByte(CMD_LCD_MODE,0xAF); // экран вкл/выкл

	nlcd_Clear(); // clear LCD
}

//******************************************************************************
// Очистка экрана
void nlcd_Clear(void)
{
	nlcd_SendByte(CMD_LCD_MODE,0x40); // Y = 0
	nlcd_SendByte(CMD_LCD_MODE,0xB0);
	nlcd_SendByte(CMD_LCD_MODE,0x10); // X = 0
	nlcd_SendByte(CMD_LCD_MODE,0x00);

	nlcd_xcurr=0; nlcd_ycurr=0;		  // Устанавливаем в 0 текущие координаты в видеобуфере

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

	if (mode) 				// DATA_LCD_MODE
	{
		nlcd_memory[nlcd_xcurr][nlcd_ycurr] = c;	// Записываем банные в видеобуфер

		nlcd_xcurr++;								// Обновляем координаты в видеобуфере

		if (nlcd_xcurr>95)
		{
			nlcd_xcurr = 0;
			nlcd_ycurr++;
		}

		if (nlcd_ycurr>8) nlcd_ycurr = 0;


		SDA_LCD_SET;								// Передача байта в LCD-контроллер
	}
	 else SDA_LCD_RESET;	// CMD_LCD_MODE

    SCLK_LCD_SET;

    for(unsigned char i=0;i<8;i++)
    {
    	SCLK_LCD_RESET;

        if(c & 0x80) SDA_LCD_SET;
         else	     SDA_LCD_RESET;

        SCLK_LCD_SET;
        c <<= 1;

//		_delay_us(NLCD_MIN_DELAY);	// *****!!!!! 34 - Минимальная задержка, при которой работает мой LCD-контроллер
    }

    CS_LCD_SET;
}

//******************************************************************************
// Вывод символа на LCD-экран NOKIA 1100 в текущее место
//  c: код символа
void nlcd_Putc(unsigned char c)
{
	if (c>127) c=c-64; 	// Переносим символы кирилицы в кодировке CP1251 в начало второй
						// половины таблицы ASCII (начиная с кода 0x80)

	for ( unsigned char i = 0; i < 5; i++ )
		 nlcd_SendByte(DATA_LCD_MODE,pgm_read_byte(&(nlcd_Font[c-32][i])));

	nlcd_SendByte(DATA_LCD_MODE,0x00); // Зазор между символами по горизонтали в 1 пиксель
}

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
// Вывод строки символов на LCD-экран NOKIA 1100 в текущее место. Если строка выходит
// за экран в текущей строке, то остаток переносится на следующую строку.
//  message: указатель на строку символов. 0x00 - признак конца строки.
void nlcd_Print(char * message)
{
	while (*message) nlcd_Putc(*message++); // Конец строки обозначен нулем
}

//******************************************************************************
// Вывод строки символов на LCD-экран NOKIA 1100 в текущее место из программной памяти.
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


//******************************************************************************
// Вывод строки символов двойной ширины на LCD-экран NOKIA 1100 в текущее место
// из оперативной памяти. Если строка выходит за экран в текущей строке, то остаток
// переносится на следующую строку.
//  message: указатель на строку символов в программной памяти. 0x00 - признак конца строки.
void nlcd_PrintWide(unsigned char * message)
{
	while (*message) nlcd_PutcWide(*message++);  // Конец строки обозначен нулем
}


//******************************************************************************
// Устанавливает курсор в необходимое положение. Отсчет начинается в верхнем
// левом углу. По горизонтали 16 знакомест, по вертикали - 8
//  x: 0..15
//  y: 0..7
void nlcd_GotoXY(char x,char y)
{
	x=x*6;	// Переходим от координаты в знакоместах к координатам в пикселях

	nlcd_xcurr=x;
	nlcd_ycurr=y;

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


//******************************************************************************
// Устанавливает курсор в пикселях. Отсчет начинается в верхнем
// левом углу. По горизонтали 96 пикселей, по вертикали - 65
//  x: 0..95
//  y: 0..64
void nlcd_GotoXY_pix(char x,char y)
{
	nlcd_xcurr=x;
	nlcd_ycurr=y/8;

	nlcd_SendByte(CMD_LCD_MODE,(0xB0|(nlcd_ycurr&0x0F)));      // установка адреса по Y: 0100 yyyy
    nlcd_SendByte(CMD_LCD_MODE,(0x00|(x&0x0F)));      // установка адреса по X: 0000 xxxx - биты (x3 x2 x1 x0)
    nlcd_SendByte(CMD_LCD_MODE,(0x10|((x>>4)&0x07))); // установка адреса по X: 0010 0xxx - биты (x6 x5 x4)
}


//******************************************************************************
// Вывод точки на LCD-экран NOKIA 1100
//  x: 0..95  координата по горизонтали (отсчет от верхнего левого угла)
//	y: 0..64  координата по вертикали
//	pixel_mode: PIXEL_ON  - для включения пикскела
//				PIXEL_OFF - для выключения пиксела
//				PIXEL_INV - для инверсии пиксела
void nlcd_Pixel(unsigned char x,unsigned char y, unsigned char pixel_mode)
{
	unsigned char temp;

	nlcd_GotoXY_pix(x,y);
	temp=nlcd_memory[nlcd_xcurr][nlcd_ycurr];

	switch(pixel_mode)
	{
    	case PIXEL_ON:
        	SetBit(temp, y%8);			// Включаем пиксел
			break;
    	case PIXEL_OFF:
     		ClearBit(temp, y%8);		// Выключаем пиксел
			break;
    	case PIXEL_INV:
     		InvBit(temp, y%8);			// Инвертируем пиксел
			break;
	}

	nlcd_memory[nlcd_xcurr][nlcd_ycurr] = temp; // Передаем байт в видеобуфер
	nlcd_SendByte(DATA_LCD_MODE,temp); // Передаем байт в контроллер
}

//******************************************************************************
// Вывод линии на LCD-экран NOKIA 1100
//  x1, x2: 0..95  координата по горизонтали (отсчет от верхнего левого угла)
//	y1, y2: 0..64  координата по вертикали
//	pixel_mode: PIXEL_ON  - для включения пикскела
//				PIXEL_OFF - для выключения пиксела
//				PIXEL_INV - для инверсии пиксела
void nlcd_Line (unsigned char x1,unsigned char y1, unsigned char x2,unsigned char y2, unsigned char pixel_mode)
{
	int dy, dx;
	signed char addx = 1, addy = 1;
	signed int 	P, diff;

	unsigned char i = 0;

	dx = abs((signed char)(x2 - x1));
	dy = abs((signed char)(y2 - y1));

	if(x1 > x2)	addx = -1;
	if(y1 > y2)	addy = -1;

	if(dx >= dy)
	{
		dy *= 2;
		P = dy - dx;

		diff = P - dx;

		for(; i<=dx; ++i)
		{
			nlcd_Pixel(x1, y1, pixel_mode);

			if(P < 0)
			{
				P  += dy;
				x1 += addx;
			}
			else
			{
				P  += diff;
				x1 += addx;
				y1 += addy;
			}
		}
	}
	else
	{
		dx *= 2;
		P = dx - dy;
		diff = P - dy;

		for(; i<=dy; ++i)
		{
			nlcd_Pixel(x1, y1, pixel_mode);

			if(P < 0)
			{
				P  += dx;
				y1 += addy;
			}
			else
			{
				P  += diff;
				x1 += addx;
				y1 += addy;
			}
		}
	}
}



//******************************************************************************
// Вывод окружности на LCD-экран NOKIA 1100
//  x: 0..95  координаты центра окружности (отсчет от верхнего левого угла)
//	y: 0..64  координата по вертикали
//  radius:   радиус окружности
//  fill:		FILL_OFF  - без заливки окружности
//				FILL_ON	  - с заливкой
//	pixel_mode: PIXEL_ON  - для включения пикскела
//				PIXEL_OFF - для выключения пиксела
//				PIXEL_INV - для инверсии пиксела

void nlcd_Circle(unsigned char x, unsigned char y, unsigned char radius, unsigned char fill, unsigned char pixel_mode)
{
	signed char  a, b, P;

	a = 0;
	b = radius;
	P = 1 - radius;

	do
	{
		if(fill)
		{
			nlcd_Line(x-a, y+b, x+a, y+b, pixel_mode);
			nlcd_Line(x-a, y-b, x+a, y-b, pixel_mode);
			nlcd_Line(x-b, y+a, x+b, y+a, pixel_mode);
			nlcd_Line(x-b, y-a, x+b, y-a, pixel_mode);
		}
		else
		{
			nlcd_Pixel(a+x, b+y, pixel_mode);
			nlcd_Pixel(b+x, a+y, pixel_mode);
			nlcd_Pixel(x-a, b+y, pixel_mode);
			nlcd_Pixel(x-b, a+y, pixel_mode);
			nlcd_Pixel(b+x, y-a, pixel_mode);
			nlcd_Pixel(a+x, y-b, pixel_mode);
			nlcd_Pixel(x-a, y-b, pixel_mode);
			nlcd_Pixel(x-b, y-a, pixel_mode);
		}

		if(P < 0) P += 3 + 2 * a++;
			else P += 5 + 2 * (a++ - b--);
	} while(a <= b);
}



//******************************************************************************
// Вывод окружности на LCD-экран NOKIA 1100
//  x1, x2: 0..95  координата по горизонтали (отсчет от верхнего левого угла)
//	y1, y2: 0..64  координата по вертикали
//	pixel_mode: PIXEL_ON  - для включения пикскела
//				PIXEL_OFF - для выключения пиксела
//				PIXEL_INV - для инверсии пиксела
void nlcd_Rect (unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char fill, unsigned char pixel_mode)
{
	if(fill)
	{			// С заливкой
		unsigned char  i, xmin, xmax, ymin, ymax;

		if(x1 < x2) { xmin = x1; xmax = x2; }	// Определяем минимальную и максимальную координату по X
		 else { xmin = x2; xmax = x1; }

		if(y1 < y2) { ymin = y1; ymax = y2; }	// Определяем минимальную и максимальную координату по Y
		 else { ymin = y2; ymax = y1; }

		for(; xmin <= xmax; ++xmin)
		{
			for(i=ymin; i<=ymax; ++i) nlcd_Pixel(xmin, i, pixel_mode);
		}
	}
	else		// Без заливки
	{
		nlcd_Line(x1, y1, x2, y1, pixel_mode);		// Рисуем стороны прямоуголиника
		nlcd_Line(x1, y2, x2, y2, pixel_mode);
		nlcd_Line(x1, y1+1, x1, y2-1, pixel_mode);
		nlcd_Line(x2, y1+1, x2, y2-1, pixel_mode);
	}
}


//******************************************************************************
// Вывод картинки на LCD-экран NOKIA 1100
//  x: 0..95  координата верхнего левого угла по горизонтали (отсчет от верхнего левого угла экрана)
//	y: 0..64  координата верхнего левого угла по вертикали
//  picture: указатель на массив с монохромной картинкой, первые 2 байта указывают соответственно
//			 размер картинки по горизонтали и вертикали
void nlcd_Pict  (unsigned char x, unsigned char y, unsigned char * picture)
{
	unsigned char pict_width = pgm_read_byte(&picture[0]);  // ширина спрайта в пикселах
	unsigned char pict_height = pgm_read_byte(&picture[1]); // высота спрайта в пикселах
	unsigned char pict_height_bank=pict_height / 8+((pict_height%8)>0?1:0); // высота спрайта в банках
	unsigned char y_pos_in_bank = y/8 + ((y%8)>0?1:0);		// позиция по y в банках (строках по 8 пикс.)

	int adr = 2; // индекс текущего байта в массиве с картинкой

	for (unsigned char i=0; i< pict_height_bank; i++)
	{ // проход построчно (по банкам)

		if (i<((NLCD_Y_RES/8)+1)) // не выводить картинку за пределами экрана
		{
			//позиционирование на новую строку
			nlcd_xcurr=x;
			nlcd_ycurr=y_pos_in_bank + i;

			nlcd_SendByte(CMD_LCD_MODE,(0xB0|((y_pos_in_bank+i)&0x0F))); // установка адреса по Y: 0100 yyyy
    		nlcd_SendByte(CMD_LCD_MODE,(0x00|(x&0x0F)));      // установка адреса по X: 0000 xxxx - биты (x3 x2 x1 x0)
    		nlcd_SendByte(CMD_LCD_MODE,(0x10|((x>>4)&0x07))); // установка адреса по X: 0010 0xxx - биты (x6 x5 x4)

			//вывод строки
			for ( unsigned char j = 0; j < pict_width; j++ )
			{
				if ((x+j) < NLCD_X_RES) nlcd_SendByte(DATA_LCD_MODE,pgm_read_byte(&(picture[adr]))); // не выводить картинку за пределами экрана
				adr++;
			}
		}
	}
}

