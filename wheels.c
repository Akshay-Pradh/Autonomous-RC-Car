/*
 * wheels.c
 *
 *  Created on: Feb 26, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

// Globals

unsigned char wheel_event = IDLE;
unsigned char intercept_event = IDLE;
unsigned char config = NONE;

unsigned char TRAVELLING = NO;

unsigned int BLACK_THRESHOLD = 500;
unsigned int WHITE_THRESHOLD = 200;
unsigned int GREY_THRESHOLD = 350;

unsigned char movement_letter = 'N';
unsigned int movement_duration = 0;


void Motion_Process(void) {

    switch (movement_letter) {
        case 'D':
            // Toggle the LCD backlite
            P6DIR ^= LCD_BACKLITE;
            iot_command_complete = YES; // set command complete flag back to high when motors stopped
            movement_letter = 'N';
            break;
        case 'A':
            // display "Arrived at 0X" on display
            strcpy(display_line[0], "Arrived 0 ");
            display_line[0][9] = movement_duration + '0';
            movement_duration = 0;      // reset the movement duration

            iot_command_complete = YES; // set command complete flag back to high when motors stopped
            movement_letter = 'N';
            break;
        case 'B':
            // switch the intercept event, set curr_time, turn motors on and switch this state to N
            Curr_Time = Time_Precise;
            Forward_On();
            intercept_event = MOVE_STRAIGHT1;

            strcpy(display_line[0], "BL Start  ");
            display_changed = TRUE;

            movement_letter = 'N';
            break;
        case 'E':
            // we have sent the ending command, time to leave the circle
            Curr_Time = Time;
            Motors_Off();
            intercept_event = FINAL_STOP;

            strcpy(display_line[0], "BL Exit   ");
            display_changed = TRUE;

            movement_letter = 'N';
            break;
        case 'F':
            Forward_Fast_On();
            Curr_Time = Time_Precise;
            movement_letter = 'S';
            break;
        case 'R':
            Reverse_On();
            Curr_Time = Time_Precise;
            movement_letter = 'S';
            break;
        case 'C':
            Spin_Clock();
            Curr_Time = Time_Precise;
            movement_letter = 'S';
            break;
        case 'L':
            Spin_Counter_Clock();
            Curr_Time = Time_Precise;
            movement_letter = 'S';
            break;
        case 'S':
            if (Time_Precise > Curr_Time + movement_duration) {
                Motors_Off();
                movement_letter = 'N';      // set to a default N/A condition
                movement_duration = 0;      // reset movement duration
                iot_command_complete = YES; // set command complete flag back to high when motors stopped
            }
            break;
        default: break;
    }
}

void Intercept_Process(void){
    switch(intercept_event) {
        case IDLE:
            break;
        case MOVE_STRAIGHT1:
            if (Time_Precise > Curr_Time + 10) {
                Motors_Off();
                Curr_Time = Time;
                intercept_event = STOP1;
            }
            break;
        case STOP1:
            if (Time > Curr_Time + 5) {
                Curve_Right();
                P2OUT |= IR_LED;
                // start the timers
                TB1CCTL0 |= CCIE;
                TB1CCTL1 |= CCIE;
                intercept_event = TURN1;
            }
            break;
        case TURN1:
            if (ADC_Right_Detect < WHITE_THRESHOLD || ADC_Right_Detect < WHITE_THRESHOLD) {
                Motors_Off();
                Curr_Time = Time;
                intercept_event = STOP2;
            }
            break;
        case STOP2:
            if (Time > Curr_Time + 5) {
                Forward_On();
                intercept_event = FIND_BLACK;
            }
            break;
        case FIND_BLACK:
            if (ADC_Right_Detect > BLACK_THRESHOLD || ADC_Left_Detect > BLACK_THRESHOLD) {
                Motors_Off();
                Curr_Time = Time;
                intercept_event = FOUND_BLACK;
                iot_command_complete = YES;     // clear the iot to receive new commands

                // LCD update: Intercept
                strcpy(display_line[0], "Intercept");
                display_changed = TRUE;
             }
            break;
        case FOUND_BLACK:
            if (Time > Curr_Time + 30) {
                Spin_Counter_Clock();
                intercept_event = TURN2;
                Curr_Time = Time_Precise;
            }
            break;
        case TURN2:
            if (Time_Precise > Curr_Time + 8) {
                Motors_Off();
                Curr_Time = Time;
                intercept_event = STOP3;

                // LCD update: BL Turn
                strcpy(display_line[0], "BL Turn   ");
                display_changed = TRUE;
            }
            break;
        case STOP3:
            if (Time > Curr_Time + 30) {
                Follow_Path();
                intercept_event = FOLLOWING;
                Curr_Time = Time;
                TRAVELLING = YES;

                // LCD update: BL Travel
                strcpy(display_line[0], "BL Travel ");
                display_changed = TRUE;
            }
            break;
        case FOLLOWING:
            // Timer until we are travelling around circle (must display)
            if (TRAVELLING && Time > Curr_Time + 40) {
                Motors_Off();
                Curr_Time = Time;
                intercept_event = STOP4;

                strcpy(display_line[0], "BL Circle ");
                display_changed = TRUE;
                break;
            }
            // if we are moving too far left
            if (ADC_Left_Detect < GREY_THRESHOLD) {       // checking for gray (avg of white and black)
                Follow_Adjust_OUT();
                intercept_event = ADJUST_OUT;
            }
            // if we are moving too far right
            if (ADC_Right_Detect < GREY_THRESHOLD) {   // checking for gray (avg of white and black)
                Follow_Adjust_IN();
                intercept_event = ADJUST_IN;
            }
            break;
        case ADJUST_IN:
            // if the Right Detect LED value is greater than grey threshold
            if (ADC_Right_Detect > GREY_THRESHOLD) {
                Follow_Path();
                intercept_event = FOLLOWING;
            }
            break;
        case ADJUST_OUT:
            // if the Right Detect LED value is greater than grey threshold
            if (ADC_Left_Detect > GREY_THRESHOLD) {
                Follow_Path();
                intercept_event = FOLLOWING;
            }
            break;
        case STOP4:
            if (Time > Curr_Time + 30) {
                Follow_Path();
                TRAVELLING = NO;                // reset the travelling flag
                intercept_event = FOLLOWING;    // jump back to the following state
            }
            break;
        case FINAL_STOP:
            if (Time > Curr_Time + 30) {
                Spin_Counter_Clock();
                Curr_Time = Time_Precise;
                intercept_event = FINAL_TURN;
            }
            break;
        case FINAL_TURN:
            if (Time_Precise > Curr_Time + 8) {
                Motors_Off();
                Curr_Time = Time;
                intercept_event = FINAL_LEAVE;
            }
            break;
        case FINAL_LEAVE:
            if (Time > Curr_Time + 30) {
                Forward_Fast_On();
                Curr_Time = Time;
                intercept_event = THE_END;
            }
            break;
        case THE_END:
            if (Time > Curr_Time + 15) {
                Motors_Off();
                intercept_event = IDLE;  // back to IDLE

                strcpy(display_line[0], "BL STOP   ");
                strcpy(display_line[1], "  Finally ");
                strcpy(display_line[2], "   Over   ");
                strncpy(display_line[3], "Time: ", 6);
                display_changed = TRUE;
                TIME_DISPLAY = NO;      // stop displaying the time
            }
            break;

        default: break;
    }
}


void Wheels_Process(void){
    switch(wheel_event) {
        case IDLE:                      // IDLE
            break;

        case CONFIGURE_WHEEL_SPEEDS:    // Configure Movement
            switch(config) {
                case FORWARD_C:
                    wheel_event = FORWARD_MOVE_START;
                    break;
                case REVERSE_C:
                    wheel_event = REVERSE_MOVE_START;
                    break;
                case SPIN:
                    wheel_event = SPIN_MOVE_START;
                    break;
                case SPIN_CC:
                    wheel_event = SPIN_CC_MOVE_START;
                    break;
                case OFF:
                    wheel_event = INITIATE_STOP;
                default: break;
            }
            break;

        case FORWARD_MOVE_START:        // Move Forward Start
            if (RIGHT_REVERSE_SPEED == WHEEL_OFF && LEFT_REVERSE_SPEED == WHEEL_OFF) wheel_event = FORWARD_ADJUST;
            else {
                // switch event to INITIATE_STOP
                wheel_event = INITIATE_STOP;
                // turn on RED_LED indicating ERROR
                P1OUT |= RED_LED;
            }
            break;

        case FORWARD_ADJUST:           // Adjust Forward
            if (RIGHT_FORWARD_SPEED < TARGET_SPEED_RIGHT) {
                RIGHT_FORWARD_SPEED += SPEED_STEP;
                if (RIGHT_FORWARD_SPEED > TARGET_SPEED_RIGHT) {
                    RIGHT_FORWARD_SPEED = TARGET_SPEED_RIGHT;   // in case of overshoot when adding speed_step
                }
            }
            if (LEFT_FORWARD_SPEED < TARGET_SPEED_LEFT) {
                LEFT_FORWARD_SPEED += SPEED_STEP;
                if (LEFT_FORWARD_SPEED > TARGET_SPEED_LEFT) {
                    LEFT_FORWARD_SPEED = TARGET_SPEED_LEFT;   // in case of overshoot when adding speed_step
                }
            }
            // forward_off()
            if (TARGET_SPEED_RIGHT == WHEEL_OFF && TARGET_SPEED_LEFT == WHEEL_OFF) {
                wheel_event = INITIATE_STOP;
            }
            // dropping to a lower speed
            if (RIGHT_FORWARD_SPEED > TARGET_SPEED_RIGHT) RIGHT_FORWARD_SPEED = TARGET_SPEED_RIGHT;
            if (LEFT_FORWARD_SPEED > TARGET_SPEED_LEFT) LEFT_FORWARD_SPEED = TARGET_SPEED_LEFT;
            break;

        case REVERSE_MOVE_START:       // Move Reverse Start
            if (RIGHT_FORWARD_SPEED == WHEEL_OFF && LEFT_FORWARD_SPEED == WHEEL_OFF) wheel_event = REVERSE_ADJUST;
            else {
                // switch event to INITIATE_STOP
                wheel_event = INITIATE_STOP;
                // turn on RED_LED indicating ERROR
                P1OUT |= RED_LED;
            }
            break;

        case REVERSE_ADJUST:           // Adjust Reverse
            if (RIGHT_REVERSE_SPEED < TARGET_SPEED_RIGHT) {
                RIGHT_REVERSE_SPEED += SPEED_STEP;
                if (RIGHT_REVERSE_SPEED > TARGET_SPEED_RIGHT) {
                    RIGHT_REVERSE_SPEED = TARGET_SPEED_RIGHT;   // in case of overshoot when adding speed_step
                }
            }
            if (LEFT_REVERSE_SPEED < TARGET_SPEED_LEFT) {
                LEFT_REVERSE_SPEED += SPEED_STEP;
                if (LEFT_REVERSE_SPEED > TARGET_SPEED_LEFT) {
                    LEFT_REVERSE_SPEED = TARGET_SPEED_LEFT;   // in case of overshoot when adding speed_step
                }
            }
            // reverse_off()
            if (TARGET_SPEED_RIGHT == WHEEL_OFF && TARGET_SPEED_LEFT == WHEEL_OFF) {
                wheel_event = INITIATE_STOP;
            }
            // dropping to a lower speed
            if (RIGHT_REVERSE_SPEED > TARGET_SPEED_RIGHT) RIGHT_REVERSE_SPEED = TARGET_SPEED_RIGHT;
            if (LEFT_REVERSE_SPEED > TARGET_SPEED_LEFT) LEFT_REVERSE_SPEED = TARGET_SPEED_LEFT;
            break;

        case SPIN_MOVE_START:       // Begin Spin Process
            // RIGHT forward LEFT reverse therefore we must make sure RIGHT reverse and LEFT forward are off
            if ((RIGHT_REVERSE_SPEED == WHEEL_OFF && LEFT_FORWARD_SPEED == WHEEL_OFF)) wheel_event = SPIN_ADJUST;
            else {
                // switch event to INITIATE_STOP
                wheel_event = INITIATE_STOP;
                // turn on RED_LED indicating ERROR
                P1OUT |= RED_LED;
             }
            break;

        case SPIN_ADJUST:
            if (RIGHT_FORWARD_SPEED < TARGET_SPEED_RIGHT) {
                RIGHT_FORWARD_SPEED += SPEED_STEP;
                if (RIGHT_FORWARD_SPEED > TARGET_SPEED_RIGHT) {
                    RIGHT_FORWARD_SPEED = TARGET_SPEED_RIGHT;       // in case of overshoot when adding speed_step
                }
            }
            if (LEFT_REVERSE_SPEED < TARGET_SPEED_LEFT) {
                LEFT_REVERSE_SPEED += SPEED_STEP;
                if (LEFT_REVERSE_SPEED > TARGET_SPEED_LEFT) {
                    LEFT_REVERSE_SPEED = TARGET_SPEED_LEFT;         // in case of overshoot when adding speed_step
                }
            }
            if (TARGET_SPEED_RIGHT == WHEEL_OFF && TARGET_SPEED_LEFT == WHEEL_OFF) {
                wheel_event = INITIATE_STOP;
            }
            break;

        case SPIN_CC_MOVE_START:     // Begin Spin Counter Clockwise Process
            // RIGHT forward LEFT reverse therefore we must make sure RIGHT reverse and LEFT forward are off
            if ((RIGHT_REVERSE_SPEED == WHEEL_OFF && LEFT_FORWARD_SPEED == WHEEL_OFF)) wheel_event = SPIN_CC_ADJUST;
            else {
                // switch event to INITIATE_STOP
                wheel_event = INITIATE_STOP;
                // turn on RED_LED indicating ERROR
                P1OUT |= RED_LED;
             }
            break;

        case SPIN_CC_ADJUST:
            if (RIGHT_REVERSE_SPEED < TARGET_SPEED_RIGHT) {
              RIGHT_REVERSE_SPEED += SPEED_STEP;
              if (RIGHT_REVERSE_SPEED > TARGET_SPEED_RIGHT) {
                  RIGHT_REVERSE_SPEED = TARGET_SPEED_RIGHT;       // in case of overshoot when adding speed_step
              }
            }
            if (LEFT_FORWARD_SPEED < TARGET_SPEED_LEFT) {
              LEFT_FORWARD_SPEED += SPEED_STEP;
              if (LEFT_FORWARD_SPEED > TARGET_SPEED_LEFT) {
                  LEFT_FORWARD_SPEED = TARGET_SPEED_LEFT;         // in case of overshoot when adding speed_step
              }
            }
            if (TARGET_SPEED_RIGHT == WHEEL_OFF && TARGET_SPEED_LEFT == WHEEL_OFF) {
              wheel_event = INITIATE_STOP;
            }
            break;

        case INITIATE_STOP:            // Begin Stop Process
            // turn all motors off
            LEFT_FORWARD_SPEED = WHEEL_OFF;
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            // initialize current time to compare to time in STOP state
            Curr_Time = Time;
            wheel_event = STOP_WHEELS;
            break;

        case STOP_WHEELS:               // Look for End of Stop Time
            if (Time > Curr_Time + 5) wheel_event = IDLE;
            break;

        default: break;
    }
}


void Motors_Off(void) {
    wheel_event = INITIATE_STOP;
}

void Forward_On(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = FORWARD_C;
    TARGET_SPEED_LEFT = SLOW_L_FORWARD;
    TARGET_SPEED_RIGHT = SLOW_R_FORWARD;
}

void Forward_Off(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = FORWARD_C;
    TARGET_SPEED_LEFT = WHEEL_OFF;
    TARGET_SPEED_RIGHT = WHEEL_OFF;
}

void Reverse_On(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = REVERSE_C;
    TARGET_SPEED_LEFT = SLOW_L;
    TARGET_SPEED_RIGHT = SLOW_R;
}

void Reverse_Off(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = REVERSE_C;
    TARGET_SPEED_LEFT = WHEEL_OFF;
    TARGET_SPEED_RIGHT = WHEEL_OFF;
}

void Spin_Clock(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = SPIN;
    TARGET_SPEED_LEFT = SLOWER;
    TARGET_SPEED_RIGHT = SLOWER;
}

void Spin_Counter_Clock(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = SPIN_CC;
    TARGET_SPEED_LEFT = SLOWER;
    TARGET_SPEED_RIGHT = SLOWER;
}

void Forward_Fast_On(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = FORWARD_C;
    TARGET_SPEED_LEFT = FAST_L_FORWARD;
    TARGET_SPEED_RIGHT = FAST_R_FORWARD;
}

void Curve_Right(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = FORWARD_C;
    TARGET_SPEED_LEFT = CURVE_SLOW;
    TARGET_SPEED_RIGHT = CURVE_FAST;
}

// Functions for following black circle

void Follow_Path(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = FORWARD_C;
    TARGET_SPEED_LEFT = FOLLOW_L;     // testing speeds for circle path
    TARGET_SPEED_RIGHT = FOLLOW_R;
}

void Follow_Adjust_IN(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = FORWARD_C;
    TARGET_SPEED_LEFT = WHEEL_OFF;
    TARGET_SPEED_RIGHT = FOLLOW_ADJ;
}

void Follow_Adjust_OUT(void){
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = FORWARD_C;
    TARGET_SPEED_LEFT = FOLLOW_ADJ;
    TARGET_SPEED_RIGHT = WHEEL_OFF;
}

