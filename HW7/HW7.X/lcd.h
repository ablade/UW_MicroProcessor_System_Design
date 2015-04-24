/* 
 * File:   lcd.h
 * Author: Alvin Baldemeca
 *
 * Created on November 23, 2013, 1:55 PM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

void send_data(unsigned char data,unsigned char cflag,
               unsigned char chk_busy,unsigned char dflag);
void epulse(void);
void load_string(const char *pt);



#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

