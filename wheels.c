/*
 * wheels.c
 *
 *  Created on: Feb 26, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

// Globals
unsigned char wheel_event = IDLE;
unsigned char config = NONE;
unsigned int BLACK_THRESHOLD = 500;
unsigned int WHITE_THRESHOLD = 200;

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
    TARGET_SPEED_LEFT = SLOW_L;
    TARGET_SPEED_RIGHT = SLOW_R;
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

void Circle_Path(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = FORWARD_C;
    TARGET_SPEED_LEFT = CIRCLE_SLOW;     // testing speeds for circle path
    TARGET_SPEED_RIGHT = CIRCLE_SLOWER;
}

void Circle_Adjust_IN(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = FORWARD_C;
    TARGET_SPEED_LEFT = WHEEL_OFF;
    TARGET_SPEED_RIGHT = CIRCLE_ADJ;
}

void Circle_Adjust_OUT(void){
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = FORWARD_C;
    TARGET_SPEED_LEFT = CIRCLE_ADJ;
    TARGET_SPEED_RIGHT = WHEEL_OFF;
}

