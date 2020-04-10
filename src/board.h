#ifndef __board__
#define __board__

#include "settings.h"
#include "ship.h"


typedef enum { MISSED = -1, UNKNOWN = 0, HIT = 1 } State;
              
typedef struct {
    
    uchar ship; // Ship index
    State state; // Hit or miss
    
} Cell;

typedef struct {
    
    uchar size; // 20x20 to 40x40
    Cell** matrix; // Size^2 cells: (idship, hit)
    uchar idx; // Current ship index. 1 to MAXSHIPS(size)
    Ship** ships; // Indexed ships
    
} Board;



Board* init_board( uchar n );
void print_board( Board* board, bool game_mode );
void rotate_board( Board* board );
void shift_board( Board* board, uchar orientation );
void copy_board( Board* dst, Board* src );
void free_board( Board* board );
bool ship_overlap( Board* dst, Board* src, Pos pos );

#endif

