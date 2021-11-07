/* 
 * grid.c module
 *
 * see grid.h for more information.
 *
 * Palmer's Posse
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"
#include "mem.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct grid {
  char* map;
  int nrows;
  int ncols;
} grid_t;


grid_t* grid_new(char* map, int nrows, int ncols){
    grid_t* grid = mem_malloc(sizeof(grid_t));
    if(grid == NULL){
        return NULL;
    }
    if(map == NULL || nrows == NULL || ncols == NULL){
        return NULL;
    }
    else{
        grid->map = map;
        grid->nrows = nrows;
        grid->ncols = ncols;
        return grid;
    }  
}



