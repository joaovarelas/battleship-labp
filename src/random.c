#include "config.h"
#include "random.h"

void init_seed(){
    srand( time( NULL ) );
}

bool rand_bool(){
    return (bool)( (rand() % 2) );
}

// Return random number in [min ; max]
byte rand_num( byte min, byte max ){
    assert( min >= 0 && max <= 255 );
    assert( min < max );
    return (rand() % ( max - min + 1 )) + min;
}


int rand_int( int min, int max ){
    assert( min < max );
    return (rand() % ( max - min + 1 )) + min;
}
