#include <htc.h>

void init()
{
    TRISDbits.TRISD1 = 0;                       // set D1 as output
    LATDbits.LATD1 = 0;                         
    TRISBbits.TRISB0 = 1;                       // set B1 as input
    ANSELBbits.ANSB0 = 0;                       
    OPTION_REGbits.nWPUEN = 0;                  
    WPUBbits.WPUB0 = 1;                          
}

void main(void)
{
    init();
    while (1)
    {
        LATDbits.LATD1 = ~PORTBbits.RB0;
    }
}
