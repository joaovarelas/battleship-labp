#include "ship.h"

Ship* init_ship(){
    
    Ship* ship = ( Ship* ) malloc ( sizeof( Ship ) );

    ship -> pos.x = -1; 
    ship -> pos.y = -1;
    ship -> size = 0;
    ship -> shot_count = 0;
    ship -> alive = true;

    return ship;
}


void copy_ship( Ship* dst, Ship* src ){

    dst -> pos.x = src -> pos.x;
    dst -> pos.y = src -> pos.y;
    dst -> size = src -> size;
    dst -> shot_count = src -> shot_count;
    dst -> alive = src -> alive;
    
    return;
}
