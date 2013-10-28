#include "kbd.h"
//extern void AddTask (void (*taskfunc)(void), u16 taskdelay, u16 taskperiod);
//  ���������� �������

void InitControl(){
	KEY_DDR=0b00111000;     //���������� �� ����
	//DDRA |= (1 << PC1)|(1 << PC2)|(1 << PC3)|(1 << PC4)|(1 << PC5)|(1 << PC6);
	KEY_PORT=0b00000111;        //����� ���� ���������� �� ���������� ��������
}
//*/
//*
uint8_t KeyCode(){

    return (flags.KeyPin);
    }
//*/
void KeyState(){

    if (((flags.KeyPressed)&&(flags.KeyReleased))||((flags.KeyPressed)&&(flags.KeyPushLong))){
        flags.KeyPressed=0;
        if (flags.KeyPushLong){
            flags.KeyPin+=8;
            //flags.KeyPushLong=0;
        }
        flags.KeyPushLong=0;
        //flags.State_Automate=1;
    }
    flags.KeyReleased=1;
    KeyCurrentCode=flags.KeyPin;
}

void KeyScan(){
    static uint8_t i=0;
  if(KEY_MASK){ // ���������� �������
    flags.KeyReleased=0;
        if (++i > 25 ) {      //�������� ������� 100-250 �����������
            if (!flags.KeyPressed){flags.KeyPressed = 1;flags.KeyPin=(KEY_MASK);}
               if ( i > 100 ){  //������� ������� 1-3 �������
                 if (!flags.KeyPushLong){

                    flags.KeyPushLong=1;
                    KeyState();
                 }
               }
        }
    }
    else {
        i=0;
        if (!flags.KeyReleased) {flags.KeyReleased=1;}
        KeyState();
    }
    SetTimerTask(KeyScan, 25);
}
