#ifndef GAME_H
#define GAME_H

#include "player.h"

Player* player;
char name[ MAX_LINE_SIZE ];
bool player1_turn;
bool game_finished;
bool host;
bool offline;
bool network;

void play_offline();
void play_online();

void start_game();

bool send_shot( Player* player );
bool receive_shot( Player* player );

#endif
