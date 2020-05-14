#ifndef CELL_H_
#define CELL_H_

#include "config.h"

typedef enum { UNKNOWN, HIT, MISS, FINISH } State;
              
typedef struct _Cell {
    
    byte ship; // Ship index
    State state; // Hit or miss
    
} Cell;


typedef struct _Pos {
    byte x, y;
} Pos;


void init_cell( Cell* cell, byte ship, State state );
void init_pos( Pos* pos, byte x, byte y );

void copy_cell( Cell* dst, Cell* src );
void copy_pos( Pos* dst, Pos* src );

bool equal_pos( Pos* p1, Pos* p2 );

#endif


