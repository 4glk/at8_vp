
#include "burner.h"
    // Initialization
void InitBurner(){

    DRIVER(PP_PUMP,OUT);     // pp -powerpump
    DRIVER(PS_LEVEL,IN);    // ps - pump sensor
    DRIVER(PS_LEVEL,PULLUP);    //bp - burner power
    DRIVER(BP_BLOW,OUT);        // bs - burner sensor
    DRIVER(BP_IGNITION,OUT);
    DRIVER(BP_VALVE,OUT);
    DRIVER(BS_PHOTO,IN);
    DRIVER(BS_PHOTO,PULLUP);
    DRIVER(BS_TEMP,IN);
    DRIVER(BS_TEMP,PULLUP);
    DRIVER(BI_LED,OUT);
    DRIVER(BP_BUZZER,OUT);
}

// ����� ������ ������� ����� ����� , ����� ����� ������ �� ��������� ��� ������� , � ������� �� ����������� ��� �������
//

void burnerStart(){
    ON(BP_BLOW);
    ON(BP_IGNITION);
    ON(BP_VALVE);
    AddTask(burnerCheck,1000);

}


void burnerStop(){
    OFF(BP_BLOW);
    OFF(BP_IGNITION);
    OFF(BP_VALVE);


}

void burnerCheck(){
    if (!ACTIVE(BS_PHOTO)) burnerStop();
    else AddTask(burnerCheck,1000);

}

void burnerWork(){




}

void pumpCheck(){
    if (!ACTIVE(PS_LEVEL)) AddTask(pumpStop,500);
    else AddTask(pumpCheck,1000);
}

void pumpStart(){
    ON(PP_PUMP);
    AddTask(pumpCheck,1000);
}

void pumpStop(){
    OFF(PP_PUMP);
}
