#include "DIO_interface.h"
#include "DIO_private.h"
#include "LCD_interface.h"
#include "LCD_private.h"
#include "KPD_interface.h"
#include "EEPROM_interface.h"

#define F_CPU 8000000UL

#define PASS 123

int main()
{
    u8 receivedPassword = 0, c = 0, key = NOT_PRESSED;
    LCD_voidInit();
    LCD_voidSendCommand(cursorOff);
    KPD_voidInit(PORTA);
    EEPROM_voidWriteByte(0, PASS);
    LCD_voidSendString((u8 *)"Password: ");
    LCD_voidSetCursorPosition(1, 0);
    while (1)
    {
        receivedPassword = 0;
        do
        {
            key = KPD_u8GetPressedKey(PORTA);
            if (key == 'c')
            {
                receivedPassword = 0;
                LCD_voidSendCommand(clearDisplay);
                LCD_voidSendString((u8 *)"Password: ");
                LCD_voidSetCursorPosition(1, 0);
            }
            else if (key != NOT_PRESSED && key != '=')
            {
                LCD_voidSendChar('*');
                receivedPassword = receivedPassword * 10 + (key - '0');
            }
        } while (key != '=');
        LCD_voidSendCommand(clearDisplay);
        if (receivedPassword == EEPROM_voidReadByte(0))
        {
            LCD_voidSendString((u8 *)"Success!");
            return 0;
        }
        else
        {
            if (c == 2)
            {
                LCD_voidSetCursorPosition(0, 2);
                LCD_voidSendString("Number of tries");
                LCD_voidSetCursorPosition(1, 5);
                LCD_voidSendString("exceeded");
                return 0;
            }
            else
            {
                LCD_voidSendString((u8 *)"Try Again.");
                LCD_voidSetCursorPosition(1, 0);
            }
            c++;
        }
    }
}