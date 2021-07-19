#include <htc.h>
#include <stdint.h>

#define _XTAL_FREQ 4000000

void PWM1_load_duty_value(uint16_t duty_value)
{
    // CCPR: capture compare PWM register (LSB)
    CCPR1L = ((duty_value & 0x03FC) >> 2);
    CCP1CON = (CCP1CON & 0xCF) | ((duty_value & 0x0003) << 4);
}

void PWM2_load_duty_value(uint16_t duty_value)
{
    // CCPR: capture compare PWM register (LSB)
    CCPR2L = ((duty_value & 0x03FC) >> 2);
    CCP2CON = (CCP2CON & 0xCF) | ((duty_value & 0x0003) << 4);
}

void PWM3_load_duty_value(uint16_t duty_value)
{
    PWM3DCH = (duty_value & 0x03FC) >> 2;
    PWM3DCL = (duty_value & 0x0003) << 6;
}

void RGB_load_value(uint16_t red, uint16_t green, uint16_t blue)
{
    PWM1_load_duty_value(red);
    PWM2_load_duty_value(green);
    PWM3_load_duty_value(blue);
}

void init()
{
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    // ANSELB: used to configure the input mode of an I/O pin to analog
    ANSELB = 0;
    // CCPTMRS: used to select a timer
    // there are three 8-bit timers with auto-reload: timer2/4/6
    // 10 selects timer6
    // 01 selects timer4
    // 00 selects timer2
    // C1TSEL: CCP1 (PWM1) timer selection
    CCPTMRSbits.C1TSEL = 0b10;
    // C2TSEL: CCP2 (PWM2) timer selection
    CCPTMRSbits.C2TSEL = 0b10;
    T6CONbits.T6CKPS = 0b00;
    T6CONbits.TMR6ON = 1;
    PR6 = 255;
    // CCP1CON: CCP1 control register
    // DC1B: PWM duty cycle least significant bits
    CCP1CONbits.DC1B = 00;
    // CCP1M<3:0> CCP1 mode select bits 
    // 11xx: PWM mode
    CCP1CONbits.CCP1M = 0b1100;
    CCP2CONbits.DC2B = 00;
    CCP2CONbits.CCP2M = 0b1100;
    // PWM3CON: PWM control register
    // PWM3EN (bit7), PWM3OUT (bit5), PWM3POL (bit4)
    // PWM3EN: PWM module enable bit (1: enabled)
    PWM3CON = 0x80;
    // MSBs of the PWM duty cycle
    PWM3DCH = 0x7F;
    PWM3DCL = 0xC0;
    // PWM timer selection control register
    // P3TSEL: PWM3 timer selection
    // 10: PWM3 is based off timer6
    CCPTMRSbits.P3TSEL = 0b10;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    // PPSLOCKED: 0: PPS is not locked
    PPSLOCKbits.PPSLOCKED = 0x00;
    // RB0PPS: PINB0 output source selection register
    // PPS: output source selection bits
    // 01100: CCP1 
    RB0PPSbits.RB0PPS = 0b01100;
    // 01101: CCP2
    RB1PPSbits.RB1PPS = 0b01101;
    RB2PPSbits.RB2PPS = 0x0E;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01;
}

void main(void)
{
    init();
    PWM1_load_duty_value(0);
    PWM2_load_duty_value(0);
    PWM3_load_duty_value(0);
    while (1)
    {
        RGB_load_value(512, 0, 0);
        __delay_ms(1000);
        
        RGB_load_value(0, 512, 0);
        __delay_ms(1000);
        
        RGB_load_value(0, 0, 512);
        __delay_ms(1000);
        
        RGB_load_value(192, 192, 0);
        __delay_ms(1000);
        
        RGB_load_value(192, 0, 192);
        __delay_ms(1000);
        
        RGB_load_value(0, 512, 512);
        __delay_ms(1000);
    }
}
