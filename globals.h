/*
 * globals.h
 *
 *  Created on: Feb 11, 2025
 *      Author: apcoo
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

/*
 * globals.c
 *
 *  Created on: Jan 30, 2025
 *      Author: Akshay Pradhan
 */

volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
unsigned int test_value;
char chosen_direction;
char change;

unsigned int wheel_move;
char forward;


volatile unsigned int count_debounce_SW1;
volatile unsigned int count_debounce_SW2;

//------------------------------------------------------------------------------
// SHAPES
//------------------------------------------------------------------------------
volatile unsigned int Last_Time_Sequence;     // a variable to identify Time_Sequence has changed
volatile unsigned int cycle_time;            // is a new time base used to control making shapes
volatile unsigned int time_change;           // is an identifier that a change has occurred

volatile unsigned char event;
volatile unsigned int time_change;

volatile unsigned int right_motor_count;
volatile unsigned int left_motor_count;
volatile unsigned int segment_count;

//-------------------------------------------------------------------------------
// PORTS
// ------------------------------------------------------------------------------

#endif /* GLOBALS_H_ */
