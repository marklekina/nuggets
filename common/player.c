/* 
 * player module
 *
 * see player.h for more information.
 *
 * Palmer's Posse, November 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "player.h"
#include "mem.h"
#include "file.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct player {
  char* name;
  char* visibility
  int number;
  char* type;
  int xPos;
  int yPos;
  
} player_t;

/**************** player_new ****************/
/*
 * see player.h for more information
 */
player_t*
player_new(char* name){
  if (name == NULL){
    return NULL;
  }
  player_t* player = mem_malloc(sizeof(player_t));
  if(player == NULL){
    return NULL;
  }
  else{
    player->name = name;
  }

}

/**************** player_delete ****************/
/*
 * see player.h for more information
 */
player_t*
player_delete(player_t* player){
  //if(NULL !=) free the things in the player
  mem_free(player);
}
/* *********************************************************************** */
/* getter methods - see player.h for documentation */
char* get_visibility(player_t* player){
  return player ? player->visibility : 0;
}

char* get_type(player_t* player){
  return player ? player->type : 0;
}

int get_xPos(player_t* player){
  return player ? player->xPos : 0;
}

int get_yPos(player_t* player){
  return player ? player->yPos : 0; 
}
/**************** set_visibility ****************/
/*
 * see player.h for more information
 */
bool
set_visibility(player_t* player, char* visibility){
  if (player == NULL || visibility == NULL){
    return false;
  }
  // have to make 
  else{
    if(()){
      new->
    }
  }

}


/**************** set_type ****************/
/*
 * see player.h for more information
 */
player_t*
set_type(player_t* player, char* type){}



/**************** set_xPos ****************/
/*
 * see player.h for more information
 */
player_t*
set_xPos(player_t* player, int xPos){}


/**************** set_yPos ****************/
/*
 * see player.h for more information
 */
player_t*
set_yPos(player_t* player, int yPos){}