#ifndef MACROS_
#define MACROS_

//#define TCNT0_const 0xd9 //217(0xEC,236 дл€ 400√ц) //5 мс дл€ 8 бит таймера на 8ћгц 200 √ц, 2,5мс на 16ћгц 400√ц
//#define TCNTCC 57724 //0xe17c 1√ц 1 сек на 16 бит таймер на 8 ћгц
//*
#define SB(port,bit) (PORT##port|=(1<<bit))		//setbit (port,bit) приклеим это слово порт куда надо и будет проще
#define CB(port,bit) (PORT##port&= ~(1<<bit))   //clearbit а лучше конечно написать порт и бит шоб пон€тнее было
#define TB(port,bit) (PORT##port^=(1<<bit))		//togglebit и это , терь не надо писать PORTC  терь только C и все
#define CH(port,bit) (PIN##port&(1<<bit))		//checkbit CH(B,3) так терь оно будет гл€детьс€





#pragma once
#include <stdbool.h>
//*
typedef struct { //булеановы переменные , зачем тратить на каждый флаг по байту ?
  bool State_Automate:1;    //обработчик
  bool KeyPressed:1;        //флаг нажати€ клавиши
  bool KeyReleased:1;       //флаг отпускани€ клавиши
  bool KeyPushLong:1;       //флаг долгово нажати€ клавиши
  bool KeyState:1;          //флаг состо€ни€ клавиши
  uint8_t  KeyPin:4;        //здесь код клавиши
  bool Fire:1;            //флаг горелки
  bool Furnace:1;         //флаг циркул€рной печи
  bool SupplyManual:1;
  bool SupplyAuto:1;          //флаг подачи
  bool ADC_Channel:1;      // канал ацп
  bool NextState:1;
  bool RunFlag:1;
 } PackedBool;
PackedBool flags;
int *KeyCodePointer;
//*/
#endif // MACROS_


