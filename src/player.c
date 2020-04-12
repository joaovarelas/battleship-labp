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


Player* setup_player( ){


    char name[ MAX_LINE_SIZE ];    
    printf( "\nPlayer Name:\n> " );
    scanf( " %[^\n]s", name );
    
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
            // Let user rotate & place ships
            uchar i = 1;
            while( i <= settings -> num_ships ) {

                printf( "\nBuilding Ship #%hhd\n", i );
                
                Board* ship = build_ship_test( i );
                place_ship_test( player -> board, ship );
                free_board( ship );
                
                i++;
            }
        }
        
        break;

    default:
        break;

    }
    
    return player;
}



Board* build_ship( uchar idx ){
    Board* tmp_board = init_board( MAX_SHIP_SIZE );

    uchar k = 0,
        pieces = 0;
    
    for( uchar i = 0; i < MAX_SHIP_SIZE; i++ ){
        for( uchar j = 0; j < MAX_SHIP_SIZE; j++ ){
            
            bool piece = settings -> ship_shape[ idx ][ k ];

            tmp_board -> matrix[ i ][ j ].ship = ( piece ) ? idx : 0 ;

            if( piece ){
                pieces++;
            }
            
            k++;
        }
    }

    tmp_board -> idx = idx;
    tmp_board -> ships[ idx ] -> size = pieces;
    
    uchar x;

    do{
        print_board( tmp_board, false );

        char msg[] = "\nDo you want to reposition the ship?\n"\
            "This ship box will be placed in game board afterwards.\n" \
            "Consider the origin (3 3)\n" \
            "\n1 - Up\n2 - Down\n3 - Left\n4 - Right" \
            "\n5 - Rotate (clockwise)\n6 - Done\n> ";
        
        printf( msg );

        scanf( " %hhd", &x );

        if( x >= 1 && x <= 4 ){
            shift_board( tmp_board, x );
        }else if( x == 5 ){
            rotate_board( tmp_board );
        }
        
    }while( x != 6 );
  
    return tmp_board;
}




void place_ship( Board* player_board, Board* ship_board ){
  
    uchar n = settings -> board_size;
    bool placed = false;

    printf( "\nEnter coordinates (x y) to place the ship:\n" );

    do{
        Board* tmp_board = init_board( n );
        copy_board( tmp_board, player_board );

        Pos pos;
        pos.x = -1;
        pos.y = -1;

        bool valid_xy = false,
            overlap = true;
        
        do{
            
            print_board( player_board, false );

            printf( "\nCoordinates (x y) considering the 5x5 ship box origin (center):\n> " );

            
            scanf( " (%hhd %hhd)", &pos.x, &pos.y );                                                            

            // Check border limits
            if( ( pos.x < 3 || pos.x > n - 2 ) || ( pos.y < 3 || pos.y > n - 2 ) ){
                printf( "\nInvalid coordinates (x y)\n" );
                continue;
            }
            valid_xy = true;

            // Check ship overlap
            overlap = ship_overlap( tmp_board, ship_board, pos );
            if( overlap ){
                printf( "\nThere is a ship placed in this position already. Choose different coordinates (x y).\n" );
                continue;
            }
            overlap = false;
            
        }while( !valid_xy || overlap );


       
        uchar span = ( MAX_SHIP_SIZE / 2 );

        // Temp. place before confirmation
        for( uchar i = 0; i < MAX_SHIP_SIZE; i++ ){
            for( uchar j = 0; j < MAX_SHIP_SIZE; j++ ){

                if( ship_board -> matrix[ i ][ j ].ship != 0 ){
                    uchar x = i + pos.x - 1 - span;
                    uchar y = j + pos.y - 1 - span;
                    tmp_board -> matrix[ x ][ y ].ship = 1;
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
                        uchar x = i + pos.x - 1 - span;
                        uchar y = j + pos.y - 1 - span;
                        player_board -> matrix[ x ][ y ].ship = idx;
                        pieces++;
                    }
                    
                }
            }

            uchar idx = ship_board -> idx;
            
            player_board -> idx = idx;    
            player_board -> ships_alive++;
            player_board -> ships[ idx ] -> size = ship_board -> ships[ idx ] -> size;
            
            placed = true;
            
        }
        
        free_board( tmp_board );
        
    }while( !placed );

    //free_board( ship_board ); /* DOUBLE FREE */

    return;
}












