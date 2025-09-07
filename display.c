/*
 * display.c
 *
 *  Created on: Jan 29, 2025
 *      Author: Akshay Pradhan
 */

#include "macros.h"

void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}


void clear_display(void) {
    strcpy(display_line[0], "          ");
    strcpy(display_line[1], "          ");
    strcpy(display_line[2], "          ");
    strcpy(display_line[3], "          ");
    display_changed = TRUE;
}
