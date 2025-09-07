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

unsigned char allow_PC_TX;            // Enable PC communication flag

volatile unsigned int usb_rx_ring_wr;   // This is an index for Writing the character into the usb ring buffer
volatile unsigned int usb_rx_ring_rd;   // This is an index for Reading the character out of the usb ring buffer

volatile unsigned int iot_rx_ring_wr;   // This is an index for Writing the character into the iot ring buffer
volatile unsigned int iot_rx_ring_rd;   // This is an index for Reading the characer out of the iot ring buffer

extern long currentBaudRate;            // global current baud rate

char USB_Char_Rx[16];                   // character ring buffer for USB serial receiving
char IOT_Char_Rx[16];                   // character ring buffer for IOT serial receiving

char transmit_array[32];                // character array for transmitting data
extern unsigned int t_index;
extern unsigned char transmitting_message;  // transmitting message flag for UCA1

extern unsigned char message_status;             // message status for PROJECT08

unsigned char command_received;
char process_buffer[4][10];
extern unsigned int pb_index_row;
extern unsigned int pb_index_col;
unsigned int process_row;
extern unsigned char process_command;   // Flag for commands process to let it know we need to process a command in the process row

//------------------------------------------------------------------------------
// IOT COMMANDS
//------------------------------------------------------------------------------

unsigned char iot_command_received;
extern unsigned char iot_command_complete;  // flag to make sure previous command is complete

extern unsigned char IOT_reset;
extern unsigned char IOT_line_ready;

extern unsigned char iot_okay_check;
extern unsigned char iot_server_check;
extern unsigned char iot_mux_check;

char IOT_buffer[4][55];
extern unsigned int iot_index_row;
extern unsigned int iot_index_col;
unsigned int iot_row;

char iot_transmit_array[32];
extern unsigned int iot_t_index;

extern unsigned int IOT_parse;
extern unsigned int ip_address_found;

char ip_address1[10];
char ip_address2[10];

char ssid_display[10];
extern unsigned int ssid_found;

// IOT Commands
extern char AT[4];
extern char ip_mac[10];
extern char ssid[12];
extern char mux[13];
extern char server[22];

// Internet Pinging
extern unsigned char ping_internet;
extern unsigned char Start_Pinging;
extern unsigned int ping_count;

//------------------------------------------------------------------------------
// DAC
// -----------------------------------------------------------------------------
unsigned int DAC_data;

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
extern volatile char ADC_DISPLAY;
extern unsigned char CALIBRATE;
extern volatile unsigned int ADC_Channel;
volatile unsigned int ADC_Left_Detect;
volatile unsigned int ADC_Right_Detect;
volatile unsigned int ADC_Thumb;

//-------------------------------------------------------------------------------
// WHEELS
// ------------------------------------------------------------------------------
extern unsigned char wheel_event;       // for basic movement state machine
extern unsigned char config;

unsigned int TARGET_SPEED_RIGHT;
unsigned int TARGET_SPEED_LEFT;

unsigned int RADIUS_TIME;
unsigned int SPIN_TIME;

extern unsigned char movement_letter;   // for choice of movement state machine
extern unsigned int movement_duration;

extern unsigned char intercept_event;   // for interception movement

//-------------------------------------------------------------------------------
// LINE DETECTION
// ------------------------------------------------------------------------------
extern unsigned int BLACK_THRESHOLD;
extern unsigned int WHITE_THRESHOLD;
extern unsigned int GREY_THRESHOLD;

extern unsigned char TRAVELLING;


#endif /* GLOBALS_H_ */
