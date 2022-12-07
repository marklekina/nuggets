#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "point.h"
#include "log.h"
#include "mem.h"

int
main(const int argc, char* argv[])
{
  log_init(stderr);
  srand(time(0));
  int row_A = rand() % 100;
  int row_B = rand() % 100;
  int col_A = rand() % 100;
  int col_B = rand() % 100;
  char symbol_A = '|';
  char symbol_B = '.';

  // point_new
  point_t* point_A = point_new(row_A, col_A, '|');
  point_t* point_B = point_new(row_B, col_B, '.');
  assert(point_A != NULL && point_B != NULL);

  // get_row
  assert(row_A == get_row(point_A));
  assert(row_B == get_row(point_B));

  // get_col
  assert(col_A == get_col(point_A));
  assert(col_B == get_col(point_B));

  // get_col
  assert(symbol_A == get_symbol(point_A));
  assert(symbol_B == get_symbol(point_B));

  point_t* point_C = point_new(row_A, col_A, '|');
  point_t* point_D = point_new(rand() % 100, rand() % 100, '#');

  // is_same_location
  assert(is_same_location(point_A, point_C));
  assert(!is_same_location(point_A, point_B));

  // is_transparent
  assert(is_transparent(point_B) && is_transparent(point_D));
  assert(!is_transparent(point_A) && !is_transparent(point_C));

  // is_room_spot
  assert(!is_room_spot(point_A));
  assert(is_room_spot(point_B));

  point_t** point_list = mem_malloc(sizeof(point_t*) * 4);
  point_t* point_E = point_new(rand() % 100, rand() % 100, '+');
  point_list[0] = point_A;
  point_list[1] = point_B;
  point_list[2] = point_C;
  point_list[3] = point_D;

  // is_point_in_list
  assert(is_point_in_list(point_A, point_list) && is_point_in_list(point_C, point_list));
  assert(!is_point_in_list(point_E, point_list));

  // point_delete
  for (int i = 0; i < 4; i++) {
    assert(point_delete(point_list[i]));
  }
  assert(!point_delete(NULL));
  mem_free(point_list);

  // print if all code passes all tests
  log_v("point module: test successful\n");
  log_done();

  // return successfully
  return(0);
}
