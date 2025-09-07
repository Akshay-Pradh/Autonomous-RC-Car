/*
 * wheels.h
 *
 *  Created on: Feb 27, 2025
 *      Author: Akshay Pradhan
 */

#ifndef WHEELS_H_
#define WHEELS_H_

void Wheels_Process(void);
void Motion_Process(void);
void Intercept_Process(void);

void Forward_On(void);
void Forward_Off(void);
void Reverse_On(void);
void Reverse_Off(void);
void Spin_Clock(void);
void Spin_Counter_Clock(void);
void Motors_Off(void);

void Curve_Right(void);
void Forward_Fast_On(void);
void Follow_Path(void);
void Follow_Adjust_IN(void);
void Follow_Adjust_OUT(void);



#endif /* WHEELS_H_ */
