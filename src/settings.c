#include "settings.h"


void load_settings(){

    // Load default settings
    settings = ( Settings* ) malloc ( sizeof( Settings ) );
    settings -> board_size = DEFAULT_BOARD_SIZE;
    settings -> num_ships = DEFAULT_NUM_SHIPS; 
   
    return;
}


void change_settings(){
    char msg[] =
        "\nCurrent configuration:\n" \
        "\nBoard size: %hhd" \
        "\nNumber of ships: %hhd" \
        "\n\nDo you want to change settings?" \
        "\n1 - Yes\n2 - No\n> ";

    printf( msg, settings -> board_size, settings -> num_ships );
    
    uchar x;
    scanf(" %hhd", &x);

    switch( x ){
    case 1:
        // change
        break;
    case 2:
        // go back
        break;
    default:
        break;
    }
          
    return;
}
