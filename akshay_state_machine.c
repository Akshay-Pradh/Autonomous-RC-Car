/*
 * akshay_state_machine.c
 *
 *  Created on: Feb 19, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

// Globals
volatile unsigned char state = WAIT;
volatile unsigned char event = NONE;

// STATE MACHINES

void RunMovement1(void) {
    switch(state){
        case WAIT:
            wait_before_case();
            break;
        case MOVE_FORWARD:
            run_forward();
            break;
        case STOP:
            if (Time > Curr_Time + 5) {     // turn off motors after 1 second
                Motors_Off();
                strcpy(display_line[0], "  PAUSING ");
                strcpy(display_line[1], "  MOTORS  ");
                strcpy(display_line[2], "  1 SEC   ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
                Curr_Time = Time;
                state = PAUSE;
            }
            break;
        case PAUSE:
            pause1();
            break;
        case PASS_MOVEMENT:
            event = MOVEMENT_2;
            state = MOVE_REVERSE;
            Curr_Time = Time;
            break;
        default: break;
    }
}

void RunMovement2(void) {
    switch(state){
        case MOVE_REVERSE:
            run_reverse();
            break;
        case STOP:
            if (Time > Curr_Time + 9) {     // turn off motors after 1 second
                Motors_Off();
                strcpy(display_line[0], "  PAUSING ");
                strcpy(display_line[1], "  MOTORS  ");
                strcpy(display_line[2], "  1 SEC   ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
                Curr_Time = Time;
                state = PAUSE;
            }
            break;
        case PAUSE:
            pause1();
            break;
        case PASS_MOVEMENT:
            event = MOVEMENT_3;
            state = MOVE_FORWARD;
            Curr_Time = Time;
            break;
        default: break;
    }
}

void RunMovement3(void) {
    switch(state){
        case MOVE_FORWARD:
            run_forward();
            break;
        case STOP:
            if (Time > Curr_Time + 5) {     // turn off motors after 1 second
            Motors_Off();
            strcpy(display_line[0], "  PAUSING ");
            strcpy(display_line[1], "  MOTORS  ");
            strcpy(display_line[2], "  1 SEC   ");
            strcpy(display_line[3], "          ");
            display_changed = TRUE;
            Curr_Time = Time;
            state = PAUSE;
            }
            break;
        case PAUSE:
            pause1();
            break;
        case PASS_MOVEMENT:
            event = MOVEMENT_4;
            state = SPIN_CLOCK;
            Curr_Time = Time;
            break;
        default: break;
    }
}

void RunMovement4(void) {
    switch(state){
        case SPIN_CLOCK:
            spinclock();
            break;
        case STOP:
            if (Time > Curr_Time + 14) {     // turn off motors after 3 seconds
                Motors_Off();
                strcpy(display_line[0], "  PAUSING ");
                strcpy(display_line[1], "  MOTORS  ");
                strcpy(display_line[2], "  2 SEC   ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
                Curr_Time = Time;
                state = PAUSE;
            }
            break;
        case PAUSE:
            pause2();
            break;
        case PASS_MOVEMENT:
            event = MOVEMENT_5;
            state = SPIN_COUNTERCLOCK;
            Curr_Time = Time;
            break;
        default: break;
    }
}

void RunMovement5(void) {
    switch(state){
        case SPIN_COUNTERCLOCK:
            spincounterclock();
            break;
        case STOP:
            if (Time > Curr_Time + 14) {     // turn off motors after 3 seconds
                Motors_Off();
                strcpy(display_line[0], "  PAUSING ");
                strcpy(display_line[1], "  MOTORS  ");
                strcpy(display_line[2], "  2 SEC   ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
                Curr_Time = Time;
                state = PAUSE;
            }
            break;
        case PAUSE:
            pause2();
            break;
        case PASS_MOVEMENT:
            event = NONE;
            state = WAIT;
            strcpy(display_line[0], " MOVEMENT ");
            strcpy(display_line[1], " SEQUENCE ");
            strcpy(display_line[2], "   OVER   ");
            strcpy(display_line[3], "    :)    ");
            display_changed = TRUE;
            break;
        default: break;
    }
}

// STATE Functions

void wait_before_case(void) {
    if (Time > Curr_Time + 5) {
        Curr_Time = Time;
        state = MOVE_FORWARD;
    }

}

void run_forward(void) {
    strcpy(display_line[0], "  MOVING  ");
    strcpy(display_line[1], "  MOTORS  ");
    strcpy(display_line[2], "  FORWARD ");
    strcpy(display_line[3], "          ");
    display_changed = TRUE;
//    Forward_On();
    state = STOP;
}
void run_reverse(void) {
    strcpy(display_line[0], "  MOVING  ");
    strcpy(display_line[1], "  MOTORS  ");
    strcpy(display_line[2], "  REVERSE ");
    strcpy(display_line[3], "          ");
    display_changed = TRUE;
//    Reverse_On();
    state = STOP;
}

void pause1(void) {
    if (Time > Curr_Time + 5) {
        state = PASS_MOVEMENT;
    }
}

void pause2(void) {
    if (Time > Curr_Time + 9) {
        state = PASS_MOVEMENT;
    }
}
void spinclock(void){
    strcpy(display_line[0], " SPINNING ");
    strcpy(display_line[1], "  MOTORS  ");
    strcpy(display_line[2], "CLOCKWISE ");
    strcpy(display_line[3], "          ");
    display_changed = TRUE;
//    P6OUT |= L_FORWARD;
//    P6OUT |= R_REVERSE;
    state = STOP;
}
void spincounterclock(void){
    strcpy(display_line[0], " SPINNING ");
    strcpy(display_line[1], "  MOTORS  ");
    strcpy(display_line[2], "  COUNTER ");
    strcpy(display_line[3], "CLOCKWISE ");
    display_changed = TRUE;
//    P6OUT |= R_FORWARD;
//    P6OUT |= L_REVERSE;
    state = STOP;
}
