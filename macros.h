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
#include "shapes.h"


// COMMON ======================================================================
#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //
#define MCLK_FREQ_MHZ           (8) // MCLK = 8MHz
#define CLEAR_REGISTER     (0X0000)


// STATES ======================================================================
#define NONE                  ('N')
#define STRAIGHT              ('L')
#define CIRCLE                ('C')
#define FIG8                  ('F')
#define TRIANGLE              ('T')
#define WAIT                  ('W')
#define START                 ('S')
#define RUN                   ('R')
#define RUN_REV               ('P')
#define RUN_TURN              ('X')
#define END                   ('E')
#define WAITING2START          (50)

// STRAIGHT
#define WHEEL_COUNT_TIME       (10)
#define RIGHT_COUNT_TIME        (3)
#define LEFT_COUNT_TIME         (8)
#define TRAVEL_DISTANCE        (10)

// CIRCLE
#define WHEEL_COUNT_TIME_CIRC  (20)
#define RIGHT_COUNT_TIME_CIRC  (16)
#define LEFT_COUNT_TIME_CIRC    (3)
#define CIRCLE_DISTANCE        (70)

// FIGURE-8
#define WHEEL_COUNT_TIME_FIG8  (20)

#define RIGHT_COUNT1_TIME_FIG8 (16)
#define RIGHT_COUNT2_TIME_FIG8  (2)

#define LEFT_COUNT1_TIME_FIG8   (3)
#define LEFT_COUNT2_TIME_FIG8  (20)

#define FIG8_DISTANCE_F        (35)
#define FIG8_DISTANCE_R        (45)

// TRIANGLE

#define WHEEL_COUNT_TIME_TURN   (20)
#define RIGHT_TURN_TIME          (7)
#define TURN_DISTANCE           (10)



#endif /* MACROS_H_ */
