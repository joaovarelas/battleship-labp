#ifndef SERVER_H_
#define SERVER_H_

int fd[2];
int fd2[2];

char fifo1[ MAX_LINE_SIZE ];
char fifo2[ MAX_LINE_SIZE ];

char hostname[ MAX_LINE_SIZE ];
unsigned short port;

char opponent_name[ MAX_LINE_SIZE ];

void host_local_game();
void join_local_game();

void host_network_game();
void join_network_game();

void send_settings();
void receive_settings();

void wait_opponent();

int select_fd( int FILENO );

#endif
