#ifndef __ship__
#define __ship__

#include "defines.h"

typedef struct {
    uchar x, y;
} Pos;

typedef struct {
    
    Pos pos; // center of 5x5 square
    uchar size; // number of ship pieces (max 25)
    uchar shot_count; // number of piece hits
    bool alive; // alive if show_count < size
    
} Ship;

Ship* init_ship( );
void copy_ship( Ship* dst, Ship* src );

#endif

