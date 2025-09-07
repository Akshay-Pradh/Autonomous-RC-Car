/*
 * serial.c
 *
 *  Created on: Mar 26, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

char NCSU_array[9] = "NCSU  #1\n";    // extra spot to newline terminate
unsigned int pb_index_row = 0;
unsigned int pb_index_col = 0;
unsigned int t_index = 0;
long currentBaudRate = 460800;

//void Init_Serial_UCA0(void){}

void Init_Serial_UCA1(void){
    //-----------------------------------------------------------------------------
    //                                               TX error (%) RX error (%)
    //  BRCLK   Baudrate UCOS16  UCBRx  UCFx    UCSx    neg   pos  neg  pos
    //  8000000    4800     1     104     2     0xD6   -0.08 0.04 -0.10 0.14
    //  8000000    9600     1      52     1     0x49   -0.08 0.04 -0.10 0.14
    //  8000000   19200     1      26     0     0xB6   -0.08 0.16 -0.28 0.20
    //  8000000   57600     1       8    10     0xF7   -0.32 0.32 -1.00 0.36
    //  8000000  115200     1       4     5     0x55   -0.80 0.64 -1.12 1.76
    //  8000000  460800     0      17     0     0x4A   -2.72 2.56 -3.76 7.28
    //----------------------------------------------------------------------------
//    int i;
//    for(i=0; i < SMALL_RING_SIZE; i++){
//        USB_Char_Rx[i] = 0x00;            // USB Rx Buffer
//    }
    usb_rx_ring_wr = BEGINNING;
    usb_rx_ring_rd = BEGINNING;

    // Configure UART 1
    UCA1CTLW0 = 0;                  // Use word register
    UCA1CTLW0 |= UCSWRST;           // Set Software reset enable
    UCA1CTLW0 |= UCSSEL__SMCLK;     // Set SMCLK as fBRCLK

    UCA1CTLW0 &= ~UCMSB;                 // MSB, LSB select
    UCA1CTLW0 &= ~UCSPB;                 // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    UCA1CTLW0 &= ~UCPEN;                 // No Parity
    UCA1CTLW0 &= ~UCSYNC;
    UCA1CTLW0 &= ~UC7BIT;
    UCA1CTLW0 |= UCMODE_0;

    //  BRCLK   Baudrate UCOS16  UCBRx  UCFx    UCSx    neg   pos  neg  pos
    //8000000  460800     0      17     0     0x4A   -2.72 2.56 -3.76 7.28

    UCA1BRW = 17;                // 460800 Baud
    UCA1MCTLW = 0x4A00;

    UCA1CTLW0 &= ~ UCSWRST;     // Set Software reset enable
//    UCA1TXBUF = 0x00;           // Prime the pump
    UCA1IE &= ~UCTXIE;          // Disable TX interrupt
    UCA1IE |= UCRXIE;           // Enable RX interrupt
}



void setBaudRate(long baud) {
    //-----------------------------------------------------------------------------
    //                                               TX error (%) RX error (%)
    //  BRCLK   Baudrate UCOS16  UCBRx  UCFx    UCSx    neg   pos  neg  pos
    //  8000000    4800     1     104     2     0xD6   -0.08 0.04 -0.10 0.14
    //  8000000    9600     1      52     1     0x49   -0.08 0.04 -0.10 0.14
    //  8000000   19200     1      26     0     0xB6   -0.08 0.16 -0.28 0.20
    //  8000000   57600     1       8    10     0xF7   -0.32 0.32 -1.00 0.36
    //  8000000  115200     1       4     5     0x55   -0.80 0.64 -1.12 1.76
    //  8000000  460800     0      17     0     0x4A   -2.72 2.56 -3.76 7.28
    //----------------------------------------------------------------------------

    if (currentBaudRate == baud) return;    // check if operation is unnecessary
    else currentBaudRate = baud;            // updated current baud rate

    UCA1CTLW0 |= UCSWRST;     // Put serial in reset

    switch(baud) {
        case 460800:
//            UCA0BRW = 17;
            UCA1BRW = 17;            // 460800 Baud
//            UCA0MCTLW = 0x4A00;
            UCA1MCTLW = 0x4A00;
            break;
        case 115200:
//            UCA0BRW = 4
            UCA1BRW = 4;            // 115200 Baud
//            UCA0MCTLW = 0x5551;
            UCA1MCTLW = 0x5551;
            break;
        default: break;
    }

    UCA1CTLW0 &= ~UCSWRST;      // Release from reset
    UCA1IE |= UCRXIE;           // Enable RX interrupt

}

