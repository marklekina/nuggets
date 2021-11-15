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

grid_t* grid_new(int nrows, int ncols);
void grid_delete(grid_t* grid);
bool load_map(grid_t* grid, FILE* fp);
void display_map(grid_t* grid);
int get_rows(grid_t* grid);
int get_cols(grid_t* grid);
char* get_map(grid_t* grid);
bool update_map(grid_t* grid, char* map);

#endif // __GRID_H
