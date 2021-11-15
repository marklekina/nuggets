/*
 * game.h - header file for game module
 *
 * this module contains:
 *     game_t struct - holds a grid object and player information
 *     functions to:
 *        i) add and remove spectators
 *       ii) add players
 *      iii) getter functions 
 *
 * Palmer's Posse, November 2021
 */

#ifndef __GAME_H
#define __GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mem.h"
#include "file.h"
#include "grid.h"
#include "player.h"

/**************** global types ****************/
typedef struct pile pile_t;
typedef struct game game_t;

game_t* game_new(FILE* fp, int nrows, int ncols);
void game_delete(game_t* game);
void remove_spectator(game_t* game);
bool add_spectator(game_t* game);
int add_player(game_t* game, char* name);
int get_num_players(game_t* game);
player_t* get_player(game_t* game, int idx);
grid_t* get_grid(game_t* game);
player_t* get_spectator(game_t* game);
point_t* get_pile_location(pile_t* pile);
pile_t* get_piles(game_t* game, int i);
bool add_pile(game_t* game, pile_t* pile);
int get_num_piles(game_t* game);
pile_t* pile_new(point_t* location, int amount);
bool set_gold_distributed(game_t* game, int goldDis);
int get_gold_distributed(game_t* game);
int get_gold_remaining(game_t* game);
int get_amount(pile_t* pile);
void pile_delete(pile_t* pile);
void change_remaining_gold(game_t* game, int amount);

#endif // __GAME_H
