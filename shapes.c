/*
 * shapes.c
 *
 *  Created on: Feb 11, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

// Globals
volatile unsigned int delay_start = 0;
volatile unsigned char state = WAIT;

unsigned int circle_count = 2;
unsigned int fig8_count = 3;
unsigned int triangle_count = 9;

//------------------------------------------------------------------------------
// Shape State Machines
//------------------------------------------------------------------------------

void Run_Straight(void){
    switch(state){
        case  WAIT:                 // Begin
            wait_case();
            break;                  //
        case  START:                // Begin
            start_case();
            break;                  //
        case  RUN:                  // Run
            straight_run_case();
            break;                  //
        case  END:                  // End
            end_case();
            break;                  //
        default: break;
    }
}

void Run_Circle(void){
    switch(state){
        case  WAIT:                 // Begin
            wait_case();
            break;                  //
        case  START:                // Begin
            start_case();
            break;                  //
        case  RUN:                  // Run
            circle_run_case();
            break;                  //
        case END:                   // End
            end_case();
            break;
        default: break;             //
    }
}

void Run_Fig8(void){
    switch(state){
        case  WAIT:                 // Begin
            wait_case();
            break;                  //
        case  START:                // Begin
            start_case();
            break;                  //
        case  RUN:                  // Run
            fig8_run_case();
            break;                  //
        case RUN_REV:
            fig8_rev_case();
            break;
        case END:                   // End
            end_case();
            break;
        default: break;             //
    }
}

void Run_Triangle(void){
    switch(state){
        case  WAIT:                 // Begin
            wait_case();
            break;                  //
        case  START:                // Begin
            start_case();
            break;                  //
        case  RUN:                  // Run
            triangle_run_case();
            break;                  //
        case  RUN_TURN:
            triangle_turn_case();
            break;
        case  END:                  // End
            end_case();
            break;
        default: break;             //
    }
}

//------------------------------------------------------------------------------
// Shape State Bubbles
//------------------------------------------------------------------------------

// COMMON

void wait_case(void){
    if(time_change){
        time_change = 0;
        if(delay_start++ >= WAITING2START){
            delay_start = 0;
            state = START;
        }
    }
}

void start_case(void){
    cycle_time = 0;
    right_motor_count = 0;
    left_motor_count = 0;
    Forward_On();
    segment_count = 0;
    state = RUN;
}

void end_case(void){
    Forward_Off();
    state = WAIT;
    event = NONE;
}

// STRAIGHT

void straight_run_case(void){
    if(time_change){
        time_change = 0;
        if(segment_count <= TRAVEL_DISTANCE){
            if(right_motor_count++ >= RIGHT_COUNT_TIME){
                P6OUT &= ~R_FORWARD;
            }
            if(left_motor_count++ >= LEFT_COUNT_TIME){
                P6OUT &= ~L_FORWARD;
            }
            if(cycle_time >= WHEEL_COUNT_TIME){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
                segment_count++;
                Forward_On();
            }
        }else{
            state = END;
        }
    }
}

// CIRCLE

void circle_run_case(void){
    if(time_change){
        time_change = 0;
        if(segment_count <= CIRCLE_DISTANCE){
            if(right_motor_count++ >= RIGHT_COUNT_TIME_CIRC){
                P6OUT &= ~R_FORWARD;
            }
            if(left_motor_count++ >= LEFT_COUNT_TIME_CIRC){
                P6OUT &= ~L_FORWARD;
            }
            if(cycle_time >= WHEEL_COUNT_TIME_CIRC){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
                segment_count++;
                Forward_On();
            }
        }else{
            if (circle_count > 0) {
                circle_count--;
                state = RUN;
            }
            else state = END;
        }
    }
}

// FIGURE-8

void fig8_run_case(void){
    if(time_change){
        time_change = 0;
        if(segment_count <= FIG8_DISTANCE_F){
            if(right_motor_count++ >= RIGHT_COUNT_TIME_CIRC){
                P6OUT &= ~R_FORWARD;
            }
            if(left_motor_count++ >= LEFT_COUNT_TIME_CIRC){
                P6OUT &= ~L_FORWARD;
            }
            if(cycle_time >= WHEEL_COUNT_TIME_FIG8){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
                segment_count++;
                Forward_On();
            }
        }else{
            if (fig8_count == 3 || fig8_count == 1) {
                fig8_count--;
                segment_count = 0;
                state = RUN_REV;
            }
        }
    }
}

void fig8_rev_case(void){
    if(time_change){
        time_change = 0;
        if(segment_count <= FIG8_DISTANCE_R){
            if(right_motor_count++ >= RIGHT_COUNT2_TIME_FIG8){
                P6OUT &= ~R_FORWARD;
            }
            if(left_motor_count++ >= LEFT_COUNT2_TIME_FIG8){
                P6OUT &= ~L_FORWARD;
            }
            if(cycle_time >= WHEEL_COUNT_TIME_FIG8){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
                segment_count++;
                Forward_On();
            }
        }else{
            if (fig8_count == 2) {
                fig8_count--;
                segment_count = 0;
                state = RUN;
            }
            else state = END;
        }
    }
}

// TRIANGLE

void triangle_run_case(void){
    if(time_change){
           time_change = 0;
           if(segment_count <= TRAVEL_DISTANCE){
               if(right_motor_count++ >= RIGHT_COUNT_TIME){
                   P6OUT &= ~R_FORWARD;
               }
               if(left_motor_count++ >= LEFT_COUNT_TIME){
                   P6OUT &= ~L_FORWARD;
               }
               if(cycle_time >= WHEEL_COUNT_TIME){
                   cycle_time = 0;
                   right_motor_count = 0;
                   left_motor_count = 0;
                   segment_count++;
                   Forward_On();
               }
           }else{
               if (triangle_count == 9 || triangle_count == 7 || triangle_count == 5 || triangle_count == 3 || triangle_count == 1){
                   triangle_count--;
                   segment_count = 0;
                   state = RUN_TURN;
               }
               else state = END;
           }
       }
}

void triangle_turn_case(void){
    if(time_change){
           time_change = 0;
           P6OUT &= ~L_FORWARD;
           if(segment_count <= TURN_DISTANCE){
               if(right_motor_count++ >= RIGHT_TURN_TIME){
                   P6OUT &= ~R_FORWARD;
               }
               if(cycle_time >= WHEEL_COUNT_TIME_TURN){
                   cycle_time = 0;
                   right_motor_count = 0;
                   left_motor_count = 0;
                   segment_count++;
                   Forward_On();
               }
           }else{
               if (triangle_count == 8 || triangle_count == 6 || triangle_count == 4 || triangle_count == 2 || triangle_count == 0){
                   triangle_count--;
                   segment_count = 0;
                   Forward_On();
                   state = RUN;
               }
           }
       }
}
