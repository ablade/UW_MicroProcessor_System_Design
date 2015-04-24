/* 
 * File:   i2c.h
 * Author: Alvin Baldemeca
 *
 * Created on November 23, 2013, 1:59 PM
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

void i2c_idle(void);
void i2c_start(void);
void i2c_rstart(void);
void i2c_stop(void);
void i2c_ack(unsigned char ackbit);
unsigned char i2c_send(unsigned char data);
unsigned char i2c_receive();
void i2c_init(void);



#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

