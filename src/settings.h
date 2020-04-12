#ifndef __settings__
#define __settings__

#include "defines.h"
#include "board.h"

typedef struct {
    
    uchar board_size;
    uchar num_ships;
    bool ship_shape[65][25];
    
} Settings;


// Global
Settings* settings;

void init_settings();
void load_settings();
void write_settings();
void change_settings();

void build_new_ship( uchar idx );

#endif
