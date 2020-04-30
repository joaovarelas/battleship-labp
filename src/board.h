#ifndef BOARD_H
#define BOARD_H

#include "ship.h"

typedef enum { MISS = -1, UNKNOWN = 0, HIT = 1 } State;
              
typedef struct _Cell {
    
    uchar ship; // Ship index
    State state; // Hit or miss
    
} Cell;

typedef struct _Board {
    
    uchar size; // 20x20 to 40x40
    Cell** matrix; // Size^2 cells: (idship, hit)
    uchar idx; // Current ship index. 1 to MAXSHIPS(size)
    Ship** ships; // Indexed ships
    uchar ships_alive; // Number of ships alive
} Board;


Board* init_board( uchar n );

void copy_board( Board* dst, Board* src );
void copy_tmp_board( Pos pos, Board* player_board, Board* ship_board, Board* tmp_board );

bool ship_overlap( Board* dst, Board* src, Pos pos );

void rotate_board( Board* board );
void shift_board( Board* board, uchar move );

void print_board( Board* board, bool game_mode );
void free_board( Board* board );

#endif

