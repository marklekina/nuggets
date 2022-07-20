/*
 * grid.h - header file for 'grid' module
 * 
 * A 'grid' is a struct holding a string representation of the game's map, as well as the number of rows and columns in the map
 *
 * Mark Lekina Rorat, December 2021
 */

#ifndef __GRID_H
#define __GRID_H

/**************** global types ****************/
typedef struct grid grid_t;  // opaque to users of the module

/**************** functions ****************/

/**************** grid_new ****************/
/* create a new grid
 * caller provides:
 *   valid pointer to map file
 * we return:
 *   pointer to a new grid, or NULL if error.
 * caller is responsible for:
 *   later calling grid_delete.
 */
grid_t* grid_new(FILE* fp);

/**************** grid_delete ****************/
/* delete the grid.
 *
 * caller provides:
 *   a valid grid pointer.
 * we guarantee:
 *   we free the map string's memory
 *   we free the grid struct's memory
 *   we ignore NULL grid.
 */
void grid_delete(grid_t* grid);

/**************** point_to_index ****************/
/* compute the index of a grid point on the map string, i.e. (x,y) -> map[idx]
 *
 * caller provides:
 *   grid pointer
 *   point pointer
 * we return:
 *   the index of the grid point on the map string, if computed successfully
 *   -1 on error 
 */
int point_to_index(grid_t* grid, point_t* point);

#endif // __GRID_H
