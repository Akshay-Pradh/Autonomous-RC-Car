/*
 * interrupts_serial.c
 *
 *  Created on: Mar 26, 2025
 *      Author: Akshay Pradhan
 */

//------------------------------------------------------------------------------
#include "macros.h"

//#pragma vector=EUSCI_A0_VECTOR
//__interrupt void eUSCI_A0_ISR(void){
//    unsigned int temp;
//    switch(__even_in_range(UCA0IV,0x08)){
//        case 0: // Vector 0 - no interrupt
//            break;
//        case 2: // Vector 2 – RXIFG
//            temp = usb_rx_ring_wr++;
//            USB_Char_Rx[temp] = UCA0RXBUF;      // RX -> USB_Char_Rx character
//            if (usb_rx_ring_wr >= (sizeof(USB_Char_Rx))){
//                usb_rx_ring_wr = BEGINNING;     // Circular buffer back to beginning
//            }
//            break;
//        case 4: // Vector 4 – TXIFG
//            // Code for Transmit
//            break;
//        default: break;
//    }
//}

#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
    unsigned char usb_value;
    switch(__even_in_range(UCA1IV,0x08)){
        case 0: // Vector 0 - no interrupt
            break;
        case 2: // Vector 2 – RXIFG
            usb_value = UCA1RXBUF;
            USB_Char_Rx[usb_rx_ring_wr] = usb_value;      // RX -> USB_Char_Rx character
            usb_rx_ring_wr++;
            if (usb_rx_ring_wr >= (sizeof(USB_Char_Rx))){
                usb_rx_ring_wr = BEGINNING;                 // Circular buffer back to beginning
            }
            break;
        case 4: // Vector 4 – TXIFG
            UCA1TXBUF = transmit_array[t_index];    // transmit character
            if (transmit_array[t_index] == '\n') {  // check if null, if so that was the last character to transmit
                t_index = 0;
                UCA1IE &= ~UCTXIE;  // disable the transmit interrupt
            }
            else  t_index++;  // increment transmit index
            break;
        default: break;
    }
}
//------------------------------------------------------------------------------




