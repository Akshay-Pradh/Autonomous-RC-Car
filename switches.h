/*
 * switches.h
 *
 *  Created on: Feb 11, 2025
 *      Author: Akshay Pradhan
 */

#ifndef SWITCHES_H_
#define SWITCHES_H_

#define PRESSED             (0)
#define RELEASED            (1)
#define DEBOUNCE_TIME      (200)
#define DEBOUNCE_RESTART    (0)
#define OKAY                (1)
#define NOT_OKAY            (0)

void Switches_Process(void);
void Switch1_Process(void);
void Switch2_Process(void);


#endif /* SWITCHES_H_ */
