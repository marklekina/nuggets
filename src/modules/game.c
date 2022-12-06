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


/**************** move_player() ****************/
/* see game.h for description */
bool
move_player(game_t* game, player_t* player, point_t* point) {
  // validate parameters
  if (game == NULL || player == NULL || point == NULL) {
    log_v("move_player: NULL pointer(s) passed to function");
    return false;
  }

  // 1. Switch locations with an occupant player (if target location is occupied)

  // loop through list of players
  for (int i = 0; i < game->num_players; i++) {
    player_t* player_occupant = game->players[i];

    // check if destination spot is occupied by any of the players
    if (is_same_location(point, get_location(player_occupant))) {

      // switch the occupant player to the moving player's location
      bool switched_location = update_location(player_occupant, get_location(player));

      // log unsuccessful location switch
      if (!switched_location) {
        log_v("move_player: error switching player locations");
        return false;
      }
      break;
    }
  }

  // 2. Move the player to the desired location

  // move the player onto the desired location
  bool moved_player = update_location(player, point);

  // log unsuccessful player movement
  if (!moved_player) {
    log_v("move_player: error moving player");
    return false;
  }

  // 3. Collect gold if necessary

  // loop through list of gold piles
  for (int i = 0; i < game->num_piles; i++) {
    pile_t* pile = game->piles[i];

    // check if destination spot is occupied by any of the gold piles
    if (is_same_location(point, get_location(pile))) {

      // add the gold into the player's wallet
      int gold_collected = collect_gold(game, player, pile);

      // log unsuccessful gold collection
      if (gold_collected < 0) {
        log_v("move_player: error collecting gold from pile");
        return false;
      }
      break;
    }
  }

  // return successfully
  return true;
}


/**************** collect_gold() ****************/
/* see game.h for description */
int
collect_gold(game_t* game, player_t* player, pile_t* pile) {
  // validate parameters
  if (game == NULL || player == NULL || pile == NULL) {
    log_v("collect_gold: NULL pointer(s) passed to function");
    return -1;
  }

  // 1. Add gold from pile to a player's wallet
  int gold_collected = get_gold(pile);
  bool added_gold = update_wallet_balance(player, gold_collected);

  // log unsuccessful addition of gold to player's wallet
  if (!added_gold) {
    log_v("collect_gold: error adding gold to player's wallet");
    return -1;
  }

  // 2. Delete the gold pile
  pile_delete(pile);

  // 3. Update the amount of gold remaining in the game
  game->gold_balance -= gold_collected;

  // return successfully
  return gold_collected;
}

// check player location to see if they're on a gold pile - server level
// add gold in pile to player's purse - player level
// update gold remaining in game - game level
// delete gold pile - game level


/**************** build_visible_mapstring() ****************/
/* see grid.h for description */
char*
build_visible_mapstring(game_t* game, player_t* player) {
  // validate parameters
  if (game == NULL || player == NULL) {
    log_v("build_visible_mapstring: NULL pointer(s) passed to function");
    return NULL;
  }

  grid_t* grid = game->grid;
  player_t** players = game->players;
  pile_t** goldPiles = game->piles;

  // validate the game's grid, gold piles and players array
  if (grid == NULL || players == NULL || goldPiles == NULL) {
    log_v("build_visible_mapstring: NULL values for grid, piles or players");
    return NULL;
  }
  // fetch grid size
  int grid_size = get_size(grid);

  // load grid points array from grid, ensure it is valid and contains grid points
  point_t** gridPoints = grid->gridPoints;
  if (gridPoints == NULL || gridPoints[i] == NULL) {
    log_v("build_visible_mapstring: error fetching gridpoints array from grid");
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

  // loop through each gridpoint
  player_location = get_location(player);
  for (int i = 0; i < grid_size; i++) {
    bool is_visible = compute_visibility(grid, player_location, gridPoints[i]);

    // replace grid point symbol with ' ' if grid point not visible to the player
    if (!is_visible) {
      mapString[i] = ' ';
    }
  }

  // return map string
  return mapString;
}
