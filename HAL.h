#ifndef HAL_H_
#define HAL_H_
#include <avr/interrupt.h>
#include <avr/io.h>
#include "macros.h"
#include "./sheduler/dispatch.h"
#include "./onewire/delay.h"
// железо
// atmega8
//#define ATMEGA8
uint16_t delay_time=100;
// atmega328p
#define ATMEGA328

// Константа для таймера Т0
// 25 мс при тактовой частоте
// 8 МГц и предделителе 1024
//#define StartFrom       0x3D //для 25 мс 40Гц
#define StartFrom       0xF8 //для 1 мс 1КГц            //на большой частоте висит в убр регистре
//#define StartFrom       0xB2 //для 10 мс 100Гц        //недостаточно для индикации
// максимальное количество задач
// инициализация для 8
#ifdef ATMEGA8

#endif // ATMEGA8

// инициализация для 328
#ifdef ATMEGA328

#endif // ATMEGA328

#endif // HAL_H_
