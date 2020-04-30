#include "config.h"
#include "game.h"
#include "random.h"

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

    uchar n = settings -> board_size;
    bool game_finished = false;
    bool player1_turn = rand_bool();


    char msg[] = "\n\n\n\nThe War Begins!\n\nPlayer \"%s\" starts the game with the first play.\n";
    
    // Who starts
    if( player1_turn ){
        printf( msg, player1 -> name );
    } else {
        printf( msg, player2 -> name );
    }


    Player* player;
    Player* enemy; 
    
    do{
        
        char msg[] = "\nIt's \"%s\" turn.\nCoordinates (x y) to fire:\n> ";

        player = player1_turn? player1 : player2;
        enemy = player1_turn ? player2 : player1;
        
        print_board( enemy -> board, true );
        printf( msg, player -> name );
      
        
        Pos pos;
        scanf( " (%hhd %hhd)", &pos.x, &pos.y );

        // Check border limits
        if( ( pos.x < 1 || pos.x > n ) || ( pos.y < 1 || pos.y > n ) ){
            printf( "\nInvalid coordinates (x y). Try again.\n" );
            continue;
        }    


        // Set enemy_board depending on current player turn 
        Board* enemy_board = enemy -> board;
        

        // Set target cell considering (x y)
        Cell* target = &enemy_board -> matrix[ pos.x - 1][ pos.y - 1];


        // Must play on cells that were not played before
        if( target -> state != UNKNOWN ){
            printf( "\nAlready played in (%hhd %hhd). Try again.\n", pos.x, pos.y );
            continue;
        }


        // Ship found at (x y). HIT!
        if( target -> ship > 0 ){

            uchar ship_idx =  target -> ship;
            Ship* enemy_ship = enemy_board -> ships[ ship_idx ];
            
            target -> state = HIT;
            enemy_ship -> shot_count++;

            // Ship has no more pieces, then its dead
            if( enemy_ship -> shot_count == enemy_ship -> size ){
                enemy_ship -> alive = false;
                enemy_board -> ships_alive--;
            }

            // If player has no more ships, game finish
            if( enemy_board -> ships_alive == 0 ){
                game_finished = true;
            }
            
            print_board( enemy_board, true );
            printf( "\nHIT!\n" );
           

        }
        // Missed :( Swap player turn
        else{

            // No ship found in target cell
            target -> state = MISS;
            player1_turn = !player1_turn;

            print_board( enemy_board, true );
            printf( "\nMISS.\n" );
        }

  
        // Wait a bit
        sleep(1);
        
    }while( !game_finished );

    printf( "\n\nGame finished!\nThe winner is \"%s\".\n\nDo you want to play again?\n",
            player -> name );
    
    return;
}

