#ifndef GAME_H
#define GAME_H

#include "player.h"

Player* player;
char name[ MAX_LINE_SIZE ];
bool player1_turn;
bool game_finished;

void play_by_turns();
void multiplayer( bool network );

void start_multiplayer_game( bool host );

bool send_shot( Player* player );
bool receive_shot( Player* player );

#endif
