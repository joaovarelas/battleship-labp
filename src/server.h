#ifndef SERVER_H_
#define SERVER_H_


char in[ MAX_LINE_SIZE ]; // Host to Guest
char out[ MAX_LINE_SIZE ]; // Guest to Host

char buffer[ MAX_BUF_SIZE ];

char opponent_name[ MAX_LINE_SIZE ];

void host_local_game( int* fd, char* name, int game_id );
void join_local_game( int* fd, char* name, int game_id );

void wait_opponent();

void end_fifo( int game_id );
    
#endif
