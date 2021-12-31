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
#include "file.h"
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
grid_new(FILE* fp)
{
  // validate map file pointer
  if (fp != NULL) {
    // compute number of rows
    int ny = file_numLines(fp);

    // compute number of columns
    char* first_line = file_readLine(fp);
    int nx = strlen(first_line);
    mem_free(first_line);

    // load map string from file
    rewind(fp);
    char* map = file_readFile(fp);
    
    // allocate memory for grid
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

/**************** point_to_index() ****************/
/* see grid.h for description */
int
point_to_index(grid_t* grid, point_t* point) {
  // validate parameters
  if (grid != NULL && point != NULL) {
    // compute index on mapstring
    int idx = (point->y * (grid->nx + 1)) + point->x;

    // return index
    return idx;
  }

  // otherwise return -1 (error)
  return -1;
}
