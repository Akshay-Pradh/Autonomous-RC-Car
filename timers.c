/*
 * timers.c
 *
 *  Created on: Feb 17, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

// Globals
volatile unsigned int Time = 0;             // time measurement (every 200ms)
volatile unsigned int Time_Precise = 0;     // time measurement (every 50ms)
volatile unsigned int Curr_Time = 0;        // current time measurement
unsigned char TIME_DISPLAY = NO;            // initialize display timer to NO

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

// Timer B1 initialization sets up both B1_0, B1_1-B1_2 and overflow

void Init_Timer_B1(void) {
    TB1CTL = TBSSEL__SMCLK;         // SMCLK source
    TB1CTL |= TBCLR;                // Resets TB1R, clock divider, count direction
    TB1CTL |= MC__CONTINOUS;        // Continuous up

    TB1CTL |= ID__8;                // Divide clock by 8
    TB1EX0 = TBIDEX__8;             // Divide clock by an additional 8

    TB1CCR0 = TB1CCR0_INTERVAL;        // CCR0 (ADC reading update, every 10ms)
    TB1CCTL0 &= ~CCIE;                 // CCR0 enable interrupt (disabled, will enable when using ADC)

    TB1CCR1 = TB1CCR1_INTERVAL;        // CCR1 enabled (ADC display update)
    TB1CCTL1 &= ~CCIE;                 // CCR1 enable interrupt (disabled, will enable when using ADC)

    TB1CCR2 = TB1CCR2_INTERVAL;        // CCR2 enabled
    TB1CCTL2 |= CCIE;                  // CCR2 enable interrupt (precise timer)

    TB1CTL &= ~TBIE;              // Disable Overflow Interrupt
    TB1CTL &= ~TBIFG;             // Clear Overflow Interrupt flag
}

// Timer B3 initialization sets up both B3_0, B3_1-B3_2 and overflow

void Init_Timer_B3(void) {
    //------------------------------------------------------------------------------
    // SMCLK source, up count mode, PWM Right Side
    // TB3.1 P6.0 LCD_BACKLITE
    // TB3.2 P6.1 R_FORWARD
    // TB3.3 P6.2 R_REVERSE
    // TB3.4 P6.3 L_FORWARD
    // TB3.5 P6.4 L_REVERSE
    //------------------------------------------------------------------------------
     TB3CTL = TBSSEL__SMCLK;    // SMCLK
     TB3CTL |= MC__UP;          // Up Mode
     TB3CTL |= TBCLR;           // Clear TAR

     PWM_PERIOD = WHEEL_PERIOD;         // PWM Period [Set this to 50005]

     TB3CCTL1 = OUTMOD_7;               // CCR1 reset/set
     LCD_BACKLITE_DIMMING = PERCENT_80;  // P6.0 Right Forward PWM duty cycle

     TB3CCTL2 = OUTMOD_7;               // CCR2 reset/set
     RIGHT_FORWARD_SPEED = WHEEL_OFF;   // P6.1 Right Forward PWM duty cycle

     TB3CCTL3 = OUTMOD_7;               // CCR3 reset/set
     LEFT_FORWARD_SPEED = WHEEL_OFF;    // P6.2 Left Forward PWM duty cycle

     TB3CCTL4 = OUTMOD_7;               // CCR4 reset/set
     RIGHT_REVERSE_SPEED = WHEEL_OFF;   // P6.3 Right Reverse PWM duty cycle

     TB3CCTL5 = OUTMOD_7;               // CCR5 reset/set
     LEFT_REVERSE_SPEED = WHEEL_OFF;    // P6.4 Left Reverse PWM duty cycle
}

//------------------------------------------------------------------------------

void Init_Timers(void) {
    Init_Timer_B0();
    Init_Timer_B1();
    Init_Timer_B3();
}

void Display_Time(void) {

    unsigned int time_val = Time - START_TIME;
    char time_char[4];
    unsigned int value = 0;
    unsigned int i;

    // populate array with 0's
    for(i = 0; i < 4; i++) {
        time_char[i] = '0';
    }

    // set decimal point
    time_char[3] = 's';

    // 999sec = 4995
    if (time_val > 4995) {
        return;
    }

    // 100sec - 999sec ------> 500 - 4995
    while (time_val >= 500) {
        time_val = time_val - 500;
        value += 1;
    }
    time_char[0] = 0x30 + value;
    value = 0;

    // 10sec - 100sec -------> 50 - 500
    while (time_val >= 50) {
        time_val = time_val - 50;
        value += 1;
    }
    time_char[1] = 0x30 + value;
    value = 0;

    // 1sec - 10sec ------> 5 - 50
    while (time_val >= 5) {
        time_val = time_val - 5;
        value += 1;
    }
    time_char[2] = 0x30 + value;
    value = 0;

    unsigned int j;
    for (j = 6; j < 10; j++) {
        display_line[3][j] = time_char[j - 6];
    }

    display_changed = TRUE;

}
//------------------------------------------------------------------------------

