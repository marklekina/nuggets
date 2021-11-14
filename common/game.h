/* 
 * game.h - header file for grid module
 *
 */

#ifndef __GAME_H
#define __GAME_H

#include <stdio.h>
#include <stdbool.h>
#include "player.h"
#include "player.c"
#include "grid.h"


/**************** global types ****************/
typedef struct game game_t;  // opaque to users of the module

/**************** functions ****************/

/**************** game_new ****************/
/* Creates a new game or returns NULL if invalid parameters
 * Create a new game with the provided map 
 * nrows and ncols provided help load the game's grid
 * The spectator is intialized empty
 * The location is the amount of players currently in the game... intialized to 0
 * initialized game is returned
 */

game_t*
game_new(FILE* map, int nrows, int ncols);

/**************** game_delete ****************/
/* 
 * deletes spectator and players in the player list
 * deletes game 
 */

void game_delete(game_t* game);

/**************** removeSpectator ****************/
/* 
 * deletes the game's spectator
 */
void removeSpectator(game_t* game);

/**************** spectatorAdd ****************/
/* 
 * Adds a new spectator to the game
 * Removes current spectator, if there is one
 * Returns true if the addition is successful
 */
bool
spectatorAdd(game_t* game, player_t* player);

/**************** game_addPlayer ****************/
/* 
 * adds player with given name and type to the running list of players in the game
 * incriments the location variable within the game
 */
bool
game_addPlayer(game_t* game, char* name, char* type);

/**************** get_location ****************/
/* 
 * returns game's location
 */
int get_location(game_t* game);
/**************** get_Player ****************/
/* 
 * returns the player in the game that matches game of given player
 */
player_t* get_Player(game_t* game, player_t* player){

/**************** game_getMap ****************/
/* 
 * returns the game's map
 */
FILE* game_getMap(game_t* game);
/**************** game_getGrid ****************/
/* 
 * returns the game's grid
 */
grid_t* game_getGrid(game_t* game);


/**************** game_getSpectator ****************/
/* 
 * returns the game's spectator
 */
player_t* game_getSpectator(game_t* game);

#endif