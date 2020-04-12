#ifndef __ship__
#define __ship__

#include "defines.h"

typedef struct {
    uchar x, y;
} Pos;

typedef struct {
    
    Pos pos; // Center of 5x5 square
    uchar size; // Number of ship pieces (max 25)
    uchar shot_count; // Number of piece hits
    bool alive; // Alive if show_count < size
    
} Ship;

Ship* init_ship();
void copy_ship( Ship* dst, Ship* src );


#endif

