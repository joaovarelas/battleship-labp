#include "config.h"
#include "cell.h"


void init_cell( Cell* cell, byte ship, State state ){
    cell -> ship = ship;
    cell -> state = state;
    return;
}

void init_pos( Pos* pos, byte x, byte y ){
    pos -> x = x;
    pos -> y = y;
    return;
}

void copy_pos( Pos* dst, Pos* src ){
    dst -> x = src -> x;
    dst -> y = src -> y;
    return;
}

void copy_cell( Cell* dst, Cell* src ){
    dst -> ship = src -> ship;
    dst -> state = src -> state;
    return;
}

