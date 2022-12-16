#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include "tester.h"
#include "point.h"

int
main(const int argc, char* argv[])
{
  // initialize testing
  srand(time(0));
  printf("%s: running tests...\n", argv[0]);
  testerInit();

  int row_A = rand() % 100;
  int row_B = rand() % 100;
  int col_A = rand() % 100;
  int col_B = rand() % 100;
  char symbol_A = '|';
  char symbol_B = '.';

  point_t* point_A, *point_B, *point_C, *point_D, *point_E;
  point_t** point_list;

  // point_new
  TRY { point_A = point_new(row_A, col_A, symbol_A); } ENDTRY;
  TRY { point_B = point_new(row_B, col_B, symbol_B); } ENDTRY;
  TEST_ASSERT(point_A && point_B);

  // get_row
  TEST(get_row(point_A), row_A);
  TEST(get_row(point_B), row_B);

  // get_col
  TEST(get_col(point_A), col_A);
  TEST(get_col(point_B), col_B);

  // get_col
  TEST_ASSERT(symbol_A == get_symbol(point_A));
  TEST_ASSERT(symbol_B == get_symbol(point_B));

  TRY { point_C = point_new(row_A, col_A, '|'); } ENDTRY;
  TRY { point_D = point_new(rand() % 100, rand() % 100, '#'); } ENDTRY;

  // is_same_location
  TEST_ASSERT(is_same_location(point_A, point_C));
  TEST_ASSERT(!is_same_location(point_A, point_B));

  // is_transparent
  TEST_ASSERT(is_transparent(point_B) && is_transparent(point_D));
  TEST_ASSERT(!is_transparent(point_A) && !is_transparent(point_C));

  // is_room_spot
  TEST_ASSERT(!is_room_spot(point_A));
  TEST_ASSERT(is_room_spot(point_B));

  TRY { point_list = malloc(sizeof(point_t*) * 4); } ENDTRY;
  TRY { point_E = point_new(rand() % 100, rand() % 100, '+'); } ENDTRY;
  TEST_ASSERT(point_list);

  point_list[0] = point_A;
  point_list[1] = point_B;
  point_list[2] = point_C;
  point_list[3] = point_D;

  // is_point_in_list
  TEST_ASSERT(is_point_in_list(point_A, point_list, 4));
  TEST_ASSERT(is_point_in_list(point_C, point_list, 4));
  TEST_ASSERT(!is_point_in_list(point_E, point_list, 4));

  // point_delete
  for (int i = 0; i < 4; i++) {
    TRY { point_delete(point_list[i]); } ENDTRY;
  }

  TRY { point_delete(NULL); } ENDTRY;
  TRY { point_delete(point_E); } ENDTRY;
  TRY { free(point_list); } ENDTRY;

  // complete testing
  testerReport(stdout, argv[0]);
  printf("%s: testing complete\n", argv[0]);
  return testerResult();
}
