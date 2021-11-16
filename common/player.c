/* 
 * player.c - player module for the CS50 nuggets project
 *
 * Palmer's Posse, November 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "mem.h"
#include "player.h"
#include "../support/message.h"


/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct point {
  int x;
  int y;
} point_t;

typedef struct player {
  char* name;
  char* type;
  point_t* location;
  char* visible_map;
  addr_t* to;
  int purse;
  char playerLetter;
} player_t;


/**************** point_new ****************/
/*
 * see player.h for detailed description
 */
point_t*
point_new(int x, int y) {
  // allocate memory for point
  point_t* pt = mem_malloc(sizeof(point_t));

  // assign x and y coordinates
  pt->x = x;
  pt->y = y;

  // return object
  return pt;
}

/**************** get_x ****************/
/*
 * see player.h for detailed description
 */
int
get_x(point_t* point) {
  if(point != NULL){
    return point->x;
  }
  return -1;
}
// assumes player letter is not NULL
void set_player_letter(player_t* player, char playerLetter){
  if(player != NULL){
    player->playerLetter = playerLetter;
  }
}
// assumes player is not NULL
char get_player_letter(player_t* player){
  return player->playerLetter;
}
/**************** get_x ****************/
/*
 * see player.h for detailed description
 */
int
get_y(point_t* point) {
  if(point != NULL){
    return point->y;
  }
  return -1;
}

/**************** player_new ****************/
/*
 * see player.h for more information
 */
player_t*
player_new(char* name, char* type) {
  // validate parameters
  if (name != NULL && type != NULL) {

    // truncate name length to MaxNameLength
    int len;
    int MaxNameLength = 50;

    if (strlen(name) < MaxNameLength) {
      len = strlen(name);
    }

    else {
      len = MaxNameLength;
    }

    // initialize player's name copy
    char player_name[len+1];
    
    // format player's name
    for (int i = 0; i < len; i++) {
      if (!isgraph(name[i]) && !isblank(name[i])) {
        player_name[i] = '_';
      }
      else {
        player_name[i] = name[i];
      }
    }
    player_name[len] = '\0';

    // replace original with formatted copy
    // strcpy(name, player_name);

    // check that valid type has been provided
    if (strcmp(type, "player") == 0 || strcmp(type, "spectator") == 0) {
      // create player object
      player_t* player = mem_malloc(sizeof(player_t));

      // assign name, type and player
      // asssign null (or zero) for the rest of player variables
      if (player != NULL) {
        player->name = name;
        player->type = type;
        player->location = NULL;
        player->visible_map = NULL;
        player->to = NULL;
        player->purse = 0;
      }

      // return player_t object
      return player;
    }
  }

  // otherwise return null
  return NULL;
}


/**************** player_delete ****************/
/*
 * see player.h for more information
 */
void
player_delete(player_t* player) {
  if (player != NULL) {
    // free name
    if (NULL != player->name) {
      mem_free(player->name);
    }

    // free type
    if (NULL != player->type) {
      mem_free(player->type);
    }

    // free location
    if (NULL != player->location) {
      mem_free(player->location);
    }

    // free visible_map
    if (NULL != player->visible_map) {
      mem_free(player->visible_map);
    }

    // free address
    if (NULL != player->to) {
      mem_free(player->to);
    }

    // finally free player
    mem_free(player);
  }
}




/**************** getters ****************/
/*
 * see player.h for detailed description
 */
/**************** get_name ****************/
char*
get_name(player_t* player) {

  // if player exists
  if (player != NULL) {
    return player->name;
  }

  // otherwise return null
  return NULL;
}

/**************** get_type ****************/
char*
get_type(player_t* player) {

  // if player exists
  if (player != NULL) {
    return player->type;
  }

  // otherwise return null
  return NULL;
}

/**************** get_location ****************/
point_t*
get_location(player_t* player) {

  // if player exists
  if (player != NULL) {
    return player->location;
  }

  // otherwise return null
  return NULL;
}

/**************** get_visible_map ****************/
char*
get_visible_map(player_t* player) {

  // if player exists
  if (player != NULL) {
    return player->visible_map;
  }

  // otherwise return null
  return NULL;
}

/**************** get_address ****************/
addr_t*
get_address(player_t* player) {

  // if player exists
  if (player != NULL) {
    return player->to;
  }

  // otherwise return null
  return NULL;
}

/**************** get_purse ****************/
int
get_purse(player_t* player) {
  // if player exists
  if (player != NULL) {
    return player->purse;
  }

  // otherwise return zero
  return 0;
}




/**************** setters ****************/
/*
 * see player.h for detailed description
 */
/**************** set_location ****************/
bool
set_location(player_t* player, int x, int y) {
  if (player != NULL) {
    // create point object
    point_t* location = point_new(x, y);

    // assign point to player
    if (location != NULL) {
      player->location = location;
      return true;
    }
  }
  return false;
}

/**************** set_visible_map ****************/
bool
set_visible_map(player_t* player, char* visible_map) {
  if (player != NULL) {
    // assign map to player
    player->visible_map = visible_map;
    return true;
  }
  return false;
}

/**************** set_address ****************/
bool
set_address(player_t* player, addr_t* to) {
  if (player != NULL) {
    // assign address to player
    player->to = to;
    return true;
  }
  return false;
}

/**************** set_name ****************/
bool
set_name(player_t* player, char* name) {
  if (player != NULL && name != NULL) {
    // assign name to player
    player->name = name;
    return true;
  }
  return false;
}


/**************** add_to_purse ****************/
/*
 * see player.h for detailed description
 */
int
add_to_purse(player_t* player, int gold_coins) {
  if (player != NULL) {
    // add coins to purse
    player->purse += gold_coins;

    // return new value of purse
    return player->purse;
  }
  
  // otherwise return 0
  return 0;
}
