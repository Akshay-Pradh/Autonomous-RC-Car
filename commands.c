/*
 * commands.c
 *
 *  Created on: Apr 5, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

unsigned char process_command = NO;     // initialize process command to NO (no command to process)
unsigned char IOT_line_ready = NO;      // initialize to IOT line not ready
unsigned char send_SSID = NO;           // initialize IOT ready to receive command
unsigned char iot_okay_check = NO;      // initialize IOT okay check to NO
unsigned char iot_mux_check = NO;       // initialize IOT Mux check flag to NO
unsigned char iot_server_check = NO;    // initialize IOT Server check flag to NO
unsigned char iot_command_complete = YES;   // command complete flag initialized YES (high)

// IOT commands
char AT[4] = "AT\r\n";
char ip_mac[10] = "AT+CIFSR\r\n";
char ssid[12] = "AT+CWJAP?\r\n";
char mux[13] = "AT+CIPMUX=1\r\n";
char server[22] = "AT+CIPSERVER=1,10809\r\n";

void Commands_Process(void) {

    if (process_command) {
        unsigned char command_char = process_buffer[process_row][1];    // the second character of our process row contains our command

        switch (command_char) {
            case '^':
                strcpy(transmit_array, "I'm here\n");        // send response back to PC through UCA1TX
                transmitting_message = YES;
                UCA1IE |= UCTXIE;

                process_command = NO;
                break;
            case 'F':
                setBaudRateUCA0(115200);                    // set the Baud Rate to 115,200 Hz (FAST)
                strcpy(transmit_array, "115,200\n");        // send response back to PC through UCA1TX
                transmitting_message = YES;
                UCA1IE |= UCTXIE;

                strcpy(display_line[2], "115,200 Hz");      // update the display
                display_changed = TRUE;

                process_command = NO;                       // done processing command
                break;
            case 'S':
                setBaudRateUCA0(9600);
                strcpy(transmit_array, "9,600\n");          // set the Baud Rate to 9,600 Hz (SLOW)
                transmitting_message = YES;
                UCA1IE |= UCTXIE;

                strcpy(display_line[2], " 9,600 Hz ");      // update the display
                display_changed = TRUE;

                process_command = NO;                       // done processing command
                break;
            default:
                strcpy(display_line[2], "  DEBUG!  ");      // update the display
                display_changed = TRUE;
                process_command = NO;                       // done processing command
                break;
        }
    }
}

void IOT_Process(void) {
    int i;

    if (iot_rx_ring_rd != iot_rx_ring_wr) {      // if we have a character to push from IOT ring buffer

        IOT_buffer[iot_index_row][iot_index_col] = IOT_Char_Rx[iot_rx_ring_rd++];

        if (iot_rx_ring_rd >= sizeof(IOT_Char_Rx)) iot_rx_ring_rd = BEGINNING;  // circular buffer back to beginning

        if (IOT_buffer[iot_index_row][iot_index_col] == '\n') {

            iot_row = iot_index_row;         // save the row index for processing
            iot_index_row++;                 // move to the next row
            IOT_line_ready = YES;            // flag that indicates we have received an entire row
            iot_okay_check = NO;             // no okay since we are processing a newline

            if (iot_index_row >= I_ROWS) iot_index_row = 0;
            iot_index_col = 0;               // reset the column to 0

            // Clear the following row
            for (i = 0; i < I_COLUMNS; i++) {         // clear the next line
               IOT_buffer[iot_index_row][i] = 0x00;
            }

        }
        else {
            switch(iot_index_col) {
                case 0:
                    if(IOT_buffer[iot_index_row][iot_index_col] == '+') {   // Got "+"
                        IOT_parse = YES;
                    }
                case 1:
                    if(IOT_buffer[iot_index_row][iot_index_col] == 'K') { // Got O"K"
                        if (send_SSID) {
                            send_SSID = NO;      // set IOT available for SSID to no
                            for (i = 0; i < sizeof(ssid); i++) {
                                 iot_transmit_array[i] = ssid[i];
                             }
                             iot_t_index = 0;       // reset iot transmit index to 0 (might not need this)
                             UCA0IE |= UCTXIE;      // enable UCA0TX
                        }
                        else if (iot_mux_check) {
                            iot_mux_check = NO;
                            iot_server_check = YES;
                        }
                        else if (iot_server_check) {
                            iot_server_check = NO;
                            for (i = 0; i < sizeof(server); i++) {
                                 iot_transmit_array[i] = server[i];
                             }
                             iot_t_index = 0;       // reset iot transmit index to 0 (might not need this)
                             UCA0IE |= UCTXIE;      // enable UCA0TX
                             Start_Pinging = YES;   // set the Start Pinging flag to HIGH
                        }
                    }
                    break;
                case 3:
                    if(IOT_buffer[iot_index_row][iot_index_col] == 'J') {   // Got +CW"J"AP
                        ssid_found = YES;
                        for (i = 0; i < sizeof(ssid_display); i++) {        // clear the SSID display array
                            ssid[i] = ' ';
                        }
                    }

                case 4:
                    if(IOT_buffer[iot_index_row][iot_index_col] == 'y') {   // Got read"y"
                       break;
                    }
                    break;
                case 5:
                    if(IOT_buffer[iot_index_row][iot_index_col] == 'G') {    // WIFI "G"OT IP
                        for (i = 0; i < sizeof(ip_mac); i++) {
                            iot_transmit_array[i] = ip_mac[i];
                        }
                        iot_t_index = 0;    // reset iot transmit index to 0 (might not need this)
                        UCA0IE |= UCTXIE;   // enable UCA0TX
                    }
                    break;
                case 10:
                    if(IOT_buffer[iot_index_row][iot_index_col] == 'I') {
                        ip_address_found = YES;
                        display_changed = TRUE;
                        for (i = 0; i < sizeof(ip_address1); i++) {    // clearing the ip address array for display (with spaces)
                            ip_address1[i] = ' ';
                            ip_address2[i] = ' ';
                        }
                    }
                    else if(IOT_buffer[iot_index_row][iot_index_col] == 'M') {
                        send_SSID = YES;    // ready for SSID command
                    }

               default: break;
            }
            iot_index_col++;        // increment the column index
            IOT_line_ready = NO;    // reset the IOT_line_ready flag when reading characters into a buffer
        }
    }
}




