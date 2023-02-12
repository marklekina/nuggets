/*
 * game.h - header file for 'game' module
 *
 * The 'game' struct keeps track of all state for the nuggets game's operation:
 *   - the map(grid) of the game
 *   - the players participating in the game
 *   - the gold piles to be collected, and
 *   - the amount of gold remaining in the game
 *
 * Copyright (c) 2022 Mark Lekina Rorat All Rights Reserved.
 */

#ifndef __GAME_H
#define __GAME_H

/**************** global types ****************/
typedef struct game game_t;

/**************** functions ****************/

/**************** game_new ****************/
/*
 * creates a new instance of the game
 *
 * caller provides:
 *   valid file pointer to the map string file
 *   maximum number of players that can participate in the game
 *   maximum number of gold piles that can be placed in the game
 *   amount of gold to be distributed, i.e. the gold amount remaining when the game starts
 *
 * we return:
 *   pointer to a new game, or NULL if error.
 *
 * caller is responsible for:
 *   later calling game_delete upon completion of the game session.
 */
game_t* game_new(FILE* fp, int max_players, int max_piles, int gold_balance);

/**************** game_delete ****************/
/*
 * deletes the game struct.
 *
 * caller provides:
 *   a valid game pointer.
 *
 * we guarantee:
 *   we free the game's grid(map) memory
 *   we free each player's memory, as well as the players array's memory
 *   we free each gold pile's memory, as well as the piles array's memory
 *   we free the game's memory
 *   we ignore NULL game.
 */
void game_delete(game_t* game);

/************ gettters ************/

/************ get_num_players ************/
/*
 *
 */
int get_num_players(game_t* game);

/************ update_num_players ************/
/*
 *
 */
void update_num_players(game_t* game);

/************ get_num_piles ************/
/*
 *
 */
int get_num_piles(game_t* game);

/************ get_gold_balance ************/
/*
 *
 */
int get_gold_balance(game_t* game);

/************ update_gold_balance ************/
/*
 *
 */
bool update_gold_balance(game_t* game, int gold_collected);

/************ get_grid ************/
/*
 *
 */
grid_t* get_grid(game_t* game);

/************ get_players ************/
/*
 *
 */
player_t** get_players(game_t* game);

/************ get_piles ************/
/*
 *
 */
pile_t** get_piles(game_t* game);

/************ get_player_by_address ************/
/*
 *
 */
player_t* get_player_by_address(game_t* game, addr_t address);

/************ get_spectator ************/
/*
 *
 */
player_t* get_spectator(game_t* game);

/************ add_player ************/
/*
 *
 */
player_t* add_player(game_t* game, const addr_t address, char* name);

/************ get_empty_room_spot ************/
/*
 *
 */
point_t* get_empty_room_spot(game_t* game);

/************ is_empty_room_spot ************/
/*
 *
 */
bool is_empty_room_spot(game_t* game, point_t* point);

/************ build_visible_mapstring ************/
/*
 *
 */
bool build_visible_mapstring(game_t* game, player_t* player);

/**************** distribute_gold ****************/
/*
 *
 */
bool distribute_gold(game_t* game, int min_piles, int max_piles);

/**************** get_target_location ****************/
/*
 *
 */
point_t* get_target_location(game_t* game, player_t* player, const char keystroke);

#endif // __GAME_H
