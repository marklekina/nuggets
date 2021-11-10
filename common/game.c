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
  int spectatorNUM;
  player_t* spectator;
  char* map; // (txt file)
  player_t* [maxnumber] players; 
} game_t;

/**************** game_new ****************/
/*
 * see game.h for more information
 */

game_t*
game_new(map){
  if (name == NULL || type == NULL){
    return NULL;
  }
  game_t* game = mem_malloc(sizeof(game_t));

  if(loadMap){
    edit load map
    game->grid = map; 
  }
  if(player == NULL){
    return NULL;
  }
  spect = 0;
  else{
    
  }

}

/**************** game_delete ****************/
/*
 * see game.h for more information
 */
game_t*
game_delete(game_t* game){
  //if(NULL !=) free the things in the player
  mem_free(game);
}

void removeSpectator(){
  delete_player(spectator);
}

bool
spectatorAdd(game_t* game, player_t* player){
  if(strcmp(player, "spectator")==1){
    return false
  }
  if(game->spectatorNum){}
  if (spectatorNUM!= 1){
    spectator = player;
    spectatorNUM = 1;
    return true
  }
  else{
    removeSpectator;
    spectator = player;
    return true;
  }

}
