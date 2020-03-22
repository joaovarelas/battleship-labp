#ifndef __settings__
#define __settings__

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    
    unsigned int board_size, num_ships;
    unsigned int* size_ships;
    
} Settings;

/* Global Settings */
Settings* settings;


void load_settings();
void change_settings();

#endif
