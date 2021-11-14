/* 
 * XYZ.c - XYZ module for the CS50 nuggets project
 *
 * Palmer's Posse, November 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "mem.h"
#include "message.h"
#include "game.h"
#include "grid.h"

bool handlePlayer(game_t* game, addr_t* to, char* playerName);
bool handleSpectator(game_t* game, addr_t* to);
bool handleKey(game_t* game, player_t* player, char key);

bool sendGrid(player_t* player);
bool sendGold(player_t* player);
bool sendDisplay(player_t* player);


/******************* handlePlayer *******************/
/*
 * see XYZ.h for detailed description
 */
bool handlePlayer(game_t* game, addr_t* to, char* playerName){
  // validate parameters
  if (game != NULL && to != NULL) {
  
    // ensure max number of players has not been reached
    const int MaxPlayers = 26;
    int num_players = get_num_players(game);
    
    if (num_players > MaxPlayers) {
      message_send(to, "QUIT Game is full: no more players can join\n");
      return false;
    }

    // ensure playerName is provided
    if (playerName == NULL) {
      message_send(to, "QUIT Sorry - you must provide player's name\n");
      return false;
    }

    // add player to game
    int player_id = add_player(game, playerName);

    // confirm that player has been added to game
    if (player_id >= 0) {
      player_t* player = get_player(game, player_id);
      if (player != NULL) {
        // set player address
        set_address(player, to);
        
        // send OK message
        char playerLetter = "a" + player_id;
        char message = strcat("OK ", to_upper(playerLetter));
        message_send(player->to, message);
        
        // send grid, gold and display information
        sendGrid(player);
        sendGold(player);
        sendDisplay(player);

        // if all successful, return true
        return true;
      }
    }
  }

  // otherwise return false
  return false;
}


bool handleSpectator(game_t* game, addr_t* to) {
  // validate parameters
  if (game != NULL && to != NULL) {

    // add spectator to game
    if (add_spectator(game)) {
      
      // set spectator address
      player_t* spectator = get_spectator(game);
      set_address(spectator, to);

      // send grid, gold and display information
      sendGrid(spectator);
      sendGold(spectator);
      sendDisplay(spectator);

      // if all successful, return true
      return true;
    }    
  }

  // otherwise return false
  return false;
}


bool handleKey(game_t* game, player_t* player, char key) {
  // validate parameters
  if (game != NULL && player != NULL) {
    
    // fetch player's address and the game's grid
    addr_t* to = get_address(player);
    grid_t* grid = get_grid(game);
    
    // case q: quit the game.
    if (tolower(key) == 'q') {
      // respond appropriately depending on player type
      if (strcmp(player->type, "player") == 0) {
        message_send(to, "QUIT Thanks for playing!\n");
        return true;
      }

      else if (strcmp(player->type, "spectator") == 0) {
        message_send(to, "QUIT Thanks for watching!\n");
        return true;
      }
    }
    
    // case h: move left, if possible
    else if (tolower(key) == 'h') {
      // fetch player's location
      point_t* old_loc = get_location(player);

      // update location if possible
      if (old_loc != NULL) {
        int x = old_loc->x - 1;
        int y = old_loc->y;

        // check whether new spot is open
        if (spot_is_open(grid, x, y)) {
          // update player location
          set_location(player, x, y);
          return true;
        }
      }
    }
    
    // case l: move right, if possible
    else if (tolower(key) == 'l') {
      // fetch player's location
      point_t* old_loc = get_location(player);

      // update location if possible
      if (old_loc != NULL) {
        int x = old_loc->x + 1;
        int y = old_loc->y;

        // check whether new spot is open
        if (spot_is_open(grid, x, y)) {
          // update player location
          set_location(player, x, y);
          return true;
        }
      }
    }
    
    // case j: move down, if possible
    else if (tolower(key) == 'j') {
      // fetch player's location
      point_t* old_loc = get_location(player);

      // update location if possible
      if (old_loc != NULL) {
        int x = old_loc->x;
        int y = old_loc->y + 1;

        // check whether new spot is open
        if (spot_is_open(grid, x, y)) {
          // update player location
          set_location(player, x, y);
          return true;
        }
      }
    }

    // case k: move up, if possible
    else if (tolower(key) == 'k') {
      // fetch player's location
      point_t* old_loc = get_location(player);

      // update location if possible
      if (old_loc != NULL) {
        int x = old_loc->x;
        int y = old_loc->y - 1;

        // check whether new spot is open
        if (spot_is_open(grid, x, y)) {
          // update player location
          set_location(player, x, y);
          return true;
        }
      }
    }
    
    // case y: move diagonally up and left, if possible
    else if (tolower(key) == 'y') {
      // fetch player's location
      point_t* old_loc = get_location(player);

      // update location if possible
      if (old_loc != NULL) {
        int x = old_loc->x - 1;
        int y = old_loc->y - 1;

        // check whether new spot is open
        if (spot_is_open(grid, x, y)) {
          // update player location
          set_location(player, x, y);
          return true;
        }
      }
    }
    
    // case u: move diagonally up and right, if possible
    else if (tolower(key) == 'u') {
      // fetch player's location
      point_t* old_loc = get_location(player);

      // update location if possible
      if (old_loc != NULL) {
        int x = old_loc->x + 1;
        int y = old_loc->y - 1;

        // check whether new spot is open
        if (spot_is_open(grid, x, y)) {
          // update player location
          set_location(player, x, y);
          return true;
        }
      }
    }
    
    // case b: move diagonally down and left, if possible
    else if (tolower(key) == 'b') {
      // fetch player's location
      point_t* old_loc = get_location(player);

      // update location if possible
      if (old_loc != NULL) {
        int x = old_loc->x - 1;
        int y = old_loc->y + 1;

        // check whether new spot is open
        if (spot_is_open(grid, x, y)) {
          // update player location
          set_location(player, x, y);
          return true;
        }
      }
    }
    
    // case n: move diagonally down and right, if possible
    else if (tolower(key) == 'n') {
      // fetch player's location
      point_t* old_loc = get_location(player);

      // update location if possible
      if (old_loc != NULL) {
        int x = old_loc->x + 1;
        int y = old_loc->y + 1;

        // check whether new spot is open
        if (spot_is_open(grid, x, y)) {
          // update player location
          set_location(player, x, y);
          return true;
        }
      }
    }

    // case invalid key: send error message
    else {
      message_send(to, "ERROR invalid key\n");
      return false;
    }
  }

  // otherwise return false
  return false;
}


