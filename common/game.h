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

/**************** functions ****************/

/**************** pile_new ****************/
/* Creates a new game or returns NULL if invalid parameters
 * Create a new game with the provided map 
 * nrows and ncols provided help load the game's grid
 * The spectator is intialized empty
 * The location is the amount of players currently in the game... intialized to 0
 * Initialized game is returned
 */
pile_t* pile_new(point_t* location, int amount);

/**************** pile_delete****************/
/* Frees pile and its location
 */
void pile_delete(pile_t* pile);

/**************** game_new ****************/
/* Creates a new game or returns NULL if invalid parameters
 * Create a new game with the provided file, rows, and column 
 * nrows and ncols provided help load the game's grid
 * The spectator is intialized empty
 * the nuumber of players is initialized to 0
 * Initialized game is returned
 */
game_t* game_new(FILE* fp, int nrows, int ncols);

/**************** game_delete ****************/
/* 
 * deletes spectator and players in the player list
 * deletes game 
 */
void game_delete(game_t* game);

/**************** remove_spectator ****************/
/* 
 * deletes the game's spectator and sends message telling them
 */
void remove_spectator(game_t* game);

/**************** add_spectator****************/
/* 
 * Adds a new spectator to the game
 * Removes current spectator, if there is one
 * Returns true if the addition is successful
 */
bool add_spectator(game_t* game);

/**************** add_player ****************/
/* 
 * adds player with given name and type to the running list of players in the game
 * incriments the number of players within the game
 * returns the player's number
 * otherwise returns invalid index
 */
int add_player(game_t* game, char* name);

/**************** get_num_player ****************/
/* 
 * returns number of players
 */
int get_num_players(game_t* game);

/**************** get_Player ****************/
/* 
 * returns the player in the location that matches the given index
 */
player_t* get_player(game_t* game, int idx);

/**************** game_getGrid ****************/
/* 
 * returns the game's grid
 */
grid_t* get_grid(game_t* game);

/**************** get_spectator ****************/
/* 
 * returns the game's spectator
 */
player_t* get_spectator(game_t* game);

/**************** get_pile_location ****************/
/* 
 * returns the pile's location 
 */
point_t* get_pile_location(pile_t* pile);

/**************** get_piles ****************/
/* 
 * returns the pile at the given index
 */
pile_t* get_piles(game_t* game, int i);


/**************** add_piles ****************/
/* 
 * adds pile to the game
 */
bool add_pile(game_t* game, pile_t* pile);

/**************** get_num_piles ****************/
/* 
 * gets number of piles in the provided game
 */
int get_num_piles(game_t* game);

/**************** set_gold_distribute ****************/
/* 
 * sets the game's gold_distributed variable
 */
bool set_gold_distributed(game_t* game, int goldDis);

/**************** get_gold_distributed ****************/
/* 
 * returns the game's gold_distributed variable
 */
int get_gold_distributed(game_t* game);

/**************** get_gold_remaining ****************/
/* 
 * returns the game's remaining amount of gold 
 */
int get_gold_remaining(game_t* game);


/**************** get_amount ****************/
/* 
 * returns amount of gold in a given pile
 */
int get_amount(pile_t* pile);

/**************** change_remaining_gold ****************/
/* 
 * modifies the amount of gold by subtracting the given amount from it
 */
void change_remaining_gold(game_t* game, int amount);

#endif // __GAME_H
