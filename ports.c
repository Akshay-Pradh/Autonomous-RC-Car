/*
 * ports.c
 *
 *  Created on: Jan 29, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

void Init_Port_1(void) {
    /*
     *RED_LED                (0x01) // 1.0 RED LED 0
     *A1_SEEED               (0x02) // 1.1 A1_SEEED
     *V_DETECT_L             (0x04) // 1.2 V_DETECT_L
     *V_DETECT_R             (0x08) // 1.3 V_DETECT_R
     *V_A4_SEEED             (0x10) // 1.4 A4_SEEED
     *V_THUMB                (0x20) // 1.5 V_THUMB
     *UCA0RXD                (0x40) // 1.6 Back Channel UCA0RXD
     *UCA0TXD                (0x80) // 1.7 Back Channel UCA0TXD
     */

    //------------------------------------------------------------------------------

    P1OUT = 0x00; // P2 set Low
    P1DIR = 0x00; // Set P2 direction to output

    // Port 1: Pin 0
    P1SEL0 &= ~RED_LED;    // RED_LED GPIO Operation
    P1SEL1 &= ~RED_LED;    // RED_LED GPIO Operation
    P1OUT &= ~RED_LED;     // Initial Value = Low / Off
    P1DIR |= RED_LED;      // Direction = output

    // Port 1: Pin 1
    P1SELC |= A1_SEEED;    // A1_SEED ADC Operation

    // Port 1: Pin 2
    P1SELC |= V_DETECT_L;  // V_DETECT_L ADC Operation

    // Port 1: Pin 3
    P1SELC |= V_DETECT_R;  // V_DETECT_R ADC Operation

    // Port 1: Pin 4
    P1SELC |= V_A4_SEEED;  // V_A4_SEEED ADC Operation

    // Port 1: Pin 5
    P1SELC |= V_THUMB;     // V_THUMB ADC Operation

    // Port 1: Pin 6
    P1SEL0 |= UCA0RXD;     // UCA0RXD Function
    P1SEL1 &= ~UCA0RXD;    // UCA0RXD Function

    // Port 1: Pin 7
    P1SEL0 |= UCA0TXD;     // UCA0TXD Function
    P1SEL1 &= ~UCA0TXD;    // UCA0TXD Function
}

void Init_Port_2(void) {
    /*
     *SLOW_CLK               (0x01) // 2.0 SLOW_CLK
     *CHECK_BAT              (0x02) // 2.1 CHECK_BAT
     *IR_LED                 (0x04) // 2.2 IR_LED
     *SW2                    (0x08) // 2.3 SW2
     *IOT_RUN_RED            (0x10) // 2.4 IOT_RUN_RED
     *DAC_ENB                (0x20) // 2.5 DAC_ENB
     *LFXOUT                 (0x40) // 2.6 XOUT
     *LFXIN                  (0x80) // 2.7 XIN
     */

    //------------------------------------------------------------------------------
     P2OUT = 0x00; // P2 set Low
     P2DIR = 0x00; // Set P2 direction to output

     // Port 2: Pin 0
     P2SEL0 &= ~SLOW_CLK;   // SLOW_CLK GPIO Operation
     P2SEL1 &= ~SLOW_CLK;   // SLOW_CLK GPIO Operation
     P2OUT &= ~SLOW_CLK;    // Initial Value = Low / Off
     P2DIR |= SLOW_CLK;     // Direction = output

     // Port 2: Pin 1
     P2SEL0 &= ~CHECK_BAT;  // CHECK_BAT GPIO Operation
     P2SEL1 &= ~CHECK_BAT;  // CHECK_BAT GPIO Operation
     P2OUT &= ~CHECK_BAT;   // Initial Value = Low / Off
     P2DIR |= CHECK_BAT;    // Direction = output

     // Port 2: Pin 2
     P2SEL0 &= ~IR_LED;     // P2_2 GPIO Operation
     P2SEL1 &= ~IR_LED;     // P2_2 GPIO Operation
     P2OUT &= ~IR_LED;      // Initial Value = Low / Off
     P2DIR |= IR_LED;       // Direction = output

     // Port 2: Pin 3
     P2SEL0 &= ~SW2;        // SW2 Operation
     P2SEL1 &= ~SW2;        // SW2 Operation
     P2PUD |= SW2;          // Configure pullup resistor
     P2DIR &= ~SW2;         // Direction = input
     P2REN |= SW2;          // Enable pullup resistor
     P2IES |= SW2;          // SW2 Hi/Lo edge interrupt
     P2IFG &= ~SW2;         // IFG SW1 cleared
     P2IE |= SW2;           // SW2 interrupt Enabled

     // Port 2: Pin 4
     P2SEL0 &= ~IOT_RUN_CPU;    // IOT_RUN_CPU GPIO Operation
     P2SEL1 &= ~IOT_RUN_CPU;    // IOT_RUN_CPU GPIO Operation
     P2OUT &= ~IOT_RUN_CPU;     // Initial Value = Low / Off
     P2DIR |= IOT_RUN_CPU;      // Direction = output

     // Port 2: Pin 5
     P2SEL0 &= ~DAC_ENB;    // DAC_ENB GPIO Operation
     P2SEL1 &= ~DAC_ENB;    // DAC_ENB GPIO Operation
     P2OUT |= DAC_ENB;      // Initial Value = High
     P2DIR |= DAC_ENB;      // Direction = output

     // Port 2: Pin 6
     P2SEL0 &= ~LFXOUT;     // XOUT Clock Operation
     P2SEL1 |= LFXOUT;      // XOUT Clock Operation

     // Port 2: Pin 7
     P2SEL0 &= ~LFXIN;      // XIN Clock Operation
     P2SEL1 |= LFXIN;       // XIN Clock Operation
     //------------------------------------------------------------------------------
};


