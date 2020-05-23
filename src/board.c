#include "board.h"
#include "settings.h"
#include "quadtree.h"

Board* init_board( byte size, BoardType type ){

    Board* board = ( Board* ) malloc ( sizeof( Board ) );

    board -> type = type;
    board -> size = size;

    board -> matrix = NULL;
    board -> qtree = NULL;
    
    if( type == QUADTREE ){
        
        Pos p, q;
        init_pos( &p, 1, 1 );
        init_pos( &q, size*size, size*size );
        board -> qtree = init_qtree( p, q );
    
    }else{

        init_matrix( board );
    }

    // Allocate enough slots for ships
    board -> ships = ( Ship** ) malloc ( ( 1 + MAX_SHIPS( settings -> board_size ) ) * sizeof( Ship* ) );
    board -> idx = 0;
    board -> ships_alive = 0;
        
    for( byte i = 1; i <= MAX_SHIPS( settings -> board_size ) ; i++ ){
        board -> ships[ i ] = init_ship();
    }
    
    return board;
}


void init_matrix( Board* board ){
    byte n = board -> size;
    
    board -> matrix = ( Cell** ) malloc ( n * sizeof( Cell* ) );
    for( byte i = 0; i < n ; i++ ){
        
        board -> matrix[ i ] = ( Cell* ) malloc ( n * sizeof( Cell ) );
        
        for( byte j = 0; j < n; j++ ){
            
            init_cell( &board -> matrix[ i ][ j ], 0, UNKNOWN );
        }
                       
    }
    return;
}



// Send it to the void
void free_board( Board* board ){

    if( board -> type == QUADTREE ){
        
        if( board -> qtree != NULL )
            free_qtree( board -> qtree );
    }else{
        
        if( board -> matrix != NULL )
            free_matrix( board );
    }
   
    for( byte i = 1; i <= MAX_SHIPS( board -> size ); i++ ){
        free( board -> ships[ i ] );
    }
    
    free( board -> ships );

    board -> ships = NULL;

    free( board );

    board = NULL;
    
    return;
}

void free_matrix( Board* board ){
    
    for( byte i = 0; i < board -> size; i++ ){
        free( board -> matrix[ i ] );
    }

    free( board -> matrix );
    board -> matrix = NULL;
    
    return;
}


