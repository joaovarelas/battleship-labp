#include "config.h"
#include "server.h"
#include "random.h"
#include "game.h"

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

        inet_ntop( AF_INET, &addr.sin_addr, host, sizeof( host ) );
        
        if( bind( fd[0], (struct sockaddr *)&addr, sizeof( struct sockaddr ) ) == 0 )
            can_bind = true;
        else
            printf( "\nError binding name to socket... Try a different port.\n" );

    }while( !can_bind );
    
    listen( fd[0], 1 );

    printf( "\nWaiting for incoming connection... (%s:%hu)\n",
            host, port );
    
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
    
    do{
        printf( "\nEnter host to connect (IP address or hostname):\n> " );
        fflush( stdout );
        fgets( buffer, sizeof( buffer ), stdin );
    }while( sscanf( buffer, "%s", host ) != 1 );

    do{
        printf( "\nEnter port to connect:\n> " );
        fflush( stdout );
        fgets( buffer, sizeof( buffer ), stdin );
    }while( sscanf( buffer, "%hu", &port ) != 1 );

    struct hostent* entity;
    struct sockaddr_in remote_addr;

    entity = gethostbyname( host );
    fd[0] = socket( AF_INET, SOCK_STREAM, 0 );
    fd[1] = fd[0]; // Duplex socket

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons( port );
    remote_addr.sin_addr = *((struct in_addr *)entity -> h_addr );
   
    connect( fd[0], (struct sockaddr *)&remote_addr, sizeof(struct sockaddr) );

    READ( buffer );
    strcpy( opponent_name, buffer );
    
    printf( "\nConnected to \"%s\" game (%s:%d)\n", opponent_name, host, port );

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


void end_fifo(){
    //printf( "\nDeleting FIFO \"%s\"\n", in );
    unlink( in );
    //printf( "\nDeleting FIFO \"%s\"\n", out );
    unlink( out );
    return;
}


void close_fd(){
    close( fd[0] );
    close( fd[1] );
    return;
}

