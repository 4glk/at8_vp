//***************************************************************************
//  File........: nokia1100_lcd_lib.h
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
#ifndef _NOKIA1100_LCD_LIB_H_
#define _NOKIA1100_LCD_LIB_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

//******************************************************************************
//******************************************************************************
// Настройка библиотеки

// Порт, к которому подключен LCD-контроллер NOKIA 1100
#define PORT_LCD PORTC
#define PIN_LCD  PINC
#define DDR_LCD  DDRC

// Номера выводов порта, к которым подключены выводы LCD-контроллера
#define SCLK_LCD_PIN    3
#define SDA_LCD_PIN     2
#define CS_LCD_PIN      1
#define RST_LCD_PIN     0

// Применять полный набор символов.
//#define FULL_CHARSET

// Применять русские символы
//#define RUSCHAR
// Применять строчные (маленькие) буквы
//#define SMALLCHAR
// Применять широкие символы
#define WIDECHAR
// Применять графику
//#define GRAPHICS
// Применить использование изображений
//#define PICTURE
// *****!!!!! Минимальная задержка, при которой работает мой LCD-контроллер
// *****!!!!! Подбирается экспериментально под конкретный контроллер
#define NLCD_MIN_DELAY	0           //34

//******************************************************************************
//******************************************************************************



//******************************************************************************
// Макросы, определения, служебные переменные

#define SCLK_LCD_SET    PORT_LCD |= (1<<SCLK_LCD_PIN)
#define SDA_LCD_SET     PORT_LCD |= (1<<SDA_LCD_PIN)
#define CS_LCD_SET      PORT_LCD |= (1<<CS_LCD_PIN)
#define RST_LCD_SET     PORT_LCD |= (1<<RST_LCD_PIN)

#define SCLK_LCD_RESET  PORT_LCD &= ~(1<<SCLK_LCD_PIN)
#define SDA_LCD_RESET   PORT_LCD &= ~(1<<SDA_LCD_PIN)
#define CS_LCD_RESET    PORT_LCD &= ~(1<<CS_LCD_PIN)
#define RST_LCD_RESET   PORT_LCD &= ~(1<<RST_LCD_PIN)

#define CMD_LCD_MODE	0
#define DATA_LCD_MODE	1

#define PIXEL_ON	0
#define PIXEL_OFF	1
#define PIXEL_INV 	2

#define FILL_OFF	0
#define FILL_ON		1

#define INV_MODE_ON		0
#define INV_MODE_OFF	1

// Разрешение дисплея в пикселях
#define NLCD_X_RES	96		// разрешение по горизонтали
#define NLCD_Y_RES	68		// разрешение по вертикали

// Видеобуфер. Работаем через буффер, так как из контроллера NOKIA1100 читать данные нельзя, а для
// графического режима нам нужно знать содержимое видеопамяти. (9 банков по 96 байт)
static unsigned char nlcd_memory[NLCD_X_RES-1][(NLCD_Y_RES/8)+1];

// Тукущие координаты (указатели) в видеобуфере
// nlcd_xcurr - в пикселах, nlcd_ycurr- в банках (строках)
static unsigned char nlcd_xcurr, nlcd_ycurr;

//******************************************************************************
// Прототипы функций

void nlcd_Init(void);
void nlcd_Clear(void);
void nlcd_SendByte(char mode,unsigned char c);
void nlcd_Putc(unsigned char c);
void nlcd_Print(char * message);
void nlcd_PrintF(unsigned char * message);
#ifdef WIDECHAR
void nlcd_PutcWide(unsigned char c);
void nlcd_PrintWide(unsigned char * message);
#endif
void nlcd_GotoXY(char x,char y);
void nlcd_Inverse(unsigned char mode);
#ifdef GRAPHICS
void nlcd_Pixel (unsigned char x,unsigned char y, unsigned char pixel_mode);
void nlcd_Line  (unsigned char x1,unsigned char y1, unsigned char x2,unsigned char y2, unsigned char pixel_mode);
void nlcd_Circle(unsigned char x, unsigned char y, unsigned char radius, unsigned char fill, unsigned char pixel_mode);
void nlcd_Rect  (unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char fill, unsigned char pixel_mode);
#endif // GRAPHICS
#ifdef PICTURE
void nlcd_Pict  (unsigned char x, unsigned char y, unsigned char * picture);
#endif
#endif /* _NOKIA1100_LCD_LIB_H_ */
