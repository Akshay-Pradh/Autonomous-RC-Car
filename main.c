//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "macros.h"

//void main(void){
void main(void){
//    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
    PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings

  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
  Init_ADC();                          // Initialize ADC (start reading values)
  Init_Serial_UCA0();                  // Initialize UCA0
//  Init_Serial_UCA1();                  // Initialize UCA1
  //P2OUT &= ~RESET_LCD;

  // Place the contents of what you want on the display, in between the quotes
  // Limited to 10 characters per line
  strcpy(display_line[0], "          ");
  strcpy(display_line[1], "          ");
  strcpy(display_line[2], "          ");
  strcpy(display_line[2], "460,800 Hz");
  display_changed = TRUE;
//  Display_Update(0,0,0,0);

  // Defaults:
//  wheel_move = 0;
//  forward = TRUE;
//  event = IDLE;

//  P2OUT |= IR_LED;        // testing IR LED
//------------------------------------------------------------------------------
// Beginning of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
    Switches_Process();                // Check for switch state change
    Display_Process();                 // Update Display
    Wheels_Process();                  // Check for Wheel Changes

    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF

    // message status -> switch case
    switch(message_status) {
        case COMPLETE:      // display the default status WAITING
            strcpy(display_line[0], "WAITING...");
            display_changed = TRUE;
            break;
        case RECEIVED:      //
            strcpy(display_line[0], " RECEIVED ");
            strcpy(display_line[1], "          ");          // clear line 2 of the display before moving
            display_changed = TRUE;
            break;
        case TRANSMIT:
            strcpy(display_line[0], " TRANSMIT ");
            display_changed = TRUE;
            break;
        case TRANSMITTED:
            strcpy(display_line[3], "          ");          // clear line 4 of the display
            strncpy(display_line[1], transmit_array, 10);   // display transmitted string to display line 2 (move from 4)
            message_status = COMPLETE;                      // return back to message status = COMPLETE
            break;
        default: break;
    }

    // check if rd != wr for IOT ring buffer
    if (iot_rx_ring_rd != iot_rx_ring_wr) {
        // we move character from ring buffer to process buffer
        process_buffer[pb_index_row][pb_index_col] = IOT_Char_Rx[iot_rx_ring_rd];
        if (IOT_Char_Rx[iot_rx_ring_rd] == '\n') {
            display_row = pb_index_row;     // save the row index for displaying
            pb_index_row++;                 // move to the next row
            if (pb_index_row >= ROWS) pb_index_row = 0;
            pb_index_col = 0;               // reset the column to 0

            // Clear the following row
            int i;
            for (i = 0; i < COLUMNS; i++) {         // clear the next line
                process_buffer[pb_index_row][i] = 0x00;
            }
            // Process received message: display on line 4
            strncpy(display_line[3], process_buffer[display_row], 10);      // displays 10 characters (not including \n)
            // Process received message: send to the transmit array
            strncpy(transmit_array, process_buffer[display_row], 11);       // copied the \n character
            display_changed = TRUE;
        }
        else {
            pb_index_col++;     // increment process buffer pointer
            // dont check for the end of column (assume that process buffer long enough to store any message)
        }

        iot_rx_ring_rd++;   // increment IOT ring rd index
        if (iot_rx_ring_rd >= (sizeof(IOT_Char_Rx))) iot_rx_ring_rd = BEGINNING;   // Circular buffer back to beginning
    }

  }

}
