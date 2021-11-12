/* 
 * grid.h - header file for grid module
 *
 */


#ifndef __GRID_H
#define __GRID_H

#include <stdio.h>
#include <stdbool.h>
#include "player.h"

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
char* load_map(grid_t* grid, FILE* fp);


/**************** grid_setMap ****************/
/*
 * see grid.h for more information
 */
bool grid_setMap(grid_t* grid, char* map);

/**************** display_map ****************/
/*
 * see grid.h for more information
 */

void display_map(grid_t* grid);


/**************** intialize_visibility ****************/
/*
 * see player.h for more information
 */
void intialize_visibility(grid_t* grid, player_t* player);

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

/**************** rid_getnRows ****************/
/*
* see grid.h for more information
*/
int rid_getnRows(grid_t* grid);
/**************** rid_getnCols ****************/
/*
* see grid.h for more information
*/
int rid_getnCols(grid_t* grid);

#endif