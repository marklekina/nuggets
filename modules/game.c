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
  int num_players;     // number of players in the game
  int num_piles;       // number of gold piles in the game
  int gold_total;      // amount of gold remaining in the game
  grid_t* grid;        // representation of the game's map
  player_t** players;  // array of gold players in the game
  pile_t** piles;      // array of gold piles in the game
} game_t;

/**************** global functions ****************/

/**************** game_new() ****************/
/* see game.h for description */
game_t*
game_new(FILE* fp, int max_players, int max_piles) {
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
  game->gold_total = 0;
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
  if (game == NULL || pile == NULL) {
    return false;
  }

  // add pile to piles array
  game->piles[game->num_piles] = pile;

  // increment number of piles in game
  game->num_piles = game->num_piles + 1;

  // return true
  return true;
}


/**************** remove_pile() ****************/
/* see game.h for description */
int
remove_pile(game_t* game, pile_t* pile) {
  // validate game and pile
  if (game == NULL || pile == NULL) {
    return -1;
  }

  // free pile memory
  pile_delete(pile);

  // decrement number of piles in game by 1
  // assumption: this function will only be called to delete piles already in the game
  game->num_piles = game->num_piles - 1;

  // return number of remaining piles if successful
  return game->num_piles;
}


/**************** add_player() ****************/
/* see game.h for description */
bool
add_player(game_t* game, player_t* player, int max_players) {
  // validate parameters
  if (game == NULL || player == NULL) {
    return false;
  }

  // check to ensure that the number of players does not exceed the maximum allowed
  if (game->num_players >= max_players) {
    return false;
  }

  // add player to player array
  game->players[game->num_players] = player;

  // increment number of players in game
  game->num_players = game->num_players + 1;

  // return true
  return true;
}


/**************** update_gold() ****************/
/* see game.h for description */
int
update_gold(game_t* game, int gold_collected) {
  // validate parameters
  if (game != NULL) {
    // remove collected gold from the remaining gold
    game->gold_total = game->gold_total - gold_collected;

    // return updated gold remainder
    return game->gold_total;
  }

  // otherwise return -1
  return -1;
}

// check player location to see if they're on a gold pile - server level
// add gold in pile to player's purse - player level
// update gold remaining in game - game level
// delete gold pile - game level

/**************** build_mapstring() ****************/
/* see grid.h for description */
char*
build_mapstring(game_t* game, player_t* player) {
  // validate game and player
  if (game == NULL || player == NULL) {
    return NULL;
  }

  // validate the game's grid, gold piles and players array
  grid_t* grid = game->grid;
  player_t** players = game->players;
  pile_t** goldPiles = game->goldPiles;
  int grid_size = get_size(grid);
  if (grid == NULL || players == NULL || goldPiles == NULL) {
    return NULL;
  }

  // load grid points array from grid, ensure it is valid and contains grid points
  point_t** gridPoints = grid->gridPoints;
  if (gridPoints == NULL || gridPoints[i] == NULL) {
    return NULL;
  }

  // loop through the grid points and lay them out in a map string
  char* mapString = (char) mem_malloc(sizeof(char) * (grid->size + 1));
  for (int i = 0; i < grid_size; i++) {
    mapString[i] = get_symbol(gridPoints[i]);
  }
  mapString[grid_size] = "\0";

  // loop through all gold piles and replace grid point symbol with asterisk
  for (int i = 0; i < strlen(goldPiles); i++) {
    point_t* pile_location = get_location(goldPiles[i]);
    int idx = get_row(pile_location) * grid->ncols + get_col(pile_location);
    mapString[idx] = "*";
  }

  // loop through all players and replace grid point symbol with player letters
  for (int i = 0; i < strlen(players); i++) {
    point_t* player_location = get_location(players[i]);
    int idx = get_row(player_location) * grid->ncols + get_col(player_location);
    if (players[i] == player) {
      mapString[idx] = "@";
    }
    else {
      mapString[idx] = get_letter(players[i]);
    }
  }

  // return map string
  return mapString;
}
