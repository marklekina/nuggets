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
game_new(FILE* map);

/**************** game_delete ****************/
/* 
 * comments
 */

game_t*
game_delete(game_t* game);

/**************** removeSpectator ****************/
/* 
 * comments
 */
void removeSpectator(game_t* game);

/**************** spectatorAdd ****************/
/* 
 * comments
 */
bool
spectatorAdd(game_t* game, player_t* player);

/**************** game_addPlayer ****************/
/* 
 * comments
 */
player_t*
game_addPlayer(game_t* game, char* name, char* type);

#endif