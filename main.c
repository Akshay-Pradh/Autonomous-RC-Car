//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "macros.h"


char password[4] = "1080";                       // password of my device
char ping[22] = "AT+PING=\"google.com\"\r\n";    // ping message array
unsigned char process_movement = NO;             // flag for processing movement based on the password

//void main(void){
void main(void){
//    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
    PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings

  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
  Init_ADC();                          // Initialize ADC (start reading values)
  Init_Serial_UCA0();                  // Initialize UCA0
  Init_Serial_UCA1();                  // Initialize UCA1
  Init_DAC();                          // Initialize DAC
  //P2OUT &= ~RESET_LCD;

  // Place the contents of what you want on the display, in between the quotes
  // Limited to 10 characters per line
  strcpy(display_line[0], " Waiting  ");
  strcpy(display_line[1], "for input ");
  strcpy(display_line[2], "          ");
  strcpy(display_line[2], "          ");
  display_changed = TRUE;
//  Display_Update(0,0,0,0);

  // Defaults:
//  wheel_move = 0;
//  forward = TRUE;
//  event = IDLE;

//  P2OUT |= IR_LED;        // testing IR LED
//------------------------------------------------------------------------------
// Beginning of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
    Commands_Process();                // Process commands from the PC
    Switches_Process();                // Check for switch state change
    Display_Process();                 // Update Display
    Wheels_Process();                  // Check for Wheel Changes
    Motion_Process();                  // Updates Motion based on IOT commands
    Intercept_Process();              // Checking for changes in Intercept State Machine
    IOT_Process();                     // Check for IOT commands

    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF

    unsigned int i, j, k;              // Loop index variables


    // Display Time Code (once we have received our first command)

    if (TIME_DISPLAY) {
        Display_Time();     // start displaying the time
    }

    // Display the Left Detect and Right Detect (during calibration)

    if (CALIBRATE && ADC_DISPLAY) {
        strcpy(display_line[2], "LD:       ");
        strcpy(display_line[3], "RD:       ");
        HEXtoBCD(ADC_Left_Detect);
        adc_line(3, 3);
        HEXtoBCD(ADC_Right_Detect);
        adc_line(4, 3);
        display_changed = TRUE;
        ADC_DISPLAY = NO;
    }



    // IOT reset code (upon initialization)
    switch (IOT_reset) {
        case RESET:
            Curr_Time = Time_Precise;    // Set the Curr_Time
            IOT_reset = RESETTING;
            break;
        case RESETTING:
            if (Time_Precise > Curr_Time + 2) {
                P3OUT |= IOT_RN_CPU;       // Set IOT_EN active high after 100ms (reset)
                IOT_reset = NONE;          // Set the reset to NONE state
            }
            break;
        default: break;
    }

   // IOT IP Display Code
    if (IOT_parse && ip_address_found && IOT_line_ready) {    // parsing through IOT message for ip_address

        IOT_parse = NO;             // reset IOT_parse flag
        ip_address_found = NO;      // reset ip address found flag
        IOT_line_ready = NO;        // reset IOT_line_ready flag

        j = 0;          // setting IOT buffer loop index to 0
        k = 2;          // setting ip_address loop index to 2

        while (IOT_buffer[iot_row][j] != '"') j++;     // look for first quote
        j++;                                           // move one past quote

        unsigned int point_count = 0;
        while (point_count < 2 && k < 10) {
            if (IOT_buffer[iot_row][j] == '.') {
                point_count++;
            }
            ip_address1[k++] = IOT_buffer[iot_row][j];
            j++;
        }

        strncpy(display_line[2], ip_address1, 10);
        display_changed = TRUE;

        k = 2;          // resetting ip_address loop index to 2

        while (IOT_buffer[iot_row][j] != '"' &&  k < 10) {    // copy characters into the ip address display array until next quote
            ip_address2[k++] = IOT_buffer[iot_row][j];
            j++;
        }

        strncpy(display_line[3], ip_address2, 10);
        display_changed = TRUE;
    }

    // SSID DISPLAY CODE
    if (IOT_parse && ssid_found && IOT_line_ready) {

        IOT_parse = NO;             // reset IOT_parse flag code
        ssid_found = NO;            // reset ssid_found flag
        IOT_line_ready = NO;        // reset IOT_line_ready flag

        j = 0;
        k = 0;

        while (IOT_buffer[iot_row][j] != '"') j++;     // look for first quote
        j++;                                           // move one past quote

        while (IOT_buffer[iot_row][j] != '"') {
            ssid_display[k++] = IOT_buffer[iot_row][j];
            j++;
        }

//        don't need to display the ssid for project 10 but keeping this function in case I want to implement it somewhere

//        strncpy(display_line[0], ssid_display, 10);
//        display_changed = TRUE;

        // once done displaying the SSID, we want to set up CIPMUX

        for (i = 0; i < sizeof(mux); i++) {
            iot_transmit_array[i] = mux[i];
        }
        iot_t_index = 0;            // reset transmit index
        UCA0IE |= UCTXIE;           // enable transmit
        iot_mux_check = YES;        // we want to make sure we get OKAY from MUX before Server connection
    }


    // CODE TO PARSE THROUGH PASSWORD
    if (IOT_parse && iot_command_received && IOT_line_ready) {

        IOT_parse = NO;                  // reset IOT_parse flag code
        iot_command_received = NO;       // reset iot_command_received flag
        IOT_line_ready = NO;             // reset IOT_line_ready flag

        j = 0;
        k = 0;

        while (IOT_buffer[iot_row][j] != '^') j++;  // increment until we find our hat character
        j++;                                        // move one past the hat character (first key of password)

        // testing the password
        process_movement = YES;

        for (i = 0; i < 4; i++) {
            if (IOT_buffer[iot_row][j++] != password[i]) {
                process_movement = NO;
            }
        }

        // save the motion character and motion duration + display most recent command
        if (process_movement) {
            clear_display();                                    // clear the display since we are about to display motion
            i = 0;                                              // reset i: display index
            display_line[3][i++] = IOT_buffer[iot_row][j];      // display the movement character
            movement_letter = IOT_buffer[iot_row][j++];         // store the movement letter after the password

            for (k = 0; k < 4; k++) {                           // storing the numbers from array as one number: movement duration
                display_line[3][i++] = IOT_buffer[iot_row][j];
                movement_duration = movement_duration * 10 + (IOT_buffer[iot_row][j++] - '0');
            }
            display_changed = TRUE;     // display changed = TRUE
            process_movement = NO;      // reset the process_movement flag

            // display the IP address
            strncpy(display_line[1], ip_address1, 10);
            strncpy(display_line[2], ip_address2, 10);
            display_changed = TRUE;
        }
        else {
            iot_command_complete = YES; // set command complete flag back to high if password is incorrect (command execution complete)
        }

    }

    // Code to ping the internet based on timer

    if (iot_command_complete && ping_internet) {
        iot_command_complete = NO;      // set the iot_command_complete to NO;
        ping_internet = NO;             // set the ping flag to NO

        for (i = 0; i < sizeof(ping); i++) {
            iot_transmit_array[i] = ping[i];
        }
        iot_t_index = 0;       // reset iot transmit index to 0 (might not need this)
        UCA0IE |= UCTXIE;      // enable UCA0TX
    }



    // CODE TO CHECK PROCESS BUFFER FOR USB RING BUFFER

    // check if rd != wr for USB ring buffer
    if (usb_rx_ring_rd != usb_rx_ring_wr) {
        // we move character from ring buffer to process buffer
        process_buffer[pb_index_row][pb_index_col] = USB_Char_Rx[usb_rx_ring_rd];
        if (USB_Char_Rx[usb_rx_ring_rd] == '\r') {

            process_row = pb_index_row;     // save the row index for displaying
            pb_index_row++;                 // move to the next row
            if (pb_index_row >= P_ROWS) pb_index_row = 0;
            pb_index_col = 0;               // reset the column to 0

            // Clear the following row
            for (i = 0; i < P_COLUMNS; i++) {         // clear the next line
                process_buffer[pb_index_row][i] = 0x00;
            }
            process_command = YES;      // process the command we have read into the process buffer
        }
        else {
            pb_index_col++;     // increment process buffer pointer
            // dont check for the end of column (assume that process buffer long enough to store any message)
        }

        usb_rx_ring_rd++;   // increment IOT ring rd index
        if (usb_rx_ring_rd >= (sizeof(USB_Char_Rx))) usb_rx_ring_rd = BEGINNING;   // Circular buffer back to beginning
    }

  }

}
