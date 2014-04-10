//*

#ifndef HAL_H_
#define HAL_H_

#include <avr/interrupt.h>
#include <avr/io.h>
#include "macros.h"
#include "./sheduler/dispatch.h"
#include "./onewire/delay.h"
extern uint16_t delay_time;

#define ATMEGA8
#define StartFrom       0xF8 //��� 1 �� 1���

#endif //HAL_H_
//*/
