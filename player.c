#include "player.h"

Player* init_player( char* name ){

    Player* p = ( Player* ) malloc ( sizeof ( Player ) );
    unsigned int n = settings -> board_size;
    
    p -> name = name;
    p -> board = init_board ( settings -> board_size  );
    p -> has_ships_alive = true;
    p -> hit_matrix = ( short** ) malloc ( n * sizeof ( short* ) );

    for( int i = 0 ; i < n ; i++ ){

        p -> hit_matrix[ i ] = ( short* ) malloc ( n * sizeof ( short ) );
        
        for( int j = 0 ; j < n ; j++){
            
            p -> hit_matrix[ i ][ j ] =  0;
        }
    }
    
    
    return p;
}




Player* setup_player( ){

    printf( "\n" );
    printf( "Player Name:\n" );
    printf( "> ");

    char name[16];
    scanf( "%s", &name );
    Player* p = init_player ( name );

    printf( "1 - Random Strategy\n" );
    printf( "2 - Custom Strategy\n" );
    printf( "> ");

    short x;
    scanf( "%hi", &x );

    switch( x ){

    case 1:

        // gen. random board
        
        break;

    case 2:

        // let user place ships
        
        break;

    default:
        break;

    }
    
    return p;
}
