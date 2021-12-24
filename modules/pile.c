/* 
 * pile.c - 'pile' module
 *
 * see pile.h for more information.
 *
 * Mark Lekina Rorat, December 2021
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
  int amount;           // gold amount in pile 
} bag_t;

/**************** global functions ****************/

/**************** pile_new() ****************/
/* see pile.h for description */
pile_t*
pile_new(point_t* location, int amount)
{
  // validate parameters
  if (location != NULL && amount > 0) {
    // allocate memory for pile
    pile_t* pile = mem_malloc(sizeof(pile_t));

    // assign location and gold amount to pile
    if (pile != NULL) {
      pile->location = location;
      pile->amount = amount;
    }

    // return struct
    return pile;
  }

  // otherwise return null
  return NULL;
}

/**************** pile_delete() ****************/
/* see pile.h for description */
void 
pile_delete(pile_t* pile)
{
  if (pile != NULL) {
    // free location
    point_delete(pile->location);
    
    // free pile memory
    mem_free(pile);
  }
}
