#include <htc.h>
#include <stdlib.h>

#define _XTAL_FREQ 4000000

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

void LCD_port(char c);
void LCD_cmd(char c);
void LCD_clear();
void LCD_set_cursor(char a, char b);
void LCD_init();
void LCD_write_char(char c);
void LCD_write_string(const char * c);
void LCD_shift_right();
void LCD_shift_left();
void LCD_write_integer(int i);
void LCD_write_float(float f);

void LCD_port(char c){
    if (c & 1)
        D4 = 1;
    else
        D4 = 0;
    if (c & 2)
        D5 = 1;
    else
        D5 = 0;
    if (c & 4)
        D6 = 1;
    else
        D6 = 0;
    if (c & 8)
        D7 = 1;
    else
        D7 = 0;
}

void LCD_cmd(char c)
{
    RS = 0;
    LCD_port(c);
    EN = 1;
    __delay_ms(1);
    EN = 0;
}

void LCD_clear()
{
    LCD_cmd(0);
    LCD_cmd(1);
}

void LCD_set_cursor(char a, char b)
{
    char temp, z, y;
    if (a == 1)
    {
        temp = 0x80 + b - 1;
        z = temp >> 4;
        y = temp & 0x0F;
        LCD_cmd(z);
        LCD_cmd(y);
    }
    else if (a == 2)
    {
        temp = 0xC0 + b - 1;
        z = temp >> 4;
        y = temp & 0x0F;
        LCD_cmd(z);
        LCD_cmd(y);
    }
}

void LCD_init()
{
    LCD_port(0x00);
    __delay_ms(10);
    LCD_cmd(0x03);
    __delay_ms(3);
    LCD_cmd(0x03);
    __delay_ms(10);
    LCD_cmd(0x03);
    LCD_cmd(0x02);
    LCD_cmd(0x02);
    LCD_cmd(0x08);
    LCD_cmd(0x00);
    LCD_cmd(0x0C);
    LCD_cmd(0x00);
    LCD_cmd(0x06);
}

void LCD_write_char(char c)
{
    char temp, y;
    temp = c & 0x0F;
    y = c & 0xF0;
    RS = 1; 
    LCD_port(y >> 4); 
    EN = 1;
    __delay_us(20);
    EN = 0;
    LCD_port(temp);
    EN = 1;
    __delay_us(20);
    EN = 0;
}

void LCD_write_string(const char * c)
{
    int i;
    for (i=0; c[i]!='\0'; i++)
        LCD_write_char(c[i]);
}

void LCD_shift_right()
{
    LCD_cmd(0x01);
    LCD_cmd(0x0C);
}

void LCD_shift_left()
{
    LCD_cmd(0x01);
    LCD_cmd(0x08);
}

void LCD_write_integer(int i)
{
    unsigned char buf[8];
    LCD_write_string(itoa(buf, i, 10));
}

void LCD_write_float(float f)
{
    char * buf11;
    int status;
    buf11 = ftoa(f, &status);
    LCD_write_string(buf11);
}

void I2C_init(void);
void Send_I2C_data(unsigned int data_byte);
unsigned int Read_I2C_data(void);
void Send_I2C_control_byte(unsigned int block_address,unsigned int RW_bit);
void Send_I2C_start_bit(void);
void Send_I2C_stop_bit(void);
void Send_I2C_ACK(void);
void Send_I2C_NAK(void);

void I2C_init(void)
{
    SSPCONbits.SSPM = 0x08;
    SSPCONbits.SSPEN = 1;
    SSPADD = 39;
    __delay_ms(10);
}

void Send_I2C_data(unsigned int data_byte)
{
    PIR1bits.SSP1IF = 0;
    SSPBUF = data_byte;
    while (!PIR1bits.SSP1IF);
}

unsigned int Read_I2C_data(void)
{
    PIR1bits.SSP1IF = 0; 
    SSPCON2bits.RCEN = 1;
    while (!PIR1bits.SSP1IF);
    return (SSPBUF);
}

void Send_I2C_control_byte(unsigned int block_address,unsigned int RW_bit)
{
    PIR1bits.SSP1IF = 0;
    SSPBUF = (((0b1010 << 4) | (block_address <<1)) + RW_bit);
    while (!PIR1bits.SSP1IF);
}

void Send_I2C_start_bit(void)
{
    PIR1bits.SSP1IF = 0; 
    SSPCON2bits.SEN = 1; 
    while (!PIR1bits.SSP1IF);
}

void Send_I2C_stop_bit(void)
{
    PIR1bits.SSP1IF = 0; 
    SSPCON2bits.PEN = 1; 
    while (!PIR1bits.SSP1IF);
}

void Send_I2C_ACK(void)
{
    PIR1bits.SSP1IF = 0; 
    SSPCON2bits.ACKDT = 0; 
    SSPCON2bits.ACKEN = 1; 
    while (!PIR1bits.SSP1IF);
}

void Send_I2C_NAK(void)
{
    PIR1bits.SSP1IF = 0; 
    SSPCON2bits.ACKDT = 1; 
    SSPCON2bits.ACKEN = 1; 
    while (!PIR1bits.SSP1IF);
}

int block_address = 0x00;
int word_address = 0x00;
int eeprom_data = 0x09;
int incoming_data;

void init(void)
{
    ANSELCbits.ANSC4 = 0;
    ANSELCbits.ANSC5 = 0;
    TRISCbits.TRISC4 = 1;
    TRISCbits.TRISC5 = 1;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00;
    RC4PPSbits.RC4PPS = 0x0011; 
    SSPDATPPSbits.SSPDATPPS = 0x0014; 
    SSPCLKPPSbits.SSPCLKPPS = 0x0015; 
    RC5PPSbits.RC5PPS = 0x0010; 
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01;
    TRISD = 0;
    ANSELD = 0; 
    LCD_init();
    __delay_ms(1000);
    LCD_clear();
}

void main(void){
    init();
    I2C_init();
    LCD_set_cursor(1,1);
    LCD_write_string("I2C Ready");
    __delay_ms(1000);
    LCD_clear();
    while (1)
    {
        LCD_set_cursor(1,1);
        LCD_write_string("Write");
        __delay_ms(1000);
        LCD_clear();
        Send_I2C_start_bit();
        Send_I2C_control_byte(block_address,0);
        Send_I2C_data(word_address); 
        Send_I2C_data(eeprom_data);
        Send_I2C_stop_bit();
        __delay_ms(200);
        LCD_set_cursor(1,1);
        LCD_write_string("Read");
        __delay_ms(1000);
        LCD_clear();
        Send_I2C_start_bit();
        Send_I2C_control_byte(block_address,0);
        Send_I2C_data(word_address);
        Send_I2C_start_bit();
        Send_I2C_control_byte(block_address,1);
        incoming_data = Read_I2C_data(); 
        Send_I2C_NAK();
        Send_I2C_stop_bit();
        LCD_set_cursor(1,1);
        LCD_write_integer(incoming_data);
        __delay_ms(1000);
        LCD_clear();
    }    
}
