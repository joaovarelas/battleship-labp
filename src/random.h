#ifndef RANDOM_H
#define RANDOM_H

void init_seed();
bool rand_bool();
byte rand_num( byte min, byte max );
int rand_int( int min, int max );

#endif
