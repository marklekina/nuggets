/*
 * game.c - game module for the CS50 nuggets project
 *
 * see game.h for more information.
 *
 * Palmer's Posse, November 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "mem.h"
#include "file.h"
#include "game.h"
#include "grid.h"
#include "player.h"


/**************** global types ****************/
typedef struct pile {
  point_t* location;
  int amount;
} pile_t;
  
typedef struct game {
  grid_t* grid;          // holds grid information
  player_t* spectator;   // one game spectator
  player_t* players[26]; // array of game players
  pile_t* piles[30];
  int num_players;       // number of players currently in the game
  int num_piles;
  int gold_distributed;
  int gold_remaining;
} game_t;


/**************** pile_new ****************/
/*
 * see game.h for more information
 */
pile_t*
pile_new(point_t* location, int amount) {
  // validate parameters
  if (location != NULL && amount >= 0) {
    
    // allocate memory for pile object
    // assign location and amount to pile
    pile_t* pile = mem_malloc(sizeof(pile_t));

    if (pile != NULL) {
      pile->location = location;
      pile->amount = amount;

      // return pile
      return pile;
    }
  }
  
  // otherwise return null
  return NULL;
}


/**************** pile_delete ****************/
/*
 * see game.h for detailed description
 */
void
pile_delete(pile_t* pile) {
  if (pile != NULL) {
    // free location
    if (pile->location != NULL) {
      mem_free(pile->location);
    }

    // free pile
    mem_free(pile);
  }
}


/**************** game_new ****************/
/*
 * see game.h for more information
 */

game_t*
game_new(FILE* fp, int nrows, int ncols) {
  // if fp != null, nrows and ncols != 0
  if (fp != NULL && nrows > 0 && ncols > 0) {
    // allocate memory for game object
    game_t* game = mem_malloc(sizeof(game_t));

    // initialize a grid object inside game to hold grid info
    // load map into grid
    if (game != NULL) {
      grid_t* grid = grid_new(nrows, ncols);
      load_map(grid, fp);
      
      // assign grid to game
      game->grid = grid;

      // initialize the rest of the variables as null (or zero)
      game->spectator = NULL;
      game->num_players = 0;
      game->gold_distributed = 0;
      game->gold_remaining = 250;
      game->num_piles = 0;
    }

    // return game object
    return game;
  }
  
  // otherwise return null
  return NULL;
}

/**************** game_delete ****************/
/*
 * see game.h for more information
 */
void
game_delete(game_t* game) {
  // free the spectator
  if (NULL != game->spectator) {
    player_delete(game->spectator);
  }

  // loop through the players array and free each of them
  if (NULL != game->players) {
    for(int i = 0; i < game->num_players; i++){

      // free the player
      if (NULL != game->players[i]) {
        player_delete(game->players[i]);
      }
    }
  }
  
  // free game object
  mem_free(game);
}


/**************** remove_spectator ****************/
/* 
 * see game.h for detailed description
 */
void
remove_spectator(game_t* game) {
  // send quit message to current spectator
  player_t* spectator = get_spectator(game);

  if (spectator != NULL) {
    addr_t* to = get_address(spectator);
    message_send(*to, "QUIT You have been replaced by a new spectator\n");

    // delete player object
    player_delete(spectator);
  }
}


/**************** add_spectator ****************/
/* 
 * see game.h for detailed description
 */
bool
add_spectator(game_t* game) {
  // validate parameters
  if (game != NULL) {
    // if a spectator is already present, kick them out
    if (game->spectator != NULL){
      remove_spectator(game);
    }

    // create player_t object of spectator type
    // assign them to game and return true
    // NB: name parameter doesn't matter for spectator, we never use it
    // to solve potential double free errors, reassign spectator name to null
    player_t* player = player_new("spectator", "spectator");
    if (player != NULL) {
      set_name(player, "");
      game->spectator = player;
      return true;
    }
  }
  
  // otherwise return false
  return false;
}

/**************** add_player ****************/
/* 
 * see game.h for description
 */
