#include "config.h"
#include "server.h"
#include "random.h"

void host_local_game( int* fd, char* name, int game_id ){
    
    sprintf( in,  PIPE_NAME_FMT, game_id, 0 );
    sprintf( out, PIPE_NAME_FMT, game_id, 1 );

    printf( "\nCreated new room ID: %d\nShare with your friend to join the game.\n", game_id );

    mkfifo( in,  0666 );
    mkfifo( out, 0666 );

    fd[0] = open( in,  O_CREAT | O_WRONLY ); // in
    fd[1] = open( out, O_CREAT | O_RDONLY ); // out

    READ( buffer );
    WRITE( name );

    printf( "\nPlayer \"%s\" joined! Starting game...\n", buffer );

    strcpy( opponent_name, buffer );
    
    return;
}


void join_local_game( int* fd, char* name, int game_id ){

    struct stat stat1, stat2;
    bool valid_id = false;
        
    do{
            
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
            return;
        }
            
    }while( !valid_id );

    WRITE( name );
    READ( buffer );
        
    printf( "\nJoined \"%s\" game successfully! (ID: %d)\n", buffer, game_id );
    strcpy( opponent_name, buffer );

    return;
}


void wait_opponent(){
    printf( "\nWaiting for opponent to be ready...\n" );

    fflush( stdout );

    WRITE( "ready" );
    READ( buffer );

    printf( "\nOpponent is ready!\nStarting the game...\n" );
    return;
}



void end_fifo( int game_id ){

    close( fd[0] );
    close( fd[1] );
     
    //printf( "\nDeleting FIFO \"%s\"\n", in );
    unlink( in );
    //printf( "\nDeleting FIFO \"%s\"\n", out );
    unlink( out );

    return;
}

