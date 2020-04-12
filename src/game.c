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
    bool player_turn = (bool)( rand() % 2 ); // player1 turns

    
    char msg[] = "\n\n\n\nThe War Begins!\n\nPlayer \"%s\" starts the game with the first play.\n";
    if( player_turn ){
        printf( msg, player1 -> name );
    } else {
        printf( msg, player2 -> name );
    }

    uchar n = settings -> board_size;
    
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
        scanf( " (%hhd %hhd)", &pos.x, &pos.y );

        // Check border limits
        if( ( pos.x < 1 || pos.x > n ) || ( pos.y < 1 || pos.y > n ) ){
            printf( "\nInvalid coordinates (x y). Try again.\n" );
            continue;
        }    

        Board* enemy_board = ( player_turn ) ? player2 -> board : player1 -> board;

        Cell* target = &enemy_board -> matrix[ pos.x - 1][ pos.y - 1];

        
        if( target -> state != UNKNOWN ){
            printf( "\nAlready played in (%hhd %hhd). Try again.\n", pos.x, pos.y );
            continue;
        }

        //printf( "\nDEBUG: x %hhd y %hhd", pos.x, pos.y );
        
        if( target -> ship > 0 ){


            uchar ship_idx =  target -> ship;
            Ship* enemy_ship = enemy_board -> ships[ ship_idx ];

            /*
            printf( "\nDEBUG: target->ship %hhd", target -> ship );
            printf( "\nDEBUG: enemy_ship->size %hhd", enemy_ship -> size );
            printf( "\nDEBUG: target->state %hhd", target -> state );
            printf( "\nDEBUG: enemy_ship->shotcount %hhd", enemy_ship -> shot_count );
            */
            
            target -> state = HIT;
            enemy_ship -> shot_count++;

            /*
            printf( "\nDEBUG: target->state %hhd", target -> state );
            printf( "\nDEBUG: enemy_ship->shotcount %hhd", enemy_ship -> shot_count );
            */
            
            if( enemy_ship -> shot_count == enemy_ship -> size ){
                enemy_ship -> alive = false;
                enemy_board -> ships_alive--;
            }

            if( enemy_board -> ships_alive == 0 ){
                game_finished = true;
            }

            //printf( "\nDEBUG: enemy_board->ships_alive %hhd", enemy_board -> ships_alive );
            
            print_board( enemy_board, true );
            printf( "\nHIT!\n" );
           

        } else {

            target -> state = MISSED;
            player_turn = !player_turn;

            print_board( enemy_board, true );
            printf( "\nMISSED SHOT.\n" );
        }

        sleep(2);
        
    }while( !game_finished ); 

    printf( "\n\nGame finished!\nThe winner is \"%s\".\n\nDo you want to play again?\n",
            ( ( player_turn ) ? player1 -> name : player2 -> name) );
    
    
    return;
}

