/*
 * player.c - 'player' module
 *
 * see player.h for more information.
 *
 * Mark Lekina Rorat, December 2021; November 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "log.h"
#include "point.h"
#include "player.h"


/**************** types ****************/
typedef struct player {
  char* name;           // player's name
  char letter;          // player's letter (as displayed on the map)
  point_t* location;    // player location
  char* visible_map;    // section of the map that's visible to the player
  int wallet;           // holds gold collected by player
} player_t;

/**************** functions ****************/

/**************** player_new() ****************/
/* see player.h for description */
player_t*
player_new(char* name, char letter, point_t* location, char* visible_map)
{
  // validate parameters
  if (name == NULL || location == NULL || visible_map == NULL) {
    log_v("player_new: NULL name, location or visible map string passed to function");
    return NULL;
  }

  if (letter == '\0') {
    log_v("player_new: invalid letter passed to function");
    return NULL;
  }

  // allocate memory for player
  player_t* player = mem_malloc(sizeof(player_t));

  // log memory allocation error
  if (player == NULL) {
    log_v("player_new: error allocating memory for player struct");
    return NULL;
  }

  // assign parameters to player
  player->name = name;
  player->letter = letter;
  player->location = location;
  player->visible_map = visible_map;
  player->wallet = 0;

  // return struct
  return player;
}


/**************** player_delete() ****************/
/* see player.h for description */
void
player_delete(player_t* player)
{
  if (player == NULL) {
    log_v("player_delete: NULL player passed to function");
    return;
  }

  // free player variables memory
  mem_free(player->name);
  mem_free(player->visible_map);
  point_delete(player->location);

  // free player memory
  mem_free(player);
}


/**************** get_name() ****************/
/* see player.h for description */
char*
get_name(player_t* player)
{
  if (player == NULL) {
    log_v("get_name: NULL player passed to function");
    return NULL;
  }
  return player->name;
}


/**************** get_letter() ****************/
/* see player.h for description */
char
get_letter(player_t* player)
{
  if (player == NULL) {
    log_v("get_letter: NULL player passed to function");
    return (char) 0;
  }
  return player->letter;
}


/**************** get_location() ****************/
/* see player.h for description */
point_t*
get_location(player_t* player)
{
  if (player == NULL) {
    log_v("get_location: NULL player passed to function");
    return NULL;
  }
  return player->location;
}


/**************** update_location() ****************/
/* see player.h for description */
bool
update_location(player_t* player, point_t* location) {
  // validate parameters
  if (player == NULL || location == NULL) {
    log_v("update_location: NULL player or location passed to function");
    return false;
  }

  // assign location to player
  player->location = location;
  return true;
}


/**************** get_visible_map() ****************/
/* see player.h for description */
char*
get_visible_map(player_t* player)
{
  if (player == NULL) {
    log_v("get_visible_map: NULL player passed to function");
    return NULL;
  }
  return player->visible_map;
}


/**************** update_visible_map() ****************/
/* see player.h for description */
bool
update_visible_map(player_t* player, char* visible_map) {
  // validate parameters
  if (player == NULL || visible_map == NULL) {
    log_v("update_visible_map: NULL player or visible map string passed to function");
    return false;
  }

  // assign visible map string to player
  player->visible_map = visible_map;
  return true;
}


/**************** get_wallet_balance() ****************/
/* see player.h for description */
int
get_wallet_balance(player_t* player) {
  if (player == NULL) {
    log_v("get_wallet_balance: NULL player passed to function");
    return -1;
  }
  return player->wallet;
}


/**************** add_to_wallet() ****************/
/* see player.h for description */
bool
update_wallet_balance(player_t* player, int gold) {
  // validate parameters
  if (player == NULL || gold <= 0) {
    log_d("update_wallet_balance: NULL player or invalid gold amount (%d) passed to function", gold);
    return false;
  }

  // otherwise add gold collected to player's wallet
  player->wallet += gold;
  return true;
}
