#include "settings.h"


void load_settings(){

    settings = ( Settings* ) malloc ( sizeof( Settings ) );
    settings -> board_size = 10;
    settings -> num_ships = 10;
    settings -> size_ships = ( int* ) malloc ( 10 * sizeof( int ) );

    int tmp[ 10 ] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

    for( int i = 0 ; i < settings -> num_ships ; i++)
        settings -> size_ships[ i ] = tmp[ i ];
    
    return;
}


void change_settings(){
    printf("\nCurrent configuration:\n");
    printf("\nBoard size: %d", settings -> board_size );
    printf("\nNumber of ships: %d", settings -> num_ships );

    printf("\nSize of ships:");
    for( int i = 0; i < settings -> num_ships; i++){
        
        printf("\n  Ship #%d: %d", i + 1, settings -> size_ships[i] );
        
    }

    printf("\n\nDo you want to change this settings?");
    printf("\n1 - Yes\n2 - No\n");
    printf("> ");
    int x;
    scanf("%d", &x);

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
