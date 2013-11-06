//#include "../macros.h"
#include <avr/io.h>
#include "../pin_macros.h"


/*
// ������������� ������ � ����� ������� ��� , ���� BUZZER D,5
// ����� �������� ����� portpins.h
// �������� �����  waste pump
#define B_PUMP_DDR      DDRB
#define B_PUMP_PORT     PORTB
#define B_PUMP_PIN      PINB
#define B_PUMP_BIT      PB3

// ������ ������  level gauge
#define B_LEVEL_DDR     DDRD
#define B_LEVEL_PORT    PORTD
#define B_LEVEL_PIN     PIND
#define B_LEVEL_BIT     PD5

// ������/��������� ����� blowing / diesel pump
#define B_BLOW_DDR      DDRB
#define B_BLOW_PORT     PORTB
#define B_BLOW_PIN      PINB
#define B_BLOW_BIT      PB4

// ������           ignition
#define B_IGNITION_DDR      DDRB
#define B_IGNITION_PORT     PORTB
#define B_IGNITION_PIN      PINB
#define B_IGNITION_BIT      PB6

//������ ������ �������   fuel valve
#define B_VALVE_DDR         DDRB
#define B_BALVE_PORT        PORTB
#define B_VALVE_PIN         PINB
#define B_VALVE_BIT         PB5

// ������� ���������� �������� �� onewire
// ���� ������     photosensor
#define B_PHOTO_DDR     DDRD
#define B_PHOTO_PORT    PORTD
#define B_PHOTO_PIN     PIND
#define B_PHOTO_BIT     PD4


//������ �����������    temperature probe
#define B_TEMP_DDR
#define B_TEMP_PORT
#define B_TEMP_PIN
#define B_TEMP_BIT     IIC  PD6


// ��������� led
#define B_LED1_DDR     DDRD
#define B_LED1_PORT     PORTD
#define B_LED1_PIN      PIND
#define B_LED1_BIT      PD6

#define B_LED2_DDR      DDRD
#define B_LED2_PORT     PORTD
#define B_LED2_PIN      PIND
#define B_LED2_BIT      PD7

// ������    buzzer
#define B_BUZZER_DDR    DDRB
#define B_BUZZER_PORT   PORTB
#define B_BUZZER_PIN    PINB
#define B_BUZZER_BIT    PB7

//#define name,port,P##port##pin) (BUZZER,B,7)
//#define SET_DDR(port,bit) (PORT##port
//*/
// PINOUT
// first letter P - Pump , B - burner
// second letter P - Power, S -Sensor, I - Indicator
#define PP_PUMP      B,3,H
#define PS_LEVEL     D,5,H
#define BP_BLOW      B,4,H
#define BP_IGNITION  B,6,H
#define BP_VALVE     B,5,H
#define BS_PHOTO     D,4,H
#define BS_TEMP      D,6,H
#define BI_LED       D,7,H
//#define B_LED2     // ���� ������ ��������� , ���� �����������
#define BP_BUZZER    B,7,H

//uint16_t up_temp, down_temp, current_temp;
//uint16_t up_level, down_level, current_level;
void InitBurner();
void burnerStart();
void burnerStop();
void burnerCheck();
void burnerWork();

extern void AddTask (void (*taskfunc)(void), uint16_t taskdelay);
