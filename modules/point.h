/*
 * point.h - header file for 'point' module
 *
 *
 * Mark Lekina Rorat, December 2021; August, October 2022
 */

#ifndef __POINT_H
#define __POINT_H

/**************** global types ****************/
typedef struct point point_t;

/**************** functions ****************/

/**************** point_new ****************/
/*
 * creates a new point
 *
 * caller provides:
 *  - valid row and column coordinates
 *  - valid grid point symbol, i.e.:
 *      - room spot ('.')
 *      - solid rock (' ')
 *      - boundary ('|', '-', '+')
 *
 * we return:
 *   pointer to a new point_t, or NULL if error.
 *
 * caller is responsible for:
 *   calling point_delete to free dynamically allocated memory.
 */
point_t* point_new(int row, int col, char symbol);

/**************** point_delete ****************/
/* delete the point.
 *
 * caller provides:
 *   a valid point_t pointer.
 *
 * we guarantee:
 *   we free the point's memory
 *   we ignore a NULL point.
 */
void point_delete(point_t* point);

/**************** get_row ****************/
/*
 * getter for the point's row coordinate.
 *
 * caller provides:
 *   a valid point_t pointer.
 *
 * function returns:
 *  row coordinate or null on error
 *
 */
int get_row(point_t* point);

/**************** get_col ****************/
/*
 * getter for the point's column coordinate.
 *
 * caller provides:
 *   a valid point_t pointer.
 *
 * function returns:
 *  column coordinate or null on error
 */
int get_col(point_t* point);

/**************** get_symbol ****************/
/*
 * getter for the point's type symbol.
 *
 * caller provides:
 *   a valid point_t pointer.
 *
 * function returns:
 *  the point's type symbol or null on error
 */
char get_symbol(point_t* point);

/**************** is_same_location ****************/
/*
 * compares the coordinates of two points to determine if they are the same grid point
 *
 * caller provides:
 *   two valid point_t pointer.
 *
 * function returns:
 *   true if they point to the same gridpoint
 *   false otherwise
 *
 */
bool is_same_location(point_t* pointA, point_t* pointB);

#endif // __POINT_H
