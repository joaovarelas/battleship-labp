#ifndef __player__
#define __player__

#include "board.h"
#include "settings.h"

typedef struct {

    char* name;
    Board* board;
    bool alive;
    
} Player;


Player* init_player( char* name );
Player* setup_player();
Board* build_ship();
void place_ship( Board* player_board, Board* ship_board );

void free_player( Player* player );

#endif
