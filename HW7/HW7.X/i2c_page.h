/* 
 * File:   i2c_page.h
 * Author: Alvin Baldemeca
 *
 * Created on November 23, 2013, 2:01 PM
 */

#ifndef I2C_PAGE_H
#define	I2C_PAGE_H

#ifdef	__cplusplus
extern "C" {
#endif
int write_byte(unsigned char data, unsigned char msb_add, unsigned char lsb_add);
int read_byte(unsigned char* rbyte, unsigned char msb_add, unsigned char lsb_add);
int write_page(char *the_page, unsigned char msb_add, unsigned char lsb_add);
int read_page(char *the_page, unsigned char msb_add, unsigned char lsb_add);
#ifdef	__cplusplus
}
#endif

#endif	/* I2C_PAGE_H */

