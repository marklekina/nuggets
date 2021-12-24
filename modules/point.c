/*
 * point.c - 'point' module
 *
 * see point.h for more information.
 *
 * Mark Lekina Rorat, December 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct point {
  int x;    // x coordinate
  int y;    // y coordinate
} point_t;

/**************** functions ****************/

/**************** point_new() ****************/
/* see point.h for description */
point_t*
point_new(int x, int y)
{
  // allocate memory for point
  point_t* point = mem_malloc(sizeof(point_t));

  // assign coordinates to point
  if (point != NULL) {
    point->x = x;
    point->y = y;
  }

  // return struct
  return point;
}

/**************** point_delete() ****************/
/* see point.h for description */
void 
point_delete(point_t* point)
{
  if (point != NULL) {
    // free the struct
    mem_free(point);
  }
}
