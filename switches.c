/*
 * switches.c
 *
 *  Created on: Feb 11, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

// Globals
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
                 Curr_Time = Time;
                 sw1_pressed = NO;
                 strcpy(display_line[0], "          ");
                 strcpy(display_line[1], "          ");
                 strcpy(display_line[2], "Baud Rate:");
                 strcpy(display_line[3], "115,200 Hz");
                 display_changed = TRUE;
                 Display_Update(0,0,0,0);    // manually update display before waiting

                 setBaudRate(115200);                        // set our baud rate to 115,200 Hz
                 while (Time < Curr_Time + 10) {             // 2 second wait before transmit
                     continue;
                 }
                 strncpy(transmit_array, NCSU_array, 10);    // copy our message into transmit array
                 UCA1IE |= UCTXIE;                           // enable transmit
     }
}

void Switch2_Process(void){
    //------------------------------------------------------------------------------
    // Switch 2 Configurations
    // Port 2 Pin 3
    //------------------------------------------------------------------------------
    if (sw2_pressed){
                  // SW2 is pressed
                  Curr_Time = Time;
                  sw2_pressed = NO;
                  strcpy(display_line[0], "          ");
                  strcpy(display_line[1], "          ");
                  strcpy(display_line[2], "Baud Rate:");
                  strcpy(display_line[3], "460,800 Hz");
                  display_changed = TRUE;
                  Display_Update(0,0,0,0);  // manually update display before waiting

                  setBaudRate(460800);
                  while (Time < Curr_Time + 10) {             // 2 second wait before transmit
                      continue;
                  }
                  strncpy(transmit_array, NCSU_array, 10);    // copy our message into transmit array
                  UCA1IE |= UCTXIE;                           // enable transmit

    }
}
