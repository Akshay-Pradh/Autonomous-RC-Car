/*
 * switches.c
 *
 *  Created on: Feb 11, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

// Globals

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
                 strcpy(display_line[0], "  SWITCH  ");
                 strcpy(display_line[1], "    1     ");
                 strcpy(display_line[2], "          ");
                 strcpy(display_line[3], "  ECE306  ");
                 display_changed = TRUE;
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
                  strcpy(display_line[0], "  SWITCH  ");
                  strcpy(display_line[1], "    2     ");
                  strcpy(display_line[2], "          ");
                  strcpy(display_line[3], "  ECE306  ");
                  display_changed = TRUE;
    }
}

