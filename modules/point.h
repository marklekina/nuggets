/*
 * point.h - header file for 'point' module
 * 
 * A 'point' is an (x, y) coordinate pair
 *
 * Mark Lekina Rorat, December 2021
 */

#ifndef __POINT_H
#define __POINT_H

/**************** global types ****************/
typedef struct point point_t;  // opaque to users of the module

/**************** functions ****************/

/**************** point_new ****************/
/* create a new point
 * caller provides:
 *   valid x and y coordinates (integers)
 * we return:
 *   pointer to a new point, or NULL if error.
 * caller is responsible for:
 *   later calling point_delete.
 */
point_t* point_new(int x, int y);

/**************** point_delete ****************/
/* delete the point.
 *
 * caller provides:
 *   a valid point pointer.
 * we guarantee:
 *   we free the point's memory
 *   we ignore NULL point.
 */
void point_delete(point_t* point);

#endif // __POINT_H
