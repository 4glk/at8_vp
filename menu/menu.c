#include "menu.h"

/** Example menu item specific enter callback function, run when the associated menu item is entered. */
static void Level1Item1_Enter(void)
{
//	puts("ENTER");
    nlcd_PrintF(PSTR("ENTER_MENU_1_1"));
 //        USART_Print("Enter Menu 1 1");
}

/** Example menu item specific select callback function, run when the associated menu item is selected. */
static void Level1Item1_Select(void)
{
//	puts("SELECT");
    nlcd_PrintF(PSTR("SELECT_MENU_1_1"));
//     USART_Print("Select Menu 1 1");
//    nlcd_GotoXY(0,3);
//	nlcd_SendByte(CMD_LCD_MODE,0xA7);
}

/** Generic function to write the text of a menu.
 *
 *  \param[in] Text   Text of the selected menu to write, in \ref MENU_ITEM_STORAGE memory space
 */
static void Generic_Write(const char* Text)
{
    if (Text);
        //puts_P(Text);
//		nlcd_Print(Text);
        nlcd_PrintF(Text);
//        USART_Print(Text);
}
//         Name      Next     Previous        Parent         Child     SelectFunc   EnterFunc  Text
MENU_ITEM(Menu_1,   Menu_2,     Menu_3,     NULL_MENU,      Menu_1_1,PositionMenuesLevel1,Level1Item1_Enter,"1 FIRE     ");
MENU_ITEM(Menu_2,   Menu_3,     Menu_1,     NULL_MENU,      NULL_MENU,PositionMenuesLevel1,   NULL,       "2 VAPORIZER");
MENU_ITEM(Menu_3,   Menu_1,     Menu_2,     NULL_MENU,      NULL_MENU,PositionMenuesLevel1,   NULL,       "3 PUMP     ");

MENU_ITEM(Menu_1_1, Menu_1_2,   Menu_1_2,   Menu_1,      NULL_MENU,PositionMenuesLevel2,   NULL,       "1.1        ");
MENU_ITEM(Menu_1_2, Menu_1_1,   Menu_1_1,   Menu_1,      NULL_MENU,PositionMenuesLevel2,   NULL,       "1.2        ");

void PositionMenuesLevel1()
{
    nlcd_GotoXY(0,3);
}

void PositionMenuesLevel2()
{
    nlcd_GotoXY(0,4);
}

void MenuInit()
{
    /* Set up the default menu text write callback, and navigate to an absolute menu item entry. */
        Menu_Navigate(&Menu_1);
    Menu_SetGenericWriteCallback(Generic_Write); //вот тут скорее всего мен€етс€ местами врем€ запуска вывода текста
                                                    // и выполнени€ функции выполнени€
}

void SwitchMenu()
{
    /* Example usage of Micromenu - here you can create your custom menu navigation system; you may wish to perform
     * other tasks while detecting key presses, enter sleep mode while waiting for user input, etc.
     */
    if (KeyCurrentCode) {
//    if (6) {
//		switch (GetButtonPress())
//        USART0_write('K');
        switch (KeyCurrentCode) {
        case BUTTON_UP:
            Menu_Navigate(MENU_PREVIOUS);
   //         USART0_write('U');
            break;
        case BUTTON_DOWN:
            Menu_Navigate(MENU_NEXT);
//            printf("Down");
  //          USART0_write('D');
            break;
        case BUTTON_LEFT:
            Menu_Navigate(MENU_PARENT);
            break;
        case BUTTON_RIGHT:
            Menu_Navigate(MENU_CHILD);
            break;
        case BUTTON_ENTER:
 //           USART_Print("Onewire");
//            usartPrintOnewire();
            Menu_EnterCurrentItem();
            break;
        case BUTTON_SCROLL_UP:
 //            USART_Print("Scroll Up");
//             usartPrintOnewire();
            nlcd_PrintF(PSTR("SROLL_UP"));
            temperatureRead();
//            nlcd_PrintWide(current_temp);
            break;
        case BUTTON_SCROLL_DOWN:
 //           USART_Print("Scroll Down");
            nlcd_PrintF(PSTR("SCROLL_DOWN"));
            temperatureStartConvert();
//            usartDebug();
//            printf("Debug USART");
            break;
        case BUTTON_FIRE:
  //           USART_Print("Fire");
            nlcd_PrintF(PSTR("FIRE_ENABLE"));
            AddTask(burnerStart,50);
  //          USART0_write('F');
 //             usartPrintOnewire();
            break;
        case BUTTON_PUMP:
 //            USART_Print("Pump");
            nlcd_PrintF(PSTR("PUMP_ENABLE"));
            AddTask(pumpStart,100);
  //          USART0_write('P');
            break;
        default:
            break;
        }
        KeyCurrentCode=0;
    }
        AddTask(SwitchMenu,100);
}
