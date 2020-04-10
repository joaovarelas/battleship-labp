#ifndef __settings__
#define __settings__

#define MAX_LINE_SIZE 32

#include "defines.h"

typedef struct {
    
    uchar board_size;
    uchar num_ships;
    bool ship_shape[65][25];
    
} Settings;


// Global
Settings* settings;

void load_settings();
void change_settings();

#endif
