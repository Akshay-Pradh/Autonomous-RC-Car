/*
 * serial.c
 *
 *  Created on: Mar 26, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

unsigned int pb_index_row = 0;
unsigned int pb_index_col = 0;

unsigned int iot_index_row = 0;
unsigned int iot_index_col = 0;

unsigned int t_index = 0;
unsigned int iot_t_index = 0;

long currentBaudRate = 115200;

unsigned char transmitting_message = NO;        // by default not transmitting message out of the transmit array
unsigned char message_status = COMPLETE;

unsigned char IOT_reset = RESET;
unsigned int IOT_parse = NO;
unsigned int ip_address_found = NO;
unsigned int ssid_found = NO;

unsigned int ping_count = 0;            // initialize ping time count to 0
unsigned char Start_Pinging = NO;       // intialize Start Pinging flag to NO
unsigned char ping_internet = NO;       // intialize Ping Internet flag to NO

void Init_Serial_UCA0(void){
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

    iot_rx_ring_wr = BEGINNING;
    iot_rx_ring_rd = BEGINNING;

    // Configure UART 0
    UCA0CTLW0 = 0;                  // Use word register
    UCA0CTLW0 |= UCSWRST;           // Set Software reset enable
    UCA0CTLW0 |= UCSSEL__SMCLK;     // Set SMCLK as fBRCLK

    UCA0CTLW0 &= ~UCMSB;                 // MSB, LSB select
    UCA0CTLW0 &= ~UCSPB;                 // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    UCA0CTLW0 &= ~UCPEN;                 // No Parity
    UCA0CTLW0 &= ~UCSYNC;
    UCA0CTLW0 &= ~UC7BIT;
    UCA0CTLW0 |= UCMODE_0;

    //  BRCLK   Baudrate UCOS16  UCBRx  UCFx    UCSx    neg   pos  neg  pos
    //8000000   115200     1      4      5      0x55   -0.80 0.64 -1.12 1.76

    UCA0BRW = 4;                // 460800 Baud
    UCA0MCTLW = 0x5551;

    UCA0CTLW0 &= ~ UCSWRST;     // Set Software reset enable
    UCA0TXBUF = 0x00;           // Prime the pump
    UCA0IE &= ~UCTXIE;          // Disable TX interrupt
    UCA0IE |= UCRXIE;           // Enable RX interrupt
}

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

    allow_PC_TX = DISABLE;              // initialize the PC TX communication flag to OFF

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
    //8000000   115200     1      4      5      0x55   -0.80 0.64 -1.12 1.76

    UCA1BRW = 4;                // 115200 Baud
    UCA1MCTLW = 0x5551;

    UCA1CTLW0 &= ~ UCSWRST;     // Set Software reset enable
    UCA1TXBUF = 0x00;           // Prime the pump
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

    UCA0CTLW0 |= UCSWRST;     // Put serial in reset
    UCA1CTLW0 |= UCSWRST;     // Put serial in reset

    switch(baud) {
        case 460800:
            UCA0BRW = 17;            // 460,800 Baud
            UCA1BRW = 17;            // 460,800 Baud
            UCA0MCTLW = 0x4A00;
            UCA1MCTLW = 0x4A00;
            break;
        case 115200:
            UCA0BRW = 4;            // 115,200 Baud
            UCA1BRW = 4;            // 115,200 Baud
            UCA0MCTLW = 0x5551;
            UCA1MCTLW = 0x5551;
            break;
        case 9600:
            UCA0BRW = 52;            // 9,600 Baud
            UCA1BRW = 52;            // 9,600 Baud
            UCA0MCTLW = 0x4911;
            UCA1MCTLW = 0x4911;
        default: break;
    }

    UCA0CTLW0 &= ~UCSWRST;      // Release UCA0 from reset
    UCA0IE |= UCRXIE;           // Enable UCA0 RX interrupt

    UCA1CTLW0 &= ~UCSWRST;      // Release UCA1 from reset
    UCA1IE |= UCRXIE;           // Enable UCA1 RX interrupt

}

void setBaudRateUCA0(long baud){
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

    UCA0CTLW0 |= UCSWRST;     // Put serial in reset

    switch(baud) {
        case 460800:
            UCA0BRW = 17;            // 460,800 Baud
            UCA0MCTLW = 0x4A00;
            break;
        case 115200:
            UCA0BRW = 4;            // 115,200 Baud
            UCA0MCTLW = 0x5551;
            break;
        case 9600:
            UCA0BRW = 52;            // 9,600 Baud
            UCA0MCTLW = 0x4911;
        default: break;
    }

    UCA0CTLW0 &= ~UCSWRST;      // Release UCA0 from reset
    UCA0IE |= UCRXIE;           // Enable UCA0 RX interrupt

}

void setBaudRateUCA1(long baud){
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

    UCA1CTLW0 |= UCSWRST;     // Put serial in reset

    switch(baud) {
        case 460800:
            UCA1BRW = 17;            // 460,800 Baud
            UCA1MCTLW = 0x4A00;
            break;
        case 115200:
            UCA1BRW = 4;            // 115,200 Baud
            UCA1MCTLW = 0x5551;
            break;
        case 9600:
            UCA1BRW = 52;            // 9,600 Baud
            UCA1MCTLW = 0x4911;
        default: break;
    }

    UCA1CTLW0 &= ~UCSWRST;      // Release UCA0 from reset
    UCA1IE |= UCRXIE;           // Enable UCA0 RX interrupt

}