void print_board( Board* board, bool game_mode ){

    byte n = board -> size;
    byte i, j;

    // X Lines
    // Y Columns
    
    printf( "\nX\\Y" );
    for( i = 1; i <= n; i++ ){
        printf( "  %02hhd", i );
    }
    
    printf( "\n   ┌" );
    for( i = 0; i < n - 1; i++ ){
        printf( "───┬" );
    }
    printf( "───┐" );

    for( i = 0 ; i < n ; i++ ) {
        printf( "\n%02hhd │", i + 1 );
        for( j = 0 ; j < n ; j++ ){
            printf( " " );

            Pos pos;
            init_pos( &pos, i, j );

            print_cell( board, pos, game_mode );
                            
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


void print_cell( Board* board, Pos pos, bool game_mode ){
    
    Cell cell;

    if( board -> type == QUADTREE ){

        pos.x++;
        pos.y++;

        QNode* node = get_node( board -> qtree, pos );

        if( node != NULL ){
            copy_cell( &cell, &node -> cell );
        }else{
            init_cell( &cell, 0, UNKNOWN );
        }
        
       
    }else{
      
        copy_cell( &cell, &board -> matrix[ pos.x ][ pos.y ]);
    }
    
    // Print by "state" or Print by "ship"
    if( game_mode ){
                
        switch( cell.state ){
        case MISS:
            printf( "•" );
            break;
        case UNKNOWN:
            printf( " " );
            break;
        case HIT:
            printf( "X" );
            break;
        default:
            break;
        }
                 
    } else {
                    
        switch( cell.ship ){
        case 0:
            printf( " " );
            break;
        case 66:
            printf( "•" );
            break;
        case 99:
            printf( "X" );
            break;
        default:
            printf( "■" );
            break;
        }
                    
    }

    return;
}



// Rotate nxn matrix 90º clockwise
// In-place n-cycle approach, swapping ring elements
void rotate_board( Board* board ){
    
    byte n = board -> size;
    
    for( byte i = 0; i < n / 2; i++){
        for( byte j = i; j < n - 1 - i; j++){

            byte tmp = board -> matrix[ i ][ j ].ship;
            
            board -> matrix[ i ][ j ].ship =  board -> matrix[ n - 1 - j ][ i ].ship;
            board -> matrix[ n - 1 - j ][ i ].ship = board -> matrix[ n - 1 - i ][ n - 1 - j ].ship;
            board -> matrix[ n - 1 - i ][ n - 1 - j ].ship =  board -> matrix[ j ][ n - 1 - i ].ship;
            board -> matrix[ j ][ n - 1 - i ].ship = tmp;
            
        }
    }
    return;
}


// Shift cells inside ship board
void shift_board( Board* board, byte move ){
  
    byte x, y, w, z;
    byte k = MAX_SHIP_SIZE - 1;
    int delta =   ( move == UP || move == LEFT ) ? 1 : -1;
    byte border = ( move == UP || move == LEFT ) ? 0 : k;


    // Check pieces at border limit
    for( byte i = 0; i <= k; i++ ){
        
        x =   VERTICAL( move ) ? border : i;
        y = HORIZONTAL( move ) ? border : i;

        // Cant move if ship is already on matrix limit
        if( board -> matrix[ x ][ y ].ship != 0 )
            return;
    }


    // Move pixels considering direction
    for( byte i = 0; i < k; i++){
        for( byte j = 0; j <= k; j++){

            x =   VERTICAL( move ) ?   ( (move == UP) ? i : k-i ) : j;
            y = HORIZONTAL( move ) ? ( (move == LEFT) ? i : k-i ) : j;
            w =   VERTICAL( move ) ?   ( (move == UP) ? i + delta : k-1-i ) : j;
            z = HORIZONTAL( move ) ? ( (move == LEFT) ? i + delta : k-1-i ) : j;

            // Move pixel
            board -> matrix[ x ][ y ].ship = board -> matrix[ w ][ z ].ship;
            
        }
    }

    // Set last border to 0's
    for( byte i = 0; i < MAX_SHIP_SIZE; i++){
       
        x =   VERTICAL( move ) ?   ( (move == UP) ? k : 0 ) : i ;
        y = HORIZONTAL( move ) ? ( (move == LEFT) ? k : 0 ) : i ;

        // Set to zero
        board -> matrix[ x ][ y ].ship = 0;
    }

    return;
}



// Copy board
void copy_board( Board* dst, Board* src ){

    // Copy matrix
    byte n = dst -> size; 
    for( byte i = 0; i < n; i++){
        for( byte j = 0; j < n; j++){

            byte ship = 0;
            
            if( src -> type == QUADTREE ){
                
                Pos pos;
                init_pos( &pos, i+1, j+1 );
                QNode* node = get_node( src -> qtree, pos );
                
                if( node != NULL ){
                    ship = node -> cell.ship;
                }else{
                    ship = 0;
                }
                                           
            }else{
                ship = src -> matrix[ i ][ j ].ship;
            }

            dst -> matrix[ i ][ j ].ship = ship;

        }
    }

    // Copy idx
    dst -> idx = src -> idx;
       
    // Copy no. of alive ships
    dst -> ships_alive = src -> ships_alive;
    
    // Copy ships
    for( byte idx = 1; idx <= MAX_SHIPS( settings -> board_size ) ; idx++ ){
        copy_ship( dst -> ships[ idx ], src -> ships[ idx ] );
    }

}


// Check ship overlap
bool ship_overlap( Board* dst, Board* ship_board, Pos pos ){
    
    byte ii = 0,
        jj = 0;

    for( byte i = pos.x - 1 - BOARD_SPAN; i <= pos.x - 1 + BOARD_SPAN; i++){
        
        jj = 0;
        for( byte j = pos.y - 1 - BOARD_SPAN; j <= pos.y - 1 + BOARD_SPAN; j++){

            if( ship_board -> matrix[ ii ][ jj ].ship != 0 ){

                if( dst -> type == QUADTREE ){

                    Pos new_pos;
                    init_pos( &new_pos, i + 1, j + 1 );
                                       
                    QNode* node = get_node( dst -> qtree, new_pos );

                    if( node != NULL )
                        return true;
                    
                }else{

                    if( dst -> matrix[ i ][ j ].ship != 0 )
                        return true;
                    
                }
           
            }
            jj++;
            
        }
        ii++;
    }

   
    
    return false;
}




void matrix_to_qtree( Board* board ){

    byte n = board -> size;

    for( byte i = 0; i < n; i++ ){
        for( byte j = 0; j < n; j++ ){

            Cell cell = board -> matrix[ i ][ j ];
            
            if( cell.ship != 0 ){

                Pos pos;
                init_pos( &pos, i + 1, j + 1 );

                insert_node( board -> qtree, init_qnode( pos, cell ) );
                
            }
            
        }
    }

    return;
}
