#include "ds18x20.h"
#include "onewire.h"
#include <stdio.h>
#include "../HAL.h"



void readTemperature();
void print_address(unsigned char* address);
unsigned char search_ow_devices(void);
void usartDebug();
void usartPrintOnewire();
uint8_t current_temp=115;
unsigned char	nDevices;	// количество сенсоров
unsigned char	owDevicesIDs[MAXDEVICES][8];	// Их ID
extern void USART0_write();
extern void timerDelayMs();
void temperatureRead();
void temperatureStartConvert();
