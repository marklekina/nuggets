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

/**************** file-local global variables ****************/
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles

/**************** global types ****************/
typedef struct game {
  grid_t* grid; // (turns map into string)
  player_t* spectator;
  FILE* map; // (txt file)
  player_t* [maxnumber] players; 
  int location;
  int seed;
} game_t;

/**************** game_new ****************/
/*
 * see game.h for more information
 */

game_t*
game_new(FILE* map){
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
    game->players = NULL;
    game->location = 0;

  }
  return game;
}

/**************** game_delete ****************/
/*
 * see game.h for more information
 */
game_t*
game_delete(game_t* game){
   if (NULL != game->spectator) {
    player_delete(game->spectator);
  }
  if (NULL != game->players) {
    for(int i = 0; i<location; i++){
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
  delete_player(game->spectator);

}

/**************** spectatorAdd ****************/
/* 
 * comments
 */
bool
spectatorAdd(game_t* game, player_t* player){
  if(strcmp(player, "spectator")==1){
    return false
  }
  if (game->spectator == NULL){
    game->spectator = player;
    return true
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
player_t*
game_addPlayer(game_t* game, char* name, char* type){
  if (name == NULL || type == NULL){
    return NULL;
  }
  player_t* player = player_new(name, type);
  game->players[location] = player; 
  game->location += 1;
  return player;
}
	
player_t* get_Player(game_t* game, player_t* player){

  if (game == NULL || player == NULL) {
    return NULL;             
  } else {
    int i = 0;
    for(int i = 0; i<location; i++){
        if(strcmp(player_getName(game->players[i]), player_getName(player)==0){
          return game->players[i];
        }
    }
    }
  return NULL;
}
char* game_getMap(game_t* game){
  if(game!= NULL && game->map != NULL){
    return game->map;
  }
  return NULL;
}
char* game_getGrid(game_t* game){
  if(game!= NULL && game->grid != NULL){
    return game->grid;
  }
  return NULL;
}

char* game_getSpectator(game_t* game){
  if(game!= NULL && game->spectator != NULL){
    return game->spectator;
  }
  return NULL;
}