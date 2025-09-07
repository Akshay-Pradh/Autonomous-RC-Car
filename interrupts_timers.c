/*
 * interrupts_timers.c
 *
 *  Created on: Feb 18, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
     //---------------------------------------------------------------------------
     // TimerB0 0 Interrupt handler
     //---------------------------------------------------------------------------
     update_display = TRUE;
     P6OUT ^= LCD_BACKLITE;      // Toggle LCD_BACKLITE
     TB0CCR0 += TB0CCR0_INTERVAL;
     // Add Offset to TBCCR0
     //---------------------------------------------------------------------------
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
    //---------------------------------------------------------------------------
    // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
    //---------------------------------------------------------------------------
    switch(__even_in_range(TB0IV,14)){
        case  0: break;
            // No interrupt
        case  2:
            // SW1
            if (debounce_in_progress_sw1) {
                debounce_count_sw1++;
                if (debounce_count_sw1 > DEBOUNCE_THRESHOLD) {
                    debounce_in_progress_sw1 = NO;
                    P4IE |= SW1;        // re-enable SW1 interrupt
                    TB0CCTL1 &= ~CCIE;  // disable debounce timer
                    TB0CCTL0 |= CCIE;   // re-enable TimerB0_0 (continue LCD blinking)
                }
            }
            break;
        case  4:
            // SW2
            if (debounce_in_progress_sw2) {
                debounce_count_sw2++;
                if (debounce_count_sw2 > DEBOUNCE_THRESHOLD) {
                    debounce_in_progress_sw2 = NO;
                    P2IE |= SW2;        // re-enable SW1 interrupt
                    TB0CCTL2 &= ~CCIE;  // disable debounce timer
                    TB0CCTL0 |= CCIE;   // re-enable TimerB0_0 (continue LCD blinking)
                }
            }
            break;
        case 14:
            // overflow
            break;
        default: break;
    }

}
//-------------------------------------------------------------------------------


