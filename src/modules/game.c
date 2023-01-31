/*
 * game.c - 'game' module
 *
 * see game.h for more information.
 *
 * Mark Lekina Rorat, December 2021; November 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
  pile_t** piles;      // array of gold piles in the game
  player_t** players;  // array of players in the game
  player_t* spectator; // special "player"; does not participate, only watches
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

  // build grid points array
  build_grid(grid);

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
  game->spectator = NULL;

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

  player_delete(game->spectator);

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


/************ update_num_players() ************/
/* see game.h for description */
void
update_num_players(game_t* game) {
  game->num_players += 1;
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

/************ update_gold_balance ************/
/* see game.h for description */
bool
update_gold_balance(game_t* game, int gold_collected) {
  // validate parameters
  if (game == NULL || gold_collected <= 0) {
    return false;
  }

  // subtract collected gold from the game's gold balance
  game->gold_balance -= gold_collected;
  return true;
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

  // check spectator
  player_t* spectator = get_spectator(game);
  if (message_eqAddr(address, get_address(spectator))) {
    return spectator;
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

/************ get_spectator ************/
/* see game.h for description */
player_t*
get_spectator(game_t* game) {
  // validate parameters
  if (game == NULL) {
    return NULL;
  }

  // otherwise return unsuccessfully
  return game->spectator;
}

/************ add_player ************/
/* see game.h for description */
player_t*
add_player(game_t* game, const addr_t address, char* name) {
  // validate parameters
  if (game == NULL || !message_isAddr(address) || name == NULL) {
    return false;
  }

  char letter;
  point_t* location;

  // initially set the map string to a string of empty spaces
  int mapstring_len = strlen(get_map_string(get_grid(game)));
  char* visible_map = mem_malloc(sizeof(char) * mapstring_len + 1);
  for (int i = 0; i < mapstring_len; i++) {
    visible_map[i] = ' ';
  }
  visible_map[mapstring_len] = '\0';

  // handle spectators
  if (strcmp(name, "_SPECTATOR_") == 0) {
    // create new spectator
    letter = '_';
    location = NULL;
    player_t* spectator = player_new(name, letter, location, visible_map, address);
    if (spectator == NULL) {
      return NULL;
    }

    // construct the spectator's visible map (AKA the entire grid; to be handled in-function)
    build_visible_mapstring(game, spectator);

    // add spectator to game
    game->spectator = spectator;

    // return successfully
    return spectator;
  }

  // get new player's index in the list of players in the game
  int idx = get_num_players(game);

  // populate player's information
  // letter
  letter = (char) 'A' + idx;

  // location
  point_t* player_spot = get_empty_room_spot(game);
  int row = get_row(player_spot);
  int col = get_col(player_spot);
  location  = point_new(row, col, letter);

  // create new player
  player_t* player = player_new(name, letter, location, visible_map, address);
  if (player == NULL) {
    return NULL;
  }

  // add player to list of players in the game
  player_t** players = get_players(game);
  players[idx] = player;

  // increment number of players in the game
  update_num_players(game);

  // update player's visible map string
  build_visible_mapstring(game, player);

  // return successfully
  return player;
}

point_t*
get_empty_room_spot(game_t* game) {
  // validate parameter
  if (game == NULL) {
    return NULL;
  }

  grid_t* grid = get_grid(game);
  point_t* gridpoint;
  int row, col;

  // randomly select rows and columns until an empty room spot is located
  do {
    row = rand() % get_nrows(grid);
    col = rand() % get_ncols(grid);
    gridpoint = get_gridpoint(grid, row, col);
  } while(!is_empty_room_spot(game, gridpoint));

  // return empty_room_spot
  return gridpoint;
}

bool
is_empty_room_spot(game_t* game, point_t* point) {
  // validate parameters
  if (game == NULL || point == NULL) {
    return false;
  }

  // check whether gridpoint is a room spot
  if (!is_room_spot(point)) {
    return false;
  }

  // check whether any of the gold piles are located on the room spot
  int num_piles = get_num_piles(game);
  pile_t** piles = get_piles(game);
  pile_t* pile;

  for (int i = 0; i < num_piles; i++) {
    pile = piles[i];
    if (is_same_location(point, get_pile_location(pile))) {
      return false;
    }
  }

  // check whether any of the players are located on the room spot
  int num_players = get_num_players(game);
  player_t** players = get_players(game);
  player_t* player;

  for (int i = 0; i < num_players; i++) {
    player = players[i];
    if (is_same_location(point, get_location(player))) {
      return false;
    }
  }

  // return true if spot is unoccupied
  return true;
}

/**************** build_visible_mapstring() ****************/
/* see game.h for description */
bool
build_visible_mapstring(game_t* game, player_t* player) {
  // validate parameters
  if (game == NULL || player == NULL) {
    return false;
  }

  // get grid information
  grid_t* grid = get_grid(game);
  int grid_size = get_size(grid);
  int ncols = get_ncols(grid);

  // load grid points array from grid
  point_t** gridPoints = get_gridpoints(grid);
  if (gridPoints == NULL || gridPoints[0] == NULL) {
    // run build_grid first
    return false;
  }

  // make a copy of the grid's map string
  char* known_mapstring = get_visible_map(player);    // player's old visible string
  char* default_mapstring = get_map_string(grid);     // complete mapstring (without occupants)
  char* mapString = strdup(default_mapstring);        // copy that will be modified and assigned to player

  // location variables
  point_t* location;
  int idx, row, col;

  // 1. mark gold piles

  // get list of gold piles
  pile_t** piles = get_piles(game);
  int num_piles = get_num_piles(game);

  // loop through all gold piles
  for (int i = 0; i < num_piles; i++) {
    // skip deleted piles
    if (piles[i] == NULL) {
      continue;
    }

    // compute pile symbol index in the mapstring
    location = get_pile_location(piles[i]);
    row = get_row(location);
    col = get_col(location);
    idx = (col - 1) + (row - 1) * (ncols + 1);

    // replace grid point symbol with asterisk
    mapString[idx] = '*';
  }

  // 2. mark players

  // get list of players
  player_t** players = get_players(game);
  int num_players = get_num_players(game);

  // loop through all players
  for (int i = 0; i < num_players; i++) {
    // skip deleted players and the spectator
    if (players[i] == NULL || is_spectator(players[i])) {
      continue;
    }

    // compute player symbol index in the mapstring
    location = get_location(players[i]);
    row = get_row(location);
    col = get_col(location);
    idx = (col - 1) + (row - 1) * (ncols + 1);

    // replace grid point symbol with player letters
    if (is_same_location(location, get_location(player))) {
      mapString[idx] = '@';
    }
    else {
      mapString[idx] = get_letter(players[i]);
    }
  }

  // the spectator sees all and knows all, their mapstring is complete at this point
  if (is_spectator(player)) {
    return update_visible_map(player, mapString);
  }

  // 3. unmark invisible grid points
  location = get_location(player);

  // loop through each gridpoint
  bool is_visible;
  for (int i = 0; i < grid_size; i++) {
    is_visible = compute_visibility(grid, location, gridPoints[i]);

    // replace invisible gridpoints with a blank space
    if (!is_visible) {
      row = get_row(gridPoints[i]);
      col = get_col(gridPoints[i]);
      idx = (col - 1) + (row - 1) * (ncols + 1);
      mapString[idx] = ' ';
    }
  }

  // 4. mark known grid points
  for (int i = 0; i < strlen(known_mapstring); i++) {
    // if gridpoint is known by player but invisible from the player's location,
    // replace blank space in the player's mapstring with default gridpoint symbol
    if (!isspace(known_mapstring[i]) && isspace(mapString[i])) {
      mapString[i] = default_mapstring[i];
    }
  }

  // map string is complete; return successfully
  return update_visible_map(player, mapString);
}


/**************** distribute_gold() ****************/
/* see server.h for description */
bool
distribute_gold(game_t* game, int min_piles, int max_piles) {
  // validate parameters
  if (game == NULL) {
    return false;
  }

  // generate random number of piles between specified range
  int num_piles = rand() % (max_piles - min_piles + 1) + min_piles;

  // 1. distribute the gold among the piles randomly

  // variables to hold gold amounts
  int gold_to_distribute = get_gold_balance(game);
  int gold_amounts[num_piles];
  int gold_avg = gold_to_distribute / num_piles;

  // distribute gold among piles in random amounts (minimum 1 nugget per pile)
  for (int i = 0; i < num_piles; i++) {
    gold_amounts[i] = rand() % gold_avg + 1;
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
  point_t* pile_locations[num_piles], *pile_location;
  int row, col;

  // compute random location to drop each gold pile
  for (int i = 0; i < num_piles; i++) {
    pile_location = get_empty_room_spot(game);
    row = get_row(pile_location);
    col = get_col(pile_location);
    pile_locations[i] = point_new(row, col, '*');
  }

  // 3. combine gold amount and location data and create gold piles

  // add new piles to the games pile list
  for (int i = 0; i < num_piles; i++) {
    pile_t* pile = pile_new(pile_locations[i], gold_amounts[i]);
    game->piles[i] = pile;
  }

  // 4. update number of gold piles in the game
  game->num_piles = num_piles;

  // return successfully
  return true;
}
