/*
 * grid.c - 'grid' module
 *
 * see grid.h for more information.
 *
 * Mark Lekina Rorat, December 2021; November 2022
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mem.h"
#include "file.h"
#include "log.h"
#include "point.h"
#include "grid.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct grid {
  int nrows;            // number of rows in the map grid
  int ncols;            // number of columns in the map grid
  int size;             // number of grid points in the grid, i.e., nrows * ncols
  char* mapString;      // original map string (without occupants)
  point_t** gridPoints; // array containing each grid point in the game
} grid_t;

/**************** functions ****************/

/**************** grid_new() ****************/
/* see grid.h for description */
grid_t*
grid_new(FILE* fp)
{
  // validate map file pointer
  if (fp == NULL) {
    log_v("grid_new: NULL file pointer passed to function");
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
    log_v("grid_new: error allocating memory for grid struct");
    return NULL;
  }

  // read map string from file
  char* mapString = file_readFile(fp);
  if (mapString == NULL) {
    log_v("grid_new: error reading map string from file");
    return NULL;
  }

  // allocate memory for grid points array
  int size = strlen(mapString);
  point_t** gridPoints = mem_malloc(sizeof(point_t*) * size);
  if (gridPoints == NULL) {
    log_v("grid_new: error allocating memory for gridpoints array");
    return NULL;
  }

  // load grid parameters into the grid return the grid struct
  grid->nrows = nrows;
  grid->ncols = ncols;
  grid->size = size;
  grid->mapString = mapString;
  grid->gridPoints = gridPoints;

  // return the grid struct
  return grid;
}


