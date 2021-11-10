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
player_new(char* name, char* type, game_t* game){
  if (name == NULL || type == NULL){
    return NULL;
  }
  if(strcmp(type, "player")==1 || strcmp(type, "spectator")==1){
    return NULL;
  }

	int count = 0;
	for(int i = 0; i<MaxNameLength; i++){
		count ++;
		if(isgraph(player->name[i]) == 0 && isblank(player->name[i]) ==0){
			player->name[i] = '_';
		}

  player_t* player = mem_malloc(sizeof(player_t));
  if(player == NULL){
    return NULL;
  }
  else{
    player->name = name;
    player->visibility = NULL;
    player->type = type;
    // are we still doing a global variable for the size of the map? need to give it a limit
    grid_t* grid = game_getGrid(game);
    char* map = grid->map;
    int length = strlen(map);
    strand(map);
    grid_delete(grid);
    player->purse = 0;
    player->xPos = rand();
    player->yPos = rand();
  }

}

/**************** player_delete ****************/
/*
 * see player.h for more information
 */
player_t*
player_delete(player_t* player){
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

}char* player_getName(player_t* player){
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
  if (player == NULL || xPos == NULL){
    return false;
  }
  player->xPos = xPos;
  if(player->xPos == NULL){
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
  if (player == NULL || yPos == NULL){
    return false;
  }
  player->yPos = yPos;
  if(player->yPos == NULL){
    return false;
  }
  else{
    return true;
  }
  return false;
}

void addToPurse(int numGold){
  if(player->purse <= GoldTotal){
    player->purse += numGold;
  }

}