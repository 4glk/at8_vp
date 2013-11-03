#include "usart.h"


//*

void USART0_write(unsigned char data){
	while ( !( UCSRA & (1<<UDRE)) ) ;
	UDR = data;
}

//    FILE usart_str = FDEV_SETUP_STREAM(USART0_write, NULL, _FDEV_SETUP_WRITE); // для функции printf
