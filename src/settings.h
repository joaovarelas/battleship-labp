#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct _Settings {
    
    uchar board_size;
    uchar num_ships;
    bool ship_shape[ MAX_SHIPS( MAX_BOARD_SIZE ) + 1 ][ MAX_SHIP_SQUARE ];
    
} Settings;


// Global
Settings* settings;

void init_settings();
void load_settings();
void write_settings();
void change_settings();

void build_new_ship( uchar idx );

#endif
