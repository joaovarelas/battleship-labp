#ifndef CONFIG_H
#define CONFIG_H

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
#define MIN_SHIPS 5
#define MAX_SHIPS( N ) ( N * N  / MAX_SHIP_SQUARE )

#define BOARD_SPAN 2
#define MAX_LINE_SIZE 32
#define FILENAME "settings/settings"

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define ROTATE 5




// Forward declarations
typedef struct _Player Player;
typedef struct _Settings Settings;
typedef struct _Pos Pos;
typedef struct _Ship Ship;
typedef struct _Cell Cell;
typedef struct _Board Board;

typedef unsigned char uchar;

#endif
