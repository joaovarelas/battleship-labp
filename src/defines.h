#ifndef __defines__
#define __defines__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>


#define MIN_BOARD_SIZE 20
#define MAX_BOARD_SIZE 40

#define MAX_SHIP_SIZE 5
#define MAX_SHIP_SQUARE 25
#define MIN_SHIPS 1
#define MAX_SHIPS( N ) ( N * N  / MAX_SHIP_SQUARE )

#define MAX_LINE_SIZE 32
#define FILENAME "settings/settings"

typedef unsigned char uchar;

#endif
