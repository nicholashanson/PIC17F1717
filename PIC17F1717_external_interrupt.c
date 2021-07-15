#include <htc.h>

#define _XTAL_FREQ 4000000

void init()
{
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    
    LATDbits.LATD1 = 0;
    
    TRISBbits.TRISB0 = 1;
    
    ANSELBbits.ANSB0 = 0;
    
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCK = 0x00;
    
    OPTION_REGbits.nWPUEN = 0;
    
    WPUBbits.WPUB0 = 1;
    
    INTPPSbits.INTPPS = 0b01000;
    
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCK = 0x01;
    
    OPTION_REGbits.INTEDG = 0;
    
    INTCONbits.INTF = 0;
    
    INTCONbits.INTE = 1;
    
    ei();
}

void main(void)
{
    init();
    while(1)
    {
        LATDbits.LATD1 = ~LATDbits.LATD1;
        __delay_ms(500);
    }
}

void __interrupt () isr(void)
{
    INTCONbits.INTF = 0;
    
    LATDbits.LATD2 = ~LATDbits.LATD2;
}
