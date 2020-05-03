#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct _Settings {
    
    byte board_size;
    byte num_ships;
    bool ship_shape[ MAX_SHIPS( MAX_BOARD_SIZE ) + 1 ][ MAX_SHIP_SQUARE ];
    
} Settings;


// Global
Settings* settings;

void init_settings();

void load_settings();
void write_settings();
void change_settings();
void restore_default();

void build_new_ship( byte idx );

#endif
