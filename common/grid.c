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

void grid_visibility(grid_t* grid, player_t* player, int x, int y){
    char* playerVisiblity = player->visibility;
    char* newVisibility = updateVisibility(grid, x, y);

    // combine finalVis = visibility and newVisbility (use helper function)
    playerVisiblity = combineVisibility(playerVisiblity, newVisibility);
    
    player->visibility = playerVisiblity;

}

char* combineVisibility(char* vis1, char* vis2){
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

char* updateVisibility(grid_t* grid, int row, int col){
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