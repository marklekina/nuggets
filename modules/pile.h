/*
 * pile.h - header file for 'pile' module
 *
 * A 'pile' is a struct that represents a location on the grid with a predetermined amount of gold nuggets in that location
 *
 * Mark Lekina Rorat, December 2021; August, October 2022
 */

#ifndef __PILE_H
#define __PILE_H

/**************** global types ****************/
typedef struct pile pile_t;

/**************** functions ****************/

/**************** pile_new ****************/
/*
 * creates a new pile
 *
 * caller provides:
 *   valid location pointer (point_t*)
 *   valid gold amount ( > 0)
 *
 * we return:
 *   pointer to a new pile, or NULL if error.
 *
 * caller is responsible for:
 *   calling pile_delete later to free the pile's memory.
 */
pile_t* pile_new(point_t* location, int gold);

/**************** pile_delete ****************/
/* delete the pile.
 *
 * caller provides:
 *   a valid pile pointer.
 *
 * we guarantee:
 *   we free the pile's location memory
 *   we free the pile's memory
 *   we ignore NULL point.
 *
 * notes:
 *  - we do not free the pile's location memory
 *  - this is because the location is only held by the pile as a reference to that grid point on the grid
 *  - the location memory is, therefore, handled at the grid level
 */
void pile_delete(pile_t* pile);

/**************** get_location ****************/
/*
 * returns the pile's location.
 *
 * caller provides:
 *   a valid pile pointer.
 *
 * we return:
 *   a pointer to the gold pile's location.
 */
point_t* get_location(pile_t* pile);

/**************** get_gold ****************/
/*
 * returns the amount of gold in the pile.
 *
 * caller provides:
 *   a valid pile pointer.
 *
 * we return:
 *   the quantity of the gold in the pile.
 */
int get_gold(pile_t* pile);

#endif // __PILE_H
