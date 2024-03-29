/*
 * player.c - 'player' module
 *
 * see player.h for more information.
 *
 * Mark Lekina Rorat, December 2021; November 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "message.h"
#include "point.h"
#include "player.h"


/**************** types ****************/
typedef struct player {
  char* name;           // player's name
  char letter;          // player's letter (as displayed on the map)
  point_t* location;    // player location
  char* visible_map;    // section of the map that's visible to the player
  int wallet;           // holds gold collected by player
  addr_t address;       // player client's Internet address
} player_t;

/**************** functions ****************/

/**************** player_new() ****************/
/* see player.h for description */
player_t*
player_new(char* name, char letter, point_t* location, char* visible_map, addr_t address)
{
  // validate parameters
  if (name == NULL || visible_map == NULL) {
    return NULL;
  }

  // allow NULL location only if spectator
  if (strcmp(name, "_SPECTATOR_") != 0 && location == NULL) {
    return NULL;
  }

  if (letter == '\0') {
    return NULL;
  }

  if (!message_isAddr(address)) {
    return NULL;
  }

  // allocate memory for player
  player_t* player = mem_malloc(sizeof(player_t));

  // memory allocation error
  if (player == NULL) {
    return NULL;
  }

  // assign parameters to player
  player->name = name;
  player->letter = letter;
  player->location = location;
  player->visible_map = visible_map;
  player->wallet = 0;
  player->address = address;

  // return struct
  return player;
}


/**************** player_delete() ****************/
/* see player.h for description */
void
player_delete(player_t* player)
{
  if (player == NULL) {
    return;
  }

  // free player variables memory
  mem_free(player->name);
  mem_free(player->visible_map);

  // free player memory
  mem_free(player);
  player = NULL;
}


/**************** get_name() ****************/
/* see player.h for description */
char*
get_name(player_t* player)
{
  if (player == NULL) {
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
    return (char) 0;
  }
  return player->letter;
}


/**************** get_address() ****************/
/* see player.h for description */
addr_t
get_address(player_t* player)
{
  if (player == NULL) {
    return message_noAddr();
  }
  return player->address;
}


/**************** get_location() ****************/
/* see player.h for description */
point_t*
get_location(player_t* player)
{
  if (player == NULL) {
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
    return false;
  }

  // assign visible map string to player
  mem_free(player->visible_map);
  player->visible_map = visible_map;
  return true;
}


/**************** get_wallet_balance() ****************/
/* see player.h for description */
int
get_wallet_balance(player_t* player) {
  if (player == NULL) {
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
    return false;
  }

  // otherwise add gold collected to player's wallet
  player->wallet += gold;
  return true;
}


/************ is_spectator() ************/
/* see player.h for description */
bool
is_spectator(player_t* player) {
  // validate parameters
  if (player == NULL) {
    return false;
  }

  // get player information
  char* name = get_name(player);
  char letter = get_letter(player);
  point_t* location = get_location(player);
  int wallet = get_wallet_balance(player);

  // check that info matches spectator information and return successfully
  if (wallet == 0 && letter == '_' && location == NULL && strcmp(name, "_SPECTATOR_") == 0) {
    return true;
  }

  // otherwise return unsuccessfully
  return false;
}


/************ update_spectator() ************/
/* see player.h for description */
bool
update_spectator(player_t* spectator, const addr_t address) {
  // validate parameters
  if (spectator == NULL || !message_isAddr(address)) {
    return false;
  }

  // set spectator's address to new address
  spectator->address = address;
  return true;
}


/************ compare_player_wallets() ************/
/* see player.h for description */
int
compare_player_wallets(const void* a, const void* b) {
  // cast arguments to player_t*
  player_t* player_a = *(player_t**) a;
  player_t* player_b = *(player_t**) b;

  // get wallet balance for both players
  int wallet_a = get_wallet_balance(player_a);
  int wallet_b = get_wallet_balance(player_b);

  // compute and return the "reverse" difference (reverse for descending order)
  return wallet_b - wallet_a;
}
