#include "config.h"
#include "ship.h"
#include "cell.h"
#include "board.h"
#include "settings.h"
#include "random.h"
#include "server.h"
#include "quadtree.h"

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
    Board* tmp_board = init_board( MAX_SHIP_SIZE, MATRIX );

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
        Board* tmp_board = init_board( n, MATRIX );

        bool overlap = true;

        // Initial position at the board center
        init_pos( &pos, n/2, n/2 );
                       
        do{
             
            copy_board( tmp_board, player_board );

            place_ship( tmp_board, ship_board, pos );

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

            move_ship( ship_board, q, &pos );
            
        }while( q != 6 || overlap );

        place_ship( player_board, ship_board, pos );
                           
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
        
        Board* tmp_board = init_board( n, MATRIX );

        bool overlap = true;

        do{

            // Random pos. for ship matrix
            pos.x = rand_num( lower, upper );
            pos.y = rand_num( lower, upper );

            byte times;

            // Random actions within ship matrix
            for( byte action = UP; action <= RIGHT; action++ ){
                times = rand_num( 0, MAX_SHIP_SIZE - 1 );
                while( times-- > 0 ) move_ship( ship_board, action, &pos );
            }
          
            // Random rotate
            times = rand_num( 0, 3 );
            while( times-- > 0 ) rotate_board( ship_board );

            copy_board( tmp_board, player_board );
            place_ship( tmp_board, ship_board, pos );
         
            // Check ship overlap
            overlap = ship_overlap( player_board, ship_board, pos );
          
        }while( overlap );

        // Place ship on player board
        place_ship( player_board, ship_board, pos );

        placed = true;
        
        free_board( tmp_board );
        
    }while( !placed );

    return;
}



// Placement on board
void place_ship( Board* dst, Board* src, Pos pos ){
    
    byte idx = src -> idx;
            
    dst -> idx = idx;    
    dst -> ships_alive++;
    dst -> ships[ idx ] -> size = src -> ships[ idx ] -> size;

    byte span = ( MAX_SHIP_SIZE / 2 );

    for( byte i = 0; i < MAX_SHIP_SIZE; i++ ){
        for( byte j = 0; j < MAX_SHIP_SIZE; j++ ){
            
            byte x = i + pos.x - 1 - span;
            byte y = j + pos.y - 1 - span;
            
            Pos new_pos;
            init_pos( &new_pos, x + 1, y + 1 );
                        
            byte ship_idx = src -> matrix[ i ][ j ].ship;
            
            if( ship_idx != 0 ){

                if( dst -> type == QUADTREE ){
                    
                    QNode* node = get_node( dst -> qtree, new_pos );

                    if( node != NULL ){
                        
                        node -> cell.ship = ship_idx;
                                                
                    }else{
                        
                        Cell new_cell;
                        init_cell( &new_cell, ship_idx, UNKNOWN );
                       
                        QNode* new_node = init_qnode( new_pos, new_cell );
                        insert_node( dst -> qtree, new_node );
                    }
                    
                }else{
                    
                    dst -> matrix[ x ][ y ].ship = ship_idx;
                }
                
            }
                
        }
    }
    
    return;
}



// Move, shift, rotate and do a flip
void move_ship( Board* ship_board, byte dir, Pos* pos ){

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
