/* 
 * player.h - header file for player module
 *
 */

#ifndef _PLAYER_H
#define __PLAYER_H

#include <stdio.h>
#include <stdbool.h>


/**************** global types ****************/
typedef struct player player_t;

/**************** functions ****************/

/**************** player_new ****************/

/* Creates a new player with the parameters
 * We return:
 *  creates a new player, or NULL if error.
 * We guarentee:
 *    
 */
player_t*
player_new(char* name, char* type, game_t* game);

/**************** player_delete ****************/
/*
 * see player.h for more information
 */
player_t*
player_delete(player_t* player);

/**************** player_getVisibility ****************/
/*
 * see player.h for more information
 */
char*
player_getVisibility(player_t* player);

/**************** player_getName ****************/
/*
 * see player.h for more information
 */
char* player_getName(player_t* player);

/**************** player_getType ****************/
/*
 * see player.h for more information
 */
char*
player_getType(player_t* player);

/**************** player_getxPos ****************/
/*
 * see player.h for more information
 */
int
player_getxPos(player_t* player);

/**************** player_getyPos ****************/
/*
 * see player.h for more information
 */
int
player_getyPos(player_t* player);

/**************** player_getTo ****************/
/*
 * see player.h for more information
 */
addr_t player_getTo(player_t* player);

/**************** set_visibility ****************/
/*
 * see player.h for more information
 */

bool
player_setVisibility(player_t* player, char* visibility);



/**************** set_type ****************/
/*
 * see player.h for more information
 */
bool
player_setType(player_t* player, char* type);



/**************** set_xPos ****************/
/*
 * see player.h for more information
 */
bool
player_setxPos(player_t* player, int xPos);



/**************** set_yPos ****************/
/*
 * see player.h for more information
 */
bool
player_setyPos(player_t* player, int yPos);

/**************** addToPurse ****************/
/*
 * see player.h for more information
 */
void addToPurse(int numGold);