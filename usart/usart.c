#include "usart.h"


//*

void USART0_write(unsigned char data){
	while ( !( UCSR0A & (1<<UDRE0)) ) ;
	UDR0 = data;
}

void readTemperature(){
                  //  printf("\r"); print_address(owDevicesIDs[i]); // �������� ���� �������� ������, ����� - �����
    				//printf(" - Thermometer DS18B20"); // �������� ��� ����������
//    				DS18x20_StartMeasureAddressed(owDevicesIDs[i]); // ��������� ���������
    				timerDelayMs(800); // ���� ������� 750 ��, ���� �������������� �����������
    				unsigned char	data[2]; // ���������� ��� �������� �������� � �������� ����� ������
//    				DS18x20_ReadData(owDevicesIDs[i], data); // ��������� ������
    				unsigned char	themperature[3]; // � ���� ������ ����� �������� �����������
    				DS18x20_ConvertToThemperature(data, themperature); // ��������������� ����������� � ���������������� ���
    				//printf(": %d.%d C", themperature[1],themperature[2]);
    				current_temp=themperature[1];
}

void print_address(unsigned char* address) {
	printf("%.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X", address[0],address[1],address[2],address[3],address[4],address[5],address[6],address[7]);
}

//unsigned char	nDevices;	// ���������� ��������
//unsigned char	owDevicesIDs[MAXDEVICES][8];	// �� ID

unsigned char search_ow_devices(void){ // ����� ���� ��������� �� ����
	unsigned char	i;
   	unsigned char	id[OW_ROMCODE_SIZE];
   	unsigned char	diff, sensors_count;
	sensors_count = 0;
	for( diff = OW_SEARCH_FIRST; diff != OW_LAST_DEVICE && sensors_count < MAXDEVICES ; ){
		OW_FindROM( &diff, &id[0] );
     // 	if( diff == OW_PRESENCE_ERR ) break;
      	if( diff == OW_DATA_ERR )	break;
      	for (i=0;i<OW_ROMCODE_SIZE;i++)
         	owDevicesIDs[sensors_count][i] = id[i];
		sensors_count++;

    }	USART0_write('S');
	return sensors_count;
}

void usartDebug(){
	nDevices = search_ow_devices(); // ���� ��� ����������
	USART0_write('X');
	USART0_write(nDevices);
//	printf('X', nDevices);

}//*/

/*
    void usartPrintOnewire(){
    	for (unsigned char i=0; i<nDevices; i++) // ������ �������� ���������� � ����������� ������
    	{
    		// ������ ���������� ����� �� ��� ��������� ����, ������� ���������� � ������ ����� �������
    		switch (owDevicesIDs[i][0])
    		{
    			case OW_DS18B20_FAMILY_CODE: { // ���� ������ ����������� DS18B20
    				printf("\r"); print_address(owDevicesIDs[i]); // �������� ���� �������� ������, ����� - �����
    				printf(" - Thermometer DS18B20"); // �������� ��� ����������
    				DS18x20_StartMeasureAddressed(owDevicesIDs[i]); // ��������� ���������
    				timerDelayMs(800); // ���� ������� 750 ��, ���� �������������� �����������
    				unsigned char	data[2]; // ���������� ��� �������� �������� � �������� ����� ������
    				DS18x20_ReadData(owDevicesIDs[i], data); // ��������� ������
    				unsigned char	themperature[3]; // � ���� ������ ����� �������� �����������
    				DS18x20_ConvertToThemperature(data, themperature); // ��������������� ����������� � ���������������� ���
    				printf(": %d.%d C", themperature[1],themperature[2]);
    				current_temp=themperature[1];
    			} break;
    			case OW_DS18S20_FAMILY_CODE: { // ���� ������ ����������� DS18B20
    				printf("\r"); print_address(owDevicesIDs[i]); // �������� ���� �������� ������, ����� - �����
    				printf(" - Thermometer DS18S20"); // �������� ��� ����������
    			} break;

    			case OW_DS1990_FAMILY_CODE: { // ���� ������ ����������� ���� DS1990
    				printf("\r"); print_address(owDevicesIDs[i]); // �������� ���� �������� ������, ����� - �����
    				printf(" - Serial button DS1990"); // �������� ��� ����������
    			} break;
    			case OW_DS2430_FAMILY_CODE: { // ���� ������� EEPROM
    				printf("\r"); print_address(owDevicesIDs[i]); // �������� ���� �������� ������, ����� - �����
    				printf(" - EEPROM DS2430"); // �������� ��� ����������
    			} break;
    			case OW_DS2413_FAMILY_CODE: { // ���� ������ ����
    				printf("\r"); print_address(owDevicesIDs[i]); // �������� ���� �������� ������, ����� - �����
    				printf(" - Switch 2413"); // �������� ��� ����������
    			} break;
    		}
    	}
    }
    //*/
