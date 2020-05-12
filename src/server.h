#ifndef SERVER_H_
#define SERVER_H_

int fd[2];
char in[ MAX_LINE_SIZE ]; // Host to Guest
char out[ MAX_LINE_SIZE ]; // Guest to Host

char opponent_name[ MAX_LINE_SIZE ];

void host_local_game( int* fd, char* name );
void join_local_game( int* fd, char* name );

void wait_opponent();

void end_fifo();
    
#endif
