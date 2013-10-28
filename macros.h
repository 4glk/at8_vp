#ifndef MACROS_
#define MACROS_

//#define TCNT0_const 0xd9 //217(0xEC,236 ��� 400��) //5 �� ��� 8 ��� ������� �� 8��� 200 ��, 2,5�� �� 16��� 400��
//#define TCNTCC 57724 //0xe17c 1�� 1 ��� �� 16 ��� ������ �� 8 ���
//*
#define SB(port,bit) (PORT##port|=(1<<bit))		//setbit (port,bit) �������� ��� ����� ���� ���� ���� � ����� �����
#define CB(port,bit) (PORT##port&= ~(1<<bit))   //clearbit � ����� ������� �������� ���� � ��� ��� �������� ����
#define TB(port,bit) (PORT##port^=(1<<bit))		//togglebit � ��� , ���� �� ���� ������ PORTC  ���� ������ C � ���
#define CH(port,bit) (PIN##port&(1<<bit))		//checkbit CH(B,3) ��� ���� ��� ����� ���������





#pragma once
#include <stdbool.h>
//*
typedef struct { //��������� ���������� , ����� ������� �� ������ ���� �� ����� ?
  bool State_Automate:1;    //����������
  bool KeyPressed:1;        //���� ������� �������
  bool KeyReleased:1;       //���� ���������� �������
  bool KeyPushLong:1;       //���� ������� ������� �������
  bool KeyState:1;          //���� ��������� �������
  uint8_t  KeyPin:4;        //����� ��� �������
  bool Fire:1;            //���� �������
  bool Furnace:1;         //���� ����������� ����
  bool SupplyManual:1;
  bool SupplyAuto:1;          //���� ������
  bool ADC_Channel:1;      // ����� ���
  bool NextState:1;
  bool RunFlag:1;
 } PackedBool;
PackedBool flags;
int *KeyCodePointer;
//*/
#endif // MACROS_


