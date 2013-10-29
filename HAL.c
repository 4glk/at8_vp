#include "HAL.h"


// atmega328p
#define ATMEGA328


// инициализация для atmega8
#ifdef ATMEGA8
/*
ISR(TIMER2_OVF_vect){
    TCNT2 = StartFrom;

//    j++;
    KeyScan();          //загнать эти функции в отдельный блок , исполняемый по
    SwitchMenu();       // флагу переполнения TOV2
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


#endif // ATMEGA8

// инициализация для atmega328
#ifdef ATMEGA328

#endif // ATMEGA328
