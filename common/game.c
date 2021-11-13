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
#include "game.h"
#include "mem.h"
#include "player.h"
#include "file.h"
#include "grid.h"


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
game_new(FILE* fp, int nrows, int ncols){
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
game_delete(game_t* game){
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


/*
 * TODO: continue making edits from here
 */

/**************** removeSpectator ****************/
/* 
 * comments
 */
void removeSpectator(game_t* game){
  //send message???
  player_delete(game->spectator);

}

/**************** spectatorAdd ****************/
/* 
 * comments
 */
bool
spectatorAdd(game_t* game, player_t* player){
  if(strcmp(player->type, "spectator")==1){
    return false;
  }
  if (game->spectator == NULL){
    game->spectator = player;
    return true;
  }
  else{
    removeSpectator(game);
    game->spectator = player;
    return true;
  }

}

/**************** game_addPlayer ****************/
/* 
 * comments
 */
bool
game_addPlayer(game_t* game, char* name, char* type){
  if (name == NULL || type == NULL){
    return false;
  }
  player_t* player = player_new(name, type);
  game->players[game->location] = player; 
  game->location += 1;
  return true;
}

int get_location(game_t* game){
   if(game!= NULL){
    return game->location;
  }
  return 0;
}
	
player_t* get_Player(game_t* game, player_t* player){

  if (game == NULL || player == NULL) {
    return NULL;             
  } 
  else {
    for(int i = 0; i<game->location; i++){
        if(strcmp(player_getName(game->players[i]), player_getName(player))==0){
          return game->players[i];
        }
    }
    }
  return NULL;
}

FILE* game_getMap(game_t* game){
  if(game!= NULL && game->map != NULL){
    return game->map;
  }
  return NULL;
}

grid_t* game_getGrid(game_t* game){
  if(game!= NULL && game->grid != NULL){
    return game->grid;
  }
  return NULL;
}

player_t* game_getSpectator(game_t* game){
  if(game!= NULL && game->spectator != NULL){
    return game->spectator;
  }
  return NULL;
}
