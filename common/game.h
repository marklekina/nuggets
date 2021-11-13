/*
 * game.h - header file for game module
 *
 * this module contains:
 *     game_t struct - holds a grid object and player information
 *     functions to:
 *        i) add and remove spectators
 *       ii) add players
 *      iii) getter functions 
 *
 * Palmer's Posse, November 2021
 */

#ifndef __GAME_H
#define __GAME_H

game_t* game_new(FILE* fp, int nrows, int ncols);
void game_delete(game_t* game);
void remove_spectator(game_t* game);
bool add_spectator(game_t* game, char* name);
bool add_player(game_t* game, char* name);
int get_num_players(game_t* game);
player_t* get_player(game_t* game, int idx);
grid_t* get_grid(game_t* game);
player_t* get_spectator(game_t* game);

#endif // __GAME_H
