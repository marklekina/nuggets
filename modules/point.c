/*
 * point.c - 'point' module
 *
 * see point.h for more information.
 *
 * Mark Lekina Rorat, December 2021, August, October 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "point.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct point {
  int row;            // row coordinate
  int col;            // column coordinate
  char symbol;        // room, wall or boundary spot
} point_t;

/**************** functions ****************/

/**************** point_new() ****************/
/* see point.h for description */
point_t*
point_new(int x, int y, char symbol)
{
  // allocate memory for point
  point_t* point = mem_malloc(sizeof(point_t));

  // assign coordinates and symbol to point
  if (point != NULL) {
    point->x = x;
    point->y = y;
    point->symbol = symbol;
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
    // free point memory
    mem_free(point);
  }
}

/**************** get_row() ****************/
/* see point.h for description */
int
get_row(point_t* point)
{
  if (point != NULL) {
    return point->row;
  }
  return NULL;
}

/**************** get_col() ****************/
/* see point.h for description */
int
get_col(point_t* point)
{
  if (point != NULL) {
    return point->col;
  }
  return NULL;
}

/**************** get_symbol() ****************/
/* see point.h for description */
char
get_symbol(point_t* point)
{
  if (point != NULL) {
    return point->symbol;
  }
  return NULL;
}

/**************** is_same_location() ****************/
/* see point.h for description */
bool
is_same_location(point_t* point_x, point_t* point_y)
{
  // validate points
  if (point_x == NULL || point_y == NULL) {
    return false;
  }

  // compare coordinates and return true if we have a match
  if (get_row(point_x) == get_row(point_y) && get_col(point_x) == get_col(point_y)) {
    return true;
  }

  // return false otherwise
  return false;
}
