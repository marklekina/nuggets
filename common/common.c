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


/**************** distribute_gold ****************/
/*
 * see header file for details
 */
void
distribute_gold(game_t* game)
{
  // initialize number of piles to be created (should be between GoldMinNumPiles and GoldMaxNumPiles)
  // initialize variable to hold total gold distributed
  int num_piles = rand() % (GoldMaxNumPiles - GoldMinNumPiles + 1) + GoldMinNumPiles;
  int goldInPurse = 0;
  
  // initiate an array of gold piles to hold pile locations and gold amounts
  for (int i = 0; i < num_piles; i++) {
    // find random location to set up a gold pile

    do {
      int x_pos = rand() % game->grid->nrows;
      int y_pos = rand() % game->grid->ncols;
    } while (!spot_is_open(game->grid, x_pos, y_pos));
    
    point_t* location = point_new(x_pos, y_pos);
    // TODO: implement a check to make sure a location isn't picked twice
    
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
    if (pile != NULL) {
      game->piles[i] = pile;
    }
  }

  // assign total amount of gold distributed to game
  game->gold_distributed = goldInPurse;
}


void
collect_gold(player_t* player, int* goldRemaining, point_t* locations[], int goldAmounts[], int numPiles)
{
  // extract player's location
  int x = player->xPos;
  int y = player->yPos;
  
  // loop through all gold piles and find the one that matches the player's location
  for (int i = 0; i < numPiles; i++) {
    if (x == locations[i]->x && y == locations[i]->y) {

      // transfer the gold in the pile to the player's purse
      // subtract it from goldRemaining
      player->purse += goldAmounts[i];
      *goldRemaining -= goldAmounts[i];

      // terminate loop
      break;
    }
  }
}
