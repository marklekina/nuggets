/* 
 * grid.h - header file for grid module
 *
 */

#ifndef _GRID_H
#define __GRID_H

#include <stdio.h>
#include <stdbool.h>
/**************** global types ****************/
typedef struct grid grid_t;  // opaque to users of the module

/**************** functions ****************/

/**************** grid_new ****************/
/* 
 * comments
 */
grid_t* grid_new(int nrows, int ncols);

/**************** grid_delete****************/
/* 
 * comments
 */
void grid_delete(grid_t* grid);

/**************** load_map ****************/
/* 
 * comments
 */
bool load_map(grid_t* grid, FILE* fp);

/**************** display_map ****************/
/*
 * see grid.h for more information
 */
void display_map(grid_t* grid);

/**************** grid_visibility ****************/
/*
* see grid.h for more information
*/
void grid_visibility(grid_t* grid, player_t* player, int x, int y);

/**************** combineVisibility ****************/
/*
* see grid.h for more information
*/

char* combineVisibility(char* vis1, char* vis2);

/**************** updateVisibility ****************/
/*
* see grid.h for more information
*/
char* updateVisibility(grid_t* grid, int row, int col);
