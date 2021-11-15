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
#include "../support/message.h"
#include "game.h"
#include "player.h"
#include "grid.h"

bool handlePlayer(game_t* game, addr_t* to, char* playerName);
bool handleSpectator(game_t* game, addr_t* to);
bool handleKey(game_t* game, player_t* player, char key);

bool sendGrid(game_t* game, player_t* player);
bool sendGold(game_t* game, player_t* player, int collected);
bool sendDisplay(game_t* game, player_t* player);
bool spot_is_open(grid_t* grid, int x, int y);

static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles

/******************* handlePlayer *******************/
/*
 * receive a message that a client wants to play and sets them up accordingly
 */
bool handlePlayer(game_t* game, addr_t* to, char* playerName){
  // validate parameters
  if (game != NULL && to != NULL) {
  
    // ensure max number of players has not been reached
    const int MaxPlayers = 26;
    int num_players = get_num_players(game);
    
    if (num_players > MaxPlayers) {
      message_send(*to, "QUIT Game is full: no more players can join\n");
      return false;
    }

    // ensure playerName is provided
    if (playerName == NULL) {
      message_send(*to, "QUIT Sorry - you must provide player's name\n");
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
        char playerLetter = 'a' + player_id;
        char message[50];
        sprintf(message, "OK %c\n", playerLetter);
	      message_send(*to, message);
        set_player_letter(player, playerLetter);

        // retrieve address and send message to it
        addr_t* to = get_address(player);
        message_send(*to, message);
        
        // send grid, gold and display information
        if(sendGrid(game, player)){
          if(sendGold(game, player, 0)){
            if(sendDisplay(game, player)){
              // if all successful, return true
              return true;
            }
          }
        }
      }
    }
  }

  // otherwise return false
  return false;
}

/******************* handleSpectator *******************/
/*
 * receive a message that a client wants to spectate and sets them up accordingly
 */
bool handleSpectator(game_t* game, addr_t* to) {
  // validate parameters
  if (game != NULL && to != NULL) {

    // add spectator to game
    if (add_spectator(game)) {
      
      // create spectator and set spectator address
      player_t* spectator = get_spectator(game);
      set_address(spectator, to);

      // send grid, gold and display information
      if(sendGrid(game, spectator)){
        if(sendGold(game, spectator, 0)){
          if(sendDisplay(game, spectator)){
            // if all successful, return true
            return true;
          }
        }
      }
    }    
  }

  // otherwise return false
  return false;
}

/******************* handleKey *******************/
/*
 * receive a message that the client input a key and handle that keystoke
 */
