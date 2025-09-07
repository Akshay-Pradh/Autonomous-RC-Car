/*
 * macros.h
 *
 *  Created on: Jan 29, 2025
 *      Author: Akshay Pradhan
 */

#ifndef MACROS_H_
#define MACROS_H_

#include "msp430.h"
#include <string.h>
#include "clocks.h"
#include "display.h"
#include "functions.h"
#include "globals.h"
#include "init.h"
#include "LCD.h"
#include "led.h"
#include "ports.h"
#include "system.h"
#include "switches.h"
#include "timers.h"
#include "akshay_state_machine.h"
#include "ADC.h"
#include "wheels.h"
#include "serial.h"


// COMMON ======================================================================
#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //
#define MCLK_FREQ_MHZ           (8) // MCLK = 8MHz
#define CLEAR_REGISTER     (0X0000)

// SERIAL ======================================================================
#define BEGINNING (0)
#define SMALL_RING_SIZE (16)
#define LARGE_RING_SIZE (32)

#define ROWS    (4)
#define COLUMNS (32)

// SWITCHES ====================================================================
#define DEBOUNCE_THRESHOLD  (4)
#define YES                 (1)
#define NO                  (0)

// PROJECT 05 MOVEMENTS ========================================================
#define MOVEMENT_1            (0x00)
#define MOVEMENT_2            (0x01)
#define MOVEMENT_3            (0x02)
#define MOVEMENT_4            (0x03)
#define MOVEMENT_5            (0x04)
#define NONE                  ('N')
#define WAIT                  ('W')
#define MOVE_FORWARD          ('F')
#define PAUSE                 ('P')
#define PASS_MOVEMENT         ('M')
#define MOVE_REVERSE          ('R')
#define SPIN_CLOCK            ('S')
#define SPIN_COUNTERCLOCK     ('C')
#define STOP                  ('T')

// TIMERS ======================================================================

// Timer B0
#define TIMER_B0_CCR0_VECTOR         TIMER0_B0_VECTOR
#define TIMER_B0_CCR1_2_OV_VECTOR    TIMER0_B1_VECTOR
#define TB0CCR0_INTERVAL    (25000)     // 8,000,000 / 8 / 8 / (1 / 200msec)
#define DEBOUNCE_INTERVAL   (25000)     // 8,000,000 / 8 / 8 / (1 / 200msec)

// Timer B1
#define TIMER_B1_CCR0_VECTOR         TIMER1_B0_VECTOR
#define TIMER_B1_CCR1_2_OV_VECTOR    TIMER1_B1_VECTOR
#define TB1CCR0_INTERVAL    (1250)      // 8,000,000 / 8 / 8 / (1 / 10msec)
#define TB1CCR1_INTERVAL    (31250)     // 8,000,000 / 8 / 8 / (1 / 250msec)
#define TB1CCR2_INTERVAL    (6250)      // 8,000,000 / 8 / 8 / (1 / 50msec)

// Timer B2
#define TIMER_B2_CCR0_VECTOR         TIMER2_B0_VECTOR
#define TIMER_B2_CCR1_2_OV_VECTOR    TIMER2_B1_VECTOR

//Timer B3
#define TIMER_B3_CCR0_VECTOR         TIMER3_B0_VECTOR
#define TIMER_B3_CCR1_2_OV_VECTOR    TIMER3_B1_VECTOR

// PWM =========================================================================
#define PWM_PERIOD              (TB3CCR0)
#define LCD_BACKLITE_DIMMING    (TB3CCR1)
#define RIGHT_FORWARD_SPEED     (TB3CCR2)
#define RIGHT_REVERSE_SPEED     (TB3CCR3)
#define LEFT_FORWARD_SPEED      (TB3CCR4)
#define LEFT_REVERSE_SPEED      (TB3CCR5)

#define SPEED_STEP      (1000)
#define WHEEL_PERIOD    (50005)
#define WHEEL_OFF       (0)

#define SLOW_R          (18000)
#define SLOW_L          (20000)
#define SLOWER          (10000)
#define FAST            (50000)

// need to mess around with these

#define CIRCLE_SLOW     (15000)
#define CIRCLE_SLOWER   (10000)
#define CIRCLE_ADJ      (7000)

#define PERCENT_100     (50000)
#define PERCENT_80      (30000)

// wheel_events
#define NONE                        ('N')
#define IDLE                        ('I')
#define CONFIGURE_WHEEL_SPEEDS      ('C')
#define FORWARD_MOVE_START          ('F')
#define FORWARD_ADJUST              ('A')
#define REVERSE_MOVE_START          ('R')
#define REVERSE_ADJUST              ('D')
#define SPIN_MOVE_START             ('G')
#define SPIN_ADJUST                 ('H')
#define INITIATE_STOP               ('N')
#define STOP_WHEELS                 ('S')

// configs
#define FORWARD_C                   ('F')
#define REVERSE_C                   ('R')
#define SPIN                        ('S')
#define OFF                         ('O')


