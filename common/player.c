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
#include <ctype.h>
#include "player.h"
#include "mem.h"
#include "../support/message.h"
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
  addr_t to;
  int purse;
} player_t;

/**************** player_new ****************/
/*
 * see player.h for more information
 */
player_t*
player_new(char* name, char* type){
  int MaxNameLength = 50;
  if (type == NULL){
    return NULL;
  }
  if(name == NULL){
    name = " ";
  }
  if(strcmp(type, "player")==1 || strcmp(type, "spectator")==1){
    return NULL;
  }
  player_t* player = mem_malloc(sizeof(player_t));

	int count = 0;
	for(int i = 0; i<MaxNameLength; i++){
		count ++;
		if(isgraph(player->name[i]) == 0 && isblank(player->name[i]) ==0){
			player->name[i] = '_';
		}

  
  if(player == NULL){
    return NULL;
  }
  else{
    player->name = name;
    player->visibility = NULL;
    player->type = type;
    // are we still doing a global variable for the size of the map? need to give it a limit
    // grid_t* grid = game_getGrid(game);
    // char* map = grid->map;
    // // if people pass in seed and then use random number from docID
    // grid_delete(grid);
    // player->purse = 0;
    // player->xPos = rand() % grid_getnRows(grid);
    // player->yPos = rand() % grid_getnCols(grid);
  }

  }
  return player;
}

/**************** player_delete ****************/
/*
 * see player.h for more information
 */
void player_delete(player_t* player){
   if (NULL != player->name) {
    mem_free(player->name);
  }
  if (NULL != player->visibility) {
    mem_free(player->visibility);
  }
  if (NULL != player->type) {
    mem_free(player->type);
  }
  
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

char* player_getName(player_t* player){
  if(player!= NULL && player->name != NULL){
    return player->name;
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
   if(player!= NULL && &player->xPos != NULL){
    return player->xPos;
  }
  return 0;
}

int player_getyPos(player_t* player){
   if(player!= NULL && &player->yPos != NULL){
    return player->yPos;
  }
  return 0;
}


/**************** set_visibility ****************/
/*
 * see player.h for more information
 */
bool
player_setVisibility(player_t* player, char* visibility){
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
player_setType(player_t* player, char* type){
  if (player == NULL || type == NULL){
    return false;
  }
  player->type = type;
  if(player->type == NULL){
    return false;
  }
  else{
    return true;
  }
  return false;
}



/**************** set_xPos ****************/
/*
 * see player.h for more information
 */
bool
player_setxPos(player_t* player, int xPos){
  if (player == NULL){
    return false;
  }
  player->xPos = xPos;
  if(&player->xPos == NULL){
    return false;
  }
  else{
    return true;
  }
  return false;
}


/**************** set_yPos ****************/
/*
 * see player.h for more information
 */
bool
player_setyPos(player_t* player, int yPos){
  if (player == NULL){
    return false;
  }
  player->yPos = yPos;
  if(&player->yPos == NULL){
    return false;
  }
  else{
    return true;
  }
  return false;
}

void addToPurse(player_t* player, int numGold, int GoldTotal){
  if(player->purse <= GoldTotal){
    player->purse += numGold;
  }

}