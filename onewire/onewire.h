#ifndef ONEWIRE_H
#define ONEWIRE_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../macros.h"

#define true 1
#define false 0

// Максимальное количество устройств на шине
#define MAXDEVICES 6

// Если для эмуляции шины используется USART
//#define UART_AS_OneWire

// Если для эмуляции 1-wire не спольльзуется USART, но используется 2 пина (вход и выход)
//#define OW_TWO_PINS

#ifdef UART_AS_OneWire
	#define USART_BAUDRATE_57600 (((F_CPU / (57600 * 16UL))) - 1)
	#define USART_BAUDRATE_115200 (((F_CPU / (115200 * 16UL))) - 1)
	#define USART_BAUDRATE_9600 (((F_CPU / (9600 * 16UL))) - 1)
#else
	#include <util/delay.h>
	#define OW_DDR DDRD
	#define OW_PORT PORTD
	#define OW_PIN PIND
	#ifndef OW_TWO_PINS //если используется один пин, укажите его номер
		#define OW_BIT 2
//		    sbi(OW_DDR,OW_BIT);
//          sbi(OW_PORT,OW_BIT);
	#else // если используются 2 пина, укажите их номера
		#define OW_BIT_OUT 3
		#define OW_BIT_IN 2
//        DDRD|=(1<<3);
//        OW_PORT|=(1<<3);
//    sbi(OW_PORT,OW_BIT_OUT);
//    cbi(OW_DDR,OW_BIT_IN);
//    cbi(OW_PORT,OW_BIT_IN);
	#endif
#endif

#define OW_CMD_SEARCHROM	0xF0
#define OW_CMD_READROM		0x33
#define OW_CMD_MATCHROM		0x55
#define OW_CMD_SKIPROM		0xCC

#define	OW_SEARCH_FIRST	0xFF		// start new search
#define	OW_PRESENCE_ERR	0xFF
#define	OW_DATA_ERR	    0xFE
#define OW_LAST_DEVICE	0x00		// last device found
//			0x01 ... 0x40: continue searching

#define OW_DS1990_FAMILY_CODE	1
#define OW_DS2405_FAMILY_CODE	5
#define OW_DS2413_FAMILY_CODE	0x3A
#define OW_DS1822_FAMILY_CODE	0x22
#define OW_DS2430_FAMILY_CODE	0x14
#define OW_DS1990_FAMILY_CODE	1
#define OW_DS2431_FAMILY_CODE	0x2d
#define OW_DS18S20_FAMILY_CODE	0x10
#define OW_DS18B20_FAMILY_CODE	0x28
#define OW_DS2433_FAMILY_CODE	0x23

// rom-code size including CRC
#define OW_ROMCODE_SIZE	8

unsigned char OW_Reset(void);
void OW_WriteBit(unsigned char bit);
unsigned char OW_ReadBit(void);
#ifndef UART_AS_OneWire
	unsigned char OW_ReadByte(void);
	void OW_WriteByte(unsigned char byte);
#else
	unsigned char OW_WriteByte(unsigned char byte);
	#define OW_ReadByte() OW_WriteByte(0xFF)
#endif
unsigned char OW_SearchROM( unsigned char diff, unsigned char *id );
void OW_FindROM(unsigned char *diff, unsigned char id[]);
unsigned char OW_ReadROM(unsigned char *buffer);
unsigned char OW_MatchROM(unsigned char *rom);

#endif
