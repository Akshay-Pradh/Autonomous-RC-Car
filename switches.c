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
                 Curr_Time = Time;
                 P2OUT |= IR_LED;     // turn IR LED ON
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
                      strcpy(display_line[0], "CALIBRATE ");
                      strcpy(display_line[1], "IR_LED ON ");
                      display_changed = TRUE;
                      P2OUT |= IR_LED;
                  }
                  if (sw2_toggle == 2){
                      strcpy(display_line[0], "BLACK SET ");
                      display_changed = TRUE;
                      BLACK_THRESHOLD = ADC_Left_Detect;
                  }
                  if (sw2_toggle == 3){
                      strcpy(display_line[0], "WHITE SET ");
                      display_changed = TRUE;
                      WHITE_THRESHOLD = ADC_Left_Detect;
                  }
                  if (sw2_toggle > 3){
                      strcpy(display_line[0], "   IDLE   ");
                      strcpy(display_line[1], "          ");
                      display_changed = TRUE;
                      P2OUT &= ~IR_LED;
                      sw2_toggle = 0;
                  }
    }
}
