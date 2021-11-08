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
  char* visibility;
  char* type;
  int xPos;
  int yPos;
  int purse;
  
} player_t;

/**************** player_new ****************/
/*
 * see player.h for more information
 */
player_t*
player_new(char* name, char* type){
  if (name == NULL || type == NULL){
    return NULL;
  }
  player_t* player = mem_malloc(sizeof(player_t));
  if(player == NULL){
    return NULL;
  }
  else{
    player->name = name;
    //should be all 0s
    player->visibility = NULL;
    player->type = type;
    // random number
    player->purse = 0;
    player->xPos = NULL;
    player->yPos = NULL;
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
char* player_getVisibility(player_t* player){
  if(player!= NULL && player->visibility != NULL){
    return player->visibility;
  }
  return NULL;
}

char* player_getType(player_t* player){
   if(player != NULL && player->type != NULL){
    return player->type;
  }
  return NULL;
}

int player_getxPos(player_t* player){
   if(player!= NULL && player->xPos != NULL){
    return player->xPos;
  }
  return 0;
}

int player_getyPos(player_t* player){
   if(player!= NULL && player->yPos != NULL){
    return player->yPos;
  }
  return 0;
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
  player->visibility = visibility;
  if(player->visibility == NULL){
    return false;
  }
  else{
    return true;
  }
  return false;


}


/**************** set_type ****************/
/*
 * see player.h for more information
 */
bool
set_type(player_t* player, char* type){}



/**************** set_xPos ****************/
/*
 * see player.h for more information
 */
bool
set_xPos(player_t* player, int xPos){}


/**************** set_yPos ****************/
/*
 * see player.h for more information
 */
bool
set_yPos(player_t* player, int yPos){}