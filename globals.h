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

//------------------------------------------------------------------------------
// SHAPES
//------------------------------------------------------------------------------
volatile unsigned int Last_Time_Sequence;     // a variable to identify Time_Sequence has changed
volatile unsigned int cycle_time;            // is a new time base used to control making shapes
volatile unsigned int time_change;           // is an identifier that a change has occurred

extern volatile unsigned char event;
extern volatile unsigned char state;

volatile unsigned int right_motor_count;
volatile unsigned int left_motor_count;
volatile unsigned int segment_count;

//-------------------------------------------------------------------------------
// PORTS
// ------------------------------------------------------------------------------

volatile unsigned int debounce_count_sw1;
volatile unsigned char debounce_in_progress_sw1;
volatile unsigned char sw1_pressed;

volatile unsigned int debounce_count_sw2;
volatile unsigned char debounce_in_progress_sw2;
volatile unsigned char sw2_pressed;

//-------------------------------------------------------------------------------
// TIMERS
// ------------------------------------------------------------------------------

extern volatile unsigned int Time;
extern volatile unsigned int Curr_Time;

#endif /* GLOBALS_H_ */
