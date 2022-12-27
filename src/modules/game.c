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

  // get list of players in the game
  int num_players = get_num_players(game);
  player_t** players = get_players(game);

  // loop through list of players and find the spectator
  for (int i = 0; i < num_players; i++) {
    if (is_spectator(players[i])) {
      return players[i];
    }
  }

  // otherwise return unsuccessfully
  return NULL;
}

/************ add_player ************/
/* see game.h for description */
player_t*
add_player(game_t* game, const addr_t address, char* name) {
  // validate parameters
  if (game == NULL || !message_isAddr(address) || name == NULL) {
    return false;
  }

  // get new player's index in the list of players in the game
  int idx = get_num_players(game);

  // populate player's information
  char letter = (char) 'A' + idx;
  point_t* location = get_empty_room_spot(game);
  int mapstring_len = strlen(get_map_string(get_grid(game)));
  char* visible_map = mem_malloc(sizeof(char) * mapstring_len + 1);

  // create new player
  player_t* player = player_new(name, letter, location, visible_map, address);
  if (player == NULL) {
    return NULL;
  }

  // update player's visible map string
  build_visible_mapstring(game, player);

  // add player to list of players in the game
  player_t** players = get_players(game);
  players[idx] = player;

  // increment number of players in the game
  update_num_players(game);

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

  // load grid points array from grid
  point_t** gridPoints = get_gridpoints(grid);
  if (gridPoints == NULL || gridPoints[0] == NULL) {
    // run build_grid first
    return false;
  }

  // make a copy of the grid's map string
  char* grid_mapstring = get_map_string(grid);
  char* mapString = get_visible_map(player);
  strcpy(mapString, grid_mapstring);

  // location variables
  point_t* location;
  int idx, row, col;
  int ncols = get_ncols(grid);

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
    if (players[i] == player) {
      mapString[idx] = '@';
    }
    else {
      mapString[idx] = get_letter(players[i]);
    }
  }

  // 3. mark visible grid spots
  location = get_location(player);

  // loop through each gridpoint
  for (int i = 0; i < grid_size; i++) {
    bool is_visible = compute_visibility(grid, location, gridPoints[i]);

    // replace grid point symbol with ' ' if grid point not visible to the player
    if (!is_visible) {
      row = get_row(gridPoints[i]);
      col = get_col(gridPoints[i]);
      idx = (col - 1) + (row - 1) * (ncols + 1);
      mapString[idx] = ' ';
    }
  }

  // return complete map string
  return true;
}
