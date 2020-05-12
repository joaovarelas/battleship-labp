#include "config.h"
#include "game.h"
#include "random.h"
#include "quadtree.h"
#include "server.h"

void play_by_turns(){

    do{
        printf( "\nSetting up Player #1\nPlayer Name:\n> " );
        fgets( buffer, sizeof( buffer ), stdin );
    }while( sscanf( buffer, "%[^\n]s", name ) != 1 );
    
    Player* player1 = init_player ( name );
    setup_player( player1 );

    

    do{
        printf( "\nSetting up Player #2\nPlayer Name:\n> " );
        fgets( buffer, sizeof( buffer ), stdin );
    }while( sscanf( buffer, "%[^\n]s", name ) != 1 );
    
    Player* player2 = init_player ( name );
    setup_player( player2 );

    
  
    byte n = settings -> board_size;

    // Select random player to start
    player1_turn = rand_bool();
    game_finished = false;
    char msg[] = "\n\nThe War Begins!\n\nPlayer \"%s\" starts the game with the first play.\n";

    // Notice who starts
    if( player1_turn ){
        printf( msg, player1 -> name );
    } else {
        printf( msg, player2 -> name );
    }


    Player* player;
    Player* enemy;
    
    do{

        Pos pos;
                    
        do{
           
            char msg[] = "\nIt's \"%s\" turn.\nCoordinates x y to fire:\n> ";

            player = player1_turn ? player1 : player2;
            enemy =  player1_turn ? player2 : player1;
        
            print_board( player -> board, true );
            printf( msg, player -> name );
                 
            fgets( buffer, sizeof( buffer ), stdin );
        }while( sscanf( buffer, "%hhu %hhu", &pos.x, &pos.y ) != 2 );

        // Check border limits
        if( ( pos.x < 1 || pos.x > n ) || ( pos.y < 1 || pos.y > n ) ){
            printf( "\nInvalid coordinates x y. Try again.\n" );
            continue;
        }    


        // Set enemy_board depending on current player turn
        Board* player_board = player -> board;
        Board* enemy_board =  enemy -> board;
        
        // Set target cell considering (x y)
        QNode* player_node = get_node( player_board -> qtree, pos );
        QNode* enemy_node =  get_node( enemy_board -> qtree, pos );

        Cell* player_target;

        // Get node at pos from player board
        // We will use our board to store game state
        // of opponent board as we hit
        if( player_node == NULL ){
            
            Cell new_cell;
            init_cell( &new_cell, 0, UNKNOWN );
            
            QNode* new_node = init_qnode( pos, new_cell );
            insert_node( player_board -> qtree, new_node );

        }


        // Go down the tree to find node in leaves
        player_node = get_node( player_board -> qtree, pos );
        player_target = &player_node -> cell;
              
        
        // Must play on cells that were not played before
        if( player_target -> state != UNKNOWN ){
            printf( "\nAlready played in %hhu %hhu. Try again.\n", pos.x, pos.y );
            sleep(2);
            continue;
        }
        

        // Ship found at (x y). HIT!
        if( enemy_node != NULL ){

            byte ship_idx =  enemy_node -> cell.ship;
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
        sleep(2);
        
    }while( !game_finished );

    printf( "\n\nGame finished!\nThe winner is \"%s\".\n\nDo you want to play again?\n",
            player -> name );

    
    free_player( player1 );
    free_player( player2 );
    
    return;
}



void multiplayer( bool network ){

    char menu[] =
        "\n1 - Create new game\n" \
        "2 - Join existing game\n> ";              
                
    byte q;
    do{
        printf( "%s", menu );
        fgets( buffer, sizeof( buffer ), stdin );
    }while( sscanf( buffer, "%hhu", &q ) != 1 );

    bool host;

    do{
        printf( "\nEnter your nickname:\n> " );
        fgets( buffer, sizeof( buffer ), stdin );
    }while( sscanf( buffer, "%[^\n]s", name ) != 1 );
    
    player = init_player( name );

    // Join game or enter existing game (room)
    // Using named pipes to synchronize both clients
    if( q == 1 ){
        
        if( network ) host_network_game(); 
        else host_local_game();
        
        host = true;
        
    }else{
        
        if( network ) join_network_game();
        else join_local_game();
        
        host = false;
    }

    setup_player( player );

    wait_opponent();
    
    game_finished = false;

    /*
      Start the game
    */
    start_multiplayer_game( host );

    
    printf( "\n\nGame finished!\nThe winner is \"%s\".\n\nDo you want to play again?\n",
            ( host ? ( player1_turn ? player -> name : opponent_name )
              : ( player1_turn ? opponent_name : player -> name ) ) );


    close_fd();
    end_fifo();
    free_player( player );
    
    return;
}


void start_multiplayer_game( bool host ){

    if( host ){
        
        // Decide who starts and send
        player1_turn = rand_bool();
        sprintf( buffer, "%d", player1_turn );
        WRITE( buffer );
    
    }else{

        // Receive who starts from host
        READ( buffer );
        player1_turn = ( atoi( buffer ) == true ); 

    }

    do{
            
        if( player1_turn ){

            if( host ){
                if( !send_shot( player ) )
                    continue;
            }else{
                receive_shot( player );}
                
        }else{
            if( host ){
                receive_shot( player );
            }else{
                if( !send_shot( player ) )
                    continue;
            }
        }

        sleep( 2 );

    }while( !game_finished );
    
    return;
}



bool send_shot( Player* player ){
    
    Pos pos;
    Board* player_board = player -> board;
    byte n = settings -> board_size;

    do {
        print_board( player_board, true );
        
        char msg[] = "\nIt's \"%s\" turn.\nCoordinates x y to fire:\n> ";
        printf( msg, player -> name );
        fflush( stdout );
        
        fgets( buffer, sizeof( buffer ), stdin );
    }while( sscanf( buffer, "%hhu %hhu", &pos.x, &pos.y ) != 2 );
                
    // Check border limits
    if( ( pos.x < 1 || pos.x > n ) || ( pos.y < 1 || pos.y > n ) ){
        printf( "\nInvalid coordinates x y. Try again.\n" );
        fflush( stdout );
        sleep(2);
        return false;
    }

    QNode* node = get_node( player -> board -> qtree, pos );

    if( node != NULL && node -> cell.state != UNKNOWN ){
        printf( "\nAlready played in %hhu %hhu. Try again.\n", pos.x, pos.y );
        fflush( stdout );
        sleep(2);
        return false;
    }

                
    // Send shot coords
    sprintf( buffer, "%hhu %hhu", pos.x, pos.y );
    fflush( stdout );
    
    WRITE( buffer );

    // Receive enemy feedback
    READ( buffer );
    fflush( stdout );



    // Set target cell considering (x y)
    QNode* player_node = get_node( player_board -> qtree, pos );

    Cell* player_target;


    if( player_node == NULL ){

        Cell new_cell;
        init_cell( &new_cell, 0, UNKNOWN );
        QNode* new_node = init_qnode( pos, new_cell );
        insert_node( player_board -> qtree, new_node );

    }

    player_node = get_node( player_board -> qtree, pos );
    player_target = &player_node -> cell;


    byte state = atoi( buffer );
    
    if( state == HIT ){
        
        player_target -> state = HIT;
        
        print_board( player_board, true );
        printf( "\nHIT!\n" );
        fflush( stdout );
                  
    }else if( state == MISS ){

        player_target -> state = MISS;
        player1_turn = !player1_turn;
        
        print_board( player_board, true );
        printf( "\nMISS.\n" );
        fflush( stdout );
        
    }else if( state == FINISH ){

        game_finished = true;
        player_target -> state = HIT;
        
        print_board( player_board, true );
        printf( "\nGame has finished.\n" );
        fflush( stdout );
        
    }else{
        printf( "\nUNKNOWN: \"%s\"\n", buffer );
        fflush( stdout );
    }

                
    return true;
}




bool receive_shot( Player* player ){

    Board* player_board = player -> board;
    
    print_board( player_board, false );
    char msg[] = "\nIt's \"%s\" turn.\nPlease wait...\n";
    printf( msg, opponent_name );
    fflush( stdout );

    // Receive shot coords from enemy
    Pos pos;

    READ( buffer );
    fflush( stdout );
    sscanf( buffer, "%hhu %hhu", &pos.x, &pos.y );


    // Set target cell considering (x y)
    QNode* player_node = get_node( player_board -> qtree, pos );

    Cell* player_target;

    
    if( player_node == NULL ){
        
        Cell new_cell;
        init_cell( &new_cell, 0, UNKNOWN );
        
        QNode* new_node = init_qnode( pos, new_cell );
        insert_node( player_board -> qtree, new_node );
        
    }

    player_node = get_node( player_board -> qtree, pos );
    player_target = &player_node -> cell;

    if( player_target -> ship > 0 ){

        byte ship_idx =  player_target -> ship;
        Ship* player_ship = player_board -> ships[ ship_idx ];

        player_ship -> shot_count++;
        if( player_ship -> shot_count == player_ship -> size ){
            
            player_ship -> alive = false;
            player_board -> ships_alive--;
            
        }
                    
        if( player_board -> ships_alive == 0 ){
            
            game_finished = true;
            player_target -> ship = 99; // Hack
            
            sprintf( buffer, "%hhu", FINISH );
            WRITE( buffer );
            print_board( player_board, false );
            printf( "\nGame has finished.\n" );
            fflush( stdout );
            
            return true;
        }
            
        player_target -> ship = 99; // Hack
        
        sprintf( buffer, "%hhu", HIT );
        WRITE( buffer );
        print_board( player_board, false );
        printf( "\nGot hit by enemy!\n" );
        fflush( stdout );
                    
    }else{
        
        player_target -> ship = 66;
        player1_turn = !player1_turn;
        
        sprintf( buffer, "%hhu", MISS );
        WRITE( buffer );
        print_board( player_board, false );
        printf( "\nEnemy missed shot.\n" );
        fflush( stdout );

    }
    
    return true;
}
