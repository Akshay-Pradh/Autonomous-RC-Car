/*
 * display.c
 *
 *  Created on: Jan 29, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

void Display_Process(void){
  if(update_display){
    if (TIME_DISPLAY) Display_Time();     // might have to change location later (or add another condition)
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}
