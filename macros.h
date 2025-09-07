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
#include "globals.c"
#include "init.h"
#include "LCD.h"
#include "led.h"
#include "ports.h"
#include "system.h"

#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //
#define MCLK_FREQ_MHZ           (8) // MCLK = 8MHz
#define CLEAR_REGISTER     (0X0000)

#endif /* MACROS_H_ */
