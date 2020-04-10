#include "game.h"

void play_by_turns(){

    printf( "\nSetting up Player #1\n" );
    Player* player1 = setup_player();

    printf( "\nSetting up Player #2\n" ); 
    Player* player2 = setup_player();


    start_game( player1, player2 );

    
    free_player( player1 );
    free_player( player2 );
    
    return;
}


void start_game( Player* player1, Player* player2 ){
    
    srand( time(NULL) );

    bool game_finished = false;
    bool player_turn = true; //(bool)( rand() % 2 );

    char msg[] = "\nPlayer \"%s\" starts the game with the first play\n";
    if( player_turn ){
        printf( msg, player1 -> name );
    } else {
        printf( msg, player2 -> name );
    }

    do{
        
        char msg[] = "\nIt's \"%s\" turn.\nCoordinates (x y) to fire:\n> ";

        if( player_turn ){
            print_board( player2 -> board, true );
            printf( msg, player1 -> name );
        } else {
            print_board( player1 -> board, true );
            printf( msg, player2 -> name );
        }
        
        Pos pos;
        scanf( " %hhd %hhd", &pos.x, &pos.y );

        // TODO: check restrictions

        Board* p = ( player_turn ) ? player2 -> board : player1 -> board;

        if( p -> matrix[ pos.x - 1][ pos.y - 1].ship > 0 ){

            p -> matrix[ pos.x - 1][ pos.y - 1].state = HIT;
            p -> ships[ p -> matrix[ pos.x - 1][ pos.y - 1].ship ] -> shot_count++;
            // TODO: check liveness

            printf( "\nHIT!\n" );
           

            // DEBUG
            /*
           
              game_finished = true;
           
            */

        } else {

            p -> matrix[ pos.x - 1][ pos.y - 1].state = MISSED;
            player_turn = !player_turn;

            
            printf( "\nMISSED.\n" );
        }
            
        sleep( 2 );
    }while( !game_finished ); 

    
    return;
}

