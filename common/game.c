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
  if (map == NULL ){
    return NULL;
  }
  game_t* game = mem_malloc(sizeof(game_t));
//POTENTIALLY FIX THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  game->map = map;
  grid_t* grid = grid_new(nrows, ncols);
  if(!load_map(grid, map)){
    return NULL;
  }
  else{
    game->grid = grid; 
    game->spectator = NULL;
    // game->players = NULL;
    game->location = 0;

  }
  return game;
}

/**************** game_delete ****************/
/*
 * see game.h for more information
 */
void game_delete(game_t* game){
   if (NULL != game->spectator) {
    player_delete(game->spectator);
  }
  if (NULL != game->players) {
    for(int i = 0; i<game->location; i++){
      player_delete(game->players[i]);
    }
  }
  mem_free(game);
}

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