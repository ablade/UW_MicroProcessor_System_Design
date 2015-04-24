/*
 * Author  : Alvin Baldemeca
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
 * FileName:        		i2c_page.c
 * Dependencies:    	See include below , PICDEM2 PLUS demo board
 * Processor:       		PIC18F4520
 * Compiler:        		XC8
 *
 * Synopsis :
 * #include "i2c_page.c"
 *   write_byte();
 *   read_byte();
 *   write_page();
 *   read_page();
 *
 * Description :
 *   write_byte() - write a byte of data to EEPROM (24LC256)
 *   read_byte()  - read a byte of data from EEPROM (24LC256)
 *   write_page() - write a page (64 bytes) of data to EEPROM (24LC256)
 *   read_page()  - read a page (64 bytes) of data from EEPROM (24LC256)
*******************************************************************************/
#include "global.h"
#include "i2c.h"

/**
 * Write a byte of data to EEPROM (24LC256)
 * @param data - the data to write
 * @param msb_add - the most significant byte of the EEPROM's memory address
 * @param lsb_add - the least significant byte of the EEPROM's memory address
 * @return 0 on success
 */
int write_byte(unsigned char data, unsigned char msb_add, unsigned char lsb_add)
{
    i2c_start();
    while(i2c_send(WRITE_EEPROM))
    {
       i2c_rstart();
    }
    while(i2c_send(msb_add));
    while(i2c_send(lsb_add));
    while(i2c_send(data));
    i2c_stop();
    return 0;
}

/**
 * Read a byte of data from EEPROM (24LC256)
 * @param rbyte - the pointer to the char to store the recieved data
 * @param msb_add - the most significant byte of the EEPROM's memory address
 * @param lsb_add - the least significant byte of the EEPROM's memory address
 * @return 0 on success
 */
int read_byte(unsigned char* rbyte, unsigned char msb_add, unsigned char lsb_add)
{
    i2c_start();
    while(i2c_send(WRITE_EEPROM))
    {
        i2c_rstart();
    }
    i2c_send(msb_add);
    i2c_send(lsb_add);
    i2c_rstart();
    i2c_send(READ_EEPROM);
    *rbyte = i2c_receive(1);
    i2c_idle();
    i2c_stop();
    return 0;
}

/**
 * Write a page (64 bytes) of data to EEPROM (24LC256)
 * @param the_page - the pointer to the page to write to the EEPROM
 * @param msb_add - the most significant byte of the EEPROM's memory address
 * @param lsb_add - the least significant byte of the EEPROM's memory address
 * @return 0 on success
 */
int write_page(char *the_page, unsigned char msb_add, unsigned char lsb_add)
{
    i2c_start();                        // Send START condition
    while(i2c_send(WRITE_EEPROM))
    {
       i2c_rstart();
    }
    i2c_send(msb_add);
    i2c_send(lsb_add);
    int i;
    for(i=0; i < PAGESIZE; i++)
    {
        i2c_send(the_page[i]);
    }
    i2c_stop();
    return 0;
}

/**
 * Read a page (64 bytes) of data from EEPROM (24LC256)
 * @param message - the pointer to store the data recieved from the EEPROM
 * @param msb_add - the most significant byte of the EEPROM's memory address
 * @param lsb_add - the least significant byte of the EEPROM's memory address
 * @return 0 on success
 */
int read_page(char *message, unsigned char msb_add, unsigned char lsb_add)
{
    i2c_start();
    while(i2c_send(WRITE_EEPROM))
    {
        i2c_rstart();
    }
    i2c_send(msb_add);
    i2c_send(lsb_add);
    i2c_rstart();
    i2c_send(READ_EEPROM);
    int i;
    for(i=0; i < PAGESIZE; i++)
    {
       i2c_ack(1);  //This was done for timing reasons
       message[i] = i2c_receive();
    }
    i2c_idle();
    i2c_stop();
    return 0;

}

