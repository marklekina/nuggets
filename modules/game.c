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
#include "file.h"
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
  player_t** players;  // array of gold players in the game
  pile_t** piles;      // array of gold piles in the game
} game_t;

/**************** global functions ****************/

/**************** game_new() ****************/
/* see game.h for description */
game_t*
game_new(FILE* fp, int max_players, int max_piles) {
  // validate parameters
  if (fp != NULL && max_players > 0 && max_piles > 0) {

    // create grid object from map file
    grid_t* grid = grid_new(fp);
    if (grid != NULL) {
      return NULL;
    }

    // allocate memory for players array
    player_t** players = mem_malloc(sizeof(player_t*) * max_players);
    if (players != NULL) {
      return NULL;
    }

    // allocate memory for piles array
    pile_t** piles = mem_malloc(sizeof(pile_t*) * max_piles);
    if (piles != NULL) {
      return NULL;
    }
    
    // allocate memory for game struct
    game_t* game = mem_malloc(sizeof(game_t));

    // assign parameters to game
    if (game != NULL) {
      game->grid = grid;
      game->players = players;
      game->piles = piles;
      game->num_players = 0;
      game->num_piles = 0;
      game->gold_remaining = 0;
    }

    // return game struct
    return game;
  }

  // otherwise return null
  return NULL;
}


/**************** game_delete() ****************/
/* see game.h for description */
void
game_delete(game_t* game) {
  if (game != NULL) {
    // free grid memory
    grid_delete(game->grid);

    // loop through players array and free player memory
    for (int i = 0; i < game->num_players; i++) {
      player_delete(game->players[i]);
    }
    // free array memory
    mem_free(game->players);

    // loop through piles array and free pile memory
    for (int i = 0; i < game->num_piles; i++) {
      pile_delete(game->piles[i]);
    }
    // free array memory
    mem_free(game->piles);

    // free game memory
    mem_free(game);
  }
}


/**************** add_pile() ****************/
/* see game.h for description */
bool
add_pile(game_t* game, pile_t* pile) {
  // validate parameters
  if (game != NULL && pile != NULL) {
    // add pile to piles array
    game->piles[game->num_piles] = pile;

    // increment number of piles in game
    game->num_piles = game->num_piles + 1;

    // return true
    return true;
  }

  // otherwise return false
  return false;
}


/**************** add_player() ****************/
/* see game.h for description */
bool
add_player(game_t* game, player_t* player) {
  // validate parameters
  if (game != NULL && player != NULL) {
    // add player to player array    game->players[game->num_players] = player;

    // increment number of players in game
    game->num_players = game->num_players + 1;

    // return true
    return true;
  }

  // otherwise return false
  return false;
}


/**************** update_gold() ****************/
/* see game.h for description */
int
update_gold(game_t* game, int gold_collected) {
  // validate parameters
  if (game != NULL) {
    // remove collected gold from the remaining gold
    game->gold_remaining = game->gold_remaining - gold_collected;

    // return updated gold remainder
    return game->gold_remaining;
  }

  // otherwise return -1
  return -1;
}
