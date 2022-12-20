#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include "tester.h"
#include "mem.h"
#include "point.h"
#include "pile.h"
#include "player.h"
#include "grid.h"
#include "game.h"

int
main(const int argc, char* argv[]) {
  // initialize testing
  srand(strlen(argv[0]));
  printf("%s: running tests...\n", argv[0]);
  testerInit();

  // set-up
  game_t* game;
  int max_players = 26;
  int max_piles = 30;
  int gold_balance = 250;
  FILE* fp = fopen("../../data/maps/big.txt", "r");
  assert(fp);

  // 1. game_new
  TRY { game = game_new(fp, max_players, max_piles, gold_balance); } ENDTRY;
  TEST_ASSERT(game);

  // 2. get_num_players
  TEST(get_num_players(game), 0);

  // 3. get_num_piles
  TEST(get_num_piles(game), 0);

  // 4. get_gold_balance
  TEST(get_gold_balance(game), gold_balance);

  // 5. get_grid
  grid_t* test_grid = grid_new(fp);
  char* test_mapstring = get_map_string(test_grid);
  TEST(strcmp(get_map_string(get_grid(game)), test_mapstring), 0);

  // 6. get_players
  TEST_ASSERT(get_players(game));

  // 7. get_piles
  TEST_ASSERT(get_piles(game));

  // 8. game_delete
  TRY { game_delete(game); } ENDTRY;

  // clean-up
  fclose(fp);
  grid_delete(test_grid);

  // print testing report
  testerReport(stdout, argv[0]);

  // print memory report if net malloc-free count is unbalanced
  if (mem_net() > 0) {
    mem_report(stderr, argv[0]);
  }

  // complete testing
  printf("%s: testing complete\n", argv[0]);
  return testerResult();
}
