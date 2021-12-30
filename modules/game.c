/* 
 * game.c - 'game' module
 *
 * see game.h for more information.
 *
 * Mark Lekina Rorat, December 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "point.h"
#include "pile.h"
#include "grid.h"
#include "player.h"
#include "game.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct game {
  grid_t* grid;        // representation of the game's map
  int num_players;     // number of players in the game
  int num_piles;       // number of gold piles in the game
  int gold_remaining;  // amount of gold remaining in the game
  player_t* players[]; // array of gold players in the game
  pile_t* piles[];     // array of gold piles in the game
} game_t;

/**************** global functions ****************/

/**************** game_new() ****************/
/* see game.h for description */

/**************** game_delete() ****************/
/* see game.h for description */

/**************** distribute_gold() ****************/
/* see game.h for description */

/**************** update_gold() ****************/
/* see game.h for description */

/**************** add_player() ****************/
/* see game.h for description */
