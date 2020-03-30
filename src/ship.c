#include "ship.h"

Ship* init_ship( Pos pos, unsigned char size ){
    
    Ship* ship = ( Ship* ) malloc ( sizeof( Ship ) );

    ship -> pos.x = pos.x; 
    ship -> pos.y = pos.y;
    ship -> size = size;
    ship -> shot_count = 0;
    ship -> alive = true;

    return ship;
}
