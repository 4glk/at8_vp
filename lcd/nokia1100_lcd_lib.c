//***************************************************************************
//  File........: nokia1100_lcd_lib.c
//  Author(s)...: Chiper
//  URL(s)......: http://digitalchip.ru/
//  Device(s)...: ATMega...
//  Compiler....: AVR-GCC
//  Description.: ������� LCD-����������� �� Nokia1100
//  Data........: 21.02.12
//  Version.....: 1.0
//***************************************************************************
//  Notice: ��� ����������� �������� LCD-����������� ������ ���� ���������� �
//  ������ � ���� �� ����� �� ����������������
//***************************************************************************

#include "nokia1100_lcd_lib.h"
#include "nokia1100_lcd_fnt.h"	// ���������� ����� (����� �������� � ����������� ������)

//******************************************************************************
// ������������� �����������
void nlcd_Init(void)
{
	// �������������� ���� �� ����� ��� ������ � LCD-������������
	DDR_LCD |= (1<<SCLK_LCD_PIN)|(1<<SDA_LCD_PIN)|(1<<CS_LCD_PIN)|(1<<RST_LCD_PIN);

	CS_LCD_RESET;
	RST_LCD_RESET;

	_delay_ms(10);            // ������� �� ����� 5�� ��� ��������� ����������(����� 5 �� ����� ����������)

	RST_LCD_SET;

	nlcd_SendByte(CMD_LCD_MODE,0x20); // ������ � ������� Vop
	nlcd_SendByte(CMD_LCD_MODE,0x90);
	nlcd_SendByte(CMD_LCD_MODE,0xA4); // all on/normal display
	nlcd_SendByte(CMD_LCD_MODE,0x2F); // Power control set(charge pump on/off)
	nlcd_SendByte(CMD_LCD_MODE,0x40); // set start row address = 0
	nlcd_SendByte(CMD_LCD_MODE,0xB0); // ���������� Y-����� = 0
	nlcd_SendByte(CMD_LCD_MODE,0x10); // ���������� X-�����, ������� 3 ����
	nlcd_SendByte(CMD_LCD_MODE,0x0);  // ���������� X-�����, ������� 4 ����
	nlcd_SendByte(CMD_LCD_MODE,0xC8); // mirror Y axis (about X axis)
	//nlcd_SendByte(CMD_LCD_MODE,0xA1); // ������������� ����� �� �����������
	nlcd_SendByte(CMD_LCD_MODE,0xAC); // set initial row (R0) of the display
	nlcd_SendByte(CMD_LCD_MODE,0x07);
	//nlcd_SendByte(CMD_LCD_MODE,0xF9); //
	nlcd_SendByte(CMD_LCD_MODE,0xAF); // ����� ���/����

	nlcd_Clear(); // clear LCD

	nlcd_SendByte(CMD_LCD_MODE,0xA7); 	// ��������������� �����
	_delay_ms(500);                		// 1/2 Sec delay
	nlcd_SendByte(CMD_LCD_MODE,0xA6); 	// ���������� ����� (�� ���������������)
	_delay_ms(1000);               		// 1 Sec delay
}

//******************************************************************************
// ������� ������
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
// �������� ����� (������� ��� ������) �� LCD-����������
//  mode: CMD_LCD_MODE - �������� �������
//		  DATA_LCD_MODE - �������� ������
//  c: �������� ������������� �����
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
 //       _delay_ms(3);             // �� ������ ��� �������
//		_delay_us(NLCD_MIN_DELAY);	// *****!!!!! 34 - ����������� ��������, ��� ������� �������� ��� LCD-����������
		_delay_us(34);	// �� ������ � ��������
     }

    CS_LCD_SET;
}

//******************************************************************************
// ����� ������� �� LCD-���������� � ������� �����
//  c: ��� �������
void nlcd_Putc(unsigned char c)
{
	for ( unsigned char i = 0; i < 5; i++ )
	{
    	nlcd_SendByte(DATA_LCD_MODE,pgm_read_byte(&(nlcd_Font[c-32][i])));
    }

	nlcd_SendByte(DATA_LCD_MODE,0x00); // ����� ����� ��������� �� ����������� � 1 �������
}

//******************************************************************************
// ����� ������ �������� �� LCD-���������� � ������� �����. ���� ������ �������
// �� ����� � ������� ������, �� ������� ����������� �� ��������� ������.
//  message: ��������� �� ������ ��������. 0x00 - ������� ����� ������.
void nlcd_Print(char * message)
{
	while (*message) // ����� ������ ��������� �����
	{
    	nlcd_Putc(*message++);
    }
}

//******************************************************************************
// ����� ������ �������� �� LCD-���������� � ������� ����� �� ����������� ������.
// ���� ������ ������� �� ����� � ������� ������, �� ������� ����������� �� ��������� ������.
//  message: ��������� �� ������ �������� � ����������� ������. 0x00 - ������� ����� ������.
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
// ����� �������� ������� �� LCD-����� NOKIA 1100 � ������� �����
//  c: ��� �������
void nlcd_PutcWide(unsigned char c)
{
	if (c>127) c=c-64; 	// ��������� ������� �������� � ��������� CP1251 � ������ ������
						// �������� ������� ASCII (������� � ���� 0x80)

	for ( unsigned char i = 0; i < 5; i++ )
	{
		unsigned char glyph = pgm_read_byte(&(nlcd_Font[c-32][i]));
	   	nlcd_SendByte(DATA_LCD_MODE,glyph);
	   	nlcd_SendByte(DATA_LCD_MODE,glyph);
    }

	nlcd_SendByte(DATA_LCD_MODE,0x00); // ����� ����� ��������� �� ����������� � 1 �������
//	nlcd_SendByte(DATA_LCD_MODE,0x00); // ����� ������� ��� �����
}


//******************************************************************************
// ����� ������ �������� ������� ������ �� LCD-����� NOKIA 1100 � ������� �����
// �� ����������� ������. ���� ������ ������� �� ����� � ������� ������, �� �������
// ����������� �� ��������� ������.
//  message: ��������� �� ������ �������� � ����������� ������. 0x00 - ������� ����� ������.
void nlcd_PrintWide(unsigned char * message)
{
	while (*message) nlcd_PutcWide(*message++);  // ����� ������ ��������� �����
}

#endif // WIDE_CHAR

//******************************************************************************
// ������������� ������ � ����������� ���������. ������ ���������� � �������
// ����� ����. �� ����������� 16 ���������, �� ��������� - 8
//  x: 0..15
//  y: 0..7
void nlcd_GotoXY(char x,char y)
{
	x=x*6;	// ��������� �� ���������� � �������� � ����������� � �����������
	nlcd_SendByte(CMD_LCD_MODE,(0xB0|(y&0x0F)));      // ��������� ������ �� Y: 0100 yyyy
    nlcd_SendByte(CMD_LCD_MODE,(0x00|(x&0x0F)));      // ��������� ������ �� X: 0000 xxxx - ���� (x3 x2 x1 x0)
    nlcd_SendByte(CMD_LCD_MODE,(0x10|((x>>4)&0x07))); // ��������� ������ �� X: 0010 0xxx - ���� (x6 x5 x4)

}

//******************************************************************************
// ������������� ����� �������� ����� ������. ������ �� ������ �� ����������, ������ �������������
//  mode: INV_MODE_ON ��� INV_MODE_OFF
void nlcd_Inverse(unsigned char mode)
{
	if (mode) nlcd_SendByte(CMD_LCD_MODE,0xA6);
	 else nlcd_SendByte(CMD_LCD_MODE,0xA7);
}

