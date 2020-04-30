#include "config.h"
#include "settings.h"
#include "game.h"
#include "random.h"

void ascii_art(){
    char logo[] =
        "   _           _   _   _           _     _       \n" \
        "  | |         | | | | | |         | |   (_)      \n" \
        "  | |__   __ _| |_| |_| | ___  ___| |__  _ _ __  \n" \
        "  | '_ \\ / _` | __| __| |/ _ \\/ __| '_ \\| | '_ \\ \n" \
        "  | |_) | (_| | |_| |_| |  __/\\__ \\ | | | | |_) |\n" \
        "  |_.__/ \\__,_|\\__|\\__|_|\\___||___/_| |_|_| .__/ \n" \
        "                                          | |    \n" \
        "                                          |_|    \n";
    printf( "\n%s", logo );
    return;
}


int main( int argc, char** argv ){
    
    init_settings();
    load_settings();

    init_seed();
    
    uchar x;

    do {

        ascii_art();

        char menu[] =
            "\n1 - New game\n" \
            "2 - Settings\n" \
            "3 - Exit\n> ";
        
        printf( "%s", menu );

        scanf( " %hhd", &x );

        switch( x ){
        
        case 1:
            {
                
                char menu[] =
                    "\n1 - Play By Turns\n" \
                    "2 - Local Multiplayer\n" \
                    "3 - Internet Multiplayer\n> ";              
                
                printf( "%s", menu );

                uchar z;
                scanf( " %hhd", &z );

                switch( z ){
                
                case 1:
                    play_by_turns();
                    break;
                case 2:
                    //local_multiplayer(); /* TODO */
                    break;
                case 3:
                    //internet_multiplayer(); /* TODO */
                    break;
                default:
                    break;
            
                }
                break;
            }
        case 2:
            change_settings();
            break;

        case 3:
            printf("\nAhoy, Captain!\n");
            exit(0);
            break;
        
        default:
            break;
        }
        
    }while( x != 3 );

    
    return 0;
}