int
add_player(game_t* game, char* name) {
  // validate parameters
  if (game != NULL && name != NULL) {
    // create player_t object of player type
    player_t* player = player_new(name, "player");

    if (player != NULL) {
      // add player to array of players
      // increment number of players in game
      game->players[game->num_players] = player;
      game->num_players += 1;

      // return player_id if successful
      return game->num_players;
    }
  }
  
  // otherwise return invalid index
  return -1;
}

/**************** add_pile ****************/
/* 
 * see game.h for description
 */
bool
add_pile(game_t* game, pile_t* pile) {
  // validate parameters
  if (game != NULL && pile != NULL) {
    // add player to array of players
    // increment number of players in game
    int i = game->num_piles;
    game->piles[i] = pile;
    game->num_piles += 1;

    // return player_id if successful
    return true;
  }
  
  // otherwise return invalid index
  return false;
}

/**************** getters ****************/
/*
 * see game.h for description
 */
int
get_num_players(game_t* game) {
  if (game != NULL) {
    return game->num_players;
  }
  return 0;
}

int
get_amount(pile_t* pile) {
  if(pile != NULL) {
    return pile->amount;
  }
  return -1;
}
int
get_gold_remaining(game_t* game) {
  if(game != NULL) {
    return game->gold_remaining;
  }
  return -1;
}
int
get_gold_distributed(game_t* game) {
  if(game != NULL) {
    return game->gold_distributed;
  }
  return -1;
}

point_t* 
get_pile_location(pile_t* pile){
  if (pile != NULL) {
    return pile->location;
  }
  return NULL;
}

player_t*
get_player(game_t* game, int idx){
  if (game != NULL) {
    if (game->players != NULL) {
      return game->players[idx];
    }
  }
  return NULL;
}

grid_t*
get_grid(game_t* game){
  if (game != NULL){
    return game->grid;
  }
  return NULL;
}

player_t*
get_spectator(game_t* game) {
  if (game!= NULL) {
    return game->spectator;
  }
  return NULL;
}

pile_t* 
get_piles(game_t* game, int i){
  if (game!= NULL && i >= 0) {
    pile_t* pile = game->piles[i];
    return pile;
  }
  return NULL;
}
int
get_num_piles(game_t* game) {
  if (game != NULL) {
    return game->num_piles;
  }
  return 0;
}
bool set_gold_distributed(game_t* game, int goldDis){
  if(game!= NULL || goldDis<0){
    return false;
  }
  game->gold_distributed = goldDis;
  return true;
}
/*************** compute_visibility ***********/
/*
 * given a player's position (x,y) and 2D array of characters representing a grid, compute the player's visibility from each point in the grid
 * return a 2D array representing visible points with ones and invisible points with zeros.
 */
