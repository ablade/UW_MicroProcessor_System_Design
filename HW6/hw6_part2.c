/*
 * Alvin Baldemeca
 * UWT TCES 430
 * Homework 6 part 2
 * Prof. Sheng
 *
 *
 * Processor: 	   PIC18F4520
 * Compiler: 	   XC8 (MICROCHIP)
 *
 * This program should be compiled using the XC8 compiler.  The micro-controller
 * used to run this program is the PIC18F4520 using the MICROCHIP PICDEM PLUS 2
 * Demo board.  PortA0 is suppose to blink an LED, which on for 0.20 seconds
 * the off for 0.60 seconds for 20 seconds and finally turns off.
 */


#include <stdio.h>
#include <stdlib.h>
#include <p18f4520.h>
#include <xc.h>

#pragma config OSC = INTIO67  //Internal frequency is 1MHz
#pragma config WDT = OFF  // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config LVP = OFF  // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config PBADEN = OFF // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)

#define INPUT 1
#define OUTPUT 0
#define INPUT_ALL 0xFF
#define OUTPUT_ALL 0x00
#define ENABLE 1
#define DISABLE 0
#define PORT_A0 TRISAbits.RA0
#define ON 1
#define OFF 0
#define SET 1
#define CLEAR 0
#define LED_A0 LATA0
#define PRIORITY_INTRP RCONbits.IPEN
#define CYCLES 4

/*
 * This macro sets timer2 as an interrupt with a prescale of 16 and post scale
 * at 16 with timer count to match at 196 counts before timer 2 interrupt.
 * Time for every interrupt is  calculated using the formula :
 * PRE * (PR2+1) * POST * (1/(Fosc/4))
 * (16 *(195 + 1)* 16 * 4 / 1MHz seconds
 * Internal Frequency of the PIC micro-controller is set to 1MHz so timer2
 * interrupts at every 0.20 sec or 200ms.
 */
#define INIT_TMR2_20MSEC()   \
        INTCONbits.PEIE = 1; \ //enable peripheral interrupts
	IPR1bits.TMR2IP = 0; \ //set to low priority interrupt
        PIE1bits.TMR2IE = 1; \ //clear timer2 flag
	T2CON = 0X7E;        \ //set tim2r2 prescalar & post scalar (16, 16)
        PR2=0xC3;             // Compare clock tick count to 195


volatile unsigned char flag = CLEAR;

/*
 * This ISR (interupt service routine) sets a flag every 0.2 sec.  This ISR
 * is set as a low priority incase we need to add a higher priority interrupt
 * if needed we could set this to high_priority.
 */
void interrupt low_priority timerInt(void)
{
    //Check if the interrupt is timer 2
    if(PIR1bits.TMR2IF)
    {
        PIR1bits.TMR2IF = CLEAR; //Clear timer 2 flag
        flag = SET; //Set flag
        
    }
}

/*
 * The main function sets PortA0 as an output to connect to a light emitting
 * diode.  It then sets up timer 2 interrupt.  The LED or PortA0 blinks such
 * that it is on for 0.20 second and off 0.60 second for 20 seconds then turns
 * off.
 */
int main() {

    int j = 0;
    int k = 1;

    PORT_A0 = OUTPUT; //set PortA0 as output for the LED
    LED_A0 = OFF; //Set the output to LOW/OFF
    PRIORITY_INTRP = ENABLE; //enable priority interrupt
    INIT_TMR2_20MSEC();
    ei(); //The same as INTCONbits.GIE = 1 or enable interrupt;
    for(;;)
    {
        if(flag & k)
        {
           flag = 0;
           if((j%CYCLES) == 0 ) //Happens once in 4 cycles. (0.2 sec On)
           {
               LED_A0 = ON;  //Output high on PortA0
           }else
           {   //off for 0.6 sec
               LED_A0 = OFF; //Output low on PortA0
           }
           j++;
           if(j>= 100) //Ensure that the LED stops blinking after 20sec(0.2 sec * 100 = 20 sec)
           {
               k = 0;
           }
        }
    }
    return (EXIT_FAILURE); //Code should never get here if it did an error occured
}



