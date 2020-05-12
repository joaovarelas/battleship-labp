#ifndef SERVER_H_
#define SERVER_H_

int fd[2];
char in[ MAX_LINE_SIZE ]; // Host to Guest
char out[ MAX_LINE_SIZE ]; // Guest to Host

char host[ MAX_LINE_SIZE ];
unsigned short port;

char opponent_name[ MAX_LINE_SIZE ];

void host_local_game();
void join_local_game();

void host_network_game();
void join_network_game();

void wait_opponent();

void close_fd();
void end_fifo();
    
#endif
