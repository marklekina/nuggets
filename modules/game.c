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
#include "log.h"
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
    log_v("game_new: NULL file pointer passed to function");
    return NULL;
  }

  // create grid object from map file
  grid_t* grid = grid_new(fp);
  if (grid == NULL) {
    log_v("game_new: error creating grid struct");
    return NULL;
  }

  // allocate memory for players array
  player_t** players = mem_malloc(sizeof(player_t*) * max_players);
  if (players == NULL) {
    log_v("game_new: error allocating memory for players array");
    return NULL;
  }

  // allocate memory for piles array
  pile_t** piles = mem_malloc(sizeof(pile_t*) * max_piles);
  if (piles == NULL) {
    log_v("game_new: error allocating memory for piles array");
    return NULL;
  }

  // allocate memory for game struct
  game_t* game = mem_malloc(sizeof(game_t));
  if (game == NULL) {
    log_v("game_new: error allocating memory for game struct");
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
    log_v("game_delete: NULL game pointer passed to function");
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


/**************** distribute_gold() ****************/
/* see game.h for description */
bool
distribute_gold(game_t* game, int min_piles, int max_piles) {
  // validate parameters
  if (game == NULL) {
    log_v("distribute_gold: NULL game pointer passed to function");
    return false;
  }

  // generate random number of piles between specified range
  int num_piles = rand() % (max_piles - min_piles + 1)) + min_piles;
  game->num_piles = num_piles;

  // 1. distribute the gold among the piles randomly

  // variables to hold gold amounts
  int gold_to_distribute = game->gold_balance;
  int gold_amounts[num_piles];
  int gold_avg = gold_to_distribute / num_piles;

  // distribute gold among piles in random amounts
  for (int i = 0; i < num_piles; i++) {
    gold_amounts[i] = rand() % gold_avg;
    gold_to_distribute -= gold_amounts[i];
  }

  // distribute the remaining gold sequentially
  int i = 0;
  while (gold_to_distribute > 0) {
    gold_amounts[i] += 1;
    gold_to_distribute -= 1;
    i = (i + 1) % num_piles;
  }

  // 2. select random room spots to place the gold piles

  // variables to hold location data
  int row, column;
  grid_t* grid = game->grid;
  point_t* pile_location;
  point_t* pile_locations[num_piles];

  // compute random location to drop each gold pile i.e., ensure that locations picked
  //  - are room spots
  //  - the locations have not been picked already for a different pile
  for (int i = 0; i < num_piles; i++) {
    do {
      row = rand() % get_nrows(grid);
      col = rand() % get_ncols(grid);
      pile_location = get_gridpoint(grid, row, col);
    }
    while(!is_room_spot(pile_location) || is_point_in_list(pile_location, pile_locations));

    // add location to list of pile locations
    pile_locations[i] = pile_location;
  }

  // 3. combine gold amount and location data and create gold piles

  // add new piles to the games pile list
  for (int i = 0; i < num_piles; i++) {
    pile_t* pile = pile_new(pile_locations[i], gold_amounts[i]);
    game->piles[i] = pile;
  }

  // return successfully
  return true;
}


/**************** add_player() ****************/
/* see game.h for description */
bool
add_player(game_t* game, player_t* player, int max_players) {
  // validate parameters
  if (game == NULL || player == NULL) {
    log_v("add_player: NULL pointer(s) passed to function");
    return false;
  }

  // check to ensure that the number of players does not exceed the maximum allowed
  if (game->num_players >= max_players) {
    log_v("add_player: maximum number of players reached");
    return false;
  }

  // add player to player array
  game->players[game->num_players] = player;

  // increment number of players in game
  game->num_players = game->num_players + 1;

  // return true
  return true;
}










/* TODO: continue here */










/**************** update_gold() ****************/
/* see game.h for description */
int
update_gold(game_t* game, int gold_collected) {
  // validate parameters
  if (game != NULL) {
    // remove collected gold from the remaining gold
    game->gold_balance = game->gold_balance - gold_collected;

    // return updated gold remainder
    return game->gold_balance;
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
  // TODO: replace grid point symbols with ' ' if grid point not visible to the player
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
