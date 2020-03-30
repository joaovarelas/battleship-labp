#include "board.h"

Board* init_board( unsigned char n ){
    
    Board* board = ( Board* ) malloc ( sizeof( Board ) );

    board -> size = n;
    board -> matrix = ( Cell** ) malloc ( n * sizeof( Cell* ) );
    board -> ships = ( Ship** ) malloc ( MAX_SHIPS( n )  * sizeof( Ship* ) );
    board -> idx = 1;
    
    int i = 0,
        j = 0;
    
    while( i < n ){
        
        board -> matrix[ i ] = ( Cell* ) malloc ( n * sizeof( Cell ) );
        
        while( j < n ){
            
            board -> matrix[ i ][ j ].ship = 0;
            board -> matrix[ i ][ j ].state = UNKNOWN;
            ++j;
        }
        ++i;
    }
    
    return board;
}


void print_board( Board* board){

    unsigned char n = board -> size;
    unsigned char i, j;

    printf( "\n   " );
    for( i = 1; i <= n; i++ ){
        printf( "  %02d", (int)i );
    }
    
    printf( "\n   ┌" );
    for( i = 0; i < n - 1; i++ ){
        printf( "───┬" );
    }
    printf( "───┐" );

    for( i = 0 ; i < n ; i++ ) {
        printf( "\n%02d │", (int)i + 1 );
        for( j = 0 ; j < n ; j++ ){
            printf( " " );

            if( board -> matrix[ i ][ j ].ship != 0 ){
                printf( "■" );
            }else{
                printf( " " );
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


// in-place n-cycle approach, swapping ring elements
void rotate_board( Board* board ){
    
    unsigned char n = board -> size;
    
    for( unsigned char i = 0; i < n / 2; i++){
        for( unsigned char j = i; j < n - 1 - i; j++){

            unsigned char tmp = board -> matrix[ i ][ j ].ship;
            
            board -> matrix[ i ][ j ].ship =  board -> matrix[ n - 1 - j ][ i ].ship;
            board -> matrix[ n - 1 - j ][ i ].ship = board -> matrix[ n - 1 - i ][ n - 1 - j ].ship;
            board -> matrix[ n - 1 - i ][ n - 1 - j ].ship =  board -> matrix[ j ][ n - 1 - i ].ship;
            board -> matrix[ j ][ n - 1 - i ].ship = tmp;
            
        }
    }
                                                     
    return;
}