void Init_Port_3(unsigned char mode) {
    /*
     *TEST_PROBE             (0x01) // 3.0 TEST_PROBE
     *OA2O                   (0x02) // 3.1 OA2O
     *OA2N                   (0x04) // 3.2 OA2N
     *OA2P                   (0x08) // 3.3 OA2P
     *SMCLK_OUT              (0x10) // 3.4 SMCLK
     *DAC_CTRL               (0x20) // 3.5 DAC_CNTL
     *IOT_LINK_CPU           (0x40) // 3.6 IOT_LINK_CPU
     *IOT_RN_CPU             (0x80) // 3.7 IOT_RN_CPU
     */

    //------------------------------------------------------------------------------
    P3OUT = 0x00; // P3 set Low
    P3DIR = 0x00; // Set P3 direction to output

    // Port 3: Pin 0
    P3SEL0 &= ~TEST_PROBE;  // TEST_PROBE GPIO Operation
    P3SEL1 &= ~TEST_PROBE;  // TEST_PROBE GPIO Operation
    P3OUT &= ~TEST_PROBE;   // Initial Value = High
    P3DIR |= TEST_PROBE;    // Direction = output

    // Port 3: Pin 1
    P3SELC |= OA2O;     // OA2O ADC Operation

    // Port 3: Pin 2
    P3SELC |= OA2N;     // OA2N ADC Operation

    // Port 3: Pin 3
    P3SELC |= OA2P;     // OA2P ADC Operation

    // Port 3: Pin 4
    // switch case for USE_SMCLK (yes(1) or no(0))
    switch(mode){
        case  USE_GPIO:
            P3SEL0 &= ~SMCLK_OUT;   // SMCLK_OUT GPIO Operation
            P3SEL1 &= ~SMCLK_OUT;   // SMCLK_OUT GPIO Operation
            P3OUT |= SMCLK_OUT;    // Initial value = Low
            P3DIR |= SMCLK_OUT;     // Direction = output
            break;
        case  USE_SMCLK:
//            // using SMCLK to test GPIO high
//            P3SEL0 &= ~SMCLK_OUT;   // SMCLK_OUT GPIO Operation
//            P3SEL1 &= ~SMCLK_OUT;   // SMCLK_OUT GPIO Operation
//            P3OUT |= SMCLK_OUT;     // Initial value = high
//            P3DIR |= SMCLK_OUT;     // Direction = output
//            break;

            P3SEL0 |= SMCLK_OUT;    // SMCLK_OUT Operation
            P3SEL1 &= ~SMCLK_OUT;   // SMCLK_OUT Operation
            P3DIR |= SMCLK_OUT;     // Direction = high
            break;
    }

    // Port 3: Pin 5
    P3SEL0 &= ~DAC_CTRL;    // DAC_CTRL GPIO Operation
    P3SEL0 &= ~DAC_CTRL;    // DAC_CTRL GPIO Operation
    P3OUT &= ~DAC_CTRL;     // Initial Value = Low
    P3DIR |= DAC_CTRL;      // Direction = output

    // Port 3: Pin 6
    P3SEL0 &= ~IOT_LINK_CPU;   // IOT_LINK_CPU GPIO Operation
    P3SEL1 &= ~IOT_LINK_CPU;   // IOT_LINK_CPU GPIO Operation
    P3OUT &= ~IOT_LINK_CPU;    // Initial Value = Low
    P3DIR |= IOT_LINK_CPU;     // Direction = output

    // Port 3: Pin 7
    P3SEL0 &= ~IOT_RN_CPU;     // IOT_RN_CPU GPIO Operation
    P3SEL1 &= ~IOT_RN_CPU;     // IOT_RN_CPU GPIO Operation
    P3OUT &= ~IOT_RN_CPU;      // Initial Value = Low
    P3DIR |= IOT_RN_CPU;       // Direction = output
    //------------------------------------------------------------------------------
};

