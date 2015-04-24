/*
 * Alvin Baldemeca
 * UWT TCES 430
 * Homework 6 part 1
 * Prof. Sheng
 *
 * Processor: 	   PIC18F4520
 * Compiler: 	   XC8 (MICROCHIP)
 *
 * This program counts repeadteadly from 0 to 9 on the LCD display of the PIC
 * DEM 2 PLUS demon board.  If the push button RB0 is pressed and release the
 * counting is reset to 0.
 *
 * Note: Most of the functions used in this program was taken from Prof. Sheng's
 * 4520LCD_2013.c program and modified to be used for the XC8 compiler.
 */

#include <stdio.h>
#include <stdlib.h>
#include <p18f4520.h>
#include <xc.h>

#pragma config WDT = OFF  // Set watch dot timer off
#pragma config LVP = OFF  // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config PBADEN = OFF // Turn off PortB A/D
#pragma config OSC = INTIO67  //Internal frequency is 1MHz

#define _XTAL_FREQ 1000000  //used by __delay_ms 
#define LCD_PWR PORTDbits.RD7
#define LCD_E PORTDbits.RD6
#define LCD_RS PORTDbits.RD4
#define LCD_RW PORTDbits.RD5
#define LCD_TXRX TRISD
#define LCD_IO_DATA PORTD
#define INPUT 1
#define OUTPUT 0
#define INPUT_ALL 0xFF
#define OUTPUT_ALL 0x00
#define ENABLE 1
#define DISABLE 0
#define READ 1
#define WRITE 0
#define MOD_NUM '0' + 10


void send_data(unsigned char data,unsigned char cflag,
               unsigned char chk_busy,unsigned char dflag);
void epulse(void);
void load_string(const char *pt);
volatile unsigned char flag = 0;
char number = '0';

void interrupt high_priority buttonRB0(void)
{
   //check if the interrupt is caused by the pin RB0
    if(INTCONbits.INT0IF)
    {
        INTCONbits.INT0IF = 0;
        flag = 1;
    }

}

/*
 * The mian function sets up RB0 as a high prority interrupt and initializes the
 * LCD on the PIC DEM 2 Plus board.  The program counts repeatedly from 0 to 9
 * if RB0 is pressed it interrupts and sets a flag to reset the count to start
 * from 0.
 * @return - 1 if the program returns then something is worng.  The forever loop
 *           should never exit.
 */
int main() {

    int i;
    TRISBbits.RB0 = 1; //set RB0 as input
    RCONbits.IPEN = 1; //enable priority interrupt
    INTCONbits.INT0IF = 0; //clear interrupt flag
    INTCONbits.INT0IE = 1; //enable interrupt
    INTCON2bits.INTEDG0 = 1; //set interrrupt on rising edge (when button is released)
    LCD_TXRX = OUTPUT_ALL; //port direction write
    LCD_IO_DATA = 0x00; //port outputs 0
    LCD_PWR = ENABLE; //power to lcd
    send_data(0x20,1,0,0); //4-bit data
    send_data(0x28,1,0,1); //2 line display
    send_data(0x28,1,0,1); //repeat
    send_data(0x06,1,0,1); //enable display set to incrementing mode
    send_data(0x0c,1,0,1); //turn on display
    send_data(0x02,1,0,1); //clear display
    ei(); //Enable interrupt same as INTCONbits.GIE = 1

    for(;;)
    {
        for(i=0; i < 10; i++)
        { //while char != null
            char s_num;
             if(flag)
             {
                 flag = 0;
                 //__delay_ms(10);
                 i = 0;
             }
                s_num = number + i; //increment pointer
                send_data(0x01,1,0,1); //Move cursor to home position
                send_data(s_num,0,1,1); //send char
                __delay_ms(700); //Delay so we can see the changing values on the LCD
        }
    }

    return (EXIT_FAILURE); //Code should never get here if it did an error occured
}

/**
 * This fucntions sends control data or ascii data to the LCD.
 * @param data The data to send to the LCD
 * @param cflag - a flag to indicate if the data sent is a command or not
 * @param chk_busy - 0 not checking if the LCD is busy, else it checks if the
 *                   LCD is busy
 * @param dflag - indicates if the lower nibble is sent if its not a control
 *                  signal
 */
void send_data(unsigned char data,unsigned char cflag,
unsigned char chk_busy, unsigned char dflag){
        char c,d,bflag;
        if(chk_busy)
        {
                LCD_TXRX = 0x0F; //read on lower 4 bits
                LCD_RS = 0; //RS=0, control signal
                LCD_RW = READ; //read
                bflag = 1; //set busy flag

                while(bflag)
                {
                        Nop();
                        LCD_E =1; //E high
                        Nop(); //__delay_us(1);
                        bflag = PORTDbits.RD3; //read busy signal
                        Nop();
                        LCD_E=0; //E low
                        epulse(); //read lower 4 bits
                }
        }
        else
            __delay_ms(200); // Needed delay if LCD chip is busy

        LCD_TXRX = 0x00; //write from port
        if(cflag)
                LCD_RS = 0; //control signal
        else
                LCD_RS = 1; //data


        LCD_RW = WRITE; //write
        c = data>>4; //shift upper nibble to lower
        d = LCD_IO_DATA & 0xF0; //clear lower nibble of port
        LCD_IO_DATA = d | c; //add lower bits for data or command
        epulse();
        if (dflag){ //send lower nibble if not 4 bit control
                c = data & 0x0F; //clear upper nibble of data
                d = LCD_IO_DATA & 0xF0; //clear lower nibble of port
                LCD_IO_DATA = d | c; //send lower nibble
                epulse();
        }
return;
}

/**
 * This is a helper function for send_data that creates a single pulse on the
 * e (enable) pin of the LCD.
 */
void epulse(void){

        Nop(); //__delay_us(1);
        LCD_E=1; //E high
        Nop();
        LCD_E=0; //E low
        Nop();
        return;
}

/**
 * This function loads an ASCII string to the LCD.
 * @param ptr - the pointer to the string to display on the LCD
 */
void load_string(const char *ptr){
        while(*ptr != 0){ //while char != null
                send_data(*ptr,0,1,1); //send char
                ptr++; //increment pointer
        }
        return;
}
