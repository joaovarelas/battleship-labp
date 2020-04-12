#include "settings.h"
#include <stdio.h>


void load_settings(){

    settings = ( Settings* ) malloc ( sizeof( Settings ) );
    
    FILE* fp;
    fp = fopen(FILENAME, "r");
    if(!fp){
        printf("Settings file not found.\n");
        exit(1);
    }
  
    char line[MAX_LINE_SIZE];
    
    fgets( line, sizeof( line ), fp );
    settings -> board_size = atoi( line );

    assert( settings -> board_size >= 20 );
    assert( settings -> board_size <= 40 );
    
    fgets( line, sizeof( line ), fp );
    settings -> num_ships = atoi( line );

    assert( settings -> num_ships >= 1 );
    assert( settings -> num_ships <= MAX_SHIPS( settings -> board_size ) );
    
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



/*

// DRAW SHIPS FORMAT


Board* build_ship( ){

Board* tmp_board = init_board( MAX_SHIP_SIZE );
uchar pieces = 0;

// constant index. 1 ship only
uchar idx = 1;
    
printf( "\nEnter coordinates (x y) to place one piece (0 0 to finish):\n" );

Pos pos;
    
do{
print_board( tmp_board, false );

printf( "\nCoordinates (x y):\n> " );
scanf( " %hhd %hhd", &pos.x, &pos.y );

// TODO: Check adjacent pixels
        
if( pos.x > 0 && pos.y > 0 ){
tmp_board -> matrix[ pos.x - 1 ][ pos.y - 1 ].ship = idx;
pieces++;
}
        
}while( pos.x > 0 && pos.y > 0 );

tmp_board -> ships[ idx ] = init_ship( pos, pieces );

uchar z;
do {

print_board( tmp_board, false );

printf( "\nDo you want to rotate it? (clockwise):\n\n1 - Yes\n2 - No\n> " );

scanf( " %hhd", &z );

if( z == 1 ){
rotate_board( tmp_board );
}
        
} while( z == 1 );
    
return tmp_board;
}
*/
