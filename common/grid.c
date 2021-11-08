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

/**************** grid_visibility ****************/
/*
 * see grid.h for more information
 */

void 
grid_visibility(grid_t* grid, player_t* player, int x, int y){
    char* playerVisiblity = get_visibility(player);
    char* newVisibility = updateVisibility(grid, x, y);

    // combine finalVis = visibility and newVisbility (use helper function)
    playerVisiblity = combineVisibility(playerVisiblity, newVisibility);
    
    set_visiblity(player, playerVisibility); 

}

/**************** combineVisibility ****************/
/*
 * see grid.h for more information
 */

char* 
combineVisibility(char* vis1, char* vis2){
    if(vis1 == NULL || vis2 == NULL){
        return NULL;
    }
    
    for(int i=0; i<strlen(vis1); i++){
        if(vis1[i] == 0){
            if(vis2[i] == 2){
                vis1[i] = 2;
            }
        }
        if(vis1[i] == 1){
            if(vis2[i] == 2){
                vis1[i] = 2;
            }
        }
        if(vis1[i] == 2){
            if(vis2[i] == 0){
                vis1[i] = 1;
            }
        }
    }

    return vis1;  
}
/**************** updateVisibility ****************/
/*
 * see grid.h for more information
 */

char* 
updateVisibility(grid_t* grid, int row, int col){
    // make a visibility string that shows what they can see
    char* newVisibility;

    int width = grid->nrows;
    int height = grid->ncols;
    
    char* map = grid->map;

    for(int i=0; i<strlen(map); i++){
        newVisibility[i] = 0;
    }

    int index;

    if(row<width && row >= 0){
        if(col< height && col>=0){
            index = (col*height)+row;
        }
    }

    // geometry
    // newVisibility[index] = 2;

    // we will change!!!!
    for(int i=0; i<strlen(map); i++){
        newVisibility[i] = 2;
    }
    
    // return that string
    return newVisibility;
}

