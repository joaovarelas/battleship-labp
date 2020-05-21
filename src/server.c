#include "config.h"
#include "server.h"
#include "random.h"
#include "game.h"
#include "ship.h"
#include "board.h"

void host_local_game(){

    int game_id = rand_int( 10000, 99999 );
    
    sprintf( fifo1, PIPE_NAME_FMT, game_id, 1 );
    sprintf( fifo2, PIPE_NAME_FMT, game_id, 2 );

    printf( "\nCreated new room ID: %d\nShare with your friend to join the game.\n", game_id );
    fflush( stdout );

    mkfifo( fifo1, 0666 );
    mkfifo( fifo2, 0666 );

    fd[IN]  = open( fifo1, O_CREAT | O_RDWR ); // Read from fifo1
    fd[OUT] = open( fifo2, O_CREAT | O_RDWR ); // Write to fifo2

    READ( buffer );
    WRITE( name );

    printf( "\nPlayer \"%s\" joined! Starting game...\n", buffer );
    fflush( stdout );

    strcpy( opponent_name, buffer );

    sleep(2);
    
    return;
}


void join_local_game(){

    struct stat stat1, stat2;
    bool valid_id = false;

    int game_id;
        
    do{

        do{
            printf( "\nEnter ID of existing room:\n> " );
            fflush( stdout );
            fgets( buffer, sizeof( buffer ), stdin );
        }while( sscanf( buffer, "%d", &game_id ) != 1 );
            
        sprintf( fifo1, PIPE_NAME_FMT, game_id, 1 );
        sprintf( fifo2, PIPE_NAME_FMT, game_id, 2 );

        stat( fifo1, &stat1 );
        stat( fifo2, &stat2 );
            
        if( S_ISFIFO( stat1.st_mode ) && S_ISFIFO( stat2.st_mode ) ){

            valid_id = true;
            fd[IN] =  open( fifo2, O_RDWR ); // Read from fifo2
            fd[OUT] = open( fifo1, O_RDWR ); // Write to fifo1
                
        }else{
            printf( "\nInvalid game ID.\n" );
            fflush( stdout );
            return;
        }
            
    }while( !valid_id );

    WRITE( name );
    READ( buffer );
        
    printf( "\nJoined \"%s\" game successfully! (ID: %d)\n", buffer, game_id );
    fflush( stdout );
    
    strcpy( opponent_name, buffer );
    sleep(2);
    
    return;
}


void host_network_game(){

    bool can_bind = false;

        
    struct sockaddr_in addr;
    struct sockaddr_in remote_addr;
    socklen_t sockin_size;
        
    do{
        do{
            printf( "\nEnter port to bind:\n> " );
            fflush( stdout );
            fgets( buffer, sizeof( buffer ), stdin );
        }while( sscanf( buffer, "%hu", &port ) != 1 );


        fd[IN] = socket( AF_INET, SOCK_STREAM, 0 );

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons( port );

        inet_ntop( AF_INET, &addr.sin_addr, hostname, sizeof( hostname ) );
        
        if( bind( fd[IN], (struct sockaddr *)&addr, sizeof( struct sockaddr ) ) == 0 )
            can_bind = true;
        else
            printf( "\nError binding name to socket... Try a different port.\n" );

    }while( !can_bind );
    
    listen( fd[IN], 1 );

    printf( "\nWaiting for incoming connection... (%s:%hu)\n", hostname, port );
    
    sockin_size = sizeof( struct sockaddr_in );
        
    fd[IN] = accept( fd[IN], (struct sockaddr *)&remote_addr, &sockin_size );
    fd[OUT] = fd[IN]; // Full duplex socket
    
    WRITE( name );
    
    READ( buffer );
    
    strcpy( opponent_name, buffer );
    
    printf( "\nReceived connection from \"%s\" (%s)\n",
            opponent_name, inet_ntoa( remote_addr.sin_addr ) );
    
    sleep(2);
     
    return;
}