char*
compute_visibility(point_t* point, grid_t* grid)
{
  // define map size
  int x = get_cols(grid);
  int y = get_rows(grid);

  // create a map array (a 2D array of characters representing a grid) from the grid object
  char map[x][y];

  int i = 0;  // track x (columns)
  int j = 0;  // track y (rows)

  char* gridMap = get_map(grid);

  // loop through the grid's map string and write it into the array
  for(int k = 0; k < strlen(get_map(grid)); k++) {
    // increment row each time we encounter a newline
    
    if(gridMap[k] == '\n') {
      j += 1;
    }
    // otherwise assign character to array, increment and modulate column
    else {
      map[i][j] = gridMap[k];
      i += 1;
      i = i % x;
    }
  }

  // initialize visibity array
  int arr[x][y];

  // loop through each point in the grid and compute its visibility from pt
  // if visible; asssign 1, otherwise; assign 0

  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      // compute change in x and change in y

      int dx = i - get_x(point);
      int dy = j - get_y(point);

      // case 1: line between points is horizontal
      if (dy == 0) {
        
        // if player and target border each other (or if they are one and the same point), target is, by default, visible to player
        if (abs(dx) <= 1) {
          arr[i][j] = 1;
        }

        // if not, loop through the points between player and target
        // note that movement is dependent on direction, take that into account using a unit step increment in the correct direction
        else {
          int step = dx / abs(dx);
          bool isVisible = true;

          for (int k = get_x(point) + step; k != i; k += step) {
            
            // if any of the points is not a room spot, mark target as invisible to player;
            // and break the loop
            if(map[k][j] != '.') {
              isVisible = false;
              break;
            }
          }

          // assign appropriate value to target
          if(isVisible) {
            arr[i][j] = 1;
          }
          else {
            arr[i][j] = 0;
          }
        }
      }

      // case 2: line between points is vertical
      if (dx == 0) {

        // if player and target border each other (or if they are one and the same point), target is, by default, visible to player
        if(abs(dy) <= 1) {
          arr[i][j] = 1;
        }

        // otherwise, loop through the points between player and target
        // note that movement is dependent on direction, take that into account using a unit step increment in the correct direction
        else {
          int step = dy / abs(dy);
          bool isVisible = true;

          for (int k = get_y(point) + step; k != j; k += step) {

            // if any of the points is not a room spot, mark target as invisible to player and break the loop
            if (map[i][k] != '.') {
              isVisible = false;
              break;
            }
          }

          // assign appropriate value to target
          if(isVisible) {
            arr[i][j] = 1;
          }
          else {
            arr[i][j] = 0;
          }
        }
      }

      // case 3: line between points is diagonal
      if(dx != 0 && dy != 0) {
        // compute gradient
        float gradient = (float) dy/dy;
        bool isVisible;
        
        // loop through all columns(x) between player and target
        isVisible = true;
        int step_x = dx / abs(dx);

        for(int k = get_x(point) + step_x; k != i; k += step_x) {

          // examine intersects
          float y_intersect = gradient * (k - get_x(point)) + get_y(point);
          float lower_pt = floor(y_intersect);
          float upper_pt = ceil(y_intersect);

          // if line intersects perfectly on grid point, inspect intersect
          if(lower_pt == upper_pt) {

            // if intersect is not a room spot, mark target as invisible to player and break the loop
            if(map[k][(int) y_intersect] != '.') {
              isVisible = false;
              break;
            }
          }
          
          // otherwise inspect lower and upper grid points
          else {
            // if both grid points are not room spots, mark target as invisible to player and break the loop
            if(map[k][(int) lower_pt] != '.' && map[k][(int) upper_pt] != '.') {
              isVisible = false;
              break;
            }
          }
        }

        // assign appropriate value to target
        if (isVisible) {
          arr[i][j] = 1;
        }
        else {
          arr[i][j] = 0;
        }


        // loop through all rows(y) between player and target
        isVisible = true;
        int step_y = dy / abs(dy);
        for (int k = get_y(point) + step_y; k != j; k += step_y) {

          // examine intersects
          float x_intersect = ((k - get_y(point)) / gradient) + get_x(point);
          float lower_pt = floor(x_intersect);
          float upper_pt = ceil(x_intersect);

          // if line intersects perfectly on grid point, inspect intersect
          if (lower_pt == upper_pt) {

            // if intersect is not a room spot, mark target as invisible to player and break the loop
            if (map[(int) x_intersect][k] != '.') {
              isVisible = false;
              break;
            }
          }
          // otherwise inspect lower and upper grid points
          else {
            // if both grid points are not room spots, mark target as invisible to player and break the loop
            if (map[(int) lower_pt][k] != '.' && map[(int) upper_pt][k] != '.') {
              isVisible = false;
              break;
            }
          }
        }

        // assign appropriate value to target
        if (isVisible) {
          arr[i][j] = 1;
        }
        else {
          arr[i][j] = 0;
        }
      }
    }
  }

  // initialize visibility string
  char visString[x * y + 1];
  int k = 0;

  // loop through array and write output into string
  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      // if spot invisible to player, print nothing
      if (arr[i][j] == 0) {
        visString[k] = ' ';
      }
      // otherwise, print appropriate character
      else {
        visString[k] = gridMap[k];
      }
    }
  }
  // return visibility string
  
  return visString;
  
}
void change_remaining_gold(game_t* game, int amount){
  if(game != NULL && game->gold_remaining > 0 && amount > 0){
    game->gold_remaining -= amount;
  }
}