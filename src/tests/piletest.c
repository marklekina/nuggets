#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include "tester.h"
#include "mem.h"
#include "point.h"
#include "pile.h"

int
main(const int argc, char* argv[]) {

  // initialize testing
  srand(strlen(argv[0]));
  printf("%s: running tests...\n", argv[0]);
  testerInit();

  int row_A = rand() % 100;
  int row_B = rand() % 100;
  int col_A = rand() % 100;
  int col_B = rand() % 100;
  char symbol_A = '|';
  char symbol_B = '.';
  point_t* location_A = point_new(row_A, col_A, symbol_A);
  point_t* location_B = point_new(row_B, col_B, symbol_B);
  int gold_A = rand() % 100;
  int gold_B = rand() % 100;

  pile_t* pile_A, *pile_B;

  // pile_new
  TRY { pile_A = pile_new(location_A, gold_A); } ENDTRY;
  TRY { pile_B = pile_new(location_B, gold_B); } ENDTRY;
  TEST_ASSERT(pile_A && pile_B);

  // get_pile_location
  TEST_ASSERT(get_pile_location(pile_A) == location_A);
  TEST_ASSERT(get_pile_location(pile_B) == location_B);

  // get_gold
  TEST(get_gold(pile_A), gold_A);
  TEST(get_gold(pile_B), gold_B);

  // pile_delete
  TRY { pile_delete(pile_A); } ENDTRY;
  TRY { pile_delete(pile_B); } ENDTRY;

  // clean-up
  point_delete(location_A);
  point_delete(location_B);

  // complete testing
  testerReport(stdout, argv[0]);

  // print memory report if net malloc-free count is unbalanced
  if (mem_net() > 0) {
    mem_report(stderr, argv[0]);
  }

  printf("%s: testing complete\n", argv[0]);
  return testerResult();
}
