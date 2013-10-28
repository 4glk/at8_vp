//***************************************************************************
//  File........: nokia1100_lcd_lib.h
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
#ifndef _NOKIA1100_LCD_LIB_H_
#define _NOKIA1100_LCD_LIB_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

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

// Применить широкие символы. функции из второй версии с графикой
//#define WIDE_CHAR

//******************************************************************************
// Макросы и определения

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

#define INV_MODE_ON		0
#define INV_MODE_OFF	1

//******************************************************************************
// Прототипы функций

void nlcd_Init(void);
void nlcd_Clear(void);
void nlcd_SendByte(char mode,unsigned char c);
void nlcd_Putc(unsigned char c);
void nlcd_Print(char * message);
void nlcd_PrintF(unsigned char * message);

#ifdef WIDE_CHAR
void nlcd_PutcWide(unsigned char c);
void nlcd_PrintWide(unsigned char * message);
#endif // WIDE_CHAR

void nlcd_GotoXY(char x,char y);
void nlcd_Inverse(unsigned char mode);

#endif /* _NOKIA1100_LCD_LIB_H_ */
