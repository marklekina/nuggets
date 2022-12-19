/*
 * server.h - header file for the nuggets game server
 *
 * Copyright (c) 2022 Mark Lekina Rorat All Rights Reserved.
 *
 */

#ifndef __SERVER_H
#define __SERVER_H

/**************** functions ****************/

/**************** distribute_gold ****************/
/*
 * drops gold piles in random spots within the game's grid
 *
 * caller provides:
 *   valid game pointer
 *
 * we return:
 *   true if the gold piles are distributed successfully
 *   false otherwise
 */
bool distribute_gold(game_t* game, int min_piles, int max_piles);

/**************** add_player ****************/
/*
 * - adds a new player to the game
 * - updates the number of players in the game
 *
 * caller provides:
 *   valid game pointer
 *   valid player pointer
 *   maximum number of players allowed in the game
 *
 * we return:
 *   true if the player is added successfully
 *   false otherwise
 */
bool add_player(game_t* game, player_t* player, int max_players);

/**************** move_player ****************/
/*
 * moves player to different location in the game's grid and perform relevant action if necessary:
 *  - switches locations with another player
 *  - collects gold if a non-empty gold pile is encountered
 *
 * caller provides:
 *   valid game pointer
 *   valid player pointer
 *   valid location pointer
 *
 * we return:
 *   true if the player is moved successfully
 *   false otherwise
 */
bool move_player(game_t* game, player_t* player, point_t* point);

/**************** collect_gold ****************/
/*
 * - adds gold from a gold pile to a player's wallet
 * - subtracts from the pile the gold collected (which, by default, is all the gold in the pile)
 * - updates gold remaining to be collected
 *
 * caller provides:
 *   valid game pointer
 *   valid player pointer
 *   valid pile pointer
 *
 * we return:
 *   the amount of gold collected by the player (if non-zero, we will broadcast a GOLD message)
 */
int collect_gold(game_t* game, player_t* player, pile_t* pile);

/**************** build_visible_mapstring ****************/
/*
 * construct the string representation of the grid portion visible to a player
 *
 * caller provides:
 *   valid game pointer
 *   valid player pointer
 *
 * we return:
 *   the map string visible to the player
 */
char* build_visible_mapstring(game_t* game, player_t* player);

#endif //__SERVER_H
