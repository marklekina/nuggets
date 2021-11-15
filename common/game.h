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
/* 
 * comments
 */

game_t*
game_new(FILE* map, int nrows, int ncols);

/**************** game_delete ****************/
/* 
 * comments
 */

void game_delete(game_t* game);

/**************** removeSpectator ****************/
/* 
 * comments
 */
void removeSpectator(game_t* game);
/**************** get_location ****************/
/* 
 * comments
 */
int get_location(game_t* game);

/**************** spectatorAdd ****************/
/* 
 * comments
 */
bool
spectatorAdd(game_t* game, player_t* player);

/**************** game_getGrid ****************/
/* 
 * comments
 */
grid_t* 
game_getGrid(game_t* game);

/**************** game_getMap ****************/
/* 
 * comments
 */
FILE* 
game_getMap(game_t* game);

/**************** game_addPlayer ****************/
/* 
 * comments
 */
bool
game_addPlayer(game_t* game, char* name, char* type);

/**************** game_getSpectator ****************/
/* 
 * comments
 */
player_t* 
game_getSpectator(game_t* game);

/**************** game_setSeed ****************/
/* 
 * comments
 */
bool game_setSeed(game_t* game, int seed);

/**************** game_setMap ****************/
/* 
 * comments
 */
void game_setMap(game_t* game, FILE* map);

/**************** game_setMap ****************/
/* 
 * comments
 */
player_t* get_Player(game_t* game, int location);

/**************** game_setMap ****************/
/* 
 * comments
 */
bool game_setGrid(game_t* game, grid_t* grid);

#endif