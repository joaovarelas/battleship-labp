#ifndef SHIP_H
#define SHIP_H

#include "config.h"
#include "cell.h"

typedef struct _Ship{
    
    Pos pos; // Center of 5x5 square
    byte size; // Number of ship pieces (max 25)
    byte shot_count; // Number of piece hits
    bool alive; // Alive if show_count < size
    
} Ship;

Ship* init_ship();
void copy_ship( Ship* dst, Ship* src );

Board* build_ship( byte idx );

void manual_place_ship( Board* player_board, Board* ship_board );
void random_place_ship( Board* player_board, Board* ship_board );
void place_ship( Board* player_board, Board* ship_board, Board* tmp_board );

void move_ship( byte dir, Pos* pos, Board* ship_board );

#endif

