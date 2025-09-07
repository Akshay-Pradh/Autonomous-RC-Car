/*
 * interrupts_serial.c
 *
 *  Created on: Mar 26, 2025
 *      Author: Akshay Pradhan
 */

//------------------------------------------------------------------------------
#include "macros.h"

#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
    unsigned char iot_value;
    switch(__even_in_range(UCA0IV,0x08)){
        case 0: // Vector 0 - no interrupt
            break;
        case 2: // Vector 2 – RXIFG
            iot_value = UCA0RXBUF;

            if (iot_value == '^') {
                if (!TIME_DISPLAY) {
                    TIME_DISPLAY = YES;         // if not displaying time, display time (when we get our first command)
                    START_TIME = Time;
                }
                if (iot_command_complete) {     // flag here makes sure that the previous command was completed, set high at the end of all command sequences
                    iot_command_complete = NO;        // set the command_complete to NO when we receive a command
                    iot_command_received = YES;       // set the iot_command_recived flag to YES when we received a new IOT command
                }
            }
            IOT_Char_Rx[iot_rx_ring_wr++] = iot_value;    // Add to ring buffer
            if (iot_rx_ring_wr >= sizeof(IOT_Char_Rx)) {
                iot_rx_ring_wr = BEGINNING;
            }

            UCA1TXBUF = iot_value;      // Send value out to PC via UCA1TXBUF
            break;
        case 4: // Vector 4 – TXIFG
            UCA0TXBUF = iot_transmit_array[iot_t_index];    // transmit character
            if (iot_transmit_array[iot_t_index] == '\n') {  // check if newline, if so that was the last character to transmit
               iot_t_index = 0;
               int i;
               for (i = 0; i < sizeof(iot_transmit_array); i++) {   // clearing the transmit array
                   iot_transmit_array[i] = 0x00;
               }
               UCA0IE &= ~UCTXIE;                                   // disable the transmit interrupt
               if (Start_Pinging)  iot_command_complete = YES;      // we can clear iot_command_complete if pinging is happening
            }
            else {
               iot_t_index++;                   // increment t_index
            }
           break;
        default: break;
    }
}

#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
    unsigned char usb_value;
    switch(__even_in_range(UCA1IV,0x08)){
        case 0: // Vector 0 - no interrupt
            break;
        case 2: // Vector 2 – RXIFG
            if (!allow_PC_TX) {
                allow_PC_TX = ENABLE;           // we received a char from PC therefore we are allowed to transmitting to PC
            }

            usb_value = UCA1RXBUF;          //  received char from PC

            if (usb_value == '^') {         // if we are about to receive a command
                command_received = YES;     // set command received flag to YES when we received a new command
            }

            if (command_received) {         // if we received a command
                USB_Char_Rx[usb_rx_ring_wr] = usb_value;
                usb_rx_ring_wr++;
                if (usb_rx_ring_wr >= (sizeof(USB_Char_Rx))){
                    usb_rx_ring_wr = BEGINNING;                  // circular buffer back to beginning
                }
                if (usb_value == '\r') command_received = NO;   // once we've received the carriage return character, our command has ended
            }
            else {
                UCA0TXBUF = usb_value;      // echo character if not used in a command
            }


        case 4: // Vector 4 – TXIFG
            if (transmitting_message){                  // might not need this, this is all setup for commands from termite
                UCA1TXBUF = transmit_array[t_index];    // transmit character
                if (transmit_array[t_index] == '\n') {  // check if newline, if so that was the last character to transmit
                    t_index = 0;
                    UCA1IE &= ~UCTXIE;               // disable the transmit interrupt
                    transmitting_message = NO;       // set transmitting message flag to NO
                }
                else {
                    t_index++;                      // increment t_index
                }
            }

            break;
        default: break;
    }
}
////------------------------------------------------------------------------------




