#include "config.h"
#include "settings.h"
#include "ship.h"
#include "board.h"
#include "server.h"

// Init global
void init_settings(){
    settings = ( Settings* ) malloc ( sizeof( Settings ) );
    return;
}


// Load from file
void load_settings(){
 
    FILE* fp;
    fp = fopen( SETTINGS_FILE, "r" );
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
    
    for( byte i = 1; i <= settings -> num_ships; i++ ){
        fgets( line, sizeof( line ), fp );

        byte j = 0;
        while( line[ j ] != '\0' ){
            
            if( line[ j ] == '1' )
                settings -> ship[ i ][ j ] = true;
            else
                settings -> ship[ i ][ j ] = false;
            
            j++;
        }
        
        settings -> ship[ i ][ MAX_SHIP_SQUARE ] = '\0';
    }

    return;
}


// Write to file
void write_settings(){
    
    FILE* fp;
    fp = fopen( SETTINGS_FILE, "w" );
    if( !fp ){
        printf( "\nSettings file not found.\n" );
        exit(1);
    }

    fprintf( fp, "%hhu\n", settings -> board_size );
    fprintf( fp, "%hhu\n", settings -> num_ships );

    for( byte i = 1; i <= settings -> num_ships; i++ ){

        byte k = 0;
        while( k < MAX_SHIP_SQUARE )
            fprintf( fp, "%hhu", settings -> ship[ i ][ k++ ] );
        
        fprintf( fp, "\n" );
    }

    fclose( fp );
    
    return;
}


// Change settings menu
void change_settings(){
    
    char msg[] =
        "\n\nCurrent configuration:\n" \
        "\nBoard size: %hhu" \
        "\nNumber of ships: %hhu\n" \
        "\nDo you want to view current ships format?" \
        "\n1 - Yes\n2 - No\n> ";

    byte q;
    do{
        printf( msg, settings -> board_size, settings -> num_ships );
        fgets( buffer, sizeof( buffer ), stdin );
    }while( sscanf( buffer, "%hhu\n", &q ) != 1 );

    if( q == 1 ){
        for( byte i = 1; i <= settings -> num_ships; i++ ){
            printf( "\nShip #%hhu\n", i );
        
            Board* tmp_ship = build_ship( i );
            print_board( tmp_ship, false );
            free_board( tmp_ship );    

        }
    }

    do{
        printf( "\n\nDo you want to change game settings?\n1 - Yes\n2 - No\n3 - Restore defaults\n> ");
        fgets( buffer, sizeof( buffer ), stdin );
    }while( sscanf( buffer, "%hhu", &q ) != 1 );
    
    switch( q ){
    case 1:
        {
            do{
                do{
                    printf( "\nNew board size (between 20 and 40):\n> " );
                    fgets( buffer, sizeof( buffer ), stdin );
                }while( sscanf( buffer, "%hhu", &settings -> board_size ) != 1 );
                
            }while( settings -> board_size < MIN_BOARD_SIZE
                    || settings -> board_size > MAX_BOARD_SIZE );
            

            do{
                
                do{
                    printf( "\nNew number of ships (between %hhu and %hhu):\n> ",
                            MIN_SHIPS,
                            MAX_SHIPS( settings -> board_size ) );
                    fgets( buffer, sizeof( buffer ), stdin );
                }while( sscanf( buffer, "%hhu", &settings -> num_ships ) != 1 );
                
            }while( settings -> num_ships < MIN_SHIPS
                    || settings -> num_ships > MAX_SHIPS( settings -> board_size ) );

            
            for( byte i = 1; i <= settings -> num_ships; i++ ){

                printf( "\nBuilding new ship #%hhu\n",  i );
                build_new_ship( i );
               
            }
            
            do{
                printf( "\nSave new settings to file?\n1 - Yes\n2 - No\n> " );
                fgets( buffer, sizeof( buffer ), stdin );
            }while( sscanf( buffer, "%hhu", &q ) != 1 );

            switch( q ){
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

    case 3:
        restore_default();
        break;
    default:
        break;
    }
    
    return;
}


// Copy contents of backup to settings file
void restore_default(){
    
    FILE* backup;
    FILE* cur;
    char ch;
    backup = fopen( SETTINGS_BACKUP, "r");
    cur = fopen( SETTINGS_FILE, "w");
    
    while( (ch = getc( backup )) != EOF )
        putc( ch, cur );
    
    fclose(backup);
    fclose(cur);

    // Reload
    load_settings();
    
    printf( "\nSettings file successfully restored!\n" );
    return;
}


// Draw new ship
void build_new_ship( byte idx ){

    Board* tmp_board;
      
    Pos pos;
    byte pieces = 0;

    bool valid_pos[ MAX_SHIP_SIZE ][ MAX_SHIP_SIZE ];
    memset( valid_pos, false, MAX_SHIP_SQUARE );

    bool confirm = false;
    do{
        tmp_board = init_board( MAX_SHIP_SIZE, MATRIX );
        
        do{
            print_board( tmp_board, false );

            do{
                printf( "\nCoordinates (x y) to place one piece. (0 0) to finish:\n> " );
                fgets( buffer, sizeof( buffer ), stdin );
            }while( sscanf( buffer, "%hhu %hhu", &pos.x, &pos.y ) != 2 );

            if( pos.x == 0 || pos.y == 0 )
                break;
        
            if( pos.x < 0 || pos.x > MAX_SHIP_SIZE ||
                pos.y < 0 || pos.y > MAX_SHIP_SIZE ){
                printf( "\nInvalid coordinates. Try again.\n" );
                continue;
            }

            Cell* cell = &tmp_board -> matrix[ pos.x - 1 ][ pos.y - 1 ];

            if( ( !valid_pos[ pos.x - 1 ][ pos.y - 1 ]
                  && pieces > 0 ) || ( cell -> ship == 1 ) ){
                printf( "\nInvalid position. Pieces must be adjacent to each other.\n" );
                continue;
            }
        

            // Valid adjacent pieces
            for( byte j = 0; j <= 2; j++ ){
                char i = j - 1;
                valid_pos[ pos.x - 1 + i ][ pos.y - 1 ] = true;
                valid_pos[ pos.x - 1 ][ pos.y - 1 + i ] = true;
            }


            // Place piece on ship matrix
            if( cell -> ship == 0 ){
                cell -> ship = 1;
                pieces++;
            }

            if( pieces < 1 ){
                printf( "\nShips must have at least 1 piece.\n" );
                continue;
            }
        
        }while( ( pos.x != 0 && pos.y != 0 ) || pieces < 1 );

    
        char msg[] =
            "\nDo you want to finish and proceed?" \
            " (Choosing \"No\" will reset the ship)\n\n" \
            "1 - Yes\n2 - No\n> ";
    
    
        byte x;
        do{
            printf( msg );
            fgets( buffer, sizeof( buffer ), stdin );
        }while( sscanf( buffer, "%hhu", &x ) != 1 );

        if( x == 2 ){
            free_board( tmp_board );
            confirm = false;
            continue;
        }
        
        confirm = true;
    
    }while( !confirm );

    byte k = 0;

    // Write to settings struct.
    for( byte x = 0; x < MAX_SHIP_SIZE; x++ ){
        for( byte y = 0; y < MAX_SHIP_SIZE; y++ ){

            if( tmp_board -> matrix[ x ][ y ].ship != 0 ){
                settings -> ship[ idx ][ k ] = true;
            }else{
                settings -> ship[ idx ][ k ] = false;
            }

            k++;
        }
    }
    
    return;
}
