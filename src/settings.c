#include "settings.h"
#include <stdio.h>


void load_settings(){

    settings = ( Settings* ) malloc ( sizeof( Settings ) );
    
    FILE* fp;
    fp = fopen("settings", "r");
    if(!fp){
        printf("Settings file not found.\n");
        exit(1);
    }
  
    char line[MAX_LINE_SIZE];
    
    fgets( line, sizeof( line ), fp );
    settings -> board_size = atoi( line );
    
    fgets( line, sizeof( line ), fp );
    settings -> num_ships = atoi( line );

    for( uchar i = 1; i <= settings -> num_ships; i++ ){
        fgets( line, sizeof( line ), fp );

        uchar j = 0;
        while( line[ j ] != '\0' ){
            
            if( line[ j ] == '1' )
                settings -> ship_shape[ i ][ j ] = true;
            else
                settings -> ship_shape[ i ][ j ] = false;
            
            j++;
        }
        
        settings -> ship_shape[ i ][ 25 ] = '\0';
    }

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
        // Change
        break;
    case 2:
        // Go back
        break;
    default:
        break;
    }
          
    return;
}
