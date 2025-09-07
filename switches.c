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

                 if (sw1_toggle == 1){
                  clear_display();
                  CALIBRATE= YES;
                  strcpy(display_line[0], "CALIBRATE ");
                  strcpy(display_line[1], "IR_LED ON ");
                  display_changed = TRUE;
                  P2OUT |= IR_LED;
                  // start the timers
                  TB1CCTL0 |= CCIE;
                  TB1CCTL1 |= CCIE;
                }
                if (sw1_toggle == 2){
                  strcpy(display_line[0], "BLACK SET ");
                  display_changed = TRUE;
                  BLACK_THRESHOLD = (ADC_Left_Detect + ADC_Right_Detect) / 2;   // use left and right average
                }
                if (sw1_toggle == 3){
                  strcpy(display_line[0], "WHITE SET ");
                  display_changed = TRUE;
                  WHITE_THRESHOLD = (ADC_Left_Detect + ADC_Right_Detect) / 2;  // use left and right average
                }
                if (sw1_toggle > 3){
                  // reconfigure the display
                  GREY_THRESHOLD = BLACK_THRESHOLD - 150;
                  clear_display();
                  strcpy(display_line[0], " Waiting  ");
                  strcpy(display_line[1], "for input ");
                  strncpy(display_line[2], ip_address1, 10);
                  strncpy(display_line[3], ip_address2, 10);
                  display_changed = TRUE;

                  // stop the timers
                  TB1CCTL0 &= ~CCIE;
                  TB1CCTL1 &= ~CCIE;
                  P2OUT &= ~IR_LED;
                  sw2_toggle = 0;

                  CALIBRATE = NO;
                }

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
    }
}
