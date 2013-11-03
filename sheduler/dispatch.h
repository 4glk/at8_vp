#include <avr/io.h>
#include "../macros.h"
//#include <avr/interrupt.h>
//#include <stdbool.h>
//#include "../HAL.h"

#define LSHED

#pragma once

typedef struct task{
    void (*pfunc) (void);    // ��������� �� �������

    uint16_t countdown;           // �������� ������
#ifndef LSHED
    uint16_t delay;                  // ����� �������� ����������
    uint8_t run:1;  // ���� ���������� ������ � �������
    void (*nextfunc)(void);
    uint16_t nextdelay;              // ������ ������� ������ ������ ��� ����� �������� ����� �������� ��������� ������
    uint16_t numRun;              // ���������� ��������
#endif // LSHED
}task;                      // ���� ������� ����� ������� �� ���������

uint16_t delay_time;   // �������� �� ������ , ���� ������� �� ���� � �����
uint16_t dt;
extern void USART0_write(unsigned char data);
// ������������ ���������� �����
#define MAXnTASKS       8
//��������� ��� UART`a
//�������� ������ 9600 ��� ������� 8 ���
#define UBRRvalue 0x0033


/// ������ ����� ///////////
volatile task TaskArray[MAXnTASKS];

/// ��������� ������� ////////
//void InitScheduler (void);      //������������� ����������

void DispatchTask (void);       //������ ��� ��������� � ������ ������ �� �����
void Idle();
void DeleteTask (uint8_t index);     //�������� ������
//TODO:����� ������� ��� ������� ����� ��������� �� ������� � ������ ����������� � ��� ���
#ifndef LSHED
void AddTask (void (*taskfunc)(void),void (*nextfunc)(void),uint16_t taskdelay,uint16_t nextdelay,uint16_t taskruns);   //���������� ������
void ResetTask(void (*resfunc)(void));               //��������� ��������� ����������� ������ , ���������� ���� ���
void UpdateScheduler(void);     //���������� ����������

#endif // LSHED
#ifdef LSHED
void AddTask (void (*taskfunc)(void), uint16_t taskdelay);
#endif


