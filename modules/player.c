/* 
 * player.c - 'player' module
 *
 * see player.h for more information.
 *
 * Mark Lekina Rorat, December 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "point.h"
#include "player.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct player {
  char* name;           // player's name
  char letter;          // player's letter (as displayed on the map)
  point_t* location;    // player location
  int purse;            // gold amount collected by player
  char* visible_map;    // section of the map that's visible to the player
} player_t;

/**************** global functions ****************/

/**************** player_new() ****************/
/* see player.h for description */
player_t*
player_new(char* name, char letter)
{
  // validate parameters
  if (name != NULL) {
    // allocate memory for player
    player_t* player = mem_malloc(sizeof(player_t));

    // assign parameters to player
    if (player != NULL) {
      player->name = name;
      player->letter = letter;
      player->location = NULL;
      player->purse = 0;
      player->visible_map = NULL;
    }

    // return struct
    return player;
  }

  // otherwise return null
  return NULL;
}

/**************** player_delete() ****************/
/* see player.h for description */
void
player_delete(player_t* player)
{
  if (player != NULL) {
    // free player variables memory
    mem_free(player->name);
    mem_free(player->visible_map);
    point_delete(player->location);
    
    // free player memory
    mem_free(player);
  }
}

/**************** setters ****************/

/**************** update_location() ****************/
/* see player.h for description */
bool
update_location(player_t* player, point_t* location) {
  // assign location to player if parameters are valid
  if (player != NULL && location != NULL) {
    player->location = location;
    return true;
  }
  
  // otherwise return false
  return false;
}

/**************** update_visible_map() ****************/
/* see player.h for description */
bool
update_visible_map(player_t* player, char* visible_map) {
  // assign visible map string to player if parameters are valid
  if (player != NULL && visible_map != NULL) {
    player->visible_map = visible_map;
    return true;
  }
  
  // otherwise return false
  return false;
}

/**************** add_to_purse() ****************/
/* see player.h for description */
int
add_to_purse(player_t* player, int gold_collected) {
  // add gold collected to player's purse if player exists
  // return total amount of gold collected by the player
  if (player != NULL) {
    player->purse += gold_collected;
    return player->purse;
  }

  // otherwise return zero
  return zero;
}
