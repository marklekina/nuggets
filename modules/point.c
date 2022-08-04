/*
 * point.c - 'point' module
 *
 * see point.h for more information.
 *
 * Mark Lekina Rorat, December 2021, August 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "point.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct point {
  int x;              // x coordinate
  int y;              // y coordinate
  char type;          // room, wall or boundary spot
  char occupant;      // player or gold pile
} point_t;

/**************** functions ****************/

/**************** point_new() ****************/
/* see point.h for description */
point_t*
point_new(int x, int y, char type)
{
  // allocate memory for point
  point_t* point = mem_malloc(sizeof(point_t));

  // assign coordinates to point
  if (point != NULL) {
    point->x = x;
    point->y = y;
    point->type = type;
    point->occupant = "";
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


/**************** get_x() ****************/
/* see point.h for description */
int
get_x(point_t* point)
{
  if (point != NULL) {
    return point->x;
  }
  return NULL;
}

/**************** get_y() ****************/
/* see point.h for description */
int
get_y(point_t* point)
{
  if (point != NULL) {
    return point->y;
  }
  return NULL;
}

/**************** set_occupant() ****************/
/* see point.h for description */
bool
set_occupant(point_t* point, char occupant)
{
  if (point != NULL) {
    point->occupant = occupant;
    return true;
  }
  return false;
}

/**************** get_occupant() ****************/
/* see point.h for description */
char
get_occupant(point_t* point)
{
  if (point != NULL) {
    return point->occupant;
  }
  return NULL;
}
