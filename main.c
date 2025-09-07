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
  //P2OUT &= ~RESET_LCD;
  // Place the contents of what you want on the display, in between the quotes
  // Limited to 10 characters per line
  strcpy(display_line[0], "   NCSU   ");
  strcpy(display_line[1], " WOLFPACK ");
  strcpy(display_line[2], "  ECE306  ");
  strcpy(display_line[3], "  GP I/O  ");
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
//    Carlson_StateMachine();            // Run a Time Based State Machine

    Switches_Process();                // Check for switch state change
    Display_Process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF

    // Creating a time value (Carlson State Machine)
//    if(Last_Time_Sequence != Time_Sequence){
//        Last_Time_Sequence = Time_Sequence;
//        cycle_time++;
//        time_change = 1;
//    }

//     Check if FORWARD and REVERSE motors are running at same time
    if ((P6IN & L_FORWARD) & (P6IN & L_REVERSE)) {
        P6OUT &= ~L_FORWARD;
        P6OUT &= ~L_REVERSE;
        P1OUT |= RED_LED;       // turn on RED_LED
    }

    if ((P6IN & R_FORWARD) & (P6IN & R_REVERSE)) {
        P6OUT &= ~R_FORWARD;
        P6OUT &= ~R_REVERSE;
        P1OUT |= RED_LED;      // turn on RED_LED
    }

    // State Machine for Project05
    switch(event) {
        case MOVEMENT_1:
            RunMovement1();
            break;
        case MOVEMENT_2:
            RunMovement2();
            break;
        case MOVEMENT_3:
            RunMovement3();
            break;
        case MOVEMENT_4:
            RunMovement4();
            break;
        case MOVEMENT_5:
            RunMovement5();
            break;
        default: break;
    }

}
//------------------------------------------------------------------------------

}

void Carlson_StateMachine(void){
    switch(Time_Sequence){
      case 250:                        //
        if(one_time){
          Init_LEDs();
          lcd_BIG_mid();
          display_changed = 1;
          one_time = 0;
        }
        Time_Sequence = 0;             //
        break;
      case 200:                        //
        if(one_time){
//          P1OUT &= ~RED_LED;            // Change State of LED 4
          P6OUT |= GRN_LED;            // Change State of LED 5
          one_time = 0;
        }
        break;
      case 150:                         //
        if(one_time){
          P1OUT |= RED_LED;            // Change State of LED 4
          P6OUT &= ~GRN_LED;            // Change State of LED 5
          one_time = 0;
        }
        break;
      case 100:                         //
        if(one_time){
//          lcd_4line();
          lcd_BIG_bot();
          P6OUT |= GRN_LED;            // Change State of LED 5
          display_changed = 1;
          one_time = 0;
        }
        break;
      case  50:                        //
        if(one_time){
          one_time = 0;
        }
        break;                         //
      default: break;
    }
}

