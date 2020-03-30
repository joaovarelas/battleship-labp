#include "player.h"

Player* init_player( char* name ){

    Player* player = ( Player* ) malloc ( sizeof ( Player ) );
    player -> name = ( char* ) malloc ( MAX_PLAYER_NAME * sizeof ( char ) );
    
    strcpy( player -> name, name);
    player -> board = init_board ( settings -> board_size  );
    player -> alive = true;
   
    return player;
}




Player* setup_player( ){
    
    printf( "\nPlayer Name:\n> " );

    char name[ MAX_PLAYER_NAME ];
    scanf( "%s", name );
    Player* player = init_player ( name );

    printf( "\n1 - Random Strategy\n" );
    printf( "2 - Custom Strategy\n> " );
    
    char x;
    scanf( " %c", &x );

    switch( x ){

    case '1':

        // gen. random board
        
        break;

    case '2':

        // let user place ships
        printf( "\nBuilding %d ships.\n", settings -> num_ships );

        Board* tmp_board = create_ship( 1 );
        
        break;

    default:
        break;

    }
    
    return player;
}


Board* create_ship( char idx ){
    
    printf( "\nEnter coordinates (x y) to place one piece (0 0 to finish):\n" );
    
    Board* board = init_board( MAX_SHIP_SIZE );

    char pieces = 0;
    
    do{
        int x = -1,
            y = -1;
    
        print_board( board );

        printf( "\nCoordinates (x y):\n> " );
        scanf( "%d %d", &x, &y );

        if( ( x == 0 || y == 0 ) && pieces > 0 ){
            break;
        }

        pieces++;  
        board -> matrix[ x - 1 ][ y - 1 ].ship = idx;

        
    }while( true );

   
    char z;
    do {

        print_board( board );

        printf( "\nDo you want to rotate it? (clockwise):\n\n1 - Yes\n2 - No\n> " );

        scanf( " %c", &z );

        if( z == '1' ){
            rotate_board( board );
        }
        
    } while( z == '1' );
    
    return board;
}
