/*
 * interrupts_ports.c
 *
 *  Created on: Feb 18, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

// Globals

#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
// Switch 1
    if (P4IFG & SW1) {
        P4IE &= ~SW1;           // Disable SW1
        P4IFG &= ~SW1;          // IFG SW1 cleared
        TB0CCTL1 &= ~CCIFG;     // Clear TimerB0 interrupt Flag for CCR1

        debounce_count_sw1 = 0;             // Reset debounce count
        debounce_in_progress_sw1 = YES;     // Initialize debounce counter

        TB0CCR1 = TB0R + DEBOUNCE_INTERVAL;     // Add Interval to TB0R for TB0CCR1
        TB0CCTL1 |= CCIE;                       // Enable TimerB0_1

        P6OUT &= ~LCD_BACKLITE;  // Turn off LCD_BACKLITE
        TB0CCTL0 &= ~CCIE;       // Disable TimerB0_0 (stop LCD blinking)

        sw1_pressed = YES;      // change sw1_pressed global to YES
    }
}
#pragma vector=PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
     // Switch 2
    if (P2IFG & SW2) {
        P2IE &= ~SW2;           // Disable SW2
        P2IFG &= ~SW2;          // IFG SW2 cleared
        TB0CCTL2 &= ~CCIFG;     // clear TimerB0 interrupt Flag for CCR2

        debounce_count_sw2 = 0;             // Reset debounce count
        debounce_in_progress_sw2 = YES;     // Initialize debounce counter

        TB0CCR2 = TB0R + DEBOUNCE_INTERVAL;     // Add Interval to TB0R for TB0CCR2
        TB0CCTL2 |= CCIE;                       // Enable TimerB0_2

        P6OUT &= ~LCD_BACKLITE;  // Turn off LCD_BACKLITE
        TB0CCTL0 &= ~CCIE;       // Disable TimerB0_0 (stop LCD blinking)

        sw2_pressed = YES;      // change sw2_pressed global to YES
    }
}




