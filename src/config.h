#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>




// Board definitions
#define MIN_BOARD_SIZE 20
#define MAX_BOARD_SIZE 40

#define MAX_SHIP_SIZE 5
#define MAX_SHIP_SQUARE 25
#define MIN_SHIPS 5
#define MAX_SHIPS( N ) ( N * N  / MAX_SHIP_SQUARE )

#define BOARD_SPAN 2
#define MAX_LINE_SIZE 32



// Board movement definitions
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define ROTATE 5

#define VERTICAL( D )   ( D == UP   || D == DOWN )
#define HORIZONTAL( D ) ( D == LEFT || D == RIGHT )



// Settings definitions
#define SETTINGS_FILE "settings/settings"
#define SETTINGS_BACKUP "settings/settings_default"


// Named Pipes
#define MAX_BUF_SIZE 1024
#define PIPE_NAME_FMT "/tmp/battleship-%d-%d"

#define WRITE( BUF )  ( write( fd[0], BUF, MAX_BUF_SIZE ) )
#define READ( BUF )   ( read(  fd[1], BUF, MAX_BUF_SIZE ) )

    

    
// QuadTree definitions
#define MAX_NODES 1600 + 1
#define VALID_RANGE( P, P1, P2 ) ( P.x >= P1.x && P.x <= P2.x && P.y >= P1.y && P.y <= P2.y )
#define IN_RANGE( P1, P2 ) ( P2.x - P1.x  <= 1 && P2.y - P1.y <= 1 )

#define HALFX( P1, P2 ) ( ( P1.x + P2.x ) / 2 )
#define HALFY( P1, P2 ) ( ( P1.y + P2.y ) / 2 )

#define NORTHSIDE( P, P1, P2 ) ( P.y <= HALFY( P1, P2 ) )
#define SOUTHSIDE( P, P1, P2 ) ( P.y  > HALFY( P1, P2 ) )
#define WESTSIDE( P, P1, P2 )  ( P.x <= HALFX( P1, P2 ) )
#define EASTSIDE( P, P1, P2 )  ( P.x  > HALFX( P1, P2 ) )

#define NORTHWEST( P, P1, P2 ) ( NORTHSIDE( P, P1, P2 ) && WESTSIDE( P, P1, P2 ) )
#define NORTHEAST( P, P1, P2 ) ( NORTHSIDE( P, P1, P2 ) && EASTSIDE( P, P1, P2 ) )
#define SOUTHWEST( P, P1, P2 ) ( SOUTHSIDE( P, P1, P2 ) && WESTSIDE( P, P1, P2 ) )
#define SOUTHEAST( P, P1, P2 ) ( SOUTHSIDE( P, P1, P2 ) && EASTSIDE( P, P1, P2 ) )



// Forward declarations
// Prevent circular dependency
typedef struct _Player Player;
typedef struct _Settings Settings;
typedef struct _Pos Pos;
typedef struct _Ship Ship;
typedef struct _Cell Cell;
typedef struct _Board Board;
typedef struct _QTree QTree;
typedef struct _QNode QNode;
typedef struct _QBranch QBranch;



// Byte (0 - 255)
typedef unsigned char byte;


// General purpose buffer
char buffer[ MAX_BUF_SIZE ];


#endif
