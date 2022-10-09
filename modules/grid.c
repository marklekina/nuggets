/*
 * grid.c - 'grid' module
 *
 * see grid.h for more information.
 *
 * Mark Lekina Rorat, December 2021
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mem.h"
#include "file.h"
#include "point.h"
#include "grid.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct grid {
  int nrows;            // number of rows in the map grid
  int ncols;            // number of columns in the map grid
  int size;             // number of grid points in the grid, i.e., nrows * ncols
  char* mapString;      // representation of the grid and its occupants in string format
  point_t** gridPoints; // array containing each grid point in the game
} grid_t;

/**************** functions ****************/

/**************** grid_new() ****************/
/* see grid.h for description */
grid_t*
grid_new(FILE* fp)
{
  // validate map text file
  if (fp == NULL) {
    return NULL;
  }

  // compute number of rows and columns in the map file, and grid size
  int nrows = file_numLines(fp);
  char* first_line = file_readLine(fp);
  int ncols = strlen(first_line);

  // free redundant string memory and rewind file pointer to start of the file
  mem_free(first_line);
  rewind(fp);

  // allocate memory for the grid
  grid_t* grid = mem_malloc(sizeof(grid_t));
  if (grid == NULL) {
    return NULL;
  }

  // read map string from file and allocate memory for grid points array
  char* mapString = file_readFile(fp);
  int size = strlen(mapString);
  point** gridPoints = (point_t*) mem_malloc(sizeof(point_t*) * size);
  if (mapString == NULL || gridPoints == NULL) {
    return NULL;
  }

  // load grid parameters into the grid and return the grid struct
  grid->nrows = nrows;
  grid->ncols = ncols;
  grid->size = size;
  grid->mapString = mapString;
  grid->gridPoints = gridPoints;
  return grid;
}

/**************** grid_delete() ****************/
/* see grid.h for description */
void
grid_delete(grid_t* grid)
{
  if (grid != NULL) {
    // free grid points memory
    for (int i = 0; i < grid->size; i++) {
      point_delete(grid->gridPoints[i]);
    }

    // free grid point array memory
    mem_free(grid->gridPoints);

    // free map string memory
    mem_free(grid->mapString);

    // free grid memory
    mem_free(grid);
  }
}

/**************** get_gridpoint() ****************/
/* see grid.h for description */
point_t*
get_gridpoint(grid_t* grid, int row, int col) {
  // validate grid
  if (grid == NULL) {
    return NULL;
  }

  // ensure row and columns fall within the grid
  if (row < 0 || col < 0 || row >= grid->nrows || col >= grid->ncols) {
    return NULL;
  }

  // return grid point at specified
  int idx = row * grid->ncols + col;
  return grid->gridPoints[idx];
}

/**************** build_grid() ****************/
/* see grid.h for description */
void
build_grid(grid_t* grid) {
  // validate grid
  if (grid == NULL) {
    return;
  }

  // ensure grid contains map string and grid points array
  if (grid->mapString == NULL || grid->gridPoints == NULL) {
    return;
  }

  // read grid points from map string to grid points array
  for (int i = 0; i < grid->size; i++) {
    // compute row and column numbers in the grid
    int row = i / grid->ncols;
    int col = i % grid->ncols;

    // specify grid point symbol
    char* symbol = mapString[i];

    // create new grid point and add it to array
    grid->gridPoints[i] = point_new(row, col, symbol);
  }
}
