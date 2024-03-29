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
    return NULL;
  }
  if (gold <= 0) {
    return NULL;
  }

  // allocate memory for pile
  pile_t* pile = mem_malloc(sizeof(pile_t));

  // memory allocation error
  if (pile == NULL) {
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
  // if NULL pointer is passed
  if (pile == NULL) {
    return;
  }

  // free pile location
  point_delete(pile->location);

  // free pile memory
  mem_free(pile);
  pile = NULL;
}

/**************** get_pile_location() ****************/
/* see pile.h for description */
point_t*
get_pile_location(pile_t* pile)
{
  if (pile == NULL) {
    return NULL;
  }
  return pile->location;
}

/**************** get_gold() ****************/
/* see pile.h for description */
int
get_gold(pile_t* pile)
{
  if (pile == NULL) {
    return 0;
  }
  return pile->gold;
}

/**************** update_pile_balance() ****************/
/* see pile.h for description */
bool
update_pile_balance(pile_t* pile) {
  if (pile == NULL) {
    return false;
  }
  pile->gold = 0;
  return true;
}
