/*
 * serial.h
 *
 *  Created on: Mar 26, 2025
 *      Author: Akshay Pradhan
 */

#ifndef SERIAL_H_
#define SERIAL_H_

void Init_Serial_UCA0(void);
void Init_Serial_UCA1(void);

void setBaudRate(long baud);
void setBaudRateUCA0(long baud);
void setBaudRateUCA1(long baud);



#endif /* SERIAL_H_ */
