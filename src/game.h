#ifndef GAME_H
#define GAME_H

#include "player.h"


bool player1_turn;
bool game_finished;

void play_by_turns();
void local_multiplayer();

void start_host_game( int* fd, Player* player );
void start_guest_game( int* fd, Player* player );

bool send_shot( Player* player );
bool receive_shot( Player* player );

#endif
