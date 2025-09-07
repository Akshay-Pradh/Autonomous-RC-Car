/*
 * timers.c
 *
 *  Created on: Feb 17, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

// Globals
volatile unsigned int Time = 0;         // time measurement (every 200ms)
volatile unsigned int Curr_Time = 0;    // current time measurement

//-------------------------------------------------------------------------------
// Timer B0 initialization sets up both B0_0, B0_1-B0_2 and overflow

void Init_Timer_B0(void) {
    TB0CTL = TBSSEL__SMCLK;         // SMCLK source
    TB0CTL |= TBCLR;                // Resets TB0R, clock divider, count direction
    TB0CTL |= MC__CONTINOUS;        // Continuous up

    TB0CTL |= ID__8;                // Divide clock by 8
    TB0EX0 = TBIDEX__8;             // Divide clock by an additional 8

    TB0CCR0 = TB0CCR0_INTERVAL;     // CCR0
    TB0CCTL0 |= CCIE;               // CCR0 enable interrupt

    TB0CCR1 = 0;                     // CCR1 initially disabled
    TB0CCTL1 &= ~CCIE;               // CCR1 disable interrupt

    TB0CCR2 = 0;                     // CCR2 initially disabled
    TB0CCTL2 &= ~CCIE;               // CCR2 disable interrupt

    TB0CTL &= ~TBIE;              // Disable Overflow Interrupt
    TB0CTL &= ~TBIFG;             // Clear Overflow Interrupt flag
}

//------------------------------------------------------------------------------

void Init_Timers(void) {
    Init_Timer_B0();
}

