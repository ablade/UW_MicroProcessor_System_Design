/* 
 * File:   global.h
 * Author: Alvin Baldemeca
 *
 * Created on November 23, 2013, 2:04 PM
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <p18f4520.h>
#include <plib/i2c.h>
#include <xc.h>

#pragma config WDT = OFF  // Set watch dot timer off
#pragma config LVP = OFF  // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config PBADEN = OFF // Turn off PortB A/D
#pragma config OSC = INTIO67  //Internal frequency is 1MHz by default
#define _XTAL_FREQ 8000000  //used by __delay_ms this should match what we set our clock to
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
#define I2C_MASTER   0b00001000 //I2C Master mode, clock = FOSC/(4*(SSPAD + 1)
#define I2C_M 8 //I2C Master mode, clock = FOSC/(4*(SSPAD + 1
#define I2C_M_E_CONF 0b00101000 //I2C Master mode, clock = FOSC/(4*(SSPAD + 1) and SSPEN = 1
#define I2C_M 8
#define BAUD_400KHZ 0X04 //24LC256 EEPROM can handle 400KHz (Speed is good)
#define WRITE_EEPROM 0b10100000
#define READ_EEPROM  0b10100001
#define PAGESIZE 64
#define SCROLL_SPEED 85

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBAL_H */

