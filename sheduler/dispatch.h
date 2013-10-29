#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "macros.h"

#pragma once
/// Типы данных //
typedef unsigned char u8;
typedef unsigned int u16;
typedef struct task{
   void (*pfunc) (void);    // указатель на функцию
   void (*nextfunc)(void);
   u16 delay;               // задержка перед первым запуском задачи
   u16 nextdelay;              // период запуска задачи теперь это будет задержка перед запуском следующей задачи
   u8 run;                  // флаг готовности задачи к запуску
   u16 numRun;
}task;                      // флаг запуска можно вынести за структуру

uint16_t delay_time;
        uint16_t dt;
extern uint16_t timer2;

/// Определения ///////////
// Константа для таймера Т0
// 25 мс при тактовой частоте
// 8 МГц и предделителе 1024
//#define StartFrom       0x3D //для 25 мс 40Гц
#define StartFrom       0xF8 //для 1 мс 1КГц            //на большой частоте висит в убр регистре
//#define StartFrom       0xB2 //для 10 мс 100Гц        //недостаточно для индикации
// максимальное количество задач
#define MAXnTASKS       8
//Константа для UART`a
//скорость обмена 9600 при частоте 8 МГц
#define UBRRvalue 0x0033


/// Массив задач ///////////
volatile task TaskArray[MAXnTASKS];

/// Прототипы фукнций ////////
void InitScheduler (void);      //инициализация диспетчера
void UpdateScheduler(void);     //обновление диспетчера
void DeleteTask (u8 index);     //удаление задачи
void ResetTask(void (*resfunc)(void));               //обнуление состояния цикличности задачи , выполнится один раз
//void resfunc;
//TODO:нужно научить эту функцию брать указатель на функцию с любыми аргументами и без них
void AddTask (void (*taskfunc)(void),void (*nextfunc)(void),uint16_t taskdelay,uint16_t nextdelay,uint16_t taskruns);   //добавление задачи
//void AddTask (void (*taskfunc)(void),void (*nextfunc)(void), u16 taskdelay, u16 taskperiod,u16 taskruns);
void DispatchTask (void);       //собсна сам диспетчер и запуск задачи по флагу
extern void Idle();

