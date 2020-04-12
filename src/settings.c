#include "settings.h"
#include <stdio.h>


void init_settings(){
    settings = ( Settings* ) malloc ( sizeof( Settings ) );
    return;
}

void load_settings(){
 
    FILE* fp;
    fp = fopen(FILENAME, "r");
    if(!fp){
        printf( "\nSettings file not found.\n" );
        exit(1);
    }
  
    char line[ MAX_LINE_SIZE ];
    
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
        
        settings -> ship_shape[ i ][ MAX_SHIP_SQUARE ] = '\0';
    }

    return;
}


void write_settings(){
    /* TODO */
    return;
}


void change_settings(){
    char msg[] =
        "\nCurrent configuration:\n" \
        "\nBoard size: %hhd" \
        "\nNumber of ships: %hhd" \
        "\n\nDo you want to change settings?" \
        "\n1 - Yes\n2 - No\n> ";

    /* TODO: print current ships */ 

    printf( msg, settings -> board_size, settings -> num_ships );
    
    uchar x;
    scanf(" %hhd", &x);

    //uchar new_board_size;
    //uchar new_num_ships;
    //bool new_ship_shape[ MAX_SHIPS( MAX_BOARD_SIZE ) + 1 ][ MAX_SHIP_SQUARE ];
    
    switch( x ){
    case 1:
        {
            // Change

            do{
                printf( "\nNew board size (between 20 and 40):\n> " );
                scanf( " %hhd", &settings -> board_size );
            }while( settings -> board_size < 20 || settings -> board_size > 40 );

            do{
                printf( "\nNew number of ships (between 1 and %hhd):\n> ",
                        MAX_SHIPS( settings -> board_size ) );
            
                scanf( " %hhd", &settings -> num_ships );
            }while( settings -> num_ships < 1
                    || settings -> num_ships > MAX_SHIPS( settings -> board_size ) );

            
            for( uchar i = 1; i <= settings -> num_ships; i++ ){

                printf( "\nBuilding new ship #%hhd\n",  i );
                build_new_ship( i );
               
            }
            
            printf( "\nSave new settings to file?\n1 - Yes\n2 - No\n> " );
            scanf( " %hhd", &x );

            switch( x ){
            case 1:

                write_settings();
                
                break;
            case 2:
                break;
            default:
                break;
            }
            
            
        }
        break;
    case 2:
        // Go back
        break;
    default:
        break;
    }
    
    return;
}




void build_new_ship( uchar idx ){

    Board* tmp_board = init_board( MAX_SHIP_SIZE );
    uchar pieces = 0;


    
    printf( "\nEnter coordinates (x y) to place one piece:\n" );

    Pos pos;
    
    do{
        print_board( tmp_board, false );

        printf( "\nCoordinates (x y) to place one piece. (0 0) to finish:\n> " );
        scanf( " (%hhd %hhd)", &pos.x, &pos.y );

        // TODO: Check adjacent pixels
        
        if( pos.x > 0 && pos.y > 0 ){
            
            Cell* cell = &tmp_board -> matrix[ pos.x - 1 ][ pos.y - 1 ];

            if( cell -> ship == 0 ){
                cell -> ship = 1;
                pieces++;
            }else{
                cell -> ship = 0;
                pieces--;
            }
        }
        
    }while( pos.x > 0 && pos.y > 0 );

    uchar k = 0;
    
    for( uchar x = 0; x < MAX_SHIP_SIZE; x++ ){
        for( uchar y = 0; y < MAX_SHIP_SIZE; y++ ){

            if( tmp_board -> matrix[ x ][ y ].ship != 0 ){
                settings -> ship_shape[ idx ][ k ] = true;
            }else{
                settings -> ship_shape[ idx ][ k ] = false;
            }

            k++;
        }
    }
    
    return;
}
