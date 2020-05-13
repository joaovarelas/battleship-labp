#include "player.h"
#include "quadtree.h"
#include "server.h"

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
void setup_player( Player* player ){

    byte q;
    do{
        printf( "\n1 - Random Strategy\n" );
        printf( "2 - Custom Strategy\n> " );
        fgets( buffer, sizeof( buffer ), stdin );
    }while( sscanf( buffer, "%hhu", &q ) != 1 );



    byte qq;
    do{

        byte i = 1;
        while( i <= settings -> num_ships ) {
                
            Board* ship = build_ship( i );

            // Place ship on board (randomly or manually)
            if( q == 1 ){
                random_place_ship( player -> board, ship );
            }else{
                manual_place_ship( player -> board, ship );
            }
        
            free_board( ship );
                
            i++;
        }
        
        do{
            print_board( player -> board, false );
            printf( "\nDo you want to use this strategy?\n1 - Yes\n2 - No\n> " );
            fgets( buffer, sizeof( buffer ), stdin );
        }while( sscanf( buffer, "%hhu", &qq ) != 1 );

        // Reset board
        if( qq != 1 ){
            free_board( player -> board );
            player -> board = init_board( settings -> board_size );
        }
    
    }while( qq != 1 );

    
    // Convert n x n cell matrix to quadtree
    matrix_to_qtree( player -> board );
    
    // Get rid of matrix
    // Game state is now supported by a quad tree structure
    free_matrix( player -> board );
    
    // Print final board to player
    print_board( player -> board, false );
    printf( "\nFinal board of player \"%s\":\n", player -> name );

    return;
}



