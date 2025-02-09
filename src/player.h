#ifndef PLAYER_H
#define PLAYER_H

#include "config.h"
#include "board.h"
#include "settings.h"
#include "ship.h"

typedef struct _Player {

    char* name;
    Board* board;
    bool alive;
    
} Player;


Player* init_player( char* name );
void setup_player( Player* player );

void free_player( Player* player );

#endif
