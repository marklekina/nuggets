/*
 * server.h - header file for the nuggets game server
 *
 * Copyright (c) 2022 Mark Lekina Rorat All Rights Reserved.
 *
 */

#ifndef __SERVER_H
#define __SERVER_H

/**************** functions ****************/

bool handleMessage(void* arg, const addr_t from, const char* message);
bool handle_play(game_t* game, const addr_t from, const char* name);
bool handle_key(game_t* game, const addr_t from, const char keystroke);
bool handle_spectate(game_t* game, const addr_t from);
void send_ok(const addr_t to, const char letter);
void send_grid(const addr_t to, const int nrows, const int ncols);
void send_gold(const addr_t to, const int n, const int p, const int r);
void send_display(const addr_t to, const char* mapstring);
void send_quit(const addr_t to, const char* explanation);
void send_error(const addr_t to, const char* explanation);
player_t* add_player(game_t* game, const addr_t address, const char* name_copy);
player_t* get_spectator(game_t* game);
addr_t get_address(player_t* spectator);
bool update_spectator(game_t* game, const addr_t address);

#endif //__SERVER_H
