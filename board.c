#include "board.h"

Board* init_board( unsigned int n ){
    Board* board = ( Board* ) malloc ( sizeof( Board ) );
    
    board -> matrix = ( int** ) malloc ( n * sizeof( int* ) );
    board -> ships = ( Ship ** ) malloc ( n * n * sizeof( Ship* ) );
    board -> size = n;
    board -> idx = 1;
    
    int i = 0,
        j = 0;
    
    while( i < n ){
        
        board -> matrix[ i ] = ( int* ) malloc ( n * sizeof( int ) );
        
        while( j < n ){
            
            board -> matrix[ i ][ j ] = 0;
            ++j;
        }
        ++i;
    }
    
    return board;
}


void print_board( Board* b ){

    for( int i = 0 ; i < b -> size ; i++){
        for( int j = 0 ; j < b -> size ; j++){

            switch ( b -> matrix[ i ][ j ] ){
            case 0:
                break;
            default:
                break;
            }
                                                
                    
            printf("%d ", b -> matrix[ i ][ j ]);
        }
        printf("\n");
    }

    return;
}


bool add_ship( Board* board, unsigned int x, unsigned int y,
               bool dir, unsigned int size ){

    // Check limits & ship overlap
    if( x < 0 || x > settings -> board_size -1  )
        return false;

    for( int i = 0; i < size; i++ ){
        if( dir ){
            if( board -> matrix[ x ][ y + i ] != 0 )
                return false;
        }else{
            if( board -> matrix[ x + i ][ y ] != 0 )
                return false;
        }
    }
    
           
    int idx = board -> idx;
    Ship* ship = init_ship( x, y, dir, size );
    board -> ships[ idx ] = ship;

    int i = 0;
    while( i < size ){

        if( dir ) {
            board -> matrix[ x ][ y + i ] = idx;
        } else {
            board -> matrix[ x + i ][ y ] = idx;
        }
        
        ++i;
    }

    board -> idx++;
    return true;
}


Board* rand_board( ){

    unsigned int n = settings -> board_size;
    Board* b = init_board ( n );

    srand( time(NULL) );

    for( int i = 0 ; i < settings -> num_ships ; i++ ){

        bool ship_added = false;

        while( !ship_added ){
            unsigned int ship_size = settings -> size_ships[ i ];
            bool rand_dir = (bool)( rand() % 2 );

            unsigned int rand_x, rand_y;

            rand_x = rand_dir ? abs( rand() % n ) : abs( ( rand() % n ) - ship_size );
            
            rand_y = rand_dir ? abs( ( rand() % n ) - ship_size ) : abs( rand() % n );
            
            if ( add_ship( b, rand_x, rand_y, rand_dir, ship_size ) )
                ship_added = true;
            
        }
        
    }

    return b;
}
