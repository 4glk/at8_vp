//#include <avr/io.h>
//#include <util/delay.h>
//#include <avr/pgmspace.h>
#include <avr/interrupt.h>
//#include "../onewire/onewire.h"
//#include "../onewire/ds18x20.h"
//#include "../onewire/delay.h"
//void usartPrintOnewire();
//extern uint16_t current_temp;
//void usartDebug();
//void readTemperature();
//unsigned char	nDevices;	// ���������� ��������
//unsigned char	owDevicesIDs[MAXDEVICES][8];	// �� ID
void USART0_write(unsigned char data);
