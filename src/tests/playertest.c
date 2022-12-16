#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include "tester.h"
#include "file.h"
#include "point.h"
#include "player.h"

int
main(const int argc, char* argv[]) {
  // initialize testing
  srand(time(0));
  printf("%s: running tests...\n", argv[0]);
  testerInit();

  // generate player information
  char* name = strdup("Test player");
  char letter = 'A';

  int row_A = rand() % 100;
  int col_A = rand() % 100;
  char symbol_A = 'A';
  point_t* location_A = point_new(row_A, col_A, symbol_A);

  FILE* fp_A = fopen("/Users/markrorat/projects/C/nuggets/data/maps/big.txt", "r");
  assert(fp_A);
  char* visible_map_A = file_readFile(fp_A);
  fclose(fp_A);

  player_t* player;

  // player_new
  TRY { player = player_new(name, letter, location_A, visible_map_A); } ENDTRY;
  TEST_ASSERT(player);

  // get_name
  TEST_ASSERT(get_name(player) == name);
  TEST_ASSERT(!get_name(NULL));

  // get_letter
  TEST_ASSERT(get_letter(player) == letter);
  TEST_ASSERT(get_letter(NULL) == (char) 0);

  // get_location
  TEST_ASSERT(is_same_location(get_location(player), location_A));
  TEST_ASSERT(!get_location(NULL));

  // get_visible_map
  TEST_ASSERT(get_visible_map(player) == visible_map_A);
  TEST_ASSERT(!get_visible_map(NULL));

  // get_wallet_balance
  TEST(get_wallet_balance(player), 0);
  TEST(get_wallet_balance(NULL), -1);

  // update player information
  int row_B = rand() % 100;
  int col_B = rand() % 100;
  char symbol_B = 'B';
  point_t* location_B = point_new(row_B, col_B, symbol_B);

  FILE* fp_B = fopen("/Users/markrorat/projects/C/nuggets/data/maps/challenge.txt", "r");
  assert(fp_B);
  char* visible_map_B = file_readFile(fp_B);
  fclose(fp_B);

  int gold_collected = rand() % 100;

  // update_location
  TEST_ASSERT(update_location(player, location_B));
  TEST_ASSERT(is_same_location(get_location(player), location_B));
  TEST_ASSERT(!update_location(NULL, location_B));
  TEST_ASSERT(!update_location(player, NULL));

  // update_visible_map
  TEST_ASSERT(update_visible_map(player, visible_map_B));
  TEST_ASSERT(get_visible_map(player) == visible_map_B);
  TEST_ASSERT(!update_visible_map(NULL, visible_map_B));
  TEST_ASSERT(!update_visible_map(player, NULL));

  // update_wallet_balance
  TEST_ASSERT(!update_wallet_balance(NULL, gold_collected));
  TEST_ASSERT(!update_wallet_balance(player, -1));
  TEST(get_wallet_balance(player), 0);
  TEST_ASSERT(update_wallet_balance(player, gold_collected));
  TEST(get_wallet_balance(player), gold_collected);

  // player_delete
  TRY { player_delete(player); } ENDTRY;

  // complete testing
  testerReport(stdout, argv[0]);
  printf("%s: testing complete\n", argv[0]);
  return testerResult();

}
