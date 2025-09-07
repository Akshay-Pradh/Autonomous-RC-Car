/*
 * switches.c
 *
 *  Created on: Feb 11, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

// Globals
volatile unsigned int sw1_toggle = 0;
volatile unsigned int sw2_toggle = 0;

void Switches_Process(void) {
    //------------------------------------------------------------------------------
    // This function calls the individual Switch Functions
    //------------------------------------------------------------------------------
     Switch1_Process();
     Switch2_Process();
    //------------------------------------------------------------------------------
}

void Switch1_Process(void){
    //------------------------------------------------------------------------------
    // Switch 1 Configurations
    // Port P4 Pin 1
    //------------------------------------------------------------------------------

     if (sw1_pressed){
                 // SW1 is pressed
                 sw1_pressed = NO;
                 sw1_toggle++;

                 if (sw1_toggle == 1) {
                    strcpy(display_line[2], "115,200 Hz");
                    display_changed = TRUE;
                    setBaudRate(115200);                        // set our baud rate to 115,200 Hz

                 }
                 else if (sw1_toggle == 2) {
                    strcpy(display_line[2], "460,800 Hz");
                    display_changed = TRUE;
                    setBaudRate(460800);                        // set our baud rate to 460,800 Hz
                 }
                 else sw1_toggle = 0;
     }
}

void Switch2_Process(void){
    //------------------------------------------------------------------------------
    // Switch 2 Configurations
    // Port 2 Pin 3
    //------------------------------------------------------------------------------
    if (sw2_pressed){
                  // SW2 is pressed
                  sw2_pressed = NO;
                  if (message_status == RECEIVED) {
                      message_status = TRANSMIT;    // change the message status to TRANSMIT
                      UCA0IE |= UCTXIE;           // enable the transmit array to send characters back to the AD3
                  }
                  // need to send the message back to the AD3 if we are in the correct state (maybe a received flag)
                  // we can enable the transmit and then disable it after


    }
}
