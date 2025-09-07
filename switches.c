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
                 sw1_pressed = NO;
                 strcpy(display_line[0], " FINDING  ");
                 strcpy(display_line[1], " BLK LINE ");
                 display_changed = TRUE;
                 Curr_Time = Time;
                 event = FIND_BLACK;
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
                  sw2_toggle++;

                  if (sw2_toggle == 1){
                      strcpy(display_line[0], " EMMITER  ");
                      strcpy(display_line[1], "    ON    ");
                      display_changed = TRUE;
                      P2OUT |= IR_LED;          // Enable Emitter
                  }
                  if (sw2_toggle == 2){
                      strcpy(display_line[0], " EMMITER  ");
                      strcpy(display_line[1], "    OFF    ");
                      display_changed = TRUE;
                      P2OUT &= ~IR_LED;        // Disable Emitter
                  }
                  if (sw2_toggle >= 3) sw2_toggle = 0;
    }
}

