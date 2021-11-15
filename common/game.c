/* 
 * game module
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
  grid_t* grid; // (turns map into string)
  player_t* spectator;
  FILE* map; // (txt file)
  player_t* players[26]; 
  int location;
  int seed;
} game_t;

/**************** game_new ****************/
/*
 * see game.h for more information
 */

game_t*
game_new(FILE* map, int nrows, int ncols){
  // null check
  if (map == NULL ){
    return NULL;
  }
  // allocate memory for the game
  game_t* game = mem_malloc(sizeof(game_t));

  // set the map and create a grid
  game->map = map;
  grid_t* grid = grid_new(nrows, ncols);
  
  // check that the map can be loaded into a string and return null if not
  if(!load_map(grid, map)){
    return NULL;
  }
  // populate the game with initial values
  else{
    game->grid = grid; 
    game->spectator = NULL;
    game->location = 0;

  }
  // return the starting game module
  return game;
}

/**************** game_delete ****************/
/*
 * see game.h for more information
 */
void game_delete(game_t* game){
  // delete the spectator, if there is one
   if (NULL != game->spectator) {
    player_delete(game->spectator);
  }
  // delete all the players, if there are any
  if (NULL != game->players) {
    for(int i = 0; i<game->location; i++){
      player_delete(game->players[i]);
    }
  }
  // free the memory of the game
  mem_free(game);
}

/**************** removeSpectator ****************/
/* 
 * see game.h for more information
 */
void removeSpectator(game_t* game){
  // delete the spectator player
  player_delete(game->spectator);

}

/**************** spectatorAdd ****************/
/* 
 * see game.h for more information
 */
bool
spectatorAdd(game_t* game, player_t* player){
  // check if the correct player type was passed in
  if(strcmp(player->type, "spectator")==1){
    return false;
  }
  // check if the game already has a spectator
  if (game->spectator == NULL){
    // if it doesn't set the spectator to the passed in player
    game->spectator = player;
    return true;
  }
  // if the gmae does have a spectator
  else{
    // remove the current one and add the passed in player
    removeSpectator(game);
    game->spectator = player;
    return true;
  }

}

/**************** game_addPlayer ****************/
/* 
 * see game.h for more information
 */
bool game_addPlayer(game_t* game, char* name, char* type){
  // null checks
  if (name == NULL || type == NULL){
    return false;
  }
  // create a new player
  player_t* player = player_new(name, type);
  // add them to the players array
  game->players[game->location] = player; 
  // iterate the location in the array
  game->location += 1;
  return true;
}

/**************** get_location ****************/
/* 
 * see game.h for more information
 */
int get_location(game_t* game){
  // return the how many players are in the game
   if(game != NULL){
    return game->location;
  }
  return 0;
}

/**************** get_Player ****************/
/* 
 * see game.h for more information
 */
player_t* get_Player(game_t* game, int location){
  if (game == NULL || location <= 0) {
    return NULL;             
  } 
  else {
    for(int i = 0; i<game->location; i++){
      if(i==location){
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

bool game_setGrid(game_t* game, grid_t* grid){
  if(game != NULL && grid != NULL){
    game->grid = grid;
    return true;
  }
  return false;
}

player_t* game_getSpectator(game_t* game){
  if(game!= NULL && game->spectator != NULL){
    return game->spectator;
  }
  return NULL;
}

bool game_setSeed(game_t* game){
  if(game!= NULL && seed >=0){
    game->seed = seed;
    return true;
  }
  return false;
}

void game_setMap(game_t* game, FILE* map){
   if(game!= NULL && map == NULL){
    game->map = map;
  }
}