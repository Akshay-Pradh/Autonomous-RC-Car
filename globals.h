/*
 * globals.h
 *
 *  Created on: Feb 11, 2025
 *      Author: Akshay Pradhan
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "macros.h"

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
// SERIAL
//------------------------------------------------------------------------------

volatile unsigned int usb_rx_ring_wr;   // This is an index for Writing the character into the array
volatile unsigned int usb_rx_ring_rd;   // This is an index for Reading the character out of the array

extern long currentBaudRate;            // global current baud rate

char USB_Char_Rx[16];                   // character buffer for USB serial receiving
char transmit_array[32];                // character array for transmitting data
extern unsigned int t_index;

extern char NCSU_array[9];
char process_buffer[4][32];
int display_row;
extern unsigned int pb_index_row;
extern unsigned int pb_index_col;


//------------------------------------------------------------------------------
// SHAPES
//------------------------------------------------------------------------------
volatile unsigned int Last_Time_Sequence;     // a variable to identify Time_Sequence has changed
volatile unsigned int cycle_time;             // is a new time base used to control making shapes
volatile unsigned int time_change;            // is an identifier that a change has occurred

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
extern volatile unsigned int Time_Precise;
extern volatile unsigned int Curr_Time;
extern unsigned char TIME_DISPLAY;
unsigned int START_TIME;

//-------------------------------------------------------------------------------
// ADC
// ------------------------------------------------------------------------------
char adc_char[4];
volatile char ADC_DISPLAY;
extern volatile unsigned int ADC_Channel;
volatile unsigned int ADC_Left_Detect;
volatile unsigned int ADC_Right_Detect;
volatile unsigned int ADC_Thumb;

//-------------------------------------------------------------------------------
// WHEELS
// ------------------------------------------------------------------------------
extern unsigned char wheel_event;
extern unsigned char config;
unsigned int TARGET_SPEED_RIGHT;
unsigned int TARGET_SPEED_LEFT;
unsigned int RADIUS_TIME;
unsigned int SPIN_TIME;

//-------------------------------------------------------------------------------
// LINE DETECTION
// ------------------------------------------------------------------------------
extern unsigned int BLACK_THRESHOLD;
extern unsigned int WHITE_THRESHOLD;


#endif /* GLOBALS_H_ */
