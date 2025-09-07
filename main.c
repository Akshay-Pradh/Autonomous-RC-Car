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
  //P2OUT &= ~RESET_LCD;
  // Place the contents of what you want on the display, in between the quotes
  // Limited to 10 characters per line
  strcpy(display_line[0], "   IDLE   ");
  strcpy(display_line[1], "          ");
  strcpy(display_line[2], "          ");
  strcpy(display_line[3], "          ");
  display_changed = TRUE;
//  Display_Update(0,0,0,0);

  // Defaults:
  wheel_move = 0;
  forward = TRUE;
  event = NONE;

//------------------------------------------------------------------------------
// Beginning of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
    Switches_Process();                // Check for switch state change
    Display_Process();                 // Update Display
    Wheels_Process();                  // Check for Wheel Changes

    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF

    // ADC Display Update (every 250ms, set high in interrupts)
    if (ADC_DISPLAY) {
        ADC_DISPLAY = 0;
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], "          ");
        HEXtoBCD(ADC_Left_Detect);
        adc_line(3, 3);
        HEXtoBCD(ADC_Right_Detect);
        adc_line(4, 3);
        display_changed = TRUE;
    }

//     State Machine for Project06
    switch (event) {
        case IDLE:
            break;
        case FIND_BLACK:
            // write a 1 second delay (Time increases every 200ms)
            if (Time > Curr_Time + 4){
                Forward_On();
                event = FOUND_BLACK;
            }
            break;

        case FOUND_BLACK:
            // if right and left values are correct
            if (ADC_Right_Detect > 600 || ADC_Left_Detect > 600) {
                Forward_Off();
                strcpy(display_line[0], "   FOUND   ");
                strcpy(display_line[1], "  BLK LINE ");
                display_changed = TRUE;
                Curr_Time = Time;
                event = LONG_STOP;
            }
            break;

        case LONG_STOP:
            if (Time > Curr_Time + 19)  {
                Spin_Clock();
                strcpy(display_line[0], " SPINNING ");
                strcpy(display_line[1], "          ");
                display_changed = TRUE;
                event = ORIENT_BLACK;
            }
            break;

        case ORIENT_BLACK:
            // if we see black again we cut the motors off
            if (ADC_Right_Detect > 600 || ADC_Left_Detect > 600) {
                Motors_Off();
                strcpy(display_line[0], "   IDLE   ");
                strcpy(display_line[1], "          ");
                display_changed = TRUE;
                event = IDLE;
            }
            break;

        default: break;
    }

  }

}
//------------------------------------------------------------------------------
