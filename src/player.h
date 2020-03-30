#ifndef __player__
#define __player__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "board.h"
#include "settings.h"

#define MAX_PLAYER_NAME 16

typedef struct {

    char* name;
    Board* board;
    bool alive;
    //Pos* plays;
    
} Player;


Player* init_player( char* name );
Player* setup_player();
Board* create_ship( char idx );


#endif
