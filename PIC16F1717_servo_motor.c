#include <htc.h>

#define _XTAL_FREQ 16000000

void servo_rotate0();
void servo_rotate90();
void servo_rotate180();

void init()
{
    TRISDbits.TRISD0 = 0;
    ANSELD = 0;
}

void main(void)
{
    init();
    while (1) 
    {
        servo_rotate0();
        servo_rotate90();
        servo_rotate180();
    }
}

void servo_rotate0()
{
    unsigned int i;
    for (i = 0; i < 50; i++)
    {
        RD0 = 1;
        __delay_us(700);
        RD0 = 0;
        __delay_us(19300);
    }
}

void servo_rotate90()
{
    unsigned int i;
    for (i = 0; i < 50; i++)
    {
        RD0 = 1;
        __delay_us(1700);
        RD0 = 0;
        __delay_us(18300);
    }
}

void servo_rotate180()
{
    unsigned int i;
    for (i = 0; i < 50; i++)
    {
        RD0 = 1;
        __delay_us(2600);
        RD0 = 0;
        __delay_us(17400);
    }
}