void Init_Port_4(void) {
    /*
     *RESET_LCD              (0x01) // 4.0 RESET_LCD
     *SW1                    (0x02) // 4.1 SW1
     *UCA1RXD                (0x04) // 4.2 UCA1RXD
     *UCA1TXD                (0x08) // 4.3 UCA1TXD
     *UCB1_CS_LCD            (0x10) // 4.4 UCB1_CS_LCD
     *UCB1CLK                (0x20) // 4.5 UCB1CLK
     *UCB1SIMO               (0x40) // 4.6 UCB1SIMO
     *UCB1SOMI               (0x80) // 4.7 UCB1SOMI
     */

    //------------------------------------------------------------------------------
     P4OUT = 0x00; // P4 set Low
     P4DIR = 0x00; // Set P4 direction to output

     // Port 4: Pin 0
     P4SEL0 &= ~RESET_LCD;  // RESET_LCD GPIO operation
     P4SEL1 &= ~RESET_LCD;  // RESET_LCD GPIO operation
     P4OUT &= ~RESET_LCD;   // Initial Value = Low / Off
     P4DIR |= RESET_LCD;    // Direction = output

     // Port 4: Pin 1
     P4SEL0 &= ~SW1;    // SW1 GPIO operation
     P4SEL1 &= ~SW1;    // SW1 GPIO operation
     P4PUD |= SW1;      // Configure pullup resistor
     P4DIR &= ~SW1;     // Direction = input
     P4REN |= SW1;      // Enable pullup resistor
     P4IES |= SW1;      // SW1 Hi/Lo edge interrupt
     P4IFG &= ~SW1;     // IFG SW1 cleared
     P4IE |= SW1;       // SW1 interrupt Enabled

     // Port 4: Pin 2
     P4SEL0 |= UCA1RXD;     // UCA1 RXD operation
     P4SEL1 &= ~UCA1RXD;    // UCA1 RXD operation

     // Port 4: Pin 3
     P4SEL0 |= UCA1TXD;     // UCA1 TXD operation
     P4SEL1 &= ~UCA1TXD;    // UCA1 TXD operation

     // Port 4: Pin 4
     P4SEL0 &= ~UCB1_CS_LCD;   // UCB1_CS_LCD GPIO operation
     P4SEL1 &= ~UCB1_CS_LCD;   // UCB1_CS_LCD GPIO operation
     P4OUT |= UCB1_CS_LCD;     // Set SPI_CS_LCD Off [High]
     P4DIR |= UCB1_CS_LCD;     // Set SPI_CS_LCD direction to output

     // Port 4: Pin 5
     P4SEL0 |= UCB1CLK;     // UCB1CLK SPI BUS operation
     P4SEL1 &= ~UCB1CLK;    // UCB1CLK SPI BUS operation

     // Port 4: Pin 6
     P4SEL0 |= UCB1SIMO;    // UCB1SIMO SPI BUS operation
     P4SEL1 &= ~UCB1SIMO;   // UCB1SIMO SPI BUS operation

     // Port 4: Pin 7
     P4SEL0 |= UCB1SOMI;    // UCB1SOMI SPI BUS operation
     P4SEL1 &= ~UCB1SOMI;   // UCB1SOMI SPI BUS operation
     //------------------------------------------------------------------------------
};

void Init_Port_5(void) {
    /*
     *V_BAT                  (0x01) // 5.0 V_BAT
     *V_5_0                  (0x02) // 5.1 V_5_0
     *V_DAC                  (0x04) // 5.2 V_DAC
     *V_3_3                  (0x08) // 5.3 V_3_3
     *IOT_BOOT_CPU           (0x10) // 5.4 IOT_BOOT_CPU
     */

    //------------------------------------------------------------------------------
    P5OUT = 0x00; // P5 set Low
    P5DIR = 0x00; // Set P5 direction to output

    // Port 5: Pin 0
    P5SELC |= V_BAT;    // V_BAT ADC Operation

    // Port 5: Pin 1
    P5SELC |= V_5_0;    // V_5_0 ADC Operation

    // Port 5: Pin 2
    P5SELC |= V_DAC;    // V_DAC ADC Operation

    // Port 5: Pin 3
    P5SELC |= V_3_3;    // V_3_3 ADC Operation

    // Port 5: Pin 4
    P5SEL0 &= ~IOT_BOOT_CPU;    // IOT_BOOT_CPU GPIO Operation
    P5SEL1 &= ~IOT_BOOT_CPU;    // IOT_BOOT_CPU GPIO Operation
    P5OUT &= ~IOT_BOOT_CPU;     // Initial Value = Low
    P5DIR |= IOT_BOOT_CPU;     // Direction = output
};

