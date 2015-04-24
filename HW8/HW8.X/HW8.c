/*
 * Alvin Baldemeca
 * HW 8
 * University of Washington Tacoma
 * TCES 430, Prof. Sheng
 * 12.3.2013
 *
 */

#include <p18f4520.h>
#include <stdio.h>
#include <stdlib.h>
#pragma config WDT = OFF  // Set watch dot timer off
#pragma config LVP = OFF  // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config PBADEN = OFF // Turn off PortB A/D
#pragma config OSC = INTIO67  //Internal frequency is 1MHz by default
#define VDD_VOLTAGE 5.0
#define MAX_DVALUE 1023
#define LCD_LINE2 0XC0
#define LCD_HOME 0X01



void init_ADCM(void);
void init_LCD(void);
void send_data(unsigned char,unsigned char,
unsigned char, unsigned char);
void epulse(void);
void delayms(unsigned char);
void delayus(unsigned char);
void load_string(const char *pt);
int volatile flag = 0;

//complete the interrupt service routine
//and make it brief
void interrupt RA0_isr(void)
{
    if(ADIF)
    {
        flag = 1; //Used to indicate that the analog conversion is done
        ADIF = 0; //Reset interrupt flag
    }

}

/*
 * The main function takes the analog voltage measured the potentiometer
 * connected to A0 and converts it to a digital value displayed on the LCD
 * of the PICDEM2
 */
void main (void)
{
    unsigned int adc_result = 0; //Use to hold the result of the ADC
    float voltage; //Used to hold the converted voltage
    unsigned char buffer[16]; //Use to hold the string representation of voltage
    init_ADCM(); //initializes the A/D converter module

    init_LCD(); //initialize the LCD on PICDEM2 board
    INTCONbits.GIE = 1;//to enable global interrupts
    INTCONbits.PEIE = 1;//to enable interrupts from peripheral devices
    PIE1bits.ADIE = 1;//to enable interrupts from A/D converter
    PIR1bits.ADIF = 0;//Clear interrupt flag

    //Develop codes here to accomplish the required task
    while(1)
    {
          ADCON0bits.GO_NOT_DONE = 1;  //Convert ADC
          if(flag) //If conversion is finished
          { 
           flag = 0; //Reset flag

           /*
            * Shift ADDRESH 8-bits and bit-OR the lower 8 bytes, we use this to
            * combine the two register in to an "unsinged int" type
            */
            adc_result = (((unsigned int)ADRESH)<<8)|(ADRESL);

            /*
             * convert the converted data into voltage
             */
            voltage = (adc_result*VDD_VOLTAGE)/MAX_DVALUE;


            load_string("RA0 Voltage : \0"); //Display "Voltage" on the screen
            send_data(LCD_LINE2,1,0,1); //set cursor to second line
            sprintf(buffer, "%.3g\0", voltage ); // Convert voltage to string
            load_string(buffer); //Display the Voltage on the screen
            delayms(200); //Needed so we can see the displayed voltage
            send_data(LCD_HOME,1,1,1); //Home position on LCD
          }
    }
}

//initializes the A/D converter module
void init_ADCM(void)
{
	//Setting the Analog channel Select bits to channel 0(AN0)
	ADCON0bits.CHS3 = 0;
	ADCON0bits.CHS2 = 0;
	ADCON0bits.CHS1 = 0;
	ADCON0bits.CHS0 = 0;

	//sets RA0 to analog
	ADCON1 = 0X00;

	//sets the A/D result format either right or left justified
	ADCON2bits.ADFM = 1; //Right justified

	//Sets the three bits for acquisition time
	ADCON2bits.ACQT2 = 0; ////////////////////////////////////////
	ADCON2bits.ACQT1 = 1; //4 instruction Period for acquisition
	ADCON2bits.ACQT0 = 0; ////////////////////////////////////////

	//Sets the A/D conversion clock
	ADCON2bits.ADCS2 = 1;  /////////////////
	ADCON2bits.ADCS1 = 0;  //FOSC/4
	ADCON2bits.ADCS0 = 0;  /////////////////

	//To turn on the A/D converter module
	ADCON0bits.ADON = 1;
}

//initializes LCD on PICDEM2
void init_LCD(void)
{
	TRISD = 0x00; //port direction write
	PORTD = 0x00; //port outputs 0
	PORTDbits.RD7 = 1; //power to lcd
	delayms(100);
	send_data(0x20,1,0,0); //4-bit data
	send_data(0x28,1,0,1); //2 line display
	send_data(0x28,1,0,1); //repeat
	send_data(0x06,1,0,1); //enable display
	send_data(0x0c,1,0,1); //turn on display
	send_data(0x02,1,0,1); //clear display
}

//Send a byte of data to the LCD
void send_data(unsigned char data,unsigned char cflag,
unsigned char chk_busy, unsigned char dflag){
	char c,d,bflag;
	if(chk_busy){
		TRISD = 0x0F; //read on lower 4 bits
		PORTDbits.RD4 = 0; //RS=0, control signal
		PORTDbits.RD5 = 1; //read
		bflag = 1; //set busy flag

		while(bflag){
			epulse(); //read lower 4 bits
			delayus(1);
			PORTDbits.RD6=1; //E high
			delayus(1);
			bflag = PORTDbits.RD3; //read busy signal
			delayus(1);
			PORTDbits.RD6=0; //E low
			epulse(); //read lower 4 bits
		}
	}
	else
		delayms(10);

	TRISD = 0x00; //write from port
	if(cflag)
		PORTDbits.RD4 = 0; //control signal
	else
		PORTDbits.RD4 = 1; //data
	PORTDbits.RD5 = 0; //write
	c = data>>4; //shift upper nibble to lower
	d = PORTD & 0xF0; //clear lower nibble of port
	PORTD = d | c; //add lower bits for data or command
	epulse();
	if (dflag){ //send lower nibble if not 4 bit control
		c = data & 0x0F; //clear upper nibble of data
		d = PORTD & 0xF0; //clear lower nibble of port
		PORTD = d | c; //send lower nibble
		epulse();
	}
return;
}


void epulse(void){
	delayus(1);
	PORTDbits.RD6=1; //E high
	delayus(1);
	PORTDbits.RD6=0; //E low
	delayus(1);
	return;
}


void delayms(unsigned char cnt){
	static unsigned char i,j;
	for (i=0; i<cnt; i++){
		j=245; //approx. 1 ms delay at 4 MHz
		asm("loop: nop");
		asm("decfsz 245,1,1");
		asm("bra loop");
	}
	return;
}

void delayus(unsigned char cnt){
	static unsigned char i; //long us delay!
	for (i=0; i<cnt; i++);
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
