/*
 * game.h - header file for 'game' module
 * 
 * The 'game' struct keeps track of all information that concerns the nuggets game operation, i.e.:
 *   - the map of the game
 *   - players participating in the game
 *   - the 'piles of gold' to be collected, and
 *   - the amount of gold remaining in the game  
 *
 * Mark Lekina Rorat, December 2021
 */

#ifndef __GAME_H
#define __GAME_H

/**************** global types ****************/
typedef struct game game_t;

/**************** functions ****************/

/**************** game_new ****************/
/* create a new instance of the game
 * caller provides:
 *   valid file pointer to the file containing the map string
 *   maximum number of players that can participate in the game
 *   maximum number of gold piles that can be placed in the game
 * we return:
 *   pointer to a new game, or NULL if error.
 * caller is responsible for:
 *   later calling game_delete upon completion of the game session.
 */
game_t* game_new(FILE* fp, int max_players, int max_piles);

/**************** game_delete ****************/
/* delete the game struct.
 *
 * caller provides:
 *   a valid game pointer.
 * we guarantee:
 *   we free the game's grid(map) memory
 *   we free each player's memory
 *   we free each gold pile's memory
 *   we free the game's memory
 *   we ignore NULL game.
 */
void game_delete(game_t* game);

/**************** add_pile ****************/
/* add a pile of gold to the game and keep track of the updated number of piles
 * caller provides:
 *   valid game pointer
 *   valid pile pointer
 * we return:
 *   true if the pile is added successfully
 *   false otherwise
 */
bool add_pile(game_t* game, pile_t* pile);

/**************** add_player ****************/
/* add a player to the game and keep track of the updated number of players
 * caller provides:
 *   valid game pointer
 *   valid player pointer
 * we return:
 *   true if the player is added successfully
 *   false otherwise
 */
bool add_player(game_t* game, player_t* player);

/**************** update_gold ****************/
/* subtract gold collected by a player from the gold remaining in the game
 * caller provides:
 *   valid game pointer
 *   gold amount collected
 * we return:
 *   the new amount of gold remaining in the game, if successful
 *   -1, if unsuccessful (i.e. invalid game pointer)
 *
 *   NB: the returned value is negative only if:
 *      i) the game pointer passed is invalid
 *     ii) the gold collected exceeds the gold remaining in the game (which, theoretically, should never happen)
 */
int update_gold(game_t* game, int gold_collected);

#endif // __GAME_H
