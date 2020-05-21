#include "config.h"
#include "game.h"
#include "random.h"
#include "quadtree.h"
#include "server.h"

void play_offline(){

    offline = true;

    // Named semaphore shared between parent & child process (fork)
    sem = sem_open( SEM_NAME, O_CREAT | O_EXCL, 0644, 1 );
    sem_unlink( SEM_NAME );

    // Setup pipes for parent & child communication
    pipe( fd );
    pipe( fd2 );
    
    int pid = fork();

    // Parent host the game
    host = ( pid != 0 ); 

    if( pid != 0 ){
        close( fd[IN] );
        close( fd2[OUT] );
        
    }else{
        close( fd2[IN] );
        close( fd[OUT] );
    }


    // Lock
    sem_wait( sem );
    
    do{
        printf( "\nEnter your nickname:\n> " );
        fgets( buffer, sizeof( buffer ), stdin );
 
    }while( sscanf( buffer, "%[^\n]s", name ) != 1 );
    
    player = init_player( name );

    setup_player( player );

    // Print few empty lines to hide board
    byte i = 100;
    while( i-- > 0 )
        putchar('\n');

    // Unlock
    sem_post( sem );


    WRITE( name );
    READ( buffer );
    strcpy( opponent_name, buffer );

    wait_opponent();
    
    // Start the game
    start_game();
    
    // Game finished
    free_player( player );

    // Close pipes file descriptors
    if( host ){
        close( fd2[IN] );
        close( fd[OUT] );
    }else{
        close( fd[IN] );
        close( fd2[OUT] );
    }

    if( pid == 0 )
        exit(0);
    
    sem_destroy( sem );
    
    return;
}



void play_online(){

    offline = false;
    
    char menu[] =
        "\n1 - Create new game\n" \
        "2 - Join existing game\n> ";              
                
    byte q;
    do{
        printf( "%s", menu );
        fgets( buffer, sizeof( buffer ), stdin );
    }while( sscanf( buffer, "%hhu", &q ) != 1 );

    do{
        printf( "\nEnter your nickname:\n> " );
        fgets( buffer, sizeof( buffer ), stdin );
    }while( sscanf( buffer, "%[^\n]s", name ) != 1 );

    player = init_player( name ); 

    
    // Join game or enter existing game (room)
    // using named pipes to synchronize both clients
    //  or using tcp/ip sockets to communicate over network
    
    if( q == 1 ){
        
        network ? host_network_game() : host_local_game();
        send_settings();
        host = true;
        
    }else{
        
        network ? join_network_game() : join_local_game();
        receive_settings();
        host = false;
    }
    

    setup_player( player );

    wait_opponent();

    
    // Start the game
    start_game();

    
    // Game finished
    free_player( player );

    close( fd[OUT] );
    
    if( !network ) {
	close( fd[IN] );
	unlink( fifo1 );
	unlink( fifo2 );
    }
    
    return;
}


void start_game(){

    game_finished = false;

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

    // Do not print twice in offline mode (same terminal)
    if( !offline || ( offline && host ) ){
        printf( "\n\nGame finished!\nThe winner is \"%s\".\n\n",
                ( host ? ( player1_turn ? player -> name : opponent_name )
                  :      ( player1_turn ? opponent_name  : player -> name ) ) );
    }
    
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


    // Create node in our tree if it doesnt exist
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
        printf( "\nERROR: received unknown data: \"%s\"\n", buffer );
        fflush( stdout );
    }

                
    return true;
}




bool receive_shot( Player* player ){

    Board* player_board = player -> board;
    
    if( !offline ) print_board( player_board, false );
    char msg[] = "\nIt's \"%s\" turn.\nPlease wait...\n";
    if( !offline ) printf( msg, opponent_name );
    fflush( stdout );

    
    Pos pos;
    
    // Receive shot coords from enemy
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

    // Get leaf node from our tree
    player_node = get_node( player_board -> qtree, pos );
    player_target = &player_node -> cell;


    // Check if valid ship has been hit
    byte ship_idx =  player_target -> ship;
    if( ship_idx > 0 && ship_idx <= MAX_SHIPS( player_board -> size ) ){
      
        Ship* player_ship = player_board -> ships[ ship_idx ];

	// Ship destroyed
        player_ship -> shot_count++;
        if( player_ship -> shot_count == player_ship -> size ){
            
            player_ship -> alive = false;
            player_board -> ships_alive--;
            
        }

	// If no ships alive then finish game (and lose)
        if( player_board -> ships_alive == 0 ){
            
            game_finished = true;
            player_target -> ship = 99; // Hack
            
            sprintf( buffer, "%hhu", FINISH );
            WRITE( buffer );
            if( !offline ) print_board( player_board, false );
            if( !offline ) printf( "\nGame has finished.\n" );
            fflush( stdout );
            
            return true;
        }
            
        player_target -> ship = 99; // Hack
        
        sprintf( buffer, "%hhu", HIT );
        WRITE( buffer );
        if( !offline ) print_board( player_board, false );
        if( !offline ) printf( "\nGot hit by enemy!\n" );
    
                    
    }else{
        
        player_target -> ship = 66; // Hack
        player1_turn = !player1_turn;
        
        sprintf( buffer, "%hhu", MISS );
        WRITE( buffer );
        if( !offline ) print_board( player_board, false );
        if( !offline ) printf( "\nEnemy missed shot.\n" );

    }
    
    fflush( stdout );
	
    return true;
}
