#include "player.h"

Player* init_player( char* name ){

    Player* player = ( Player* ) malloc ( sizeof ( Player ) );
    player -> name = ( char* ) malloc ( MAX_LINE_SIZE * sizeof ( char ) );
    
    strcpy( player -> name, name);
    player -> board = init_board ( settings -> board_size  );
    player -> alive = true;
   
    return player;
}


// Send it to the void
void free_player( Player* player ){

    free_board( player -> board );
    free( player -> name );
    free( player );
    
    return;
}

// Setup player's name and board configuration
Player* setup_player( ){


    char name[ MAX_LINE_SIZE ];    
    printf( "\nPlayer Name:\n> " );
    scanf( " %[^\n]s", name );
    
    Player* player = init_player ( name );
        
    printf( "\n1 - Random Strategy\n" );
    printf( "2 - Custom Strategy\n> " );
    
    uchar x;
    scanf( " %hhd", &x );


    uchar i = 1;
    while( i <= settings -> num_ships ) {
                
        Board* ship = build_ship( i );

        // Place ship on board (randomly or manually)
        if( x == 1 ){
            random_place_ship( player -> board, ship );
        }else{
            manual_place_ship( player -> board, ship );
        }
        
        free_board( ship );
                
        i++;
    }

    // Print final board to player
    printf( "\nFinal board of player \"%s\":\n", player -> name );
    print_board( player -> board, false );

    // Print some spaces to hide board
    uchar n = settings -> board_size;
    while( n-- > 0 )
        printf( "\n" );
    return player;
}



