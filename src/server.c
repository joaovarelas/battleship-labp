#include "config.h"
#include "server.h"
#include "random.h"
#include "game.h"
#include "ship.h"
#include "board.h"

void host_local_game(){

    int game_id = rand_int( 10000, 99999 );
    
    sprintf( in,  PIPE_NAME_FMT, game_id, 0 );
    sprintf( out, PIPE_NAME_FMT, game_id, 1 );

    printf( "\nCreated new room ID: %d\nShare with your friend to join the game.\n", game_id );
    fflush( stdout );

    mkfifo( in,  0666 );
    mkfifo( out, 0666 );

    fd[0] = open( in,  O_CREAT | O_WRONLY ); // in
    fd[1] = open( out, O_CREAT | O_RDONLY ); // out

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
            
        sprintf( in, PIPE_NAME_FMT, game_id, 0 );
        sprintf( out, PIPE_NAME_FMT, game_id, 1 );

        stat( in, &stat1 );
        stat( out, &stat2 );
            
        if( S_ISFIFO( stat1.st_mode ) && S_ISFIFO( stat2.st_mode ) ){

            valid_id = true;
            fd[1] = open( in,  O_RDONLY ); // out
            fd[0] = open( out, O_WRONLY ); // in
                
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


        fd[0] = socket( AF_INET, SOCK_STREAM, 0 );

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons( port );

        inet_ntop( AF_INET, &addr.sin_addr, hostname, sizeof( hostname ) );
        
        if( bind( fd[0], (struct sockaddr *)&addr, sizeof( struct sockaddr ) ) == 0 )
            can_bind = true;
        else
            printf( "\nError binding name to socket... Try a different port.\n" );

    }while( !can_bind );
    
    listen( fd[0], 1 );

    printf( "\nWaiting for incoming connection... (%s:%hu)\n", hostname, port );
    
    sockin_size = sizeof( struct sockaddr_in );
        
    fd[0] = accept( fd[0], (struct sockaddr *)&remote_addr, &sockin_size );
    fd[1] = fd[0]; // Duplex socket
    
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
        fd[0] = socket( AF_INET, SOCK_STREAM, 0 );
        fd[1] = fd[0]; // Duplex socket
       
        remote_addr.sin_family = AF_INET;
        remote_addr.sin_port = htons( port );
        remote_addr.sin_addr = *((struct in_addr *)entity -> h_addr );
   
        int c = connect( fd[0], (struct sockaddr *)&remote_addr, sizeof(struct sockaddr) );

        if( entity == NULL || fd[0] == -1 || c == -1 )
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
    printf( "\nWaiting for opponent to be ready...\n" );

    fflush( stdout );

    WRITE( "ready" );
    READ( buffer );

    printf( "\nOpponent is ready!\nStarting the game...\n" );
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
