/*
 * grid.c - 'grid' module
 *
 * see grid.h for more information.
 *
 * Mark Lekina Rorat, December 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "grid.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct grid {
  char* map; // game map in string representation
  int nx;    // number of columns in grid
  int ny;    // number of rows in grid
} grid_t;

/**************** functions ****************/

/**************** grid_new() ****************/
/* see grid.h for description */
grid_t*
grid_new(char* map, int nx, int ny)
{
  // validate parameters
  if (map != NULL && nx > 0 && ny > 0) {
    // allocate memory for point
    grid_t* grid = mem_malloc(sizeof(grid_t));

    // assign parameters to grid
    if (grid != NULL) {
      grid->map = map;
      grid->nx = nx;
      grid->ny = ny;
    }

    // return struct
    return grid;
  }
  
  // otherwise return null
  return NULL;
}

/**************** grid_delete() ****************/
/* see grid.h for description */
void
grid_delete(grid_t* grid)
{
  if (grid != NULL) {
    // free map string memory
    if (grid->map != NULL) {
      mem_free(grid->map);
    }

    // free grid memory
    mem_free(grid);
  }
}
