#ifndef __player__
#define __player__

#include "board.h"

typedef struct {

    char* name;
    Board* board;
    bool alive;
    
} Player;


Player* init_player( char* name );
Player* setup_player();
void free_player( Player* player );

Board* build_ship( uchar idx );
void place_ship( Board* player_board, Board* ship_board );

#endif