void Init_Port_6(void) {
    /*
     *LCD_BACKLITE          (0x01) // 6.0 LCD_BACKLITE
     *R_FORWARD             (0x02) // 6.1 R_FORWARD
     *L_FORWARD             (0x04) // 6.2 L_FORWARD
     *R_REVERSE             (0x08) // 6.3 R_REVERSE
     *L_REVERSE             (0x10) // 6.4 L_REVERSE
     *P6_5                  (0x20) // 6.5 P6_5
     *GRN_LED               (0x40) // 6.6 GRN_LED
     */

     //------------------------------------------------------------------------------
    P6OUT = 0x00; // P6 set Low
    P6DIR = 0x00; // Set P6 direction to output

    // Port 6: Pin 0
    P6SEL0 |= LCD_BACKLITE;     // LCD_BACKLITE TB3.1
    P6SEL1 &= ~LCD_BACKLITE;    // LCD_BACKLITE TB3.1
    P6DIR |= LCD_BACKLITE;      // Direction = output

    // Port 6: Pin 1
    P6SEL0 |= R_FORWARD;        // R_FORWARD TB3.2
    P6SEL1 &= ~R_FORWARD;       // R_FORWARD TB3.2
    P6DIR |= R_FORWARD;         // Direction = output

    // Port 6: Pin 2
    P6SEL0 |= L_FORWARD;         // L_FORWARD TB3.3
    P6SEL1 &= ~L_FORWARD;       // L_FORWARD TB3.3
    P6DIR |= L_FORWARD;         // Direction = output

    // Port 6: Pin 3
    P6SEL0 |= R_REVERSE;        // R_REVERSE TB3.4
    P6SEL1 &= ~R_REVERSE;       // R_REVERSE TB3.4
    P6DIR |= R_REVERSE;         // Direction = output

    // Port 6: Pin 4
    P6SEL0 |= L_REVERSE;        // L_REVERSE TB3.5
    P6SEL1 &= ~L_REVERSE;       // L_REVERSE TB3.5
    P6DIR |= L_REVERSE;         // Direction = output

    // Port 6: Pin 5
    P6SEL0 &= ~P6_5;       // P6_5 GPIO Operation
    P6SEL1 &= ~P6_5;       // P6_5 GPIO Operation
    P6OUT &= ~P6_5;        // Initial Value = Low
    P6DIR |= P6_5;         // Direction = output

    // Port 6: Pin 6
    P6SEL0 &= ~GRN_LED;    // GRN_LED GPIO Operation
    P6SEL1 &= ~GRN_LED;    // GRN_LED GPIO Operation
    P6OUT &= ~GRN_LED;     // Initial Value = Low
    P6DIR |= GRN_LED;      // Direction = output
};

void Init_Ports(void) {
    Init_Port_1();                     // Initialize Port 1
    Init_Port_2();                     // Initialize Port 2
    Init_Port_3(USE_GPIO);             // Initialize Port 3: contain USE_SMCLK parameter (0/1)
    Init_Port_4();                     // Initialize Port 4
    Init_Port_5();                     // Initialize Port 5
    Init_Port_6();                     // Initialize Port 6
}

void Motors_Off(void) {
    wheel_event = INITIATE_STOP;
}

void Forward_On(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = FORWARD_C;
    TARGET_SPEED_LEFT = SLOW;
    TARGET_SPEED_RIGHT = SLOW;
}

void Forward_Off(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = FORWARD_C;
    TARGET_SPEED_LEFT = WHEEL_OFF;
    TARGET_SPEED_RIGHT = WHEEL_OFF;
}

void Reverse_On(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = REVERSE_C;
    TARGET_SPEED_LEFT = SLOW;
    TARGET_SPEED_RIGHT = SLOW;
}

void Reverse_Off(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = REVERSE_C;
    TARGET_SPEED_LEFT = WHEEL_OFF;
    TARGET_SPEED_RIGHT = WHEEL_OFF;
}

void Spin_Clock(void) {
    wheel_event = CONFIGURE_WHEEL_SPEEDS;
    config = SPIN;
    TARGET_SPEED_LEFT = SLOWER;
    TARGET_SPEED_RIGHT = SLOWER;
}
