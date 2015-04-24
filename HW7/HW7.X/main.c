/*
 * Alvin Baldemeca
 * HW 7
 * University of Washington Tacoma
 * TCES 430, Prof. Sheng
 * 11.23.2013
 *
 */

/*******************************************************************************
 * FileName:        		main.c
 * Dependencies:    	See include below , PICDEM2 PLUS demo board
 * Processor:       		PIC18F4520
 * Compiler:        		XC8
 *
 * Homework Description:
 * 
 * Based on the I2C program template provided on Canvas course website, write a 
 * C program to page (64 bytes) write/read the 24LC256 EEPROM sitting on the 
 * demonstration board.
 * 
 * You will use I2C synchronous serial communication to write/read to/from the 
 * EEPROM. How can you tell if the communication does really happen? Design your
 * own way to verify that the page data in EEPROM has been correctly read back.
 * 
 * You should first understand the principles of I2C protocol (by reading the 
 * MCU and the EEPROM datasheets, as well as the lecture notes), and then 
 * complete the tasks based on the program template. If you notice any comments
 * on the program template not clear, feel free to modify and improve that. If 
 * you find some code sections not necessary in doing the job, you may also 
 * remove them.
 * 
 * You can NOT use the XC8 I2C library functions in this assignment/lab. Submit 
 * the lab report summarizing your lab experience and observation, as well as 
 * the MPLAB project zip file to Canvas by 11:59pm, Nov. 26.
*******************************************************************************/


#include "global.h"
#include "i2c.h"
#include "lcd.h"
#include "i2c_page.h"
#define MSB_ADD 0x00
#define LSB_ADD 0X00
void init_lcd();



/**
 * The main fucntion writes a page (64 bytes) of data  to 24LC256 EEPROM on the
 * PICDEM2 board and retrieves the page.  Then the ASCII characters are written
 * and displayed it on the LCD.  The string stored in "mypage" will be displayed
 * on the LCD scrolling forever.
 */
void main (void){

    const char mypage[64] = "My message to you : "\
    "Have a Merry Christmas and a Happy New Year\0";
    char message[64];
    i2c_init(); // Initiate for I2C
    write_page(mypage, MSB_ADD, LSB_ADD);
    read_page(message, MSB_ADD, LSB_ADD);
    init_lcd(); //Initiate LCD
    load_string(message); //Load the page to LCD
    while (1)
    {
        send_data(0x18,1,1,1); //scroll display
        __delay_ms(SCROLL_SPEED); //Slow down the scroll
        __delay_ms(SCROLL_SPEED); //Slow down the scroll
    }
}

/**
 * Initializes the LCD on the PICDEM2 PLUS demo board for single line.
 */
void init_lcd()
{
   LCD_TXRX = OUTPUT_ALL; //port direction write
    LCD_IO_DATA = 0x00; //port outputs 0
    LCD_PWR = ENABLE; //power to lcd
    send_data(0x20,1,1,0); //4-bit data
    send_data(0x20,1,1,1); //1 line display
    send_data(0x06,1,1,1); //enable display set to incrementing mode
    send_data(0x0c,1,1,1); //turn on display
    send_data(0x02,1,1,1); //clear display
    send_data(0x01,1,1,1); //Move cursor to home position
}