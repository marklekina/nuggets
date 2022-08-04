/*
 * point.h - header file for 'point' module
 *
 *
 * Mark Lekina Rorat, December 2021, August 2022
 */

#ifndef __POINT_H
#define __POINT_H

/**************** global types ****************/
typedef struct point point_t;

/**************** functions ****************/

/**************** point_new ****************/
/* create a new point
 * caller provides:
 *  valid x and y coordinates (integers)
 *  valid grid point type, i.e., one of the following:
 *      - room spot ('.'')
 *      - solid rock (' ')
 *      - boundary ('|', '-', '+')
 * we return:
 *   pointer to a new point_t, or NULL if error.
 * caller is responsible for:
 *   assigning an occupant if necessary
 *   calling point_delete.
 */
point_t* point_new(int x, int y, char type);

/**************** point_delete ****************/
/* delete the point.
 *
 * caller provides:
 *   a valid point_t pointer.
 * we guarantee:
 *   we free the point's memory
 *   we ignore NULL point.
 */
void point_delete(point_t* point);

/**************** get_x ****************/
/*
 * getter for the point's x coordinate.
 * caller provides:
 *   a valid point_t pointer.
 *
 * function returns:
 *  x coordinate or null on error
 *
 */
int get_x(point_t* point);

/**************** get_y ****************/
/*
 * getter for the point's y coordinate.
 *
 * caller provides:
 *   a valid point_t pointer.
 *
 * function returns:
 *  y coordinate or null on error
 */
int get_y(point_t* point);

/**************** set_occupant ****************/
/*
 * setter for the point's occupant, i.e., either a player or gold point_delete
 *  by default, the point has no occupant
 *
 * caller provides:
 *   a valid point_t pointer.
 *   a valid pile (*) or player (@, A-Z) char symbol
 *
 * function returns:
 *   true if successful
 *   false otherwise
 *
 */
bool set_occupant(point_t* point, char occupant);

/**************** get_occupant ****************/
/*
 * getter for the point's occupant
 * caller provides:
 *   a valid point_t pointer.
 *
 * we return:
 *   a valid pile (*) or player (@, A-Z) char symbol
 *
 */
char get_occupant(point_t* point);

#endif // __POINT_H
