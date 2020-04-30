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
#define SETTINGS_FILE "settings/settings"
#define SETTINGS_BACKUP "settings/settings_default"

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define ROTATE 5

#define VERTICAL( D ) ( D == UP || D == DOWN )
#define HORIZONTAL( D ) ( D == LEFT || D == RIGHT )




// Forward declarations
// Prevent circular dependency
typedef struct _Player Player;
typedef struct _Settings Settings;
typedef struct _Pos Pos;
typedef struct _Ship Ship;
typedef struct _Cell Cell;
typedef struct _Board Board;

// Byte (0 - 255)
typedef unsigned char uchar;


#endif
