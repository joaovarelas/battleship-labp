#include "config.h"
#include "ship.h"
#include "board.h"
#include "settings.h"

Ship* init_ship(){
    
    Ship* ship = ( Ship* ) malloc ( sizeof( Ship ) );

    ship -> pos.x = -1; 
    ship -> pos.y = -1;
    ship -> size = 0;
    ship -> shot_count = 0;
    ship -> alive = true;

    return ship;
}


void copy_ship( Ship* dst, Ship* src ){

    dst -> pos.x = src -> pos.x;
    dst -> pos.y = src -> pos.y;
    dst -> size = src -> size;
    dst -> shot_count = src -> shot_count;
    dst -> alive = src -> alive;
    
    return;
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
  
    return tmp_board;
}


void manual_place_ship( Board* player_board, Board* ship_board ){
  
    uchar n = settings -> board_size;
    bool placed = false;

    printf( "\nEnter coordinates (x y) to place the ship:\n" );

    uchar z;
    Pos pos;
    do{
        Board* tmp_board = init_board( n );

        bool overlap = true;
               
        do{
            
            pos.x = 3;
            pos.y = 3;
            
            copy_tmp_board( pos, player_board, ship_board, tmp_board );

            print_board( tmp_board, false );

            // Check ship overlap
            if( !ship_overlap( player_board, ship_board, pos ) ){
                overlap = false;
            }else{
                overlap = true;
                char msg[] = "\nThere is a ship placed in this position already.\n" \
                    "Choose a different place.\n";
                printf( msg );
            }
            

            char msg[] = "\nPlace the ship on board:\n"     \
                "\n1 - Up\n2 - Down\n3 - Left\n4 - Right"   \
                "\n5 - Rotate (clockwise)\n6 - Done\n> ";
            
            printf( msg );
            
            scanf( " %hhd", &z );

            move_ship( z, pos, ship_board );

            
        }while( z != 6 || overlap );

        place_ship( player_board, ship_board, tmp_board );
            
        placed = true;
        
        free_board( tmp_board );
    }while( !placed );


    return;
}



void random_place_ship( Board* player_board, Board* ship_board ){
    
    uchar n = settings -> board_size;
    bool placed = false;

    uchar z;

    Pos pos;
    do{
        Board* tmp_board = init_board( n );

        bool overlap = true;
        
        do{

            pos.x = (rand() % ( (n-3) - 3 + 1)) + 3;
            pos.y = (rand() % ( (n-3) - 3 + 1)) + 3;

            copy_tmp_board( pos, player_board, ship_board, tmp_board );

            //print_board( tmp_board, false );

            // Check ship overlap
            if( !ship_overlap( player_board, ship_board, pos ) ){
                overlap = false;
            }else{
                overlap = true;
                continue;
            }
            
            z = (rand() % ( 6 - 1 + 1)) + 1;
            
            move_ship( z, pos, ship_board );
            
        }while( z != 6 || overlap );

        place_ship( player_board, ship_board, tmp_board );

        placed = true;
        
        free_board( tmp_board );
        
    }while( !placed );

 
    return;
}



void place_ship( Board* player_board, Board* ship_board, Board* tmp_board ){
    
    // Placement on player -> board
    copy_board( player_board, tmp_board );

    uchar idx = ship_board -> idx;
            
    player_board -> idx = idx;    
    player_board -> ships_alive++;
    player_board -> ships[ idx ] -> size = ship_board -> ships[ idx ] -> size;
            
    return;
}


void move_ship( uchar z, Pos pos, Board* ship_board ){

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

    return;
}
