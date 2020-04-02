#ifndef __board__
#define __board__


#include "ship.h"
#include "settings.h"


typedef enum { MISSED = -1, UNKNOWN = 0, HIT = 1 } State;
              
typedef struct {
    
    uchar ship; // ship index
    State state; // hit or miss
    
} Cell;

typedef struct {
    
    uchar size; // 20x20 to 40x40
    Cell** matrix; // size² cells: (idship, hit)
    uchar idx; // current ship index. 1 to MAXSHIPS(size)
    Ship** ships; // indexed ships
    
} Board;



Board* init_board( uchar n );
void print_board( Board* board, bool game_mode );
void rotate_board( Board* board );
void copy_board( Board* dst, Board* src );

void free_board( Board* board );
#endif

