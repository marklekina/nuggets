/*
 * player.h - header file for 'player' module
 *
 * A 'player' is a struct that holds information relating to a specific client in the game.
 *
 * Mark Lekina Rorat, December 2021; October 2022
 */

#ifndef __PLAYER_H
#define __PLAYER_H

#include "point.h"
#include "message.h"

/**************** global types ****************/
typedef struct player player_t;

/**************** functions ****************/

/**************** player_new ****************/
/*
 * creates a new player
 *
 * caller provides:
 *  a valid player's name (string)
 *  a valid player's letter representation (character)
 *  a valid room spot location in the grid (point_t*)
 *  a string with the portion of the grid that is visible to the player
 *
 * we return:
 *   pointer to a new player, or NULL if error.
 *
 * caller is responsible for:
 *   calling player_delete to free the player's memory.
 */
player_t* player_new(
  char* name,
  char letter,
  point_t* location,
  char* visible_map,
  addr_t address
);

/**************** player_delete ****************/
/*
 * deletes the player.
 *
 * caller provides:
 *   a valid player pointer.
 *
 * we guarantee:
 *   we free the player's name string
 *   we free the player's visible map string
 *   we free the player's location memory
 *   we free the player's memory
 *   we ignore NULL player.
 */
void player_delete(player_t* player);

/**************** get_name ****************/
/*
 * returns the player's name.
 *
 * caller provides:
 *   a valid player pointer.
 *
 * we return:
 *   the player's name.
 */
char* get_name(player_t* player);

/**************** get_letter ****************/
/*
 * returns the player's letter representation on the grid.
 *
 * caller provides:
 *   a valid player pointer.
 *
 * we return:
 *   the player's letter.
 */
char get_letter(player_t* player);

/**************** get_location ****************/
/*
 * returns the player's location.
 *
 * caller provides:
 *   a valid player pointer.
 *
 * we return:
 *   a pointer to the player's location.
 */
point_t* get_location(player_t* player);

/**************** get_address ****************/
/*
 * returns the player client's Internet address.
 *
 * caller provides:
 *   a valid player pointer.
 *
 * we return:
 *   the player's address.
 */
addr_t get_address(player_t* player);

/**************** update_location ****************/
/*
 * sets player's location to new location
 *
 * caller provides:
 *    a valid pointer to a player
 *    a valid location pointer
 *
 * we return:
 *    true if the player's location is successfully updated
 *    false if update is unsuccessful
 */
bool update_location(player_t* player, point_t* location);

/**************** get_visible_map ****************/
/*
 * returns the player's visible section of the grid.
 *
 * caller provides:
 *   a valid player pointer.
 *
 * we return:
 *   the section of the grid visible to the player.
 */
char* get_visible_map(player_t* player);

/**************** update_visible_map ****************/
/*
 * updates the section of the grid visible to a player
 *
 * caller provides:
 *    a valid pointer to the player
 *    a valid pointer to the map string
 *
 * we return:
 *    true if the visible map string is successfully updated
 *    false if update is unsuccessful
 */
bool update_visible_map(player_t* player, char* visible_map);

/**************** get_wallet_balance ****************/
/*
 * returns the player's gold balance.
 *
 * caller provides:
 *   a valid player pointer.
 *
 * we return:
 *   the quantity of the gold remaining in the player's wallet.
 */
int get_wallet_balance(player_t* player);

/**************** update_wallet_balance ****************/
/*
 * updates the player's gold nuggets balance after collecting gold from a pile
 *
 * caller provides:
 *    a valid pointer to a player
 *    amount of gold nuggets collected (should be > 0)
 *
 * we return:
 *    true if successful
 *    false otherwise
 */
bool update_wallet_balance(player_t* player, int gold);


/**************** is_spectator ****************/
/*
 *
 */
bool is_spectator(player_t* player);


/**************** update_spectator ****************/
/*
 *
 */
 bool update_spectator(player_t* spectator, const addr_t address);

#endif // __PLAYER_H
