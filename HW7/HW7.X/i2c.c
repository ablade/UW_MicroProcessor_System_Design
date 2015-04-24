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
 * FileName:        		i2c.c
 * Dependencies:    	See include below , PICDEM2 PLUS demo board
 * Processor:       		PIC18F4520
 * Compiler:        		XC8
 *
 * Synopsis :
 *  #include "i2c.c"
 *   i2c_idle();
 *   i2c_start();
 *   i2c_rstart();
 *   i2c_stop();
 *   i2c_ack(unsigned char ackbit);
 *   i2c_send(unsigned char data);
 *   i2c_receive();
 *   i2c_init()
 *
 * Description :
 *   i2c_idle()   - checks for idle conditions
 *   i2c_start()  - sends a start condition
 *   i2c_rstart() - sends a restart condition
 *   i2c_stop()   - sends a stop condition
 *   i2c_ack(unsigned char ackbit) - sends acknowldege or no acknowledge
 *   i2c_send(unsigned char data) - send a byte of data
 *   i2c_receive() - recieves a byte of data
 *   i2c_init() - initializes microcontroller for I2C
*******************************************************************************/



#include "global.h"

/**
 * Used for Inter-Integrated Circuit (I2C) communication to check for when the
 * lines are idel
 */
void i2c_idle(void){
    while((SSPCON2 & 0x1F)| SSPSTATbits.R_NOT_W); //Check all flags for idle
    return;
}

/**
 * Used for Inter-Integrated Circuit (I2C) to have the micro-controller send a
 * start condition SDA (LOW to HIGH), SCL (HIGH).  This is a feature of the
 * PIC18F microcontroler i.e. SSCON2bits.SEN.
 */
void i2c_start(void){
	i2c_idle(); 	        //wait for idle bus
	SSPCON2bits.SEN	= 1;	//initiate start
	while(SSPCON2bits.SEN); //and wait for start to finish
	return;
}

/**
 * Used for Inter-Integrated Circuit (I2C) to have the micro-controller send a
 * re-start condition SDA (LOW to HIGH), SCL (HIGH) with out issuing a stop.
 * This is a feature of the PIC18F microcontroler i.e. SSCON2bits.RSEN
 */
void i2c_rstart(void){
	i2c_idle(); 	                //wait for idle bus
	SSPCON2bits.RSEN=1;
	while(SSPCON2bits.RSEN);	//and wait for restart to finish
	return;
}

/**
 * Used for Inter-Integrated Circuit (I2C) to have the micro-controller send a
 * stop condition SDA (HIGH to LOW), SCL (HIGH).  This is a feature of the
 * PIC18F microcontroler i.e. SSCON2bits.PEN
 */
void i2c_stop(void){
	i2c_idle();	            //wait for idle bus
	SSPCON2bits.PEN= 1;	    //initiate stop
	while(SSPCON2bits.PEN);     //and wait for stop to finish
	return;
}

/**
 * Used for Inter-Integrated Circuit (I2C) to have the micro-controller send a
 * ACKNOWLEDGE or NO-ACKNOWLEDGE. stop condition SDA (HIGH to LOW), SCL (HIGH).
 * This is a feature of the PIC18F microcontroler i.e. SSCON2bits.ACKDT
 *
 * @param ackbit - 1 for acknowledge, 0 for no-acknowlege
 */
void i2c_ack(unsigned char ackbit){
    if ( ackbit )
    {
        SSPCON2bits.ACKDT=0; //set acknowledge bit
    }
    else
    {
        SSPCON2bits.ACKDT=1; //set no acknowledge bit
    }
    i2c_idle();
    ACKEN=1; //initiate acknowledgement
    i2c_idle(); //wait for ack to finish
    return;
}

/**
 * Used for Inter-Integrated Circuit (I2C) to have the micro-controller send
 * data to a slave device (client for the politically correct).
 *
 * @param data - the data to send
 */
unsigned char i2c_send(unsigned char data)
{
    i2c_idle();
    SSPBUF = data;           // write single byte to SSPBUF
    if ( SSPCON1bits.WCOL )      // test if write collision occurred
       return ( 1 );              // if WCOL bit is set return 1
    else
    {
      while( SSPSTATbits.BF );   // wait until write cycle is complete
      i2c_idle();                 // ensure module is idle
      if ( SSPCON2bits.ACKSTAT ) // test for ACK condition received
        return ( 2 );			// return NACK
      else return ( 0 );              //return ACK
    }
}

/**
 * Used for Inter-Integrated Circuit (I2C) to have the micro-controller recieve
 * data from a slave device (client for the politically correct).
 *
 * @return - the recieved byte
 */
unsigned char i2c_receive(){
    unsigned char data;
    i2c_idle();
    SSPCON2bits.RCEN = ENABLE;// enable master for 1 byte reception
    while(SSPCON2bits.RCEN);
    while (!SSPSTATbits.BF); // wait until byte received
    data = SSPBUF;
    //  i2c_ack(ackbit);
  return ( data );
}

/**
 * @breif Initializes the PIC18F micro-controller for Master I2C mode.
 *
 * TODO - should make this a macro since its only used once, makes the main func
 * bigger but should initialize faster (obsessed with speed!!!)
 */
void i2c_init(void){

    OSCCONbits.IRCF = 111; //8MHz Internal clock
    /*
     * Enable Master Synchronous Serial Port and configures SDA and SCL pins as
     * serial port pins for I2C protocol.
     *
     * SSPCON1 = I2C_M_E_CONF;  Equivalent to code below (faster)
     */
    SSPCON1bits.SSPEN = ENABLE;
    SSPCON1bits.SSPM = I2C_M;  //initialize I2C mode

    /*
     * This is not needed if used on the PICDEM2 Plus board but is good practice
     * as for manufacturer's recomendations
     */
    SSPSTATbits.CKE = ENABLE; //Enable SMBus specific inputs
    SSPSTATbits.SMP = ENABLE; //Disable maximum rate of change of output voltage per unit of time

    /*
     * Set SDA and CLK as inputs on the micro-controller
     */
    TRISC3 = INPUT; //CLOCK
    TRISC4 = INPUT; //DATA

    SSPADD = BAUD_400KHZ; // 8MHZ /(4*( 0X04 + 1) = 400KHZ, SSPAD = 0X04
    return;
}
