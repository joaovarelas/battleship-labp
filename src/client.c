#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "settings.h"


void ascii_art(){
    printf( "   _           _   _   _           _     _       \n" );
    printf( "  | |         | | | | | |         | |   (_)      \n" );
    printf( "  | |__   __ _| |_| |_| | ___  ___| |__  _ _ __  \n" );
    printf( "  | '_ \\ / _` | __| __| |/ _ \\/ __| '_ \\| | '_ \\ \n" );
    printf( "  | |_) | (_| | |_| |_| |  __/\\__ \\ | | | | |_) |\n" );
    printf( "  |_.__/ \\__,_|\\__|\\__|_|\\___||___/_| |_|_| .__/ \n" );
    printf( "                                          | |    \n" );
    printf( "                                          |_|    \n" );
    return;
}


int main( int argc, char** argv ){
    
    ascii_art();
    load_settings();
    
    char x;

    //do {
  
    printf( "\n1 - New game\n" );
    printf( "2 - Settings\n" );
    printf( "3 - Exit\n" );
    printf( "\n> " );
    scanf( " %c", &x );

    switch( x ){
        
    case '1':
        printf( "\n" );
        printf( "1 - Play By Turns\n" );
        printf( "2 - Local Multiplayer\n" );
        printf( "3 - Internet Multiplayer\n" );
        printf( "\n> " );

        scanf( " %c", &x );

        switch( x ){
                
        case '1':
            play_by_turns();
            break;
        case '2':
            //local_multiplayer();
            break;
        case '3':
            //internet_multiplayer();
            break;
        default:
            break;
            
        }
        break;
        
    case '2':
        change_settings();
        break;

    case '3':
        printf("\nAhoy, Captain!\n");
        exit(0);
        break;
        
    default:
        break;
    }
        
    // }while( true );

    
    return 0;
}