bool
spot_is_open(grid_t* grid, int x, int y) {
  if (grid = NULL || x < 0 || y < 0) {
    return false;
  }
  
  char invalid_spots[4];   // holds spots which a player cannot occupy
  invalid_spots[0] = ' ';  // solid rock
  invalid_spots[1] = '-';  // horizontal boundary
  invalid_spots[2] = '|';  // vertical boundary
  invalid_spots[3] = '+';  // corner boundary
  
  // find spot in map
  int idx = y * (grid->ncols + 1) + x;
  char spot = grid->map[idx];

  // check whether spot can be occupied by player
  for (int i = 0; i < 4; i++) {
    if (spot == invalid_spots[i]) {
      return false;
    }
  }

  // if not, return true
  return true;
}


void
update_display(game_t* game) {
  // validate parameters
  if (game != NULL) {
    if (game->grid != NULL) {

      // initialize copy of original map
      int len = strlen(game->grid->original_map);
      char map[len + 1];
      strcpy(map, game->grid->original_map);
      map[len] = '\0';

      // loop through players and update their locations on map
      int num_players = get_num_players(game);
      for (int i = 0; i < num_players; i++) {
        player_t* player = get_player(game, i);

        // get x and y coordinates
        int x = get_location(player)->x;
        int y = get_location(player)->y;

        // find spot in map string and update it
        int idx = y * (grid->ncols + 1) + x;
        map[idx] = player->letter;
      }

      // loop through gold piles and update their locations on map
      // TODO: add this variables in game
      int num_piles = get_num_piles(game);
      for (int i = 0; i < num_piles; i++) {
        point_t* pile = get_pile(game, i);

        // get x and y coordinates
        int x = pile->x;
        int y = pile->y;

        // find spot in map string and update it
        int idx = y * (grid->ncols + 1) + x;
        map[idx] = '*';
      }

      // refresh game's map
      update_map(game->grid, map);
    }
  }
}


// TODO: finish modifying this module from here

bool sendGrid(grid_t* grid, addr_t to){
	// put together a string message that includes GRID nrows ncols
	if(grid != NULL){
		int nrows = grid_getnRows(grid);
		int ncols = grid_getnCols(grid);

		char* charRows = convertInt(nrows);
		char* charCols = convertInt(ncols);

		// send the grid the client
		char* message = strcat("GRID ", charRows);
		message = strcat(message, " ");
		message = strcat(message, charCols);

		message_send(to, message);	
		return true;
	}
	return false;	
}

// !!
bool sendGold(game_t* game, addr_t to){
	// put together a string message that includes GOLD # nuggets, purse count, remaining gold

	// check if has been updated, if so send to clients
	// message_send(to, "GOLD %d %d %d", collected, purse, remaining);	
	return false;
}

bool sendDisplay(char* map, addr_t to){
	// retrieve the text version of the map and send it to client in a message
	if(map!= NULL){
		char* message = strcat("DISPLAY\n ", map);
		message_send(to, message);	
		return true;
	}
	return false;
}
