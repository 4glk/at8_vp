#include "../macros.h"

// найти описание файла portpins.h
// мазутный насос  waste pump
#define B_PUMP_DDR      DDRB
#define B_PUMP_PORT     PORTB
#define B_PUMP_PIN      PINB
#define B_PUMP_BIT      PB3

// датчик уровня  level gauge
#define B_LEVEL_DDR     DDRD
#define B_LEVEL_PORT    PORTD
#define B_LEVEL_PIN     PIND
#define B_LEVEL_BIT     PD5

// поддув/дизельный насос blowing / diesel pump
#define B_BLOW_DDR      DDRB
#define B_BLOW_PORT     PORTB
#define B_BLOW_PIN      PINB
#define B_BLOW_BIT      PB4

// розжиг           ignition
#define B_IGNITION_DDR      DDRB
#define B_IGNITION_PORT     PORTB
#define B_IGNITION_PIN      PINB
#define B_IGNITION_BIT      PB6

//клапан подачи топлива   fuel valve
#define B_VALVE_DDR         DDRB
#define B_BALVE_PORT        PORTB
#define B_VALVE_PIN         PINB
#define B_VALVE_BIT         PB5

// датчики желательно повесить на onewire
// фото датчик     photosensor
#define B_PHOTO_DDR     DDRD
#define B_PHOTO_PORT    PORTD
#define B_PHOTO_PIN     PIND
#define B_PHOTO_BIT     PD4

/*
//датчик температуры    temperature probe
#define B_TEMP_DDR
#define B_TEMP_PORT
#define B_TEMP_PIN
#define B_TEMP_BIT     IIC
//*/

// индикация led
#define B_LED1_DDR     DDRD
#define B_LED1_PORT     PORTD
#define B_LED1_PIN      PIND
#define B_LED1_BIT      PD6

#define B_LED2_DDR      DDRD
#define B_LED2_PORT     PORTD
#define B_LED2_PIN      PIND
#define B_LED2_BIT      PD7

// буззер    buzzer
#define B_BUZZER_DDR    DDRB
#define B_BUZZER_PORT   PORTB
#define B_BUZZER_PIN    PINB
#define B_BUZZER_BIT    PB7


void burnerStart();
void burnerStop();
void burnerCheck();
void burnerWork();