Board* build_ship_test( uchar idx ){
    Board* tmp_board = init_board( MAX_SHIP_SIZE );

    uchar k = 0,
        pieces = 0;
    
    for( uchar i = 0; i < MAX_SHIP_SIZE; i++ ){
        for( uchar j = 0; j < MAX_SHIP_SIZE; j++ ){
            
            bool piece = settings -> ship_shape[ idx ][ k ];

            tmp_board -> matrix[ i ][ j ].ship = ( piece ) ? idx : 0 ;

            if( piece ){
                pieces++;
            }
            
            k++;
        }
    }

    tmp_board -> idx = idx;
    tmp_board -> ships[ idx ] -> size = pieces;
  
    return tmp_board;
}


void place_ship_test( Board* player_board, Board* ship_board ){
  
    uchar n = settings -> board_size;
    bool placed = false;

    printf( "\nEnter coordinates (x y) to place the ship:\n" );

    uchar z;

    do{
        Board* tmp_board = init_board( n );

        Pos pos;
        pos.x = 3;
        pos.y = 3;


        bool overlap = true;
               
        do{
            
            copy_board( tmp_board, player_board );

            uchar span = ( MAX_SHIP_SIZE / 2 );

            // Temp. place before confirmation
            for( uchar i = 0; i < MAX_SHIP_SIZE; i++ ){
                for( uchar j = 0; j < MAX_SHIP_SIZE; j++ ){

                    if( ship_board -> matrix[ i ][ j ].ship != 0 ){
                        uchar x = i + pos.x - 1 - span;
                        uchar y = j + pos.y - 1 - span;
                        tmp_board -> matrix[ x ][ y ].ship = 1;
                    }
                
                }
            }

        
            print_board( tmp_board, false );

            char msg[] = "\nPlace the ship on board:\n"     \
                "\n1 - Up\n2 - Down\n3 - Left\n4 - Right"   \
                "\n5 - Rotate (clockwise)\n6 - Done\n> ";
            
            printf( msg );
            
            scanf( " %hhd", &z );                                                          


            switch( z ){
            case 1:
                if( pos.x > 3 )
                    pos.x--;
                else
                    shift_board( ship_board, z );
                break;
                
            case 2:
                if( pos.x < settings -> board_size - 2 )
                    pos.x++;
                else
                    shift_board( ship_board, z );
                break;
                
            case 3:
                if( pos.y > 3)
                    pos.y--;
                else
                    shift_board( ship_board, z );
                break;

            case 4:
                if( pos.y < settings -> board_size - 2 )
                    pos.y++;
                else
                    shift_board( ship_board, z );
                break;
                
            case 5:
                rotate_board( ship_board );
                break;

            default:
                break;
                
            }


            if( !ship_overlap( player_board, ship_board, pos ) ){
                overlap = false;
            }else{
                overlap = true;
                char msg[] = "\nThere is a ship placed in this position already.\n" \
                    "Choose a different place.\n";
                printf( msg );
            }


            
        }while( z != 6 || overlap );


        // Placement on player -> board
        copy_board( player_board, tmp_board );

        uchar idx = ship_board -> idx;
            
        player_board -> idx = idx;    
        player_board -> ships_alive++;
        player_board -> ships[ idx ] -> size = ship_board -> ships[ idx ] -> size;
            
        placed = true;
        

        free_board( tmp_board );
    }while( !placed );


    return;
}

