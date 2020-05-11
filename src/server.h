#ifndef SERVER_H_
#define SERVER_H_


char in[ MAX_LINE_SIZE + 1 ]; // Host to Guest
char out[ MAX_LINE_SIZE + 1 ]; // Guest to Host

char buffer[ MAX_LINE_SIZE + 1 ];

void host_local_game( int* fd, char* name, int game_id );
void join_local_game( int* fd, char* name, int game_id );

void end_fifo( int game_id );
    
#endif
