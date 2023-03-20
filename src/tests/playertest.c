#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include "tester.h"
#include "mem.h"
#include "file.h"
#include "point.h"
#include "player.h"
#include "message.h"

int
main(const int argc, char* argv[]) {
  // initialize testing
  srand(strlen(argv[0]));
  printf("%s: running tests...\n", argv[0]);
  testerInit();

  // generate player information
  char* name = strdup("Test player");
  char letter = 'A';

  int row_A = rand() % 100;
  int col_A = rand() % 100;
  char symbol_A = 'A';
  point_t* location_A = point_new(row_A, col_A, symbol_A);

  FILE* fp_A = fopen("../../data/maps/big.txt", "r");
  assert(fp_A);
  char* visible_map_A = file_readFile(fp_A);
  fclose(fp_A);

  player_t* player;

  // generate random address for player
  addr_t address, spec_address;
  message_random_address(&address);

  // player_new
  TRY { player = player_new(name, letter, location_A, visible_map_A, address); }
  ENDTRY;
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

  // get_address
  TEST_ASSERT(message_eqAddr(address, get_address(player)));

  // update player information
  int row_B = rand() % 100;
  int col_B = rand() % 100;
  char symbol_B = 'B';
  point_t* location_B = point_new(row_B, col_B, symbol_B);

  FILE* fp_B = fopen("../../data/maps/challenge.txt", "r");
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

  // generate spectator information
  player_t *spectator;
  char* spec_name = strdup("_SPECTATOR_");
  char* visible_map_C = strdup(visible_map_B);

  // generate random address for spectator
  message_random_address(&spec_address);

  // create spectator
  TRY { spectator = player_new(spec_name, '_', NULL, visible_map_C, spec_address); }
  ENDTRY;
  TEST_ASSERT(spectator);

  // is_spectator
  TEST_ASSERT(is_spectator(spectator));
  TEST_ASSERT(!is_spectator(player));

  // update_spectator
  TEST_ASSERT(message_eqAddr(spec_address, get_address(spectator)));
  TEST_ASSERT(update_spectator(spectator, address));
  TEST_ASSERT(message_eqAddr(address, get_address(spectator)));

  // player_delete
  TRY { 
    player_delete(player);
    player_delete(spectator);
  }
  ENDTRY;

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
