/*
 * game.c - game module for the CS50 nuggets project
 *
 * see game.h for more information.
 *
 * Palmer's Posse, November 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mem.h"
#include "file.h"
#include "game.h"
#include "grid.h"
#include "player.h"


/**************** global types ****************/
typedef struct game {
  grid_t* grid;          // holds grid information
  player_t* spectator;   // one game spectator
  player_t* players[26]; // array of game players
  int num_players;       // number of players currently in the game
} game_t;

/**************** game_new ****************/
/*
 * see game.h for more information
 */

game_t*
game_new(FILE* fp, int nrows, int ncols) {
  // if fp != null, nrows and ncols != 0
  if (fp != NULL && nrows > 0 && ncols > 0) {
    // allocate memory for game object
    game_t* game = mem_malloc(sizeof(game_t));

    // initialize a grid object inside game to hold grid info
    // load map into grid
    if (game != NULL) {
      grid_t* grid = grid_new(nrows, ncols);
      load_map(grid, fp);
      
      // assign grid to game
      game->grid = grid;

      // initialize the rest of the variables as null (or zero)
      game->spectator = NULL;
      game->players = NULL;
      game->num_players = 0;
    }

    // return game object
    return game;
  }
  
  // otherwise return null
  return NULL;
}

/**************** game_delete ****************/
/*
 * see game.h for more information
 */
void
game_delete(game_t* game) {
  // free the spectator
  if (NULL != game->spectator) {
    player_delete(game->spectator);
  }

  // loop through the players array and free each of them
  if (NULL != game->players) {
    for(int i = 0; i < game->num_players; i++){

      // free the player
      if (NULL != game->players[i]) {
        player_delete(game->players[i]);
      }
    }
  }
  
  // free game object
  mem_free(game);
}


/**************** remove_spectator ****************/
/* 
 * see game.h for detailed description
 */
void
remove_spectator(game_t* game) {
  // send quit message to current spectator
  player_t* spectator = get_spectator(game);

  if (spectator != NULL) {
    message_send(spectator->to, "QUIT You have been replaced by a new spectator\n");

    // delete player object
    player_delete(spectator);
  }
}


/**************** add_spectator ****************/
/* 
 * see game.h for detailed description
 */
bool
add_spectator(game_t* game) {
  // validate parameters
  if (game != NULL) {
    // if a spectator is already present, kick them out
    if (game->spectator != NULL){
      removeSpectator(game);
    }

    // create player_t object of spectator type
    // assign them to game and return true
    // NB: name parameter doesn't matter for spectator, we never use it
    // to solve potential double free errors, reassign spectator name to null
    player_t* player = player_new("spectator", "spectator");
    if (player != NULL) {
      player->name = NULL;
      game->spectator = player;
      return true;
    }
  }
  
  // otherwise return false
  return false;
}

/**************** add_player ****************/
/* 
 * see game.h for description
 */
int
add_player(game_t* game, char* name) {
  // validate parameters
  if (game != NULL && name != NULL) {
    // create player_t object of player type
    player_t* player = player_new(name, "player");

    if (player != NULL) {
      // add player to array of players
      // increment number of players in game
      game->players[game->num_players] = player;
      game->num_players += 1;

      // return player_id if successful
      return game->num_players;
    }
  }
  
  // otherwise return invalid index
  return -1;
}


/**************** getters ****************/
/*
 * see game.h for description
 */
int
get_num_players(game_t* game) {
  if (game != NULL) {
    return game->num_players;
  }
  return 0;
}

player_t*
get_player(game_t* game, int idx){
  if (game != NULL) {
    if (game->players != NULL) {
      return game->players[idx];
    }
  }
  return NULL;
}

grid_t*
get_grid(game_t* game){
  if (game != NULL){
    return game->grid;
  }
  return NULL;
}

player_t*
get_spectator(game_t* game) {
  if (game!= NULL) {
    return game->spectator;
  }
  return NULL;
}
