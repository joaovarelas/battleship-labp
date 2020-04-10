#ifndef __defines__
#define __defines__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#define MIN_BOARD_SIZE 20
#define MAX_BOARD_SIZE 40

#define DEFAULT_BOARD_SIZE 10
#define DEFAULT_NUM_SHIPS 1

#define MAX_SHIP_SIZE 5
#define MAX_PLAYER_NAME 16
#define MAX_SHIPS( N ) ( N * N  / 25 )


typedef unsigned char uchar;

#endif