void join_network_game(){

    struct hostent* entity;
    struct sockaddr_in remote_addr;

    bool connected = false;
    
    do{
        
        do{
            printf( "\nEnter host to connect (IP address or hostname):\n> " );
            fflush( stdout );
            fgets( buffer, sizeof( buffer ), stdin );
        }while( sscanf( buffer, "%s", hostname ) != 1 );

        do{
            printf( "\nEnter port to connect:\n> " );
            fflush( stdout );
            fgets( buffer, sizeof( buffer ), stdin );
        }while( sscanf( buffer, "%hu", &port ) != 1 );

 
        entity = gethostbyname( hostname );
        fd[IN] = socket( AF_INET, SOCK_STREAM, 0 );
        fd[OUT] = fd[IN]; // Full duplex socket
       
        remote_addr.sin_family = AF_INET;
        remote_addr.sin_port = htons( port );
        remote_addr.sin_addr = *((struct in_addr *)entity -> h_addr_list[0] );
   
        int c = connect( fd[OUT], (struct sockaddr *)&remote_addr, sizeof(struct sockaddr) );

        if( entity == NULL || fd[OUT] == -1 || c == -1 )
            printf( "\nError connecting to server... Try again.\n" );
        else
            connected = true;

    }while( !connected );

    
    READ( buffer );
    strcpy( opponent_name, buffer );
    
    printf( "\nConnected to \"%s\" game (%s:%d)\n", opponent_name, hostname, port );

    WRITE( name );
    
    sleep(2);
    
    return;
}

void wait_opponent(){
    if( !offline ) printf( "\nWaiting for opponent to be ready...\n" );

    fflush( stdout );

    WRITE( "ready" );
    READ( buffer );

    if( !offline ) printf( "\nOpponent is ready!\nStarting the game...\n" );
    fflush( stdout );
    return;
}

void send_settings(){
    printf( "\nSending settings...\n" );

    sprintf( buffer, "%hhu", settings -> board_size );
    WRITE( buffer );

    printf( "\nSent board size! (%s)\n", buffer );
 
    
    sprintf( buffer, "%hhu", settings -> num_ships );
    WRITE( buffer );

    printf( "\nSent number of ships! (%s)\n", buffer );

    char ship_str[ MAX_SHIP_SQUARE + 1 ];
    for( byte k = 1; k <= settings -> num_ships; k++ ){
        
        for( byte i = 0; i < MAX_SHIP_SQUARE; i++ )
            ship_str[ i ] = ( settings -> ship[ k ][ i ] ? '1' : '0' );

        ship_str[ MAX_SHIP_SQUARE ] = '\0';
        
        sprintf( buffer, "%s", ship_str );
        WRITE( buffer );

        printf( "\nSent ship #%hhu format! (%s)\n", k, buffer );
    }
    
    return;
}


void receive_settings(){

    printf( "\nReceiving settings...\n" );

    READ( buffer );
    settings -> board_size = (byte)atoi( buffer );

    printf( "\nReceived board size! (%s)\n", buffer );
        
    READ( buffer );
    settings -> num_ships = (byte)atoi( buffer );

    printf( "\nReceived number of ships! (%s)\n", buffer );

    for( int k = 1; k <= settings -> num_ships; k++ ){
        
        READ( buffer );
        
        for( int i = 0; i < MAX_SHIP_SQUARE; i++ )
            settings -> ship[ k ][ i ] = ( buffer[ i ] == '1' ? true : false );

        printf( "\nReceived ship #%hhu format! (%s)\n", k, buffer );
        
        Board* tmp_board = build_ship( k );
        print_board( tmp_board, false );
        free_board( tmp_board );
        
    }

    
    return;
}



int select_fd( int FILENO ){

    int _fd;

    if( FILENO == IN ){

        _fd = ( offline && host ) ?  fd2[IN]  : fd[IN];
        
    }else if( FILENO == OUT ){

        _fd = ( offline && !host ) ? fd2[OUT] : fd[OUT];
         
    }else{

        printf( "\nDEBUG: Unknown select_fd()\n" );
        _fd = ERR;
    }

    return _fd;
    
}
