#ifndef __settings__
#define __settings__

#include "board.h"

typedef struct {
    
    uchar board_size;
    uchar num_ships;
    
} Settings;


// Global
Settings* settings;

void load_settings();
void change_settings();

#endif
