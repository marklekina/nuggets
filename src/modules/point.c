/*
 * point.c - 'point' module
 *
 * see point.h for more information.
 *
 * Mark Lekina Rorat, December 2021, August, October 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
  point_t* point = malloc(sizeof(point_t));

  // memory allocation error
  if (point == NULL) {
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
  // if NULL pointer is passed
  if (point == NULL) {
    return;
  }

  // free point memory
  free(point);
}

/**************** get_row() ****************/
/* see point.h for description */
int
get_row(point_t* point)
{
  // if NULL pointer is passed
  if (point == NULL) {
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
  // if NULL pointer is passed
  if (point == NULL) {
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
  // if NULL pointer is passed
  if (point == NULL) {
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
    return false;
  }

  // compare coordinates and return true if we have a match
  if (get_row(pointA) == get_row(pointB) && get_col(pointA) == get_col(pointB)) {
    return true;
  }

  // return false otherwise
  return false;
}

/**************** is_transparent() ****************/
/* see point.h for description */
bool
is_transparent(point_t* point) {
  // validate parameters
  if (point == NULL) {
    return false;
  }

  // "see-through" spots are either room spots('.') or passage spots('#')
  // return true if a room spot is either of those
  if (get_symbol(point) == '.' || get_symbol(point) == '#') {
    return true;
  }

  // otherwise return false
  return false;
}


/**************** is_room_spot() ****************/
/* see point.h for description */
bool
is_room_spot(point_t* point) {
  // validate parameters
  if (point == NULL) {
    return false;
  }

  // return true if the point is a room spot
  if (get_symbol(point) == '.') {
    return true;
  }

  // otherwise return false
  return false;
}


/**************** is_point_in_list() ****************/
/* see point.h for description */
bool
is_point_in_list(point_t* point, point_t* point_list[], int size) {
  // validate parameters
  if (point == NULL || point_list == NULL) {
    return false;
  }

  // loop through list of points and compare each point to the given point
  for (int i = 0; i < size; i++) {
    // return true if same point
    if (is_same_location(point, point_list[i])) {
      return true;
    }
  }

  // return false otherwise
  return false;
}
