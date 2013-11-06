
#include "burner.h"
    // Initialization
void InitBurner(){

    DRIVER(PP_PUMP,OUT);
    DRIVER(PS_LEVEL,IN);
    DRIVER(PS_LEVEL,PULLUP);
    DRIVER(BP_BLOW,OUT);
    DRIVER(BP_IGNITION,OUT);
    DRIVER(BP_VALVE,OUT);
    DRIVER(BS_PHOTO,IN);
    DRIVER(BS_PHOTO,PULLUP);
    DRIVER(BS_TEMP,IN);
    DRIVER(BS_TEMP,PULLUP);
    DRIVER(BI_LED,OUT);
    DRIVER(BP_BUZZER,OUT);
}

void burnerStart(){
    TOGGLE(BP_BLOW);


}


void burnerStop(){



}

void burnerCheck(){



}

void burnerWork(){




}