bool handleKey(game_t* game, player_t* player, char key) {
  // validate parameters
  if (game != NULL && player != NULL) {
    
    // fetch player's address and the game's grid
    addr_t* to = get_address(player);
    grid_t* grid = get_grid(game);
    
    // case q: quit the game.
    if (tolower(key) == 'q') {
      char* type = get_type(player);
      // respond appropriately depending on player type
      if (strcmp(type, "player") == 0) {
        message_send(*to, "QUIT Thanks for playing!\n");
        return true;
      }

      else if (strcmp(type, "spectator") == 0) {
        message_send(*to, "QUIT Thanks for watching!\n");
        return true;
      }
    }
    
    // case h: move left, if possible
    else if (tolower(key) == 'h') {
      // fetch player's location
      point_t* old_loc = get_location(player);

      // update location if possible
      if (old_loc != NULL) {
        int x = get_x(old_loc) - 1;
        int y = get_y(old_loc);

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
        int x = get_x(old_loc) + 1;
        int y = get_y(old_loc);

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
        int x = get_x(old_loc);
        int y = get_y(old_loc) + 1;

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
        int x = get_x(old_loc);
        int y = get_y(old_loc) - 1;

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
        int x = get_x(old_loc) - 1;
        int y = get_y(old_loc) - 1;

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
        int x = get_x(old_loc) + 1;
        int y = get_y(old_loc) - 1;

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
        int x = get_x(old_loc) - 1;
        int y = get_y(old_loc) + 1;

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
        int x = get_x(old_loc) + 1;
        int y = get_y(old_loc) + 1;

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
      message_send(*to, "ERROR invalid key\n");
      return false;
    }
  }

  // otherwise return false
  return false;
}

/******************* spot_is_open *******************/
/*
 * checks if a client can stand on a specific spot on the grid
 */
bool spot_is_open(grid_t* grid, int x, int y) {
  if (grid == NULL || x < 0 || y < 0) {
    return false;
  }
  
  char invalid_spots[4];   // holds spots which a player cannot occupy
  invalid_spots[0] = ' ';  // solid rock
  invalid_spots[1] = '-';  // horizontal boundary
  invalid_spots[2] = '|';  // vertical boundary
  invalid_spots[3] = '+';  // corner boundary
  
  // find spot in map
  int idx = y * (get_cols(grid) + 1) + x;
  char* map = get_map(grid);
  char spot = map[idx];

  // check whether spot can be occupied by player
  for (int i = 0; i < 4; i++) {
    if (spot == invalid_spots[i]) {
      return false;
    }
  }

  // if not, return true
  return true;
}

/******************* update_display *******************/
/*
 * update all the player locations and gold locations, updating them if needed
 */
void update_display(game_t* game) {
  // validate parameters
  if (game != NULL) {
    // get the grid and do null check
    grid_t* grid = get_grid(game);
    if (grid != NULL) {
      // pull out the map
      char* originalMap = get_map(grid);

      // initialize copy of original map
      int len = strlen(originalMap);
      char map[len + 1];
      strcpy(map, originalMap);
      map[len] = '\0';

      // loop through players and update their locations on map
      int num_players = get_num_players(game);
      for (int i = 0; i < num_players; i++) {
        player_t* player = get_player(game, i);

        // get x and y coordinates
        point_t* point = get_location(player);
        int x = get_x(point);
        int y = get_y(point);

        // find spot in map string and update it
        int idx = y * (get_cols(grid) + 1) + x;
        map[idx] = get_player_letter(player);
      }

      // loop through gold piles and update their locations on map
      int num_piles = get_num_piles(game);
      for (int i = 0; i < num_piles; i++) {
        pile_t* pile = get_piles(game, i);
        // null check on pile
        if (pile != NULL) {
          // get x and y coordinates
          point_t* point = get_pile_location(pile);
          
          int x = get_x(point);
          int y = get_y(point);

          // find spot in map string and update it
          int idx = y * (get_cols(grid) + 1) + x;
          map[idx] = '*';
        }
      }

      // refresh game's map
      update_map(get_grid(game), map);
    }
  }
}

/**************** distribute_gold ****************/
/*
 * initialize piles, locations of piles, gold per pile and arrya holding all piles
 */
void distribute_gold(game_t* game){
  // initialize number of piles to be created (should be between GoldMinNumPiles and GoldMaxNumPiles)
  // initialize variable to hold total gold distributed
  int num_piles = rand() % (GoldMaxNumPiles - GoldMinNumPiles + 1) + GoldMinNumPiles;
  int goldInPurse = 0;
  
  // initiate an array of gold piles to hold pile locations and gold amounts
  for (int i = 0; i < num_piles; i++) {
    // find random location to set up a gold pile
    grid_t* grid = get_grid(game);
    int x_pos;
    int y_pos;

    do {
      x_pos = rand() % get_rows(grid);
      y_pos = rand() % get_cols(grid);
    } while (!spot_is_open(grid, x_pos, y_pos));
    
    point_t* location = point_new(x_pos, y_pos);

    // compute random amount of gold to drop
    // determine average gold to put in a pile
    int avgGold = GoldTotal / num_piles;

    // determine random amount of gold to put in pile (between 1/2 and 3/2 of avgGold)
    // subtract amount from remaining gold
    int amount = rand() % (avgGold + 1) + (avgGold/2);
    goldInPurse += amount;
  
    // add location and amount to pile
    pile_t* pile = pile_new(location, amount);

    // add pile to game array
    if(pile != NULL){
      add_pile(game, pile);
    }

  }

  // assign total amount of gold distributed to game
  set_gold_distributed(game, goldInPurse);
}

/**************** player_on_gold ****************/
/*
 * check if a player is standing on gold
 */
pile_t* player_on_gold(game_t* game, int x, int y) {
  // validate parameters
  if (game != NULL && x >= 0 && y >= 0) {

    // loop through all piles
    for (int i = 0; i < get_num_piles(game); i++) {
      pile_t* pile = get_piles(game, i);
      
      if (pile != NULL) {
        // get location of the pile
        point_t* location = get_pile_location(pile);
        
        // compare coordinates and return pile if they match
        if (x == get_x(location) && y == get_y(location)) {
          return pile;
        }
      }
    }
  }

  // otherwise return false
  return NULL;
}

/**************** collect_gold ****************/
/*
 * adds gold to the players purse and takes it out of game play
 */
void collect_gold(game_t* game, player_t* player, pile_t* pile)
{ 
  // validate parameters
  if (game != NULL && player != NULL && pile != NULL) {

    // transfer the gold in the pile to the player's purse
    int amount = get_amount(pile);

    add_to_purse(player, amount);
    change_remaining_gold(game, amount);


    // send gold message to player
    sendGold(game, player, get_amount(pile));
    
    // delete pile
    pile_delete(pile);
  }
}

/**************** sendGrid ****************/
/*
 * sends a message with grid as well as the rows and columns of the grid to the client
 */
bool sendGrid(game_t* game, player_t* player) {
  // validate parameters
  if (game != NULL && player != NULL) {

    // get nrows and ncols from game's grid
    grid_t* grid = get_grid(game);
    int nrows = get_rows(grid);
    int ncols = get_cols(grid);

    // build message string
    char message[50];
    sprintf(message, "GRID %d %d\n", nrows, ncols);
    
    // send the client the message
    message_send(*get_address(player), message);	
    return true;
  }

  // otherwise return false
  return false;	
}


/**************** sendGold ****************/
/*
 * send the number of nuggets, the amount in the player's purse and the amount of gold remaining to the client
 */
bool sendGold(game_t* game, player_t* player, int collected){
  // validate parameters
  if (game != NULL && player != NULL) {

    // get gold info from game's grid and player
    int purse = get_purse(player);
    int remaining = get_gold_remaining(game);

    // build message string
    char message[50];
    sprintf(message, "GOLD %d %d %d\n", collected, purse, remaining);
    
    // send the client the message
    message_send(*get_address(player), message);	
    return true;
  }

  // otherwise return false
  return false;	
}

/**************** sendDisplay ****************/
/*
 * will send a string interpretation of the map, the section of the map depends on the player
 */
bool sendDisplay(game_t* game, player_t* player) {
  // validate parameters
  if (game != NULL && player != NULL) {
    char* map;

    // fetch the entire map if the player is a spectator
    if (strcmp(get_type(player), "spectator") == 0) {
      map = get_map(get_grid(game));
    }

    // otherwise fetch only the part of the map visible to the player
    else {
      map = get_visible_map(player);
    }
    
    // build message string
    char message[50];
    sprintf(message, "DISPLAY\n%s", map);
    
    // send the client the message
    message_send(*get_address(player), message);	
    return true;
  }

  // otherwise return false
  return false;	
}
