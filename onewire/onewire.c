#include "onewire.h"

#ifdef UART_AS_OneWire
	#include <avr/interrupt.h>
#endif
/*
#define sbi(reg,bit) reg |= (1<<bit)
#define cbi(reg,bit) reg &= ~(1<<bit)
#define ibi(reg,bit) reg ^= (1<<bit)
#define CheckBit(reg,bit) (reg&(1<<bit))
//*/
//extern void RunTasks(unsigned char tasks);

void OthersTasks(void){
//	RunTasks(0xFF);
}

#ifndef UART_AS_OneWire
void OW_Set(unsigned char mode)
{
//    USART0_write('3');
#ifndef OW_TWO_PINS
	if (mode) {
		cbi(OW_PORT, OW_BIT);
		sbi(OW_DDR, OW_BIT);
	//	USART0_write('2');
	}
	else {
		cbi(OW_PORT, OW_BIT);
		cbi(OW_DDR, OW_BIT);
	//	USART0_write('1');
	}
#else
	if (mode) cbi(OW_PORT, OW_BIT_OUT);
	else sbi(OW_PORT, OW_BIT_OUT);
#endif
}

unsigned char OW_CheckIn(void)
{
#ifndef OW_TWO_PINS
	return CheckBit(OW_PIN, OW_BIT);
#else
	return CheckBit(OW_PIN, OW_BIT_IN);
#endif
}

#endif

unsigned char OW_Reset(void)
{
#ifdef UART_AS_OneWire
	UCSR0B=(1<<RXEN0)|(1<<TXEN0);
	//9600
	UBRR0L = USART_BAUDRATE_9600;
	UBRR0H = (USART_BAUDRATE_9600 >> 8);
	UCSR0A &= ~(1<<U2X0);

	while(CheckBit(UCSR0A, RXC0)) UDR0; //Зачистка буферов
	cli();
	UDR0 = 0xF0;
	UCSR0A = (1<<TXC0);
	sei();
	//while(!CheckBit(UCSRA, TXC)) OthersTasks();
	while(!CheckBit(UCSR0A, RXC0)) OthersTasks();
	if (UDR0 != 0xF0) return 1;
 return 0;
#else
	unsigned char	status;
	OW_Set(1);
	_delay_us(480);
	OW_Set(0);
	_delay_us(60);
	//Store line value and wait until the completion of 480uS period
	status = OW_CheckIn();
	_delay_us(420);
	//Return the value read from the presence pulse (0=OK, 1=WRONG)
 return !status;
#endif
//	return 1 if found
//	return 0 if not found
}

void OW_WriteBit(unsigned char bit)
{
#ifdef UART_AS_OneWire
	//115200
	UBRR0L = USART_BAUDRATE_115200;
	UBRR0H = (USART_BAUDRATE_115200 >> 8);
	UCSR0A |= (1<<U2X0);

	unsigned char	d = 0x00;
	while(CheckBit(UCSR0A, RXC0)) UDR0; //Зачистка буферов
	if (bit) d = 0xFF;
	cli();
	UDR0 = d;
	UCSR0A=(1<<TXC0);
	sei();
	while(!CheckBit(UCSR0A,TXC0));
	while(CheckBit(UCSR0A, RXC0)) UDR0; //Зачистка буферов
#else
	//Pull line low for 1uS
	OW_Set(1);
	_delay_us(1);
	//If we want to write 1, release the line (if not will keep low)
	if(bit) OW_Set(0);
	//Wait for 60uS and release the line
	_delay_us(60);
	OW_Set(0);
#endif
}

