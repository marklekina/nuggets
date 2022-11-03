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
#include "log.h"
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
point_new(int row, int col, char symbol)
{
  // allocate memory for point
  point_t* point = mem_malloc(sizeof(point_t));

  // log memory allocation error
  if (point == NULL) {
    log_v("point_new: error allocating memory for point struct");
    return NULL;
  }

  // assign coordinates and symbol to point
  point->row = row;
  point->col = col;
  point->symbol = symbol;

  // return struct
  return point;
}

/**************** point_delete() ****************/
/* see point.h for description */
void
point_delete(point_t* point)
{
  // log if NULL pointer is passed
  if (point == NULL) {
    log_v("point_delete: NULL point passed to function");
    return;
  }

  // free point memory
  mem_free(point);
}

/**************** get_row() ****************/
/* see point.h for description */
int
get_row(point_t* point)
{
  // log if NULL pointer is passed
  if (point == NULL) {
    log_v("get_row: NULL point passed to function");
    return -1;
  }

  // return row number
  return point->row;
}

/**************** get_col() ****************/
/* see point.h for description */
int
get_col(point_t* point)
{
  // log if NULL pointer is passed
  if (point == NULL) {
    log_v("get_col: NULL point passed to function");
    return -1;
  }

  // return column number
  return point->col;
}

/**************** get_symbol() ****************/
/* see point.h for description */
char
get_symbol(point_t* point)
{
  // log if NULL pointer is passed
  if (point == NULL) {
    log_v("get_symbol: NULL point passed to function");
    return '!';
  }

  // return symbol
  return point->symbol;
}

/**************** is_same_location() ****************/
/* see point.h for description */
bool
is_same_location(point_t* pointA, point_t* pointB)
{
  // validate points
  if (pointA == NULL || pointB == NULL) {
    log_v("is_same_location: NULL point passed to function");
    return false;
  }

  // compare coordinates and return true if we have a match
  if (get_row(pointA) == get_row(pointB) && get_col() == get_col(pointB)) {
    return true;
  }

  // return false otherwise
  return false;
}
