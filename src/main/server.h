/*
 * server.h - header file for the nuggets game server
 *
 * Copyright (c) 2022 Mark Lekina Rorat All Rights Reserved.
 *
 */

#ifndef __SERVER_H
#define __SERVER_H

/**************** functions ****************/

// handlers
bool handleMessage(void* arg, const addr_t from, const char* message);
bool handle_play(game_t* game, const addr_t from, const char* name);
bool handle_key(game_t* game, const addr_t from, const char keystroke);
bool handle_spectate(game_t* game, const addr_t from);

// senders
void send_ok(const addr_t to, const char letter);
void send_grid(const addr_t to, const int nrows, const int ncols);
void send_gold(const addr_t to, const int n, const int p, const int r);
void send_display(const addr_t to, const char* mapstring);
void send_quit(const addr_t to, const char* explanation);
void send_error(const addr_t to, const char* explanation);

// other functions
bool collect_gold(game_t* game, player_t* player, pile_t* pile);
point_t* move_player(game_t* game, player_t* player, const char keystroke);
point_t* sprint_player(game_t* game, player_t* player, const char keystroke);
bool run_move_sequence(game_t* game, player_t* player, point_t* target);
char* compile_game_over_report(game_t* game);

#endif //__SERVER_H
