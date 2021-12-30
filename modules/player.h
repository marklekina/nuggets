/*
 * player.h - header file for 'player' module
 * 
 * A 'player' is a struct that represents a client in the game. It holds information such as the player's name, letter (as represented on the map), location, gold collected by the player, and the section of the map that is visible to the player.
 *
 * Mark Lekina Rorat, December 2021
 */

#ifndef __PLAYER_H
#define __PLAYER_H

/**************** global types ****************/
typedef struct player player_t;

/**************** functions ****************/

/**************** player_new ****************/
/* create a new player
 * caller provides:
 *   valid player's name (string)
 *   valid player's letter representation 
 * we return:
 *   pointer to a new player, or NULL if error.
 * caller is responsible for:
 *   later calling player_delete.
 */
player_t* player_new(char* name, char letter);

/**************** player_delete ****************/
/* delete the player.
 *
 * caller provides:
 *   a valid player pointer.
 * we guarantee:
 *   we free the player's name string
 *   we free the player's visible_map string
 *   we free the player's location memory
 *   we free the player's memory
 *   we ignore NULL player.
 */
void player_delete(player_t* player);

/**************** update_location ****************/
/* set player's location to new location
 * 
 * caller provides:
 *    a valid pointer to a player
 *    a valid location pointer
 * we return:
 *    true if the player's location is successfully updated
 *    false if update is unsuccessful
 */
bool update_location(player_t* player, point_t* location);

/**************** update_visible_map ****************/
/* assign to the player the section of the map visible to them
 *
 * caller provides:
 *    a valid pointer to a player
 *    a valid pointer to the visible map string
 *
 * we return:
 *    true if the visible map string is successfully updated
 *    false if update is unsuccessful
 */
bool update_visible_map(player_t* player, char* visible_map);

/**************** add_to_purse ****************/
/* add gold collected by the player to their purse
 *
 * caller provides:
 *    a valid pointer to a player
 *    amount of gold collected (should be > 0)
 * 
 * we return:
 *    the new amount of gold in the player's purse if successful
 *    zero if unsuccessful
 */
int add_to_purse(player_t* player, int gold_collected);

#endif // __PLAYER_H
