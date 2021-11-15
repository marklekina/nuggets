/*
 * grid.h - header file for grid module
 *
 * this module contains:
 *     grid_t struct - holds a map string, number of rows and number of columns
 *     functions to:
 *        i) load map string into the grid
 *       ii) display the map
 *      iii) getter functions 
 *
 * Palmer's Posse, November 2021
 */


#ifndef __GRID_H
#define __GRID_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mem.h"
#include "file.h"

/**************** global types ****************/
typedef struct grid grid_t;  // opaque to users of the module

/**************** functions ****************/

/**************** grid_new ****************/
/* 
 * Creates a new grid or returns NULL if error
 * Creates a new grid with the provided nrows and ncols as its dimensions
 * it initializes the map and original map as NULL
 * returns initialized grid
 */
grid_t* grid_new(int nrows, int ncols);

/**************** grid_delete****************/
/* 
 * deletes the map and original map in the grid as well as the grid itself
 */
void grid_delete(grid_t* grid);

/**************** load_map ****************/
/* 
 *takes in a grid and file and sets the grid's map to the first line of the file 
 * The original map also gets set to that map
 * nrows then gets set to the number of lines in the file
 * ncols gets set to the string length of the first line
 * returns true but if something fails, returns false
 */
bool load_map(grid_t* grid, FILE* fp);

/**************** display_map ****************/
/*
 * prints the map string
 */
void display_map(grid_t* grid);

/**************** get_rows ****************/
/*
* returns the row's value for the grid
*/
int get_rows(grid_t* grid);

/**************** get_cols ****************/
/*
* returns the column's value for the grid
*/
int get_cols(grid_t* grid);

/**************** get_map ****************/
/*
* returns the map for the grid
*/
char* get_map(grid_t* grid);

/**************** get_original_map ****************/
/*
* returns the grid's original map
*/
char* get_original_map(grid_t* grid);

/**************** update_map ****************/
/*
* replaces the grid's map with the provided map
*/
bool update_map(grid_t* grid, char* map);

#endif // __GRID_H
