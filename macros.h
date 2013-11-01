#ifndef MACROS_
#define MACROS_

#define SB(port,bit) (PORT##port|=(1<<bit))		//setbit (port,bit) �������� ��� ����� ���� ���� ���� � ����� �����
#define CB(port,bit) (PORT##port&= ~(1<<bit))   //clearbit � ����� ������� �������� ���� � ��� ��� �������� ����
#define TB(port,bit) (PORT##port^=(1<<bit))		//togglebit � ��� , ���� �� ���� ������ PORTC  ���� ������ C � ���
#define CH(port,bit) (PIN##port&(1<<bit))		//checkbit CH(B,3) ��� ���� ��� ����� ���������

// ������
// atmega8
//#define ATMEGA8

// atmega328p
#define ATMEGA328

// ������� ����
#pragma once
#include <stdbool.h>
//*
typedef struct { //��������� ���������� , ����� ������� �� ������ ���� �� ����� ?
//  bool State_Automate:1;    //����������
  bool KeyPressed:1;        //���� ������� �������
  bool KeyReleased:1;       //���� ���������� �������
  bool KeyPushLong:1;       //���� ������� ������� �������
  bool KeyState:1;          //���� ��������� �������
  uint8_t  KeyPin:4;        //����� ��� �������
//  bool Fire:1;            //���� �������
//  bool Furnace:1;         //���� ����������� ����
//  bool SupplyManual:1;
//  bool SupplyAuto:1;          //���� ������
//  bool ADC_Channel:1;      // ����� ���
//  bool NextState:1;
  bool RunFlag:1;
 } PackedBool;
PackedBool flags;
//int *KeyCodePointer;
//*/

#define TASCII(numb) (numb+48)

#define true 1
#define false 0

#define vuchar volatile unsigned char
#define vuint volatile unsigned int

#define sbi(reg,bit) reg |= (1<<bit)
#define cbi(reg,bit) reg &= ~(1<<bit)
#define ibi(reg,bit) reg ^= (1<<bit)
#define CheckBit(reg,bit) (reg&(1<<bit))

void RunTasks(void);


#endif // MACROS_

