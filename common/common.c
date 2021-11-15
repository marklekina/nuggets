/*
 * common.c - common code for CS50 nuggets project (will be put in correct files later)
 *
 * Palmer's Posse, November 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**************** global types ****************/
typedef struct grid {
  //note: strlen(map) == nrows * ncols
  char* map;
  int nrows;
  int ncols;
} grid_t;

typedef struct point {
  int x;
  int y;
} point_t;

/*************** function prototypes ***********/
char* compute_visibility(point_t* player, grid_t* grid);
void distribute_gold(grid_t* grid);
void distribute_gold_helper(grid_t* grid, point_t* locations[], int goldAmounts[]);

/*************** compute_visibility ***********/
/*
 * given a player's position (x,y) and 2D array of characters representing a grid, compute the player's visibility from each point in the grid
 * return a 2D array representing visible points with ones and invisible points with zeros.
 */
char*
compute_visibility(point_t* player, grid_t* grid)
{
  // define map size
  int x = grid->ncols;
  int y = grid->nrows;

  // create a map array (a 2D array of characters representing a grid) from the grid object
  char map[x][y];

  int i = 0;  // track x (columns)
  int j = 0;  // track y (rows)

  // loop through the grid's map string and write it into the array
  for (int k = 0; k < strlen(grid->map); k++) {
    // increment row each time we encounter a newline
    if (grid->map[k] == '\n') {
      j += 1;
    }
    // otherwise assign character to array, increment and modulate column
    else {
      map[i][j] = grid->map[k];
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
      int dx = i - player->x;
      int dy = j - player->y;

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
          for (int k = player->x + step, k != i, k += step) {
            
            // if any of the points is not a room spot, mark target as invisible to player;
            // and break the loop
            if (map[k][j] != '.') {
              isVisible = false;
              break;
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

      // case 2: line between points is vertical
      if (dx == 0) {

        // if player and target border each other (or if they are one and the same point), target is, by default, visible to player
        if (abs(dy) <= 1) {
          arr[i][j] = 1;
        }

        // otherwise, loop through the points between player and target
        // note that movement is dependent on direction, take that into account using a unit step increment in the correct direction
        else {
          int step = dy / abs(dy);
          bool isVisible = true;
          for (int k = player->y + step, k != j, k += step) {

            // if any of the points is not a room spot, mark target as invisible to player and break the loop
            if (map[i][k] != '.') {
              isVisible = false;
              break;
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

      // case 3: line between points is diagonal
      if (dx != 0 && dy != 0) {
        // compute gradient
        float gradient = (float) dy/dy;
        bool isVisible;
        
        // loop through all columns(x) between player and target
        isVisible = true;
        int step_x = dx / abs(dx);
        for (int k = player->x + step_x, k != i, k += step_x) {

          // examine intersects
          float y_intersect = gradient * (k - player->x) + player->y;
          float lower_pt = floor(y_intersect);
          float upper_pt = ceil(y_intersect);

          // if line intersects perfectly on grid point, inspect intersect
          if (lower_pt == upper_pt) {

            // if intersect is not a room spot, mark target as invisible to player and break the loop
            if (map[k][(int) y_intersect] != '.') {
              isVisible = false;
              break;
            }
          }
          
          // otherwise inspect lower and upper grid points
          else {
            // if both grid points are not room spots, mark target as invisible to player and break the loop
            if (map[k][(int) lower_pt] != '.' && map[k][(int) upper_pt] != '.') {
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
        for (int k = player->y + step_y, k != j, k += step_y) {

          // examine intersects
          float x_intersect = ((k - player->y) / gradient) + player->x;
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
        visString[k] = " ";
      }
      // otherwise, print appropriate character
      else {
        visString[k] = grid->map[k];
      }
    }
  }
  // return visibility string
  return visString;
}


point_t*
player_on_gold(game_t* game, int x, int y) {
  // validate parameters
  if (game != NULL && x >= 0 && y >= 0) {

    // loop through all piles
    for (int i = 0; i < game->num_piles; i++) {
      pile_t* pile = game->piles[i];
      
      if (pile != NULL) {
        // get location of the pile
        point_t* location = pile->location;
        
        // compare coordinates and return pile if they match
        if (x == location->x && y == location->y) {
          return pile;
        }
      }
    }
  }

  // otherwise return false
  return NULL;
}


void
collect_gold(game_t* game, player_t* player, pile_t* pile)
{ 
  // validate parameters
  if (game != NULL && player != NULL && pile != NULL) {

    // transfer the gold in the pile to the player's purse
    // subtract it from goldRemaining
    player->purse += pile->amount;
    game->gold_remaining -= pile->amount;

    // send gold message to player
    sendGold(game, player, pile->amount);
    
    // delete pile
    pile_delete(pile);
  }
}
