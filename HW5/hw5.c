/*
 * Alvin Baldemeca
 * TCES 430, UWT
 * Homework 5
 * Prof. Sheng
 * 11/12/2013
 */
/**
 * This program is meant to be compiled by the XC8 compiler for the pic18F4520,
 * it is written to run on Microchips PICDEM 2 PLUS demo board.  The display
 * perform the following according to the homework description below:
 *
 * When the program is executed, the initial display should appear empty with
 * the display window and cursor in the home position.
 *
 * When the button on RA4 is pressed and released, two lines of the display
 * should first be loaded with up to 16 characters in each line in the visible
 * portion of the display.
 *
 * When the button is pressed and released a second time, the displayed
 * characters should scroll to the left by 16 characters so that the display
 * is empty.
 * 
 * A third button press should load a second pair of lines in the now visible
 * part of the display and those lines should scroll off with another button
 * press. This should be repeated for a third time.
 *
 * Finally, another button press should blank the display and start the process
 * over.
 *
 * @author : Alvin Baldemeca
 * Note: The code here was taken from the provided code by Dr. Sheng's
 * 4520LCD_2013.c , parts of the code was modified to be compatible with the
 * XC8 compiler.
 * @version 11.12.2013 *
 */


#include <p18f4520.h>
#include <xc.h>

#pragma config WDT = OFF
#pragma config LVP = OFF  // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

#define _XTAL_FREQ 20000000  //external occilator frequency used by __delay_ms etc.
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
#define BTN_A4 
#define READ 1
#define WRITE 0
#define S2_RELEASE PORTAbits.RA4
#define S2_PRESS !PORTAbits.RA4


void send_data(unsigned char data,unsigned char cflag,
unsigned char chk_busy,unsigned char dflag);
void epulse(void);
void load_string(const char *pt);

/*
 * The strings below are stored in program memory and used to display the
 * message to the LCD.
 */
const char* arr1 = "1111111111111111\0";
const char* arr2 = "*****Hello!*****\0";
const char* arr3 = "2222222222222222\0";
const char* arr4 = "***Greetings****\0";
const char* arr5 = "3333333333333333\0";
const char* arr6 = "***Line 2 ******\0";


unsigned char state = 0; //Used to hold the current state of the program.

/**
 * The main function contains switch statements that represents the 7 states
 * that the LCD could be in.  The program on the PICDEM 2 Plus board performs
 * the following:
 *
 * When the program is executed, the initial display should appear empty with
 * the display window and cursor in the home position.
 *
 * When the button on RA4 is pressed and released, two lines of the display
 * should first be loaded with up to 16 characters in each line in the visible
 * portion of the display.
 *
 * When the button is pressed and released a second time, the displayed
 * characters should scroll to the left by 16 characters so that the display
 * is empty.
 *
 * A third button press should load a second pair of lines in the now visible
 * part of the display and those lines should scroll off with another button
 * press. This should be repeated for a third time.
 *
 * Finally, another button press should blank the display and start the process
 * over.
 */
void main (void){
    
        int i = 0;
	LCD_TXRX = OUTPUT_ALL; //port direction write
	LCD_IO_DATA = 0x00; //port outputs 0
	LCD_PWR = ENABLE; //power to lcd

        TRISA4 = INPUT; //Set Port A4 to input
        //LATA4 = 0;
	//__delay_ms(1);
	send_data(0x20,1,0,0); //4-bit data
	send_data(0x28,1,0,1); //2 line display
	send_data(0x28,1,0,1); //repeat
	send_data(0x06,1,0,1); //enable display set to incrementing mode
	send_data(0x0c,1,0,1); //turn on display
	send_data(0x02,1,0,1); //clear display
        // __delay_ms(3);

	while(1){
            switch (state){
                case 0:
                send_data(0x01,1,0,1); //clear display and return to home position
                __delay_ms(3);
                while(S2_RELEASE); __delay_ms(10);
                while(S2_PRESS); __delay_ms(10);
                state = 1;
                break;

                case 1:
                load_string(arr1); //send string to lcd
                __delay_ms(3);
                send_data(0xc0,1,0,1); //set cursor to second line
                __delay_ms(3);
                load_string(arr2);__delay_ms(3);
                while(S2_RELEASE); __delay_ms(10);
                while(S2_PRESS); __delay_ms(10);
                state = 2;
                break;
        
                case 2:
                i = 0;
                while(S2_RELEASE)
                {
                    if( i < 16)
                    {
                        send_data(0x18,1,0,1); //scroll display
                        __delay_ms(10);
                        i++;
                    }
                }
                while(S2_PRESS); __delay_ms(10);
                send_data(0x01,1,0,1);
                state = 3;
                break;
                
                case 3:
                 load_string(arr3); //send string to lcd
                __delay_ms(3);
                send_data(0xc0,1,0,1); //set cursor to second line
                __delay_ms(3);
                load_string(arr4);__delay_ms(3);
                while(S2_RELEASE); __delay_ms(10);
                while(S2_PRESS); __delay_ms(10);
                state = 4;
                break;

                case 4:
                i = 0;
                while(S2_RELEASE)
                {
                    if( i < 16)
                    {
                        send_data(0x18,1,0,1); //scroll display
                        __delay_ms(10);
                        i++;
                    }
                }
                while(S2_PRESS); __delay_ms(10);
                send_data(0x01,1,0,1);
                state = 5;
                break;

                case 5:

                load_string(arr5); //send string to lcd
                __delay_ms(3);
                send_data(0xc0,1,0,1); //set cursor to second line
                __delay_ms(3);
                load_string(arr6);__delay_ms(3);
                while(S2_RELEASE); __delay_ms(10);
                while(S2_PRESS); __delay_ms(10);
                state = 6;
                break;

                case 6:
                i = 0;
                while(S2_RELEASE)
                {
                    if( i < 16)
                    {
                        send_data(0x18,1,0,1); //scroll display
                        __delay_ms(10);
                        i++;
                    }
                }
                while(S2_PRESS); __delay_ms(10);
                send_data(0x01,1,0,1);
                state = 0;
                break;

            }
	}
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
            __delay_ms(10);

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
