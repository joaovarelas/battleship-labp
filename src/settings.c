#include "config.h"
#include "settings.h"
#include "ship.h"
#include "board.h"

void init_settings(){
    settings = ( Settings* ) malloc ( sizeof( Settings ) );
    return;
}

void load_settings(){
 
    FILE* fp;
    fp = fopen( FILENAME, "r" );
    if( !fp ){
        printf( "\nSettings file not found.\n" );
        exit(1);
    }
  
    char line[ MAX_LINE_SIZE ];
    
    fgets( line, sizeof( line ), fp );
    settings -> board_size = atoi( line );

    assert( settings -> board_size >= MIN_BOARD_SIZE );
    assert( settings -> board_size <= MAX_BOARD_SIZE );
    
    fgets( line, sizeof( line ), fp );
    settings -> num_ships = atoi( line );

    assert( settings -> num_ships >= MIN_SHIPS );
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
    
    FILE* fp;
    fp = fopen( FILENAME, "w" );
    if( !fp ){
        printf( "\nSettings file not found.\n" );
        exit(1);
    }

    fprintf( fp, "%hhd\n", settings -> board_size );
    fprintf( fp, "%hhd\n", settings -> num_ships );

    for( uchar i = 1; i <= settings -> num_ships; i++ ){

        uchar k = 0;
        while( k < MAX_SHIP_SQUARE )
            fprintf( fp, "%hhd", settings -> ship_shape[ i ][ k++ ] );
        
        fprintf( fp, "\n" );
    }

    fclose( fp );
    
    return;
}


void change_settings(){
    char msg[] =
        "\n\nCurrent configuration:\n" \
        "\nBoard size: %hhd" \
        "\nNumber of ships: %hhd\n";

    printf( msg, settings -> board_size, settings -> num_ships );

    for( uchar i = 1; i <= settings -> num_ships; i++ ){
        printf( "\nShip #%hhd\n", i );

        /*
          Board* tmp_ship = build_ship( i );
          print_board( tmp_ship, false );
          free_board( tmp_ship );
        */

        /* TODO: ask to show ship shapes */
        
    }
           
    printf( "\n\nDo you want to change settings?\n1 - Yes\n2 - No\n> ");
    
        
    uchar x;
    scanf(" %hhd", &x);
    
    switch( x ){
    case 1:
        {
            do{
                printf( "\nNew board size (between 20 and 40):\n> " );
                scanf( " %hhd", &settings -> board_size );
                
            }while( settings -> board_size < MIN_BOARD_SIZE
                    || settings -> board_size > MAX_BOARD_SIZE );

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
      
    printf( "\nEnter coordinates (x y) to place one piece:\n" );

    Pos pos;
    uchar pieces = 0;

    
    do{
        print_board( tmp_board, false );

        printf( "\nCoordinates (x y) to place one piece. (0 0) to finish:\n> " );
        scanf( " (%hhd %hhd)", &pos.x, &pos.y );

        if( pos.x < 0 || pos.x > MAX_SHIP_SIZE ||
            pos.y < 0 || pos.y > MAX_SHIP_SIZE ){
            printf( "\nInvalid coordinates. Try again.\n" );
            continue;
        }

        // TODO: Check adjacent pixel

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

        if( pieces < 1 ){
            printf( "\nShips must have at least 1 piece.\n" );
            continue;
        }
        
    }while( ( pos.x != 0 && pos.y != 0 )|| pieces < 1 );

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
