/*
 * visibility.c - visibility module for CS50 nuggets project
 *
 * Palmer's Posse, November 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*************** function prototypes ***********/
int main(int argc, char* argv[]);
int[][] computeVisibility(point_t* pt, char[nx][ny] grid);

int
main(int argc, char* argv[])
{
  // TODO: make this make sense
  char[][] map;
  int [][] visibilityArray = computeVisibility(map);

  // print out visibility array
}


/*************** computeVisibility ***********/
/*
 * given a player's position (x,y) and 2D array of characters representing a grid, compute the player's visibility from each point in the grid
 * return a 2D array representing visible points with ones and invisible points with zeros.
 */
int[][]
computeVisibility(point_t* player, char[x][y] grid)
{
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
            if (grid[k][j] != '.') {
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
            if (grid[i][k] != '.') {
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
            if (grid[k][(int) y_intersect] != '.') {
              isVisible = false;
              break;
            }
          }
          
          // otherwise inspect lower and upper grid points
          else {
            // if both grid points are not room spots, mark target as invisible to player and break the loop
            if (grid[k][(int) lower_pt] != '.' && grid[k][(int) upper_pt] != '.') {
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
            if (grid[(int) x_intersect][k] != '.') {
              isVisible = false;
              break;
            }
          }
          // otherwise inspect lower and upper grid points
          else {
            // if both grid points are not room spots, mark target as invisible to player and break the loop
            if (grid[(int) lower_pt][k] != '.' && grid[(int) upper_pt][k] != '.') {
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
}
