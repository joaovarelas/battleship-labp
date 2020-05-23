#ifndef BOARD_H
#define BOARD_H

#include "ship.h"

typedef enum { MATRIX, QUADTREE } BoardType;

typedef enum { UP = 1, DOWN, LEFT, RIGHT, ROTATE } Move;

typedef struct _Board {

    BoardType type;
    byte size; // 20x20 to 40x40

    //union {
    Cell** matrix; // Size^2 cells
    QTree* qtree; // Save ship pieces as nodes
    //} u;
    
    byte idx; // Current ship index. 1 to MAXSHIPS(size)
    Ship** ships; // Indexed ships
    byte ships_alive; // Number of ships alive
    
} Board;


Board* init_board( byte size, BoardType type );
void init_matrix( Board* board );

void copy_board( Board* dst, Board* src );

void rotate_board( Board* board );
void shift_board( Board* board, byte move );

bool ship_overlap( Board* dst, Board* ship_board, Pos pos );

void print_board( Board* board, bool game_mode );
void print_cell( Board* board, Pos pos, bool game_mode );

void matrix_to_qtree( Board* board );

void free_board( Board* board );
void free_matrix( Board* board );

#endif

