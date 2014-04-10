#include "usart.h"


//*

void USART0_write(unsigned char data){
	while ( !( UCSRA & (1<<UDRE)) ) ;
	UDR = data;
}


void USART_Print(const char *USART_String) {
 uint8_t c;
 while ((c=*USART_String++))
 {
 USART0_write(c);
 }
 }//USART_Print
