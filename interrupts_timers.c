/*
 * interrupts_timers.c
 *
 *  Created on: Feb 18, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

unsigned char DISPLAY_TIME = 0x00;

//-------------------------------------------------------------------------------
//                                  TIMER 0
//-------------------------------------------------------------------------------


#pragma vector=TIMER_B0_CCR0_VECTOR
__interrupt void Timer_B0_CCR0_ISR(void){
     //---------------------------------------------------------------------------
     // TimerB0 0 Interrupt handler
     //---------------------------------------------------------------------------
     update_display = TRUE;
     Time++;
     TB0CCR0 += TB0CCR0_INTERVAL;   // Add Offset to TB0CCR0
     //---------------------------------------------------------------------------
}

#pragma vector=TIMER_B0_CCR1_2_OV_VECTOR
__interrupt void Timer_B0_CCR1_2_OV_VECTOR_ISR(void){
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
//                                  TIMER 1
//-------------------------------------------------------------------------------

#pragma vector=TIMER_B1_CCR0_VECTOR
__interrupt void Timer_B1_CCR0_ISR(void){
    //---------------------------------------------------------------------------
    // TimerB1 0 Interrupt handler
    //---------------------------------------------------------------------------
    ADCCTL0 |= ADCSC;               // Start next ADC sample (every 10msec)
    TB1CCR0 += TB1CCR0_INTERVAL;    // Add Offset to TB1CCR0
    //---------------------------------------------------------------------------
}

#pragma vector=TIMER_B1_CCR1_2_OV_VECTOR
__interrupt void Timer_B1_CCR1_2_OV_VECTOR_ISR(void){
    //---------------------------------------------------------------------------
    // TimerB1 1-2, Overflow Interrupt Vector (TBIV) handler
    //---------------------------------------------------------------------------
    switch(__even_in_range(TB1IV,14)){
        case  0: break;
            // No interrupt
        case  2:
            // ADC Display update interrupt
            ADC_DISPLAY = 1;
            TB1CCR1 += TB1CCR1_INTERVAL;
            break;
        case  4:
            Time_Precise++;
            TB1CCR2 += TB1CCR2_INTERVAL;
            break;
        case 14:
            // overflow
            break;
        default: break;
    }
}


//-------------------------------------------------------------------------------
//                                  TIMER 2
//-------------------------------------------------------------------------------


