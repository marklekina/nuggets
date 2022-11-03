/*
 * pile.c - 'pile' module
 *
 * see pile.h for more information.
 *
 * Mark Lekina Rorat, December 2021, August 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "log.h"
#include "point.h"
#include "pile.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct pile {
  point_t* location;    // pile location
  int gold;           // gold amount in pile
} pile_t;

/**************** global functions ****************/

/**************** pile_new() ****************/
/* see pile.h for description */
pile_t*
pile_new(point_t* location, int gold)
{
  // validate parameters
  if (location == NULL) {
    log_v("pile_new: NULL point passed to function");
    return NULL;
  }
  if (gold <= 0) {
    log_d("pile_new: invalid gold amount (%d) allocated to pile", gold);
    return NULL;
  }

  // allocate memory for pile
  pile_t* pile = mem_malloc(sizeof(pile_t));

  // log memory allocation error
  if (pile == NULL) {
    log_v("pile_new: error allocating memory for pile struct");
    return NULL;
  }

  // assign location and gold amount to pile
  pile->location = location;
  pile->gold = gold;

  // return struct
  return pile;
}

/**************** pile_delete() ****************/
/* see pile.h for description */
void
pile_delete(pile_t* pile)
{
  // log if NULL pointer is passed
  if (pile == NULL) {
    log_v("pile_delete: NULL pile passed to function");
    return;
  }

  // free pile memory
  point_delete(pile->location);
  mem_free(pile);
}

/**************** get_location() ****************/
/* see pile.h for description */
point_t*
get_location(pile_t* pile);
{
  if (pile == NULL) {
    log_v("get_location: NULL pile passed to function");
    return NULL;
  }
  return pile->location;
}

/**************** get_gold() ****************/
/* see pile.h for description */
int
get_gold(pile_t* pile);
{
  if (pile == NULL) {
    log_v("get_gold: NULL pile passed to function");
    return -1;
  }
  return pile->gold;
}
