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
  event = IDLE;

//  P2OUT |= IR_LED;        // testing IR LED
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
        strcpy(display_line[2], "LD:       ");
        strcpy(display_line[3], "RD:       ");
        HEXtoBCD(ADC_Left_Detect);
        adc_line(3, 3);
        HEXtoBCD(ADC_Right_Detect);
        adc_line(4, 3);
        display_changed = TRUE;
    }

//     State Machine for Project07
    switch (event) {
        case IDLE:
            break;

        case FIND_BLACK:
           // write a 1 second delay (Time increases every 200ms)
           // we will need to write a value to keep track of the time it takes us to find the black line and use that to go back to center
           if (Time > Curr_Time + 4){
               Forward_On();
               strcpy(display_line[0], " LOOKING! ");
               display_changed = TRUE;
               Curr_Time = Time_Precise;
               event = FOUND_BLACK;
           }
           break;

        case FOUND_BLACK:
            // if right and left values are correct
           if (ADC_Right_Detect > BLACK_THRESHOLD || ADC_Left_Detect > BLACK_THRESHOLD) {
               Forward_Off();
               strcpy(display_line[0], "  FOUND!  ");
               display_changed = TRUE;
               RADIUS_TIME = Time_Precise - Curr_Time;  // we use this time to get back to the center
               Curr_Time = Time;
               event = STOP;
           }
           break;

        case STOP:
           if (Time > Curr_Time + 14)  {
               Curr_Time = Time_Precise;    // will be used to calculate spin time for return
               Spin_Clock();
               strcpy(display_line[0], " SPINNING ");
               display_changed = TRUE;
               event = ORIENT_BLACK;
           }
           break;

        case ORIENT_BLACK:
           if (ADC_Right_Detect > BLACK_THRESHOLD || ADC_Left_Detect > BLACK_THRESHOLD) {
              Motors_Off();
              strcpy(display_line[0], " CIRCLING ");
              display_changed = TRUE;
              SPIN_TIME = Time_Precise - Curr_Time;
              Curr_Time = Time;
              event = SMALL_STOP;
           }
           break;

        case SMALL_STOP:
           if (Time > Curr_Time + 5) {
               Circle_Path();
               START_TIME = Time;
               TIME_DISPLAY = TRUE;  // start the display timer
               Curr_Time = Time;
               event = CIRCLING;
           }
           break;

        case CIRCLING:
           // eventually will check Curr_Time against Time to have it circle twice
           // currently infinitely loops here

           // if we are moving inside the circle
           if (Time < Curr_Time + 204) {
               if (ADC_Right_Detect < BLACK_THRESHOLD - 100) {      // -100 for some extra tolerance while adjusting
                  strcpy(display_line[0], " ADJ_INW! ");
                  display_changed = TRUE;
                  Circle_Adjust_IN();
                  event = ADJUST_IN;
              }

              // if we are moving outside the circle
              if (ADC_Left_Detect < BLACK_THRESHOLD - 100) {       // -100 for some extra tolerance while adjusting
                  strcpy(display_line[0], " ADJ_OUT! ");
                  display_changed = TRUE;
                  Circle_Adjust_OUT();
                  event = ADJUST_OUT;
              }
           }
           else {
               Motors_Off();
               TIME_DISPLAY = FALSE;
               Curr_Time = Time;
               event = ORIENT_WHITE;
           }
           break;

        case ADJUST_OUT:
           if (ADC_Left_Detect > BLACK_THRESHOLD) {
               strcpy(display_line[0], " CIRCLING ");
               display_changed = TRUE;
               Circle_Path();
               event = CIRCLING;
           }
           break;

        case ADJUST_IN:
           if (ADC_Right_Detect > BLACK_THRESHOLD) {
               strcpy(display_line[0], " CIRCLING ");
               display_changed = TRUE;
               Circle_Path();
               event = CIRCLING;
           }
           break;

        case ORIENT_WHITE:
           if (Time > Curr_Time + 5) {  // 1 second break between Motors_Off() and Spin_Clock()
               strcpy(display_line[0], " SPINNING ");
               display_changed = TRUE;
               Spin_Clock();
               Curr_Time = Time_Precise;
               event = ORIENT_CENTER;
           }
           break;

       case ORIENT_CENTER:
           if (Time_Precise - Curr_Time >= SPIN_TIME - 1) { // slight adjustment for accuracy
               Motors_Off();
               strcpy(display_line[0], "HEADED BCK");
               display_changed = TRUE;
               Curr_Time = Time;
               event = MOVE_CENTER;
           }
           break;

       case MOVE_CENTER:
           if (Time > Curr_Time + 14) {     // 3 second break after spinning before moving forward
               Forward_On();
               Curr_Time = Time_Precise;
               event = END;
           }
           break;

       case END:
           if (Time_Precise - Curr_Time >= RADIUS_TIME) {   // move forward as long as it took for us to get from center to circumference
               Motors_Off();
               strcpy(display_line[0], "   DONE   ");
               display_changed = TRUE;
               P2OUT &= ~IR_LED;
               event = IDLE;
           }

        default: break;
    }

  }

}
