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
#include "file.h"

/**************** file-local global variables ****************/
//copy in variables

/**************** global types ****************/
typedef struct game {
  grid_t* grid; // (turns map into string)
  player_t* spectator;
  FILE* map; // (txt file)
  player_t* [maxnumber] players; 
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
  if(!load_map(map)){
    return NULL;
  }
  else{
    game->grid = map; 
    game->spectator = NULL;
    game->players = NULL;

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
    for(int i = 0; i<MaxPlayers; i++){
      player_delete(game->players[i]);
    }
  }
  mem_free(game);
}

void removeSpectator(game_t* game){
  //send message???
  delete_player(game->spectator);

}

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

bool
addPlayer(game_t* game, char* name, char* type){
  
}
	

///getters and setters
// add player(game and player) 
// gold distribution
//variables for gold gold-player just picked up 
//total gold left 
//update game, variables 

updategame(){}
player_t* get__Player(game_t* game, player_t* player){

  if (game == NULL || player == NULL) {
    return NULL;             
  } else {
    int i = 0;
    while(i<=MaxPlayers && i != NULL){
        if(strcmp(player_getName(game->players[i]), player_getName(player)==0){
          return game ->players[i];
        }
        i+=1;

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