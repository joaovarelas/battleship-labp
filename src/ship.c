#include "config.h"
#include "ship.h"
#include "cell.h"
#include "board.h"
#include "settings.h"
#include "random.h"
#include "server.h"

Ship* init_ship(){
    
    Ship* ship = ( Ship* ) malloc ( sizeof( Ship ) );

    init_pos( &ship -> pos, 0, 0);
    ship -> size = 0;
    ship -> shot_count = 0;
    ship -> alive = true;

    return ship;
}


void copy_ship( Ship* dst, Ship* src ){

    copy_pos( &dst -> pos, &src -> pos ); 
    dst -> size = src -> size;
    dst -> shot_count = src -> shot_count;
    dst -> alive = src -> alive;
    
    return;
}


// Build ship from settings specification, given index idx
Board* build_ship( byte idx ){
    Board* tmp_board = init_board( MAX_SHIP_SIZE );

    byte k = 0;
    byte pieces = 0;
    
    for( byte i = 0; i < MAX_SHIP_SIZE; i++ ){
        for( byte j = 0; j < MAX_SHIP_SIZE; j++ ){
            
            bool piece = settings -> ship[ idx ][ k ];

            tmp_board -> matrix[ i ][ j ].ship = ( piece ) ? idx : 0 ;

            if( piece )
                pieces++;
                        
            k++;
        }
    }

    tmp_board -> idx = idx;

    tmp_board -> ships[ idx ] -> size = pieces;
  
    return tmp_board;
}


void manual_place_ship( Board* player_board, Board* ship_board ){
  
    byte n = settings -> board_size;
    bool placed = false;

    printf( "\nEnter coordinates (x y) to place the ship:\n" );

    byte q;
    Pos pos;
    do{
        Board* tmp_board = init_board( n );

        bool overlap = true;

        // Initial position at the board center
        init_pos( &pos, n/2, n/2 );
                       
        do{
             
            copy_tmp_board( pos, player_board, ship_board, tmp_board );

            print_board( tmp_board, false );

            // Check ship overlap
            overlap = ship_overlap( player_board, ship_board, pos );

            if( overlap ){
                char msg[] = "\nThere is a ship placed in this position already.\n" \
                    "Choose a different place.\n";
                printf( msg );
            }
            

            char msg[] = "\nPlace the ship on board:\n"     \
                "\n1 - Up\n2 - Down\n3 - Left\n4 - Right"   \
                "\n5 - Rotate (clockwise)\n6 - Done\n> ";


            do{
                printf( msg );
                fgets( buffer, sizeof( buffer ), stdin );
            }while( sscanf( buffer, "%hhu", &q ) != 1 );

            move_ship( q, &pos, ship_board );
            
        }while( q != 6 || overlap );

        place_ship( player_board, ship_board, tmp_board );
            
        placed = true;
        
        free_board( tmp_board );
        
    }while( !placed );
    
    return;
}


void random_place_ship( Board* player_board, Board* ship_board ){
    
   
    byte n = settings -> board_size;
    
    byte upper = n - BOARD_SPAN;
    byte lower = MAX_SHIP_SIZE - BOARD_SPAN;
    
    bool placed = false;

    Pos pos;
    
    do{
        Board* tmp_board = init_board( n );

        bool overlap = true;


        do{

            byte times;

            // Random actions within ship matrix
            for( byte action = UP; action <= RIGHT; action++ ){
                times = rand_num( 0, MAX_SHIP_SIZE - 1 );
                while( times-- > 0 ) shift_board( ship_board, action );
            }

            // Random rotate
            times = rand_num( 0, 3 );
            while( times-- > 0 ) rotate_board( ship_board );
            
            
            // Random pos. for ship matrix
            pos.x = rand_num( lower, upper );
            pos.y = rand_num( lower, upper );

            copy_tmp_board( pos, player_board, ship_board, tmp_board );

            // Check ship overlap
            overlap = ship_overlap( player_board, ship_board, pos );
            if( overlap )
                continue;
          
            
        }while( overlap );

        place_ship( player_board, ship_board, tmp_board );

        placed = true;
        
        free_board( tmp_board );
        
    }while( !placed );

    
 
    return;
}



// Placement on player -> board
void place_ship( Board* player_board, Board* ship_board, Board* tmp_board ){
    
    copy_board( player_board, tmp_board );

    byte idx = ship_board -> idx;
            
    player_board -> idx = idx;    
    player_board -> ships_alive++;
    player_board -> ships[ idx ] -> size = ship_board -> ships[ idx ] -> size;
            
    return;
}



// Move, shift, rotate and do a flip
void move_ship( byte dir, Pos* pos, Board* ship_board ){

    switch( dir ){
    case UP:
        if( pos -> x > 3 )
            pos -> x--;
        else
            shift_board( ship_board, UP );
        break;
                
    case DOWN:
        if( pos -> x < settings -> board_size - 2 )
            pos -> x++;
        else
            shift_board( ship_board, DOWN );
        break;
                
    case LEFT:
        if( pos -> y > 3)
            pos -> y--;
        else
            shift_board( ship_board, LEFT );
        break;

    case RIGHT:
        if( pos -> y < settings -> board_size - 2 )
            pos -> y++;
        else
            shift_board( ship_board, RIGHT );
        break;
                
    case ROTATE:
        rotate_board( ship_board );
        break;

    default:
        break;
                
    }

    return;
}
