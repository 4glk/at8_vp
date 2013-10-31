#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "../macros.h"
//#include "../HAL.h"

#pragma once
/// ���� ������ //
//typedef unsigned char u8;
//typedef unsigned int u16;
typedef struct task{
   void (*pfunc) (void);    // ��������� �� �������
   void (*nextfunc)(void);
   uint16_t delay;                  // ����� �������� ����������
      uint16_t countdown;           // �������� ������
   uint16_t nextdelay;              // ������ ������� ������ ������ ��� ����� �������� ����� �������� ��������� ������
   uint8_t run;                  // ���� ���������� ������ � �������
   uint16_t numRun;

}task;                      // ���� ������� ����� ������� �� ���������

extern uint16_t delay_time;
        uint16_t dt;
extern uint16_t timer2;

/// ����������� ///////////
// ��������� ��� ������� �0
// 25 �� ��� �������� �������
// 8 ��� � ������������ 1024
//#define StartFrom       0x3D //��� 25 �� 40��
//#define StartFrom       0xF8 //��� 1 �� 1���            //�� ������� ������� ����� � ��� ��������
//#define StartFrom       0xB2 //��� 10 �� 100��        //������������ ��� ���������
// ������������ ���������� �����
#define MAXnTASKS       8
//��������� ��� UART`a
//�������� ������ 9600 ��� ������� 8 ���
#define UBRRvalue 0x0033


/// ������ ����� ///////////
volatile task TaskArray[MAXnTASKS];

/// ��������� ������� ////////
//void InitScheduler (void);      //������������� ����������
void UpdateScheduler(void);     //���������� ����������
void DeleteTask (uint8_t index);     //�������� ������
void ResetTask(void (*resfunc)(void));               //��������� ��������� ����������� ������ , ���������� ���� ���
//void resfunc;
//TODO:����� ������� ��� ������� ����� ��������� �� ������� � ������ ����������� � ��� ���
void AddTask (void (*taskfunc)(void),void (*nextfunc)(void),uint16_t taskdelay,uint16_t nextdelay,uint16_t taskruns);   //���������� ������
//void AddTask (void (*taskfunc)(void),void (*nextfunc)(void), u16 taskdelay, u16 taskperiod,u16 taskruns);
void DispatchTask (void);       //������ ��� ��������� � ������ ������ �� �����
void Idle();

