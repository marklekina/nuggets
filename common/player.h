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

point_t* point_new(int x, int y);
player_t* player_new(char* name, char* type);
void player_delete(player_t* player);
char* get_name(player_t* player);
char* get_type(player_t* player);
point_t* get_location(player_t* player);
char* get_visible_map(player_t* player);
addr_t* get_address(player_t* player);
int get_purse(player_t* player);
bool set_location(player_t* player, int x, int y);
bool set_visible_map(player_t* player, char* visible_map);
bool set_address(player_t* player, addr_t* to);
int add_to_purse(player_t* player, int gold_coins);

#endif // __PLAYER_H
