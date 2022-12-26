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
point_t* move_player(player_t* player, const char keystroke);
point_t* sprint_player(player_t* player, const char keystroke);
point_t* get_target_location(player_t* player, const char keystroke);
bool run_move_sequence(player_t* player, point_t* target);
bool collect_gold(game_t* game, player_t* player, pile_t* pile);
char* compile_game_over_report(game_t* game);

// TODO: implement this
bool distribute_gold(game_t* game, int min_piles, int max_piles);
bool build_visible_mapstring(game_t* game, player_t* player);
player_t* add_player(game_t* game, const addr_t address, const char* name);

#endif //__SERVER_H
