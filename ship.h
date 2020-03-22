#ifndef __ship__
#define __ship__

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    
    unsigned int x, y, size;
    bool dir, alive;
    
} Ship;

Ship* init_ship( unsigned int x, unsigned int y, bool dir, unsigned int size );

#endif
