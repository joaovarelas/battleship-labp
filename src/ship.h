#ifndef __ship__
#define __ship__

#include <stdlib.h>
#include <stdbool.h>

#define MAX_SHIP_SIZE 5

typedef struct {
    unsigned char x, y;
} Pos;

typedef struct {
    
    Pos pos; // center of 5x5 square
    unsigned char size; // number of ship pieces
    unsigned char shot_count; // number of piece hits
    bool alive; // alive if show_count < size
    
} Ship;

Ship* init_ship( Pos pos, unsigned char size );

#endif


