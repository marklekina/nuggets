/* 
 * player.h - header file for grid module
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
/*
 * see player.h for more information
 */
player_t*
player_new(char* name);

/**************** player_delete ****************/
/*
 * see player.h for more information
 */
player_t*
player_delete(player_t* player);

/**************** get_visibility ****************/
/*
 * see player.h for more information
 */
player_t*
get_visibility(player_t* player);

/**************** set_visibility ****************/
/*
 * see player.h for more information
 */
player_t*
set_visibility(player_t* player, char* visibility);

/**************** get_type ****************/
/*
 * see player.h for more information
 */
player_t*
get_type(player_t* player);

/**************** set_type ****************/
/*
 * see player.h for more information
 */
player_t*
set_type(player_t* player, char* type);

/**************** get_xPos ****************/
/*
 * see player.h for more information
 */
player_t*
get_xPos(player_t* player);

/**************** set_xPos ****************/
/*
 * see player.h for more information
 */
player_t*
set_xPos(player_t* player, int xPos);

/**************** get_yPos ****************/
/*
 * see player.h for more information
 */
player_t*
get_yPos(player_t* player);

/**************** set_yPos ****************/
/*
 * see player.h for more information
 */
player_t*
set_yPos(player_t* player, int yPos);