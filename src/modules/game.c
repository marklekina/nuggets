/*
 * game.c - 'game' module
 *
 * see game.h for more information.
 *
 * Mark Lekina Rorat, December 2021; November 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "file.h"
#include "message.h"
#include "point.h"
#include "pile.h"
#include "grid.h"
#include "player.h"
#include "game.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct game {
  int num_players;     // number of players in the game
  int num_piles;       // number of gold piles in the game
  int gold_balance;    // amount of gold remaining in the game
  grid_t* grid;        // representation of the game's map
  player_t** players;  // array of players in the game
  pile_t** piles;      // array of gold piles in the game
} game_t;

/**************** global functions ****************/

/**************** game_new() ****************/
/* see game.h for description */
game_t*
game_new(FILE* fp, int max_players, int max_piles, int gold_balance) {
  // validate parameters
  if (fp == NULL) {
    return NULL;
  }

  // create grid object from map file
  grid_t* grid = grid_new(fp);
  if (grid == NULL) {
    return NULL;
  }

  // allocate memory for players array
  player_t** players = mem_malloc(sizeof(player_t*) * max_players);
  if (players == NULL) {
    return NULL;
  }

  // allocate memory for piles array
  pile_t** piles = mem_malloc(sizeof(pile_t*) * max_piles);
  if (piles == NULL) {
    return NULL;
  }

  // allocate memory for game struct
  game_t* game = mem_malloc(sizeof(game_t));
  if (game == NULL) {
    return NULL;
  }

  // assign parameters to game
  game->num_players = 0;
  game->num_piles = 0;
  game->gold_balance = gold_balance;
  game->grid = grid;
  game->players = players;
  game->piles = piles;

  // return game struct
  return game;
}


/**************** game_delete() ****************/
/* see game.h for description */
void
game_delete(game_t* game) {
  if (game == NULL) {
    return;
  }

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


/************ get_num_players() ************/
/* see game.h for description */
int
get_num_players(game_t* game) {
  return game->num_players;
}

/************ get_num_piles() ************/
/* see game.h for description */
int
get_num_piles(game_t* game) {
  return game->num_piles;
}

/************ get_gold_balance ************/
/* see game.h for description */
int
get_gold_balance(game_t* game) {
  return game->gold_balance;
}

/************ get_grid ************/
/* see game.h for description */
grid_t*
get_grid(game_t* game) {
  return game->grid;
}

/************ get_players ************/
/* see game.h for description */
player_t**
get_players(game_t* game) {
  return game->players;
}

/************ get_piles ************/
/* see game.h for description */
pile_t**
get_piles(game_t* game) {
  return game->piles;
}


/************ get_player_by_address ************/
/* see game.h for description */
player_t*
get_player_by_address(game_t* game, addr_t address) {
  // validate parameters
  if (game == NULL || !message_isAddr(address)) {
    return NULL;
  }

  // get list of players in the game
  int num_players = get_num_players(game);
  player_t** players = get_players(game);

  // loop through list and return player if the address matches
  for (int i = 0; i < num_players; i++) {
    if (message_eqAddr(address, get_address(players[i]))) {
      return players[i];
    }
  }

  // otherwise return unsuccessfully
  return NULL;
}
