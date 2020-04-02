#include "board.h"

Board* init_board( uchar n ){
    
    Board* board = ( Board* ) malloc ( sizeof( Board ) );

    board -> size = n;
    board -> matrix = ( Cell** ) malloc ( n * sizeof( Cell* ) );
    board -> ships = ( Ship** ) malloc ( ( 1 + MAX_SHIPS( n ) ) * sizeof( Ship* ) );
    board -> idx = 0;

    // Initialize
    for( uchar i = 0; i < n ; i++ ){
        
        board -> matrix[ i ] = ( Cell* ) malloc ( n * sizeof( Cell ) );
        
        for( uchar j = 0; j < n; j++ ){
            
            board -> matrix[ i ][ j ].ship = 0;
            board -> matrix[ i ][ j ].state = UNKNOWN;

        }
    }

    for( uchar i = 1; i <= MAX_SHIPS( n ); i++ ){
        board -> ships[ i ] = init_ship();
    }
    
    return board;
}


void print_board( Board* board, bool game_mode ){

    uchar n = board -> size;
    uchar i, j;

    printf( "\n   " );
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

            
            if( game_mode ){

                switch( board -> matrix[ i ][ j ].state ){
                case MISSED:
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

    printf( " STATE, SHIP: %d, %d\n",  board -> matrix[ i ][ j ].state,  board -> matrix[ i ][ j ].ship ); 
    return;
}


// in-place n-cycle approach, swapping ring elements
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


// copy ship positions only for temp. operations
void copy_board( Board* dst, Board* src ){
    uchar n = dst -> size; 
    for( uchar i = 0; i < n; i++){
        for( uchar j = 0; j < n; j++){
            dst -> matrix[ i ][ j ].ship = src -> matrix[ i ][ j ].ship;
        }
    }

}


// send it to the void
void free_board( Board* board ){

    uchar i;

    for( i = 0; i < board -> size; i++ ){
        free( board -> matrix[ i ] );    
    }
    free( board -> matrix );

    
    for( i = 1; i <= board -> idx; i++ ){
        free( board -> ships[ i ] );
    }
    free( board -> ships );

    free( board );
    
    return;
}

