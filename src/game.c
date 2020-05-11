#include "config.h"
#include "game.h"
#include "random.h"
#include "quadtree.h"
#include "server.h"

void play_by_turns(){
    char name[ MAX_LINE_SIZE ];
    
    printf( "\nSetting up Player #1\nPlayer Name:\n> " );
    scanf( " %[^\n]s", name );
    Player* player1 = init_player ( name );
    setup_player( player1 );

    
    printf( "\nSetting up Player #2\n" );
    scanf( " %[^\n]s", name );
    Player* player2 = init_player ( name );
    setup_player( player2 );

    
    
    byte n = settings -> board_size;
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

        player = player1_turn ? player1 : player2;
        enemy =  player1_turn ? player2 : player1;
        
        print_board( player -> board, true );
        printf( msg, player -> name );
      
        
        Pos pos;
        scanf( " (%hhu %hhu)", &pos.x, &pos.y );

        // Check border limits
        if( ( pos.x < 1 || pos.x > n ) || ( pos.y < 1 || pos.y > n ) ){
            printf( "\nInvalid coordinates (x y). Try again.\n" );
            continue;
        }    


        // Set enemy_board depending on current player turn
        Board* player_board = player -> board;
        Board* enemy_board = enemy -> board;
        
        // Set target cell considering (x y)
        QNode* player_node = get_node( player_board -> qtree, pos );
        QNode* enemy_node = get_node( enemy_board -> qtree, pos );

        Cell* player_target;
        Cell* enemy_target;


        // Get node at pos from player board
        // We will use our board to store game state
        // of opponent board as we hit
        if( player_node != NULL ){
            player_target = &player_node -> cell;
        }else{
            init_cell( player_target, 0, UNKNOWN );
            insert_node( player_board -> qtree, init_qnode( pos, *player_target ) );
        }
            

        // Get node at pos from enemy board
        // Must handle enemy ships to count shots
        // and check liveness
        if( enemy_node != NULL ){
            enemy_target = &enemy_node -> cell;
        }else{
            init_cell( enemy_target, 0, UNKNOWN );
            insert_node( enemy_board -> qtree, init_qnode( pos, *enemy_target ) );
        }
  
        
        // Must play on cells that were not played before
        if( player_target -> state != UNKNOWN ){
            printf( "\nAlready played in (%hhu %hhu). Try again.\n", pos.x, pos.y );
            sleep(1);
            continue;
        }
        

    
        // Ship found at (x y). HIT!
        if( enemy_target -> ship > 0 ){

            byte ship_idx =  enemy_target -> ship;
            Ship* enemy_ship = enemy_board -> ships[ ship_idx ];
      
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


            
            // Use our board to store opponent board as we hit
            player_target -> state = HIT;
            
            print_board( player_board, true );
            printf( "\nHIT!\n" );
           

        }
        // Missed :( Swap player turn
        else{

            // No ship found in target cell
            player_target -> state = MISS;
            player1_turn = !player1_turn;

            print_board( player_board, true );
            printf( "\nMISS.\n" );
        }

  
        // Wait a bit
        sleep(1);
        
    }while( !game_finished );

    printf( "\n\nGame finished!\nThe winner is \"%s\".\n\nDo you want to play again?\n",
            player -> name );

    
    free_player( player1 );
    free_player( player2 );
    
    return;
}






void local_multiplayer(){

    char menu[] =
        "\n1 - Create new game\n" \
        "2 - Join existing game\n> ";              
                
    printf( "%s", menu );

    byte z;
    scanf( " %hhu", &z );


    int game_id;

    
    Player* player;
    char name[ MAX_LINE_SIZE ];
      
    printf( "\nEnter your nickname:\n> " );
    scanf( " %[^\n]s", name );
    player = init_player( name );

    if( z == 1 ){
        game_id = rand_int( 10000, 99999 );
        host_local_game( fd, name, game_id );
    }else{
        printf( "\nEnter ID of existing game:\n> " );
        scanf( " %d", &game_id );
        join_local_game( fd, name, game_id );
    }

    setup_player( player );

  
    printf( "\nWaiting for opponent to be ready..." );

    fflush( stdout );

    WRITE( "ready" );
    READ( buffer );

    printf( "\nOpponent is ready!\nStarting the game..." );

    /* TODO */
    
    //end_fifo( game_id );
    free_player( player );
    
    return;
}

