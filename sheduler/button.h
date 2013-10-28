#ifndef BUTTON_H
#define BUTTON_H

#include "HAL.h"
#include "EERTOS.h"
#include "avrlibtypes.h"
#include "avrlibdefs.h"



#define Open	(BTN_PIN & 1<<BTN)
#define Close	(!Open)

extern void bt_scan(void);
void bt_ok(void);

extern u08 bt1,bt2,bt3,bt4,bt5,bt_l,bt_l2,bt_al;
extern u16 bt_mode_time;

#endif
