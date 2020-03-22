#ifndef __board__
#define __board__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "ship.h"
#include "settings.h"

typedef struct {
    
    unsigned int size;
    int** matrix;
    int idx;
    Ship** ships;
    
} Board;


Board* init_board( unsigned int n );
void print_board( Board* b );
bool add_ship( Board* board, unsigned int x, unsigned int y, bool dir, unsigned int size);
Board* rand_board( );

#endif