/**************** build_grid() ****************/
/* see grid.h for description */
void
build_grid(grid_t* grid) {
  // validate grid
  if (grid == NULL) {
    log_v("grid_delete: NULL grid pointer passed to function");
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


/**************** grid_delete() ****************/
/* see grid.h for description */
void
grid_delete(grid_t* grid)
{
  // validate map text file
  if (grid == NULL) {
    log_v("grid_delete: NULL grid pointer passed to function");
    return NULL;
  }

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


/**************** get_map_string() ****************/
/* see grid.h for description */
char*
get_map_string(grid_t* grid) {
  // validate grid
  if (grid == NULL) {
    log_v("get_map_string: NULL grid pointer passed to function");
    return NULL;
  }

  // return grid map string
  return grid->mapString;
}


/**************** get_gridpoints() ****************/
/* see grid.h for description */
point**
get_gridpoints(grid_t* grid) {
  // validate grid
  if (grid == NULL) {
    log_v("get_gridpoints: NULL grid pointer passed to function");
    return NULL;
  }

  // return grid points array
  return grid->gridPoints;
}


/**************** get_gridpoint() ****************/
/* see grid.h for description */
point_t*
get_gridpoint(grid_t* grid, int row, int col) {
  // validate grid
  if (grid == NULL) {
    log_v("get_gridpoint: NULL grid pointer passed to function");
    return NULL;
  }

  // ensure row and column numbers fall within the grid
  // TODO: test range for edge cases (0, nrows + 1, ncols + 1)
  if (row < 0 || row >= grid->nrows) {
    log_d("get_gridpoint: row %d is outside the grid's range", row);
    return NULL;
  }

  if (col < 0  || col >= grid->ncols) {
    log_d("get_gridpoint: column %d is outside the grid's range", col);
    return NULL;
  }

  // return grid point at specified coordinates
  int idx = row * grid->ncols + col;
  return grid->gridPoints[idx];
}


/**************** get_size() ****************/
/* see grid.h for description */
int
get_size(grid_t* grid) {
  // validate grid
  if (grid == NULL) {
    log_v("get_size: NULL grid pointer passed to function");
    return -1;
  }

  // return grid size
  return grid->size;
}


/**************** get_nrows() ****************/
/* see grid.h for description */
int
get_nrows(grid_t* grid) {
  // validate grid
  if (grid == NULL) {
    log_v("get_nrows: NULL grid pointer passed to function");
    return -1;
  }

  // return no. of rows in the grid
  return grid->nrows;
}


/**************** get_ncols() ****************/
/* see grid.h for description */
int
get_ncols(grid_t* grid) {
  // validate grid
  if (grid == NULL) {
    log_v("get_ncols: NULL grid pointer passed to function");
    return -1;
  }

  // return no. of columns in the grid
  return grid->ncols;
}


/**************** compute_visibility() ****************/
/* see grid.h for description */
bool
compute_visibility(grid_t* grid, point_t* pointA, point_t* pointB) {
  // validate grid
  if (grid == NULL) {
    log_v("compute_visibility: NULL grid pointer passed to function");
    return false;
  }

  // validate point
  if (pointA == NULL || pointB == NULL) {
    log_v("compute_visibility: NULL grid point pointer passed to function");
    return false;
  }

  // fetch coordinates from grid points
  int row_a = get_row(pointA);
  int row_b = get_row(pointB);
  int col_a = get_col(pointA);
  int col_b = get_col(pointB);

  // TODO: implement visibility algorithm here

  return true;
}

void
compute_intersection(int row_a, int col_a, int row_b, int col_b) {
  // compute distance between points
  int row_diff = row_b - row_a;
  int col_diff = col_b - col_a;

  // compute direction of movement
  int row_step = row_diff / abs(row_diff);
  int col_step = col_diff / abs(col_diff);

  // edge case: the two points lie on the same vertical line
  if (col_diff == 0) {
    // moving stepwise by row
    if (abs(row_diff) > 1) {
      printf("moving stepwise by row\n");
      // variables to hold column intersections
      double col_i;
      int col_floor;
      int col_ceil;

      // loop through each row between the points
      for (int row_i = row_a + row_step; row_i != row_b; row_i += row_step) {
        // compute points of intersection
        col_i = col_a;
        col_floor = (int) floor(col_i);
        col_ceil = (int) ceil(col_i);

        // print point(s) of intersection
        // TODO: replace this with returning the grid point with specified coordinates
        printf("\tcolumn %d, row %d\n", col_floor, row_i);
        if (col_floor != col_ceil) {
          printf("\tcolumn %d, row %d\n", col_i, col_ceil);
        }
      }
    }
    return;
  }

  // edge case: the two points lie on the same horizontal line
  if (row_diff == 0) {
    // moving stepwise by column
    if (abs(col_diff) > 1) {
      printf("moving stepwise by column\n");
      // variables to hold row intersections
      double row_i;
      int row_floor;
      int row_ceil;

      // loop through each column between the points
      for (int col_i = col_a + col_step; col_i != col_b; col_i += col_step) {
        // compute points of intersection
        row_i = row_a;
        row_floor = (int) floor(row_i);
        row_ceil = (int) ceil(row_i);

        // print point(s) of intersection
        // TODO: replace this with returning the grid point with specified coordinates
        printf("\tcolumn %d, row %d\n", col_i, row_floor);
        if (row_floor != row_ceil) {
          printf("\tcolumn %d, row %d\n", col_i, row_ceil);
        }
      }
    }
    return;
  }

  // compute slope
  double slope = (float) row_diff / col_diff;

  // moving stepwise by column
  if (abs(col_diff) > 1) {
    printf("moving stepwise by column\n");

    // variables to hold row intersections
    double row_i;
    int row_floor;
    int row_ceil;

    // loop through each column between the points
    for (int col_i = col_a + col_step; col_i != col_b; col_i += col_step) {
      // compute points of intersection
      row_i = row_a + slope * (col_i - col_a);
      row_floor = (int) floor(row_i);
      row_ceil = (int) ceil(row_i);

      // print point(s) of intersection
      // TODO: replace this with returning the grid point with specified coordinates
      printf("\tcolumn %d, row %d\n", col_i, row_floor);
      if (row_floor != row_ceil) {
        printf("\tcolumn %d, row %d\n", col_i, row_ceil);
      }
    }
  }

  // moving stepwise by row
  if (abs(row_diff) > 1) {
    printf("moving stepwise by row\n");
    // variables to hold column intersections
    double col_i;
    int col_floor;
    int col_ceil;

    // loop through each row between the points
    for (int row_i = row_a + row_step; row_i != row_b; row_i += row_step) {
      // compute points of intersection
      col_i = col_a + (row_i - row_a) / slope;
      col_floor = (int) floor(col_i);
      col_ceil = (int) ceil(col_i);

      // print point(s) of intersection
      // TODO: replace this with returning the grid point with specified coordinates
      printf("\tcolumn %d, row %d\n", col_floor, row_i);
      if (col_floor != col_ceil) {
        printf("\tcolumn %d, row %d\n", col_i, col_ceil);
      }
    }
  }
}
