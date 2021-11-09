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
/* none */

/**************** global types ****************/
typedef struct game {
  
  
} game_t;

/**************** game_new ****************/
/*
 * see game.h for more information
 */
game_t*
game_new(){
  if (name == NULL || type == NULL){
    return NULL;
  }
  game_t* game = mem_malloc(sizeof(game_t));
  if(player == NULL){
    return NULL;
  }
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

