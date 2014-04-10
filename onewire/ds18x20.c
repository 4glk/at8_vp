#include "ds18x20.h"

char DS18x20_StartMeasure(void)
{
	//Reset, skip ROM and start temperature conversion
	if (!OW_Reset()) return 0;
	OW_WriteByte(OW_CMD_SKIPROM);
	OW_WriteByte(THERM_CMD_CONVERTTEMP);
	return 1;
}

char DS18x20_StartMeasureAddressed(unsigned char* rom)
{
	//Reset and start temperature conversion
	if (!OW_Reset()) return 0;
	OW_MatchROM(rom);
	OW_WriteByte(THERM_CMD_CONVERTTEMP);
	return 1;
}

#ifdef DS18X20_CHECK_CRC
#define CRC8INIT	0x00
#define CRC8POLY	0x18              //0X18 = X^8+X^5+X^4+X^0

unsigned char crc8(unsigned char *data_in, unsigned int number_of_bytes_to_read )
{
	unsigned char	crc;
	unsigned int	loop_count;
	unsigned char	bit_counter;
	unsigned char	data;
	unsigned char	feedback_bit;

	crc = CRC8INIT;
	
	for (loop_count = 0; loop_count != number_of_bytes_to_read; loop_count++)
	{ 
		data = data_in[loop_count];

		bit_counter = 8;
		do { 
			feedback_bit = (crc ^ data) & 0x01;
			if (feedback_bit==0x01) crc = crc ^ CRC8POLY;

			crc = (crc >> 1) & 0x7F;
			if (feedback_bit==0x01) crc = crc | 0x80;

			data = data >> 1;
			bit_counter--;
		}
		while (bit_counter > 0);
	}
	return crc;
}
#endif 

char DS18x20_ReadData(unsigned char *rom, unsigned char *buffer)
{
	//Reset, skip ROM and send command to read Scratchpad
	if (!OW_Reset()) return 0;
	if (rom[0]==0)	OW_WriteByte(OW_CMD_SKIPROM);
	else OW_MatchROM(rom);
	OW_WriteByte(THERM_CMD_RSCRATCHPAD);
	
#ifdef DS18X20_CHECK_CRC
	unsigned char	buff[9] = {1,2,3,4,5,6,7,8,9};
	for (unsigned char i=0; i<9; i++) buff[i] = OW_ReadByte();
	buffer[0] = buff[0]; buffer[1] = buff[1];
	if (crc8(buff, 9)) return 0;	// если контрольная сумма не совпала, возвращаем ошибку
#else 
	//Read Scratchpad (only 2 first bytes)
	buffer[0] = OW_ReadByte(); // Read TL
	buffer[1] = OW_ReadByte(); // Read TH	
#endif

	return 1;
}

void DS18x20_ConvertToThemperature(unsigned char* data, unsigned char* themp)
{
	//Store temperature integer digits and decimal digits
	themp[1] = data[0]>>4;
	themp[1] |= (data[1]&0x07)<<4;
	//Store decimal digits
	themp[2] = data[0]&0xf;
	themp[2] *= 5;	
	if (data[1]>0xFB){
		themp[1] = 127-themp[1];
		themp[0] = '-';
	} 
	else if((data[0]==0x00)&&(data[1]==0x00)) themp[0] = ' '; else themp[0] = '+';	
}

