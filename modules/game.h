/*
 * game.h - header file for 'game' module
 *
 * The 'game' struct keeps track of all state for the nuggets game's operation:
 *   - the map(grid) of the game
 *   - the players participating in the game
 *   - the gold piles to be collected, and
 *   - the amount of gold remaining in the game
 *
 * Mark Lekina Rorat, December 2021; October 2022
 */

#ifndef __GAME_H
#define __GAME_H

/**************** global types ****************/
typedef struct game game_t;

/**************** functions ****************/

/**************** game_new ****************/
/*
 * creates a new instance of the game
 *
 * caller provides:
 *   valid file pointer to the map string file
 *   maximum number of players that can participate in the game
 *   maximum number of gold piles that can be placed in the game
 *   amount of gold to be distributed, i.e. the gold amount remaining when the game starts
 *
 * we return:
 *   pointer to a new game, or NULL if error.
 *
 * caller is responsible for:
 *   later calling game_delete upon completion of the game session.
 */
game_t* game_new(FILE* fp, int max_players, int max_piles, int gold_balance);

/**************** game_delete ****************/
/*
 * deletes the game struct.
 *
 * caller provides:
 *   a valid game pointer.
 *
 * we guarantee:
 *   we free the game's grid(map) memory
 *   we free each player's memory, as well as the players array's memory
 *   we free each gold pile's memory, as well as the piles array's memory
 *   we free the game's memory
 *   we ignore NULL game.
 */
void game_delete(game_t* game);

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
bool distribute_gold(game_t* game);

/**************** add_player ****************/
/*
 * - adds a new player to the game
 * - updates the number of players in the game
 *
 * caller provides:
 *   valid game pointer
 *   valid player pointer
 *
 * we return:
 *   true if the player is added successfully
 *   false otherwise
 */
bool add_player(game_t* game, player_t* player);

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

#endif // __GAME_H
