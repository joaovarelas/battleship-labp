#ifndef SHIP_H
#define SHIP_H

#include "config.h"

typedef struct _Pos {
    uchar x, y;
} Pos;


typedef struct _Ship{
    Pos pos; // Center of 5x5 square
    uchar size; // Number of ship pieces (max 25)
    uchar shot_count; // Number of piece hits
    bool alive; // Alive if show_count < size
    
} Ship;

Ship* init_ship();
void copy_ship( Ship* dst, Ship* src );
void move_ship( uchar option, Pos pos, Board* ship_board );
Board* build_ship( uchar idx );
void manual_place_ship( Board* player_board, Board* ship_board );
void random_place_ship( Board* player_board, Board* ship_board );
void place_ship( Board* player_board, Board* ship_board, Board* tmp_board );

#endif

