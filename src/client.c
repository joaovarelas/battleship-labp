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
   
   
    char menu[] =
	"\n1 - New game\n" \
	"2 - Settings\n" \
	"3 - Exit\n> ";
    

    byte q;

    do{

	ascii_art();
	
	do{
	    printf( menu );
	    fgets( buffer, sizeof( buffer ), stdin );
	}while( sscanf( buffer, "%hhu", &q ) != 1 );
        
	switch( q ){
        
	case 1:
	    {
                
		char menu[] =
		    "\n1 - Play by turns\n" \
		    "2 - Local multiplayer\n" \
		    "3 - Internet multiplayer\n> ";              
                
		byte qq;
	    
		do{
		    printf( menu );
		    fgets( buffer, sizeof( buffer ), stdin );
		}while( sscanf( buffer, "%hhu", &qq ) != 1 );


		switch( qq ){
                
		case 1:
		    play_offline();
		    break;
		case 2:
		    network = false;
		    play_online();
		    break;
		case 3:
		    network = true;
		    play_online();
		    break;
		default:
		    break;
            
		}
		q = 3;
		break;
	    }
	case 2:
	    change_settings();
	    break;

	default:
	    break;
	}
        
    }while( q != 3 );

    printf("\nAhoy, Captain!\n");
    free( settings );
    
    return 0;
}
