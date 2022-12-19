/*
 * grid.h - header file for 'grid' module
 *
 * A 'grid' is a struct that holds the representation of the game's map
 *
 * Mark Lekina Rorat, December 2021; October 2022
 */

#ifndef __GRID_H
#define __GRID_H

/**************** global types ****************/
typedef struct grid grid_t;  // opaque to users of the module

/**************** functions ****************/

/**************** grid_new ****************/
/*
 * creates a new grid
 *
 * caller provides:
 *   valid pointer to map file
 *
 * we return:
 *   pointer to a new grid, or NULL if error.
 *
 * caller is responsible for:
 *   later calling grid_delete to free the grid's memory.
 */
grid_t* grid_new(FILE* fp);

/**************** build_grid ****************/
/*
 * constructs the grid points array.
 *
 * caller provides:
 *   a valid grid pointer.
 *
 * we guarantee:
 *   we populate the grid's grid points array as defined by the grid's map string.
 */
void build_grid(grid_t* grid);

/**************** grid_delete ****************/
/*
 * deletes the grid.
 *
 * caller provides:
 *   a valid grid pointer.
 *
 * we guarantee:
 *   we free the map string's memory
 *   we free the array of grid points (and its constituent grid points)
 *   we free the grid struct's memory
 *   we ignore NULL grid.
 */
void grid_delete(grid_t* grid);

/**************** get_map_string ****************/
/*
 * returns the grid's string representation.
 *
 * caller provides:
 *   a valid grid pointer.
 *
 * we return:
 *   the grid's map string.
 */
char* get_map_string(grid_t* grid);

/**************** get_gridpoints ****************/
/*
 * returns the grid's array representation.
 *
 * caller provides:
 *   a valid grid pointer.
 *
 * we return:
 *   the grid's array of grid points.
 */
point_t** get_gridpoints(grid_t* grid);

/**************** get_gridpoint ****************/
/*
 * returns a specific grid point from the grid points array.
 *
 * caller provides:
 *   a valid grid pointer
 *   row coordinate
 *   column coordinate
 *
 * we return:
 *   a pointer to the specified grid point.
 */
point_t* get_gridpoint(grid_t* grid, int row, int col);

/**************** get_size ****************/
/*
 * returns the grid's size.
 *
 * caller provides:
 *   a valid grid pointer.
 *
 * we return:
 *   the grid's size.
 */
int get_size(grid_t* grid);

/**************** get_nrows ****************/
/*
 * returns the number of rows in the grid.
 *
 * caller provides:
 *   a valid grid pointer.
 *
 * we return:
 *   the number of rows in the grid.
 */
int get_nrows(grid_t* grid);

/**************** get_ncols ****************/
/*
 * returns the number of columns in the grid.
 *
 * caller provides:
 *   a valid grid pointer.
 *
 * we return:
 *   the number of columns in the grid.
 */
int get_ncols(grid_t* grid);

/**************** compute_visibility ****************/
/*
 * compute whether pointA is visible from pointB
 *
 * caller provides:
 *   valid grid pointer
 *   two valid grid point pointers
 *
 * we return:
 *   true if visible, false otherwise
 */
bool compute_visibility(grid_t* grid, point_t* pointA, point_t* pointB);

#endif // __GRID_H
