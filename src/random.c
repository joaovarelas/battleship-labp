#include "config.h"
#include "random.h"

void init_seed(){
    srand( time( NULL ) );
}

bool rand_bool(){
    return (bool)( (rand() % 2) );
}

// Return random number in [min ; max] (range 0 to 255)
uchar rand_num( uchar min, uchar max ){
    assert( min >= 0 && max <= 255 );
    assert( min < max );
    return (rand() % ( max - min + 1 )) + min;
}
