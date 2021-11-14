/* 
 * player.h - header file for player module
 *
 */

#ifndef __PLAYER_H
#define __PLAYER_H

#include <stdio.h>
#include <stdbool.h>
#include "../support/message.h"


/**************** global types ****************/
typedef struct player player_t;

/**************** functions ****************/

/**************** player_new ****************/

/* 
 * 
 * creates a new player, or NULL if error.
 * The player is initialize with the given name and type and the visibility is set to NULL
 * This then returns the new player 
 *    
 */
player_t*
player_new(char* name, char* type);

/**************** player_delete ****************/
/*
 * deletes player as well as their name, type, and visibility.
 */
void
player_delete(player_t* player);

/**************** player_getVisibility ****************/
/*
 * returns player's visibility string
 */
char*
player_getVisibility(player_t* player);

/**************** player_getName ****************/
/*
 * returns player's name
 */
char* player_getName(player_t* player);

/**************** player_getType ****************/
/*
 * returns player's type
 */
char*
player_getType(player_t* player);

/**************** player_getxPos ****************/
/*
 * returns player's xPos
 */
int
player_getxPos(player_t* player);

/**************** player_getyPos ****************/
/*
 * returns player's yPos
 */
int
player_getyPos(player_t* player);



/**************** set_visibility ****************/
/*
 * Changes visibility to the given one
 * returns true if this is successful
 */

bool
player_setVisibility(player_t* player, char* visibility);



/**************** set_type ****************/
/*
 * Changes type to the given one
 * returns true if this is successful
 */
bool
player_setType(player_t* player, char* type);



/**************** set_xPos ****************/
/*
 * Changes xPos to the given one
 * returns true if this is successful
 */
bool
player_setxPos(player_t* player, int xPos);



/**************** set_yPos ****************/
/*
 * Changes yPos to the given one
 * returns true if this is successful
 */
bool
player_setyPos(player_t* player, int yPos);

/**************** addToPurse ****************/
/*
 * Makes sure the player's gold ammount is within the global range
 * if so, add the given amount of gold to the player's purse
 */
void addToPurse(player_t* player, int numGold, int GoldTotal);

/**************** getPurse ****************/
/*
 * returns player's purse
 */
int getPurse(player_t* player);

#endif
