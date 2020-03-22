#include "ship.h"

Ship* init_ship( unsigned int x, unsigned int y, bool dir, unsigned int size ){
    Ship* ship = ( Ship* ) malloc ( sizeof ( Ship ) );

    ship -> x = x;
    ship -> y = y;
    ship -> size = size;
    ship -> dir = dir;
    ship -> alive = true;
    
    return ship;
}

