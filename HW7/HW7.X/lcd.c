/*
 * Author  : Alvin Baldemeca
 * Credits : These functions were modified from the code provided by Dr. Sheng
 *           from University of Washington Tacoma, TCES 430.
 *
 * Version : 11.23.2013
 *
 * Copy Right (C):
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version. ©
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/*******************************************************************************
 * FileName:        		lcd.c
 * Dependencies:    	See include below , PICDEM2 PLUS demo board
 * Processor:       		PIC18F4520
 * Compiler:        		XC8
 *
 * Synopsis :
 *  #include "lcd.c"
 *   epulse();
 *   send_data();
 *   load_string();
 *
 * Description :
 *   epulse() - sends pluse to LCD
 *   send_data() - sends data to LCD
 *   load_string() - sends string to LCD
*******************************************************************************/



#include "global.h"

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
 * This fucntions sends control data or ascii data to the LCD.
 * @param data The data to send to the LCD
 * @param cflag - a flag to indicate if the data sent is a command or not
 * @param chk_busy - 0 not checking if the LCD is busy, else it checks if the
 *                   LCD is busy
 * @param dflag - indicates if the lower nibble is sent if its not a control
 *                  signal
 */
void send_data(unsigned char data,
               unsigned char cflag,
               unsigned char chk_busy,
               unsigned char dflag)
{

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
        __delay_ms(10); // Wiat just incase LCD is bussy

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
    if (dflag)
    { //send lower nibble if not 4 bit control
        c = data & 0x0F; //clear upper nibble of data
        d = LCD_IO_DATA & 0xF0; //clear lower nibble of port
        LCD_IO_DATA = d | c; //send lower nibble
        epulse();
    }

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
