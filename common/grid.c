/* 
 * grid module
 *
 * see grid.h for more information.
 *
 * Palmer's Posse, November 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grid.h"
#include "mem.h"
#include "file.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct grid {
  char* map;
  int nrows;
  int ncols;
} grid_t;


/**************** grid_new ****************/
/*
 * see grid.h for more information
 */
grid_t*
grid_new(int nrows, int ncols){
  // verify that parameters are valid
  // otherwise return null
  if (nrows <= 0 || ncols <= 0){
    return NULL;
  }
  
  // allocate memory for grid
  // return null if unsuccessful
  grid_t* grid = mem_malloc(sizeof(grid_t));
  if (grid == NULL){
    return NULL;
  }

  // initialize grid variables
  else {
    grid->map = NULL;
    grid->nrows = nrows;
    grid->ncols = ncols;

    // return grid object
    return grid;
  }  
}


/**************** grid_delete ****************/
/*
 * see grid.h for more information
 */
void
grid_delete(grid_t* grid)
{
  // free the map string
  if (NULL != grid->map) {
    mem_free(grid->map);
  }
  // free grid object
  mem_free(grid);
}

/**************** load_map ****************/ 
/*
 * see grid.h for more information
 */
bool
load_map(grid_t* grid, FILE* fp)
{
  if (grid != NULL && fp != NULL) {
    // read map from map file into a string
    char* map = file_readFile(fp);

    // assign map to the grid
    // return true if successful
    if (map != NULL) {
      grid->map = map;
      return true;
    }
  }
  // otherwise return false
  return false;
}

/**************** display_map ****************/
/*
 * see grid.h for more information
 */
void
display_map(grid_t* grid)
{
  if (grid != NULL) {
    // loop through all rows
    for(int i = 0; i < grid->nrows; i++) {
      // loop through all columns
      for (int j = 0; j < grid->ncolumns; j++) {
        // compute character's position
        int charPos = (i+1) * (j+1) - 1;
        // print character
        putchar(grid->map[charPos]);
      }
    }
  }
}
