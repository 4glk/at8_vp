#ifndef HAL_H_
#define HAL_H_
#include <avr/interrupt.h>
#include <avr/io.h>
#include "macros.h"
#include "./sheduler/dispatch.h"
#include "./onewire/delay.h"
// ������
// atmega8
//#define ATMEGA8
uint16_t delay_time=100;
// atmega328p
#define ATMEGA328

// ��������� ��� ������� �0
// 25 �� ��� �������� �������
// 8 ��� � ������������ 1024
//#define StartFrom       0x3D //��� 25 �� 40��
#define StartFrom       0xF8 //��� 1 �� 1���            //�� ������� ������� ����� � ��� ��������
//#define StartFrom       0xB2 //��� 10 �� 100��        //������������ ��� ���������
// ������������ ���������� �����
// ������������� ��� 8
#ifdef ATMEGA8

#endif // ATMEGA8

// ������������� ��� 328
#ifdef ATMEGA328

#endif // ATMEGA328

#endif // HAL_H_
