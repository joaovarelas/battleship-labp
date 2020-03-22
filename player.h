#ifndef __player__
#define __player__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "board.h"
#include "settings.h"

typedef struct {

    char* name;
    Board* board;
    bool has_ships_alive;
    short** hit_matrix; // 0 - unknown ; 1 - hit ; 2 - missed
    
} Player;


Player* init_player( char* name );
Player* setup_player();

#endif