unsigned char OW_ReadBit(void)
{
#ifdef UART_AS_OneWire
	//115200
	UBRR0L = USART_BAUDRATE_115200;
	UBRR0H = (USART_BAUDRATE_115200 >> 8);
	UCSR0A |= (1<<U2X0);

	unsigned char	c;
	while(CheckBit(UCSR0A, RXC0)) UDR0; //Зачистка буферов
	cli();
	UDR0 = 0xFF;
	UCSR0A=(1<<TXC0);
	sei();
	while(!CheckBit(UCSR0A, TXC0));
	while(!CheckBit(UCSR0A, RXC0));
	c = UDR0;
	if (c>0xFE) return 1;
	return 0;
#else
	unsigned char	bit=0;
	//Pull line low for 1uS
	OW_Set(1);
	_delay_us(1);
	//Release line and wait for 14uS
	OW_Set(0);
	_delay_us(14);
	//Read line value
	if(OW_CheckIn()) bit=1;
	//Wait for 45uS to end and return read value
	_delay_us(45);
	return bit;
#endif
}

#ifdef UART_AS_OneWire
unsigned char OW_WriteByte(unsigned char byte)
{
	unsigned char	i = 8;
	//115200
	UBRR0L = USART_BAUDRATE_115200;
	UBRR0H = (USART_BAUDRATE_115200 >> 8);
	UCSR0A |= (1<<U2X0);

	do
	{
		unsigned char	d = 0x00;
		if (byte&1) d = 0xFF;
		cli();
		UDR0 = d;
		UCSR0A=(1<<TXC0);
		sei();
		OthersTasks();
		while(!CheckBit(UCSR0A,RXC0)) OthersTasks();
		byte>>=1;
		if (UDR0>0xFE) byte|=128;
	}
	while(--i);

	return byte&255;
}
#else
void OW_WriteByte(unsigned char byte)
{
	for (unsigned char i=0; i<8; i++) OW_WriteBit(CheckBit(byte, i));
}

unsigned char OW_ReadByte(void)
{
	unsigned char n=0;

	for (unsigned char i=0; i<8; i++) if (OW_ReadBit()) sbi(n, i);

	return n;
}
#endif

unsigned char OW_SearchROM( unsigned char diff, unsigned char *id )
{
	unsigned char i, j, next_diff;
	unsigned char b;

	if(!OW_Reset())
		return OW_PRESENCE_ERR;       // error, no device found

	OW_WriteByte(OW_CMD_SEARCHROM);     // ROM search command
	next_diff = OW_LAST_DEVICE;      // unchanged on last device

	i = OW_ROMCODE_SIZE * 8;         // 8 bytes
	do
	{
		j = 8;                        // 8 bits
		do
		{
			b = OW_ReadBit();			// read bit
			if( OW_ReadBit() )
			{ // read complement bit
				if( b )                 // 11
				return OW_DATA_ERR;  // data error
			}
			else
			{
				if( !b ) { // 00 = 2 devices
				if( diff > i || ((*id & 1) && diff != i) ) {
						b = 1;               // now 1
						next_diff = i;       // next pass 0
					}
				}
			}
         OW_WriteBit( b );               // write bit
         *id >>= 1;
         if( b ) *id |= 0x80;			// store bit
         i--;
		}
		while( --j );
		id++;                            // next byte
    }
	while( i );
	return next_diff;                  // to continue search
}

void OW_FindROM(unsigned char *diff, unsigned char id[])
{
	while(1)
    {
		*diff = OW_SearchROM( *diff, &id[0] );
    	if ( *diff==OW_PRESENCE_ERR || *diff==OW_DATA_ERR ||
    		*diff == OW_LAST_DEVICE ) return;
    	//if ( id[0] == DS18B20_ID || id[0] == DS18S20_ID )
		return;
    }
}

unsigned char OW_ReadROM(unsigned char *buffer)
{
	if (!OW_Reset()) return 0;
	OW_WriteByte(OW_CMD_READROM);
	for (unsigned char i=0; i<8; i++)
	{
		buffer[i] = OW_ReadByte();
	}
 return 1;
}

unsigned char OW_MatchROM(unsigned char *rom)
{
 	if (!OW_Reset()) return 0;
	OW_WriteByte(OW_CMD_MATCHROM);
	for(unsigned char i=0; i<8; i++)
	{
		OW_WriteByte(rom[i]);
	}
 return 1;
}
