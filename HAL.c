#include "HAL.h"

void InitScheduler (void);

void timerDelayInit(void);
void timerDelayMs(unsigned long int ms);


// atmega328p
#define ATMEGA328


// инициализаци€ дл€ atmega8
#ifdef ATMEGA8
/*
ISR(TIMER2_OVF_vect){
    TCNT2 = StartFrom;

//    j++;
    KeyScan();          //загнать эти функции в отдельный блок , исполн€емый по
    SwitchMenu();       // флагу переполнени€ TOV2
//       TIFR = 1<<TOV2;
 //   TB(C,4);
}
//*/

ISR(TIMER2_OVF_vect){
    timer2++;
   TCNT2 = StartFrom;
   if (flags.RunFlag==0)delay_time--;
   if (delay_time==0)flags.RunFlag=1;
}

void InitScheduler (void){
   u8 i;
   TCCR2 |= (1<<CS02)|(0<<CS01)|(0<<CS00);   // устанавливаем прескалер - 1024(101) 256(100) 64(011) 8(010) 0(001) off(000)
   TIFR = 1<<TOV0;   // очищаем флаг прерывани€ таймера “0
   TIMSK |= 1<<TOIE2;   // разрешаем прерывание по переполнению
   TCNT2 = StartFrom;    // загружаем начальное зн. в счетный регистр
   for (i=0; i<MAXnTASKS; i++) DeleteTask(i);   // очищаем массив задач
}

void USART_init()
{
	// Set baud rate
	UBRRH = 0;
	UBRRL = 51;
	UCSRA = 0;
	// Enable receiver and transmitter
	UCSRB = (1<<TXEN);
	// Set frame format
	UCSRC = (1<<UCSZ1) | (1<<UCSZ0) | (1<<URSEL);
}

void timerDelayInit(void){
    #if (T_PRESCALER==8)
        TCCR0 = (1<<CS01); // prescaler = 8
    #elif (T_PRESCALER==64)
        TCCR0 = (1<<CS01) | (1<<CS00); // prescaler = 64
    #elif (T_PRESCALER==256)
        TCCR0 = (1<<CS02); // prescaler = 256
    #else
        #warning T_PRESCALER not defined
    #endif
}

void timerDelayMs(unsigned long int ms)
{
	while(ms--){
		TCNT0 = 0;
		while(TCNT0 < TIKS_1MS) {
			#ifdef RUN_TASKS
				RunTasks();
			#endif
		}
	}
}

#endif // ATMEGA8

// инициализаци€ дл€ atmega328
#ifdef ATMEGA328
ISR(TIMER2_OVF_vect){
//    timer2++;
   TCNT2 = StartFrom;
   if (flags.RunFlag==0)delay_time--;
   if (delay_time==0)flags.RunFlag=1;
}

void InitScheduler (void){
   u8 i;
   TCCR2A |= (1<<CS02)|(0<<CS01)|(0<<CS00);   // устанавливаем прескалер - 1024(101) 256(100) 64(011) 8(010) 0(001) off(000)
   TIFR2 = 1<<TOV0;   // очищаем флаг прерывани€ таймера “0
   TIMSK2 |= 1<<TOIE2;   // разрешаем прерывание по переполнению
   TCNT2 = StartFrom;    // загружаем начальное зн. в счетный регистр
   for (i=0; i<MAXnTASKS; i++) DeleteTask(i);   // очищаем массив задач
}

void USART_init()
{
	// Set baud rate
	UBRR0H = 0;
	UBRR0L = 51;
	UCSR0A = 0;
	// Enable receiver and transmitter
	UCSR0B = (1<<TXEN0);
	// Set frame format
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);// | (1<<URSEL);
}

void timerDelayInit(void){
    #if (T_PRESCALER==8)
        TCCR0B = (1<<CS01); // prescaler = 8
    #elif (T_PRESCALER==64)
        TCCR0B = (1<<CS01) | (1<<CS00); // prescaler = 64
    #elif (T_PRESCALER==256)
        TCCR0B = (1<<CS02); // prescaler = 256
    #else
        #warning T_PRESCALER not defined
    #endif
}

void timerDelayMs(unsigned long int ms)
{
	while(ms--){
		TCNT0 = 0;
		while(TCNT0 < TIKS_1MS) {
			#ifdef RUN_TASKS
				RunTasks();
			#endif
		}
	}
}


#endif // ATMEGA328
