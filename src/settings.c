#include "settings.h"


void load_settings(){

    // default settings
    settings = ( Settings* ) malloc ( sizeof( Settings ) );
    settings -> board_size = 20;
    settings -> num_ships = 3; // testing, change later
   
    return;
}


void change_settings(){
    
    printf("\nCurrent configuration:\n");
    printf("\nBoard size: %d", settings -> board_size );
    printf("\nNumber of ships: %d", settings -> num_ships );

    printf("\n\nDo you want to change this settings?");
    printf("\n1 - Yes\n2 - No\n");
    printf("> ");

    char x;
    scanf(" %c", &x);

    switch( x ){
    case '1':
        // change
        break;
    case '2':
        // go back
        break;
    default:
        break;
    }
          
    return;
}
