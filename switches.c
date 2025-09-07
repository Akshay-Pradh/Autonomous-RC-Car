/*
 * switches.c
 *
 *  Created on: Feb 11, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

// Globals
volatile unsigned char okay_to_look_at_switch1 = OKAY;
volatile unsigned char sw1_position = RELEASED;
volatile unsigned int count_debounce_SW1;

volatile unsigned char okay_to_look_at_switch2 = OKAY;
volatile unsigned char sw2_position = RELEASED;
volatile unsigned int count_debounce_SW2;

volatile unsigned int shape_sel = 0;


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
     if (okay_to_look_at_switch1 && sw1_position){
         if (!(P4IN & SW1)){
                 sw1_position = PRESSED;
                 okay_to_look_at_switch1 = NOT_OKAY;
                 count_debounce_SW1 = DEBOUNCE_RESTART;
                 shape_sel++;
                 // do what you want with button press

                 if (shape_sel == 1){
                     strcpy(display_line[0], "  SELECT  ");
                     strcpy(display_line[1], "  CIRCLE  ");
                     strcpy(display_line[2], "          ");
                     strcpy(display_line[3], "  ECE306  ");
                     display_changed = TRUE;

                 }
                 if (shape_sel == 2){
                     strcpy(display_line[0], "  SELECT  ");
                     strcpy(display_line[1], "  FIGURE  ");
                     strcpy(display_line[2], "  EIGHT   ");
                     strcpy(display_line[3], "  ECE306  ");
                     display_changed = TRUE;
                 }
                 if (shape_sel == 3){
                     strcpy(display_line[0], "  SELECT  ");
                     strcpy(display_line[1], " TRIANGLE ");
                     strcpy(display_line[2], "          ");
                     strcpy(display_line[3], "  ECE306  ");
                     display_changed = TRUE;
                 }
                 if (shape_sel >= 4) shape_sel = 0;
         }
     }
     if (count_debounce_SW1 <= DEBOUNCE_TIME){
         count_debounce_SW1++;
     }else{
         okay_to_look_at_switch1 = OKAY;
         if (P4IN & SW1){
             sw1_position = RELEASED;
         }
     }
}

void Switch2_Process(void){
    //------------------------------------------------------------------------------
    // Switch 2 Configurations
    // Port 2 Pin 3
    //------------------------------------------------------------------------------
    if (okay_to_look_at_switch2 && sw2_position){
          if (!(P2IN & SW2)){
                  sw2_position = PRESSED;
                  okay_to_look_at_switch2 = NOT_OKAY;
                  count_debounce_SW2 = DEBOUNCE_RESTART;

                  if (shape_sel == 1){
                      strcpy(display_line[0], "   TEST   ");
                      strcpy(display_line[1], "  CIRCLE  ");
                      strcpy(display_line[2], "          ");
                      strcpy(display_line[3], "  ECE306  ");
                      display_changed = TRUE;
                      event = CIRCLE;
                  }
                  if (shape_sel == 2){
                      strcpy(display_line[0], "   TEST   ");
                      strcpy(display_line[1], "  FIGURE  ");
                      strcpy(display_line[2], "  EIGHT   ");
                      strcpy(display_line[3], "  ECE306  ");
                      display_changed = TRUE;
                      event = FIG8;
                  }
                  if (shape_sel == 3){
                      strcpy(display_line[0], "   TEST   ");
                      strcpy(display_line[1], " TRIANGLE ");
                      strcpy(display_line[2], "          ");
                      strcpy(display_line[3], "  ECE306  ");
                      display_changed = TRUE;
                      event = TRIANGLE;
                  }

          }
      }
      if (count_debounce_SW2 <= DEBOUNCE_TIME){
          count_debounce_SW2++;
      }else{
          okay_to_look_at_switch2 = OKAY;
          if (P2IN & SW2){
              sw2_position = RELEASED;
          }
      }
}

