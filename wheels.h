/*
 * wheels.h
 *
 *  Created on: Feb 27, 2025
 *      Author: Akshay Pradhan
 */

#ifndef WHEELS_H_
#define WHEELS_H_

void Wheels_Process(void);

void Forward_On(void);
void Forward_Off(void);
void Reverse_On(void);
void Reverse_Off(void);
void Spin_Clock(void);
void Motors_Off(void);

void Circle_Path(void);
void Circle_Adjust_IN(void);
void Circle_Adjust_OUT(void);



#endif /* WHEELS_H_ */
