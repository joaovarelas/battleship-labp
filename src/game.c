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
        // Parent and child must have different seed
        init_seed();
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

    // Exchange player nicknames
    WRITE( name );
    READ( buffer );
    
    strcpy( opponent_name, buffer );
  
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

    // Terminate child process
    if( pid == 0 )
        exit(0);
    
    sem_destroy( sem );
    
    return;
}



void play_online(){

    offline = false;
    
    char menu[] =
        "\n[1] Create new game\n" \
        "[2] Join existing game\n> ";              
                
    byte q;
    do{
        printf( "%s", menu );
        fgets( buffer, sizeof( buffer ), stdin );
    }while( sscanf( buffer, "%hhu", &q ) != 1 );

    do{
        printf( "\nEnter your nickname:\n> " );
        fgets( buffer, sizeof( buffer ), stdin );
    }while( sscanf( buffer, "%[^\n]s", name ) != 1 );

    
    // Join game or enter existing game (room)
    // using named pipes to synchronize both clients
    // or using tcp/ip sockets to communicate over network
    
    if( q == 1 ){
        
        network ? host_network_game() : host_local_game();
        send_settings();
        host = true;
        
    }else{
        
        network ? join_network_game() : join_local_game();
        receive_settings();
        host = false;
    }

    player = init_player( name ); 
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
    byte n = settings -> board_size;
    BoardType board_type = player -> board -> type;

    do {
        print_board( player -> board, true );
        
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

    Cell* target = NULL;
    QNode* node = NULL;
    
    if( board_type == QUADTREE ){
        
        node = get_node( player -> board -> qtree, pos );

        if( node != NULL ){
            target = &node -> cell;
        }
        
    }else{

        target = &player -> board -> matrix[ pos.x - 1 ][ pos.y - 1 ];
 
    }

    if( target != NULL && target -> state != UNKNOWN ){
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

    
    // Create node in our tree if it doesnt exist
    if( board_type == QUADTREE && node == NULL ){

        Cell new_cell;
        init_cell( &new_cell, 0, UNKNOWN );
	
        QNode* new_node = init_qnode( pos, new_cell );
        insert_node( player -> board -> qtree, new_node );

        node = get_node( player -> board -> qtree, pos );
        target = &node -> cell;

    }

    byte state = atoi( buffer );
    
    if( state == HIT ){
        
        target -> state = HIT;
        
        print_board( player -> board, true );
        printf( "\nHIT!\n" );
        fflush( stdout );
                  
    }else if( state == MISS ){

        target -> state = MISS;
        player1_turn = !player1_turn;
        
        print_board( player -> board, true );
        printf( "\nMISS.\n" );
        fflush( stdout );
        
    }else if( state == FINISH ){

        game_finished = true;
        target -> state = HIT;
        
        print_board( player -> board, true );
        printf( "\nGame has finished.\n" );
        fflush( stdout );
        
    }else{
        printf( "\nERROR: received unknown data send_shot(): \"%s\"\n", buffer );
        fflush( stdout );
    }

                
    return true;
}




bool receive_shot( Player* player ){

    BoardType board_type = player -> board -> type;
    
    if( !offline ) print_board( player -> board, false );
    char msg[] = "\nIt's \"%s\" turn.\nPlease wait...\n";
    if( !offline ) printf( msg, opponent_name );
    fflush( stdout );

    
    Pos pos;
    
    // Receive shot coords from enemy
    READ( buffer );
    fflush( stdout );
    sscanf( buffer, "%hhu %hhu", &pos.x, &pos.y );

    QNode* node;

    Cell* target;

    if( board_type == QUADTREE ){
        
        node = get_node( player -> board -> qtree, pos );
        
        if( node == NULL ){
        
            Cell new_cell;
            init_cell( &new_cell, 0, UNKNOWN );
            
            QNode* new_node = init_qnode( pos, new_cell );
            insert_node( player -> board -> qtree, new_node );

        }

        // Get leaf node from our tree
        node = get_node( player -> board -> qtree, pos );
        target = &node -> cell;
        
    }else{
        
        target = &player -> board -> matrix[ pos.x - 1 ][ pos.y - 1 ];
    }


    // Check if valid ship has been hit
    byte ship_idx = target -> ship;
    if( ship_idx > 0 && ship_idx <= MAX_SHIPS( player -> board -> size ) ){
      
        Ship* player_ship = player -> board -> ships[ ship_idx ];

	// Ship destroyed
        player_ship -> shot_count++;
        
        if( player_ship -> shot_count == player_ship -> size ){
            
            player_ship -> alive = false;
            player -> board -> ships_alive--;
            
        }

	// If no ships alive then finish game (and lose)
        if( player -> board -> ships_alive == 0 ){
            
            game_finished = true;
            target -> ship = HIT_SHIP; // Hack
            
            sprintf( buffer, "%hhu", FINISH );
            WRITE( buffer );
            
            if( !offline ) print_board( player -> board, false );
            if( !offline ) printf( "\nGame has finished.\n" );
            
            fflush( stdout );
            
            return true;
        }
            
        target -> ship = HIT_SHIP; // Hack
        
        sprintf( buffer, "%hhu", HIT );
        WRITE( buffer );
        
        if( !offline ) print_board( player -> board, false );
        if( !offline ) printf( "\nGot hit by enemy!\n" );
    
                    
    }else{
        
        target -> ship = MISS_SHIP; // Hack
        player1_turn = !player1_turn;
        
        sprintf( buffer, "%hhu", MISS );
        WRITE( buffer );
        
        if( !offline ) print_board( player -> board, false );
        if( !offline ) printf( "\nEnemy missed shot.\n" );

    }
    
    fflush( stdout );
	
    return true;
}
