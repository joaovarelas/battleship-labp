#include "player.h"

Player* init_player( char* name ){

    Player* player = ( Player* ) malloc ( sizeof ( Player ) );
    player -> name = ( char* ) malloc ( MAX_PLAYER_NAME * sizeof ( char ) );
    
    strcpy( player -> name, name);
    player -> board = init_board ( settings -> board_size  );
    player -> alive = true;
   
    return player;
}

void free_player( Player* player ){

    free_board( player -> board );
    free( player -> name );
    free( player );
    
    return;
}


Player* setup_player( ){


    char name[ MAX_PLAYER_NAME ];    
    printf( "\nPlayer Name:\n> " );
    scanf( "%s", name );

    Player* player = init_player ( name );
        
    printf( "\n1 - Random Strategy\n" );
    printf( "2 - Custom Strategy\n> " );
    
    uchar x;
    scanf( " %hhd", &x );

    switch( x ){

    case 1:
        // TODO: Generate rand. board
        break;

    case 2:
        {
            // Let user build & place ships
            uchar i = 1;
            while( i <= settings -> num_ships ) {

                printf( "\nBuilding Ship #%hhd\n", i );
                
                Board* ship = build_ship();
                place_ship( player -> board, ship );
                
                i++;
            }
        }
        
        break;

    default:
        break;

    }
    
    return player;
}


Board* build_ship( ){

    Board* tmp_board = init_board( MAX_SHIP_SIZE );
    uchar pieces = 0;

    // constant index. 1 ship only
    uchar idx = 1;
    
    printf( "\nEnter coordinates (x y) to place one piece (0 0 to finish):\n" );

    Pos pos;
    
    do{
        print_board( tmp_board, false );

        printf( "\nCoordinates (x y):\n> " );
        scanf( " %hhd %hhd", &pos.x, &pos.y );

        // TODO: Check adjacent pixels
        
        if( pos.x > 0 && pos.y > 0 ){
            tmp_board -> matrix[ pos.x - 1 ][ pos.y - 1 ].ship = idx;
            pieces++;
        }
        
    }while( pos.x > 0 && pos.y > 0 );

    tmp_board -> ships[ idx ] = init_ship( pos, pieces );

    uchar z;
    do {

        print_board( tmp_board, false );

        printf( "\nDo you want to rotate it? (clockwise):\n\n1 - Yes\n2 - No\n> " );

        scanf( " %hhd", &z );

        if( z == 1 ){
            rotate_board( tmp_board );
        }
        
    } while( z == 1 );
    
    return tmp_board;
}



void place_ship( Board* player_board, Board* ship_board ){
  
    uchar n = settings -> board_size;
    bool placed = false;

    printf( "\nEnter coordinates (x y) to place the ship:\n" );

    do{
        Board* tmp_board = init_board( n );
        copy_board( tmp_board, player_board );

        print_board( player_board, false );

        printf( "\nCoordinates (x y) considering the 5x5 ship origin (center):\n> " );

        Pos pos;
        scanf( " %hhd %hhd", &pos.x, &pos.y );

        // TODO: Check restrictions

        uchar span = ( MAX_SHIP_SIZE / 2 );


        // Temp. place before confirmation
        for( uchar i = 0; i < MAX_SHIP_SIZE; i++ ){
            for( uchar j = 0; j < MAX_SHIP_SIZE; j++ ){

                if( ship_board -> matrix[ i ][ j ].ship != 0 ){
                    tmp_board -> matrix[ i + pos.x - 1 - span ][ j + pos.y - 1 - span ].ship = 1;
                }
                
            }
        }
        
        print_board( tmp_board, false );

        uchar z;
        printf( "\nDo you want to place it at (%d %d)?\n\n1 - Yes\n2 - No\n> ", pos.x, pos.y);
        scanf( "%hhd", &z );


        uchar pieces = 0;
        
        if( z == 1 ){

            // Placement on player -> board
            
            for( uchar i = 0; i < MAX_SHIP_SIZE; i++ ){
                for( uchar j = 0; j < MAX_SHIP_SIZE; j++ ){
                    
                    if( ship_board -> matrix[ i ][ j ].ship != 0 ){
                        uchar idx = player_board -> idx + 1;
                        player_board -> matrix[ i + pos.x - 1 - span ][ j + pos.y - 1 - span ].ship = idx;
                        pieces++;
                    }
                    
                }
            }

            player_board -> idx++;

            player_board -> ships[ player_board -> idx ] = init_ship( pos, pieces );

            copy_ship( player_board -> ships[ player_board -> idx ], ship_board -> ships[ 1 ] );
            placed = true;
            
        }
        
        free_board( tmp_board );
        
    }while( !placed );

    free_board( ship_board );
    
    return;
}
