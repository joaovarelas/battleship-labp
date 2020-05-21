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

	byte qq;
	do{
	    printf( "\n1 - Random Strategy\n2 - Custom Strategy\n> " );
	    fgets( buffer, sizeof( buffer ), stdin );
	}while( sscanf( buffer, "%hhu", &qq ) != 1 );


	byte idx = 1;
	while( idx <= settings -> num_ships ) {

	    Board* ship = build_ship( idx );

	    // Place ship on board (randomly or manually)
	    if( qq == 1 )
                random_place_ship( player -> board, ship );
	    else
                manual_place_ship( player -> board, ship );
	
	    free_board( ship );
                
	    idx++;
	}

        
	print_board( player -> board, false );
	    
	do{
	    printf( "\nDo you want to proceed with this board?\n1 - Yes\n2 - No\n> " );
	    fgets( buffer, sizeof( buffer ), stdin );
	}while( sscanf( buffer, "%hhu", &q ) != 1 );

	if( q != 1 ){
	    
	    // Reset board and place ships over again
	    free_board( player -> board );
	    player -> board = init_board( settings -> board_size );
	    
	}
	
    }while( q != 1 );
    

    
    #ifdef QUADTREE
    
    // Convert n x n cell matrix to quadtree
    matrix_to_qtree( player -> board );
    
    // Get rid of matrix
    // Game state is now supported by a quad tree structure
    free_matrix( player -> board );

    #pragma message( "[ + ] USING QUADTREE DATA STRUCTURE" )

    #else

    // Keep matrix
    #pragma message( "[ + ] USING MATRIX DATA STRUCTURE" )

    #endif

   
    
    // Print final board to player
    print_board( player -> board, false );
    printf( "\nFinal board of player \"%s\":\n", player -> name );

    
    return;
}



