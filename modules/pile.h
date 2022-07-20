/*
 * pile.h - header file for 'pile' module
 * 
 * A 'pile' is a struct that represents a location on the grid and a predetermined amount of gold in that location 
 *
 * Mark Lekina Rorat, December 2021
 */

#ifndef __PILE_H
#define __PILE_H

/**************** global types ****************/
typedef struct pile pile_t;  // opaque to users of the module

/**************** functions ****************/

/**************** pile_new ****************/
/* create a new pile
 * caller provides:
 *   valid location pointer (point_t*)
 *   valid gold amount ( > 0)
 * we return:
 *   pointer to a new pile, or NULL if error.
 * caller is responsible for:
 *   later calling pile_delete.
 */
pile_t* pile_new(point_t* location, int amount);

/**************** pile_delete ****************/
/* delete the pile.
 *
 * caller provides:
 *   a valid pile pointer.
 * we guarantee:
 *   we free the pile's location memory
 *   we free the pile's memory
 *   we ignore NULL point.
 */
void pile_delete(pile_t* pile);

#endif // __PILE_H
