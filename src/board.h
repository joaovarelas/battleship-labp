#ifndef __board__
#define __board__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "ship.h"
#include "settings.h"

#define MAX_SHIPS( N ) ( N * N  / 25 )


typedef enum { MISSED = -1, UNKNOWN = 0, HIT = 1 } CellState;
              
typedef struct {
    
    unsigned char ship; // ship index
    CellState state; // hit or miss
    
} Cell;

typedef struct {
    
    unsigned char size; // 20x20 to 40x40
    Cell** matrix; // size² cells: (idship, hit)
    unsigned char idx; // current ship index. 1 to MAXSHIPS(size)
    Ship** ships; // indexed ships
    
} Board;



Board* init_board( unsigned char n );
void print_board( Board* board );
void rotate_board( Board* board );

#endif

