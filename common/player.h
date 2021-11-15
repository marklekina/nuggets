/*
 * player.h - header file for player module
 *
 * this module contains:
 *     player_t struct - holds player information
 *     point_t struct - holds a single (x,y) coordinate
 *     functions to:
 *        i) create (and delete) the aforementioned structs
 *       ii) getter functions
 *      iii) setter functions
 *       iv) update player information 
 *
 * Palmer's Posse, November 2021
 */

#ifndef __PLAYER_H
#define __PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "mem.h"
#include "../support/message.h"

/**************** global types ****************/
typedef struct point point_t;
typedef struct player player_t;

/**************** functions ****************/

/**************** point_new ****************/
/* 
 * intializes x and y to provided values
 * returns new point
 */
point_t* point_new(int x, int y);

/**************** get_x ****************/
/*
 * returns point's x value
 */
int get_x(point_t* point);

/**************** get_y ****************/
/*
 * returns point's y value
 */
int get_y(point_t* point);
#endif // __PLAYER_H


/**************** player_new ****************/
/* 
 * creates a new player, or NULL if error.
 * The provided name is validated and is set as player's name
 * The provided type is validated and set 
 * The visible map, location, address are set to NULL
 * Purse is set to 0
 * This then returns the new player or NULL if error 
 */
player_t* player_new(char* name, char* type);

/**************** player_delete ****************/
/*
 * deletes player as well as their name, type, location, address, and visible map.
 */
void player_delete(player_t* player);

/**************** get_name ****************/
/*
 * returns player's name 
 */
char* get_name(player_t* player);

/**************** get_type ****************/
/*
 * returns player's type
 */
char* get_type(player_t* player);

/**************** get_location ****************/
/*
 * returns player's location
 */
point_t* get_location(player_t* player);

/**************** get_visible_map ****************/
/*
 * returns player's visible map
 */
char* get_visible_map(player_t* player);

/**************** get_address ****************/
/*
 * returns player's address
 */
addr_t* get_address(player_t* player);

/**************** get_purse ****************/
/*
 * returns the value of the players purse
 */
int get_purse(player_t* player);

/**************** set_location ****************/
/*
 * changes location to a point made from the given x and y values
 * returns true if successful
 */
bool set_location(player_t* player, int x, int y);

/**************** set_visible_map ****************/
/*
 * changes visible to the given string
 * returns true if successful
 */
bool set_visible_map(player_t* player, char* visible_map);

/**************** set_address ****************/
/*
 * changes address to given address
 * returns true if successful
 */
bool set_address(player_t* player, addr_t* to);

/**************** set_name ****************/
/*
 * changes name to the given name
 * returns true if successful
 */
bool set_name(player_t* player, char* name);
/**************** add_to_purse ****************/
/*
 * changes address to given address
 * returns purse value
 */
int add_to_purse(player_t* player, int gold_coins);



