#include <avr/io.h>
#include "../macros.h"
//#include <avr/interrupt.h>
//#include <stdbool.h>
//#include "../HAL.h"

#define LSHED

#pragma once

typedef struct task{
    void (*pfunc) (void);    // указатель на функцию

    uint16_t countdown;           // обратный отсчет
#ifndef LSHED
    uint16_t delay;                  // время задержки выполнения
    uint8_t run:1;  // флаг готовности задачи к запуску
    void (*nextfunc)(void);
    uint16_t nextdelay;              // период запуска задачи теперь это будет задержка перед запуском следующей задачи
    uint16_t numRun;              // количество запусков
#endif // LSHED
}task;                      // флаг запуска можно вынести за структуру

uint16_t delay_time;   // задержка до старта , штоб счетчик не ушел в минус
uint16_t dt;
extern void USART0_write(unsigned char data);
// максимальное количество задач
#define MAXnTASKS       8
//Константа для UART`a
//скорость обмена 9600 при частоте 8 МГц
#define UBRRvalue 0x0033


/// Массив задач ///////////
volatile task TaskArray[MAXnTASKS];

/// Прототипы фукнций ////////
//void InitScheduler (void);      //инициализация диспетчера

void DispatchTask (void);       //собсна сам диспетчер и запуск задачи по флагу
void Idle();
void DeleteTask (uint8_t index);     //удаление задачи
//TODO:нужно научить эту функцию брать указатель на функцию с любыми аргументами и без них
#ifndef LSHED
void AddTask (void (*taskfunc)(void),void (*nextfunc)(void),uint16_t taskdelay,uint16_t nextdelay,uint16_t taskruns);   //добавление задачи
void ResetTask(void (*resfunc)(void));               //обнуление состояния цикличности задачи , выполнится один раз
void UpdateScheduler(void);     //обновление диспетчера

#endif // LSHED
#ifdef LSHED
void AddTask (void (*taskfunc)(void), uint16_t taskdelay);
#endif


