#ifndef __settings__
#define __settings__

#include <stdlib.h>
#include <stdio.h>

#include "board.h"

typedef struct {
    
    unsigned char board_size;
    unsigned char num_ships;
    
} Settings;

// global var
Settings* settings;


void load_settings();
void change_settings();

#endif
