/* 
 * grid.c - grid module for the CS50 nuggets project
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
  if (nrows > 0 && ncols > 0) {

    // allocate memory for grid
    grid_t* grid = mem_malloc(sizeof(grid_t));

    // initialize grid variables
    if (grid != NULL) {
      grid->map = NULL;     // will load map into this variable, for now set to null
      grid->nrows = nrows;
      grid->ncols = ncols;
    }

    // return grid object
    // note that if mem_malloc failed for grid, null will be returned 
    return grid;
  }

  // otherwise return null
  return NULL;
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
  // validate arguments
  if (grid != NULL && fp != NULL) {
    // read map from map file into a string
    char* map = file_readFile(fp);

    // assign map to the grid
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
  // validate that grid and grid->map exist
  if (grid != NULL) {
    if (grid->map != NULL) {
      // print map string
      puts(grid->map);
    }
  }
}


/**************** getters ****************/
/*
 * see grid.h for more information
 */

int
get_rows(grid_t* grid) {
  if (grid != NULL){
    return grid->nrows;
  }
  return 0;
}

int
get_cols(grid_t* grid) {
  if (grid != NULL){
    return grid->ncols;
  }
  return 0;
}

char*
get_map(grid_t* grid) {
  if (grid != NULL){
    return grid->map;
  }
  return NULL;
}
