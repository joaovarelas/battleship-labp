#include "board.h"
#include "settings.h"

Board* init_board( uchar n ){
    
    Board* board = ( Board* ) malloc ( sizeof( Board ) );

    board -> size = n;
    board -> matrix = ( Cell** ) malloc ( n * sizeof( Cell* ) );
    board -> ships = ( Ship** ) malloc ( ( 1 + MAX_SHIPS( settings -> board_size ) ) * sizeof( Ship* ) );
    board -> idx = 0;
    board -> ships_alive = 0;

    for( uchar i = 0; i < n ; i++ ){
        
        board -> matrix[ i ] = ( Cell* ) malloc ( n * sizeof( Cell ) );
        
        for( uchar j = 0; j < n; j++ ){
            
            board -> matrix[ i ][ j ].ship = 0;
            board -> matrix[ i ][ j ].state = UNKNOWN;

        }
    }

    for( uchar i = 1; i <= MAX_SHIPS( settings -> board_size ) ; i++ ){
        board -> ships[ i ] = init_ship();
    }
    
    return board;
}


// Send it to the void
void free_board( Board* board ){

    uchar i;

    for( i = 0; i < board -> size; i++ ){
        free( board -> matrix[ i ] );    
    }
    free( board -> matrix );

    
    for( i = 1; i <= MAX_SHIPS( board -> size ); i++ ){
        free( board -> ships[ i ] );
    }
    free( board -> ships );

    free( board );
    
    return;
}


void print_board( Board* board, bool game_mode ){

    uchar n = board -> size;
    uchar i, j;

    // X Lines
    // Y Columns
    
    printf( "\nX\\Y" );
    for( i = 1; i <= n; i++ ){
        printf( "  %02hhd", i );
    }
    
    printf( "\n   ┌" );
    for( i = 0; i < n - 1; i++ ){
        printf( "───┬" );
    }
    printf( "───┐" );

    for( i = 0 ; i < n ; i++ ) {
        printf( "\n%02hhd │", i + 1 );
        for( j = 0 ; j < n ; j++ ){
            printf( " " );


	    // Print by "state" or Print by "ship"
            if( game_mode ){

                switch( board -> matrix[ i ][ j ].state ){
                case MISS:
                    printf( "•" );
                    break;
                case UNKNOWN:
                    printf( " " );
                    break;
                case HIT:
                    printf( "x" );
                    break;
                default:
                    break;
                }

            } else {
                
                switch( board -> matrix[ i ][ j ].ship ){
                case 0:
                    printf( " " );
                    break;
                default:
                    printf( "■" );
                    break;
                }
                            
            }
            
            printf( " │" );
        }

        if( i == n - 1 )
            break;
        
        printf( "\n   ├");
        for( j = 0; j < n - 1; j++ ){
            printf( "───┼" );
        }
        printf( "───┤" );
    }

    printf( "\n   └" );
    for( i = 0; i < n - 1; i++ ){
        printf( "───┴" );
    }
    printf( "───┘\n" );    

    return;
}


// Temporary placement before final confirmation
void copy_tmp_board( Pos pos, Board* player_board, Board* ship_board, Board* tmp_board ){

    copy_board( tmp_board, player_board );

    uchar span = ( MAX_SHIP_SIZE / 2 );

    for( uchar i = 0; i < MAX_SHIP_SIZE; i++ ){
        for( uchar j = 0; j < MAX_SHIP_SIZE; j++ ){
            uchar ship_idx = ship_board -> matrix[ i ][ j ].ship;
            
            if( ship_idx != 0 ){
                uchar x = i + pos.x - 1 - span;
                uchar y = j + pos.y - 1 - span;
                tmp_board -> matrix[ x ][ y ].ship = ship_idx;
            }
                
        }
    }
   
    return;
}