// PROJECT07_STATE_MACHINE =====================================================

// events
#define IDLE            ('I')
#define FIND_BLACK      ('F')
#define FOUND_BLACK     ('B')
#define STOP            ('T')
#define ORIENT_BLACK    ('O')
#define SMALL_STOP      ('S')
#define CIRCLING        ('C')
#define ADJUST_OUT      ('U')
#define ADJUST_IN       ('N')
#define ORIENT_WHITE    ('W')
#define ORIENT_CENTER   ('R')
#define MOVE_CENTER     ('M')
#define END             ('E')

// PORTS =======================================================================
#define FALSE                  (0x00) //
#define TRUE                   (0x01) //
#define MOTOR                  (0x00) //
#define SMCLK_OFF              (0x00) //
#define SMCLK_ON               (0x01) //
#define PORTS                  (0x00) // RED LED 0
#define PWM_MODE               (0x01) // GREEN LED 1
#define STEP                   (2000)
#define FORWARD                (0x00) // FORWARD
#define REVERSE                (0x01) // REVERSE

// Port 1 Pins
#define RED_LED                (0x01) // 1.0 RED LED 0
#define A1_SEEED               (0x02) // 1.1 A1_SEEED
#define V_DETECT_L             (0x04) // 1.2 V_DETECT_L
#define V_DETECT_R             (0x08) // 1.3 V_DETECT_R
#define V_A4_SEEED             (0x10) // 1.4 A4_SEEED
#define V_THUMB                (0x20) // 1.5 V_THUMB
#define UCA0RXD                (0x40) // 1.6 Back Channel UCA0RXD
#define UCA0TXD                (0x80) // 1.7 Back Channel UCA0TXD

// Port 2 Pins
#define SLOW_CLK               (0x01)   // 2.0 SLOW_CLK
#define CHECK_BAT              (0x02)   // 2.1 CHECK_BAT
#define IR_LED                 (0x04)   // 2.2 IR LED
#define SW2                    (0x08)   // 2.3 SW2
#define IOT_RUN_CPU            (0x10)   // 2.4 IOT_RUN_RED
#define DAC_ENB                (0x20)   // 2.5 DAC_ENB
#define LFXOUT                 (0x40)   // 2.6 XOUT
#define LFXIN                  (0x80)   // 2.7 XIN
#define P2PUD                  (P2OUT)  // P2PUD easier to read than P2OUT for pull-up

// Port 3 Pins
#define TEST_PROBE             (0x01) // 3.0 TEST PROBE
#define OA2O                   (0x02) // 3.1 OA2O
#define OA2N                   (0x04) // 3.2 Photodiode Circuit
#define OA2P                   (0x08) // 3.3 Photodiode Circuit
#define SMCLK_OUT              (0x10) // 3.4 SMCLK
#define DAC_CTRL               (0x20) // 3.5 DAC_CTRLL
#define IOT_LINK_CPU           (0x40) // 3.6 IOT_LINK_CPU
#define IOT_RN_CPU             (0x80) // 3.7 IOT_RN_CPU              1
#define USE_GPIO               (0x00)
#define USE_SMCLK              (0x01)

// Port 4 Pins
#define RESET_LCD              (0x01)   // 4.0 RESET_LCD
#define SW1                    (0x02)   // 4.1 SW1
#define UCA1RXD                (0x04)   // 4.2 Back Channel UCA1RXD
#define UCA1TXD                (0x08)   // 4.3 Back Channel UCA1TXD
#define UCB1_CS_LCD            (0x10)   // 4.4 Chip Select
#define UCB1CLK                (0x20)   // 4.5 SPI mode - clock output—UCB1CLK
#define UCB1SIMO               (0x40)   // 4.6 UCB1SIMO
#define UCB1SOMI               (0x80)   // 4.7 UCB1SOMI
#define P4PUD                  (P4OUT)  // P4PUD easier to read than P4OUT for pull-up

// Port 5 Pins
#define V_BAT                  (0x01) // 5.0 V_BAT
#define V_5_0                  (0x02) // 5.1 V_5_0
#define V_DAC                  (0x04) // 5.2 V_DAC
#define V_3_3                  (0x08) // 5.3 V_3_3
#define IOT_BOOT_CPU           (0x10) // 5.4 IOT_BOOT_CPU           1

// Port 6 Pins
#define LCD_BACKLITE           (0x01) // 6.0 LCD_BACKLITE
#define R_FORWARD              (0x02) // 6.1 P6_0_PWM
#define R_REVERSE              (0x04) // 6.2 P6_1_PWM
#define L_FORWARD              (0x08) // 6.3 P6_2_PWM
#define L_REVERSE              (0x10) // 6.4 P6_3_PWM
#define P6_5                   (0x20) // 6.5
#define GRN_LED                (0x40) // 6.6 GREEN LED


#endif /* MACROS_H_ */