// Rotate nxn matrix 90º clockwise
// In-place n-cycle approach, swapping ring elements
void rotate_board( Board* board ){
    
    uchar n = board -> size;
    
    for( uchar i = 0; i < n / 2; i++){
        for( uchar j = i; j < n - 1 - i; j++){

            uchar tmp = board -> matrix[ i ][ j ].ship;
            
            board -> matrix[ i ][ j ].ship =  board -> matrix[ n - 1 - j ][ i ].ship;
            board -> matrix[ n - 1 - j ][ i ].ship = board -> matrix[ n - 1 - i ][ n - 1 - j ].ship;
            board -> matrix[ n - 1 - i ][ n - 1 - j ].ship =  board -> matrix[ j ][ n - 1 - i ].ship;
            board -> matrix[ j ][ n - 1 - i ].ship = tmp;
            
        }
    }
    return;
}


// Shift cells inside ship board
void shift_board( Board* board, uchar move ){
  
    uchar x, y, w, z;
    uchar k = MAX_SHIP_SIZE - 1;
    int delta = ( move == UP || move == LEFT ) ? 1 : -1;
    uchar border = ( move == UP || move == LEFT ) ? 0 : k;


    // Check pieces at border limit
    for( uchar i = 0; i <= k; i++ ){
        
        x = VERTICAL( move ) ? border : i;
        y = HORIZONTAL( move ) ? border : i;

        // Cant move if ship is already on matrix limit
        if( board -> matrix[ x ][ y ].ship != 0 )
            return;
    }


    // Move pixels considering direction
    for( uchar i = 0; i < k; i++){
        for( uchar j = 0; j <= k; j++){

            x = VERTICAL( move ) ? ( (move == UP) ? i : k-i ) : j;
            y = HORIZONTAL( move ) ? ( (move == LEFT) ? i : k-i ) : j;
            w = VERTICAL( move ) ? ( (move == UP) ? i + delta : k-1-i ) : j;
            z = HORIZONTAL( move ) ? ( (move == LEFT) ? i + delta : k-1-i ) : j;

            // Move pixel
            board -> matrix[ x ][ y ].ship = board -> matrix[ w ][ z ].ship;
            
        }
    }

    // Set last border to 0's
    for( uchar i = 0; i < MAX_SHIP_SIZE; i++){
       
        x = VERTICAL( move ) ? ( (move == UP) ? k : 0 ) : i ;
        y = HORIZONTAL( move ) ? ( (move == LEFT) ? k : 0 ) : i ;

        // Set to zero
        board -> matrix[ x ][ y ].ship = 0;
    }

    return;
}



// Copy board
void copy_board( Board* dst, Board* src ){

    // Copy matrix
    uchar n = dst -> size; 
    for( uchar i = 0; i < n; i++){
        for( uchar j = 0; j < n; j++){
            dst -> matrix[ i ][ j ].ship = src -> matrix[ i ][ j ].ship;
        }
    }

    // Copy idx
    dst -> idx = src -> idx;

    // Copy ships
    for( uchar idx = 1; idx <= MAX_SHIPS( settings -> board_size ) ; idx++ ){
        copy_ship( dst -> ships[ idx ], src -> ships[ idx ] );
    }

    // Copy no. of alive ships
    dst -> ships_alive = src -> ships_alive;
}


// Check ship overlap
bool ship_overlap( Board* dst, Board* src, Pos pos ){

    uchar ii = 0,
        jj = 0;
    
    for( uchar i = pos.x - 1 - BOARD_SPAN; i <= pos.x - 1 + BOARD_SPAN; i++){
        
        jj = 0;
        for( uchar j = pos.y - 1 - BOARD_SPAN; j <= pos.y - 1 + BOARD_SPAN; j++){

            // Overlap detected if there is a pixel of another ship
            // where we want to place the ship
            if( src -> matrix[ ii ][ jj ].ship != 0
                && dst -> matrix[ i ][ j ].ship != 0 ){
                return true;
            
            }
            jj++;
            
        }
        ii++;
    }
    
    return false;
}


