#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include "mem.h"
#include "file.h"
#include "message.h"
#include "tester.h"
#include "point.h"
#include "pile.h"
#include "player.h"
#include "grid.h"
#include "game.h"

int
main(const int argc, char* argv[]) {
  // initialize testing
  srand(getpid());
  printf("%s: running tests...\n", argv[0]);
  testerInit();

  // setup
  // game constants
  int min_piles = 10;
  int max_piles = 30;
  int max_players = 26;
  int gold_balance = 250;

  // game map file
  FILE* fp = fopen("../../data/maps/big.txt", "r");
  assert(fp);

  // game_new
  game_t* game;
  TRY { game = game_new(fp, max_players, max_piles, gold_balance); } ENDTRY;
  TEST_ASSERT(game);

  // distribute_gold
  TEST_ASSERT(distribute_gold(game, min_piles, max_piles));

  // get_num_piles
  int num_piles;
  TRY { num_piles = get_num_piles(game); } ENDTRY;
  TEST_ASSERT(num_piles >= min_piles && num_piles <= max_piles);

  // get_piles
  // update_gold_balance
  // get_gold_balance
  pile_t** piles;
  bool pile_balance_valid = true;

  TRY { 
    piles = get_piles(game);
  } ENDTRY;

  // check that:
  //  - each pile has at least one gold nugget
  //  - the sum of gold in each pile equals total gold
  for (int i = 0; i < num_piles; i++) {
    int pile_balance = get_gold(piles[i]);
    update_gold_balance(game, pile_balance);

    if (pile_balance < 1) {
      pile_balance_valid = false;
      break;
    }
  }
  TEST_ASSERT(pile_balance_valid);
  TEST(get_gold_balance(game), 0);

  // get_grid
  grid_t* grid;

  rewind(fp);
  char* mapstring = file_readFile(fp);

  TRY {
    grid = get_grid(game);
  } ENDTRY;

  char* grid_mapstring = get_map_string(grid);
  TEST(strcmp(grid_mapstring, mapstring), 0);

  // add_player

  // create random addresses for each player
  addr_t addr_one, addr_two, addr_spectator;
  message_random_address(&addr_one);
  message_random_address(&addr_two);
  message_random_address(&addr_spectator);

  // add players to game
  player_t *player_one, *player_two, *spectator;
  TRY {
    player_one = add_player(game, addr_one, strdup("John Doe"));
    player_two = add_player(game, addr_two, strdup("Jane Doe"));
    spectator = add_player(game, addr_spectator, strdup("_SPECTATOR_"));
  } ENDTRY;

  TEST_ASSERT(player_one && player_two && spectator);
  TEST_ASSERT(message_eqAddr(addr_one, get_address(player_one)));
  TEST(strcmp("Jane Doe", get_name(player_two)), 0);

  // get_num_players
  TEST(get_num_players(game), 2);

  // get_spectator
  player_t* game_spectator = get_spectator(game);
  TEST(strcmp(get_name(spectator), get_name(game_spectator)), 0);
  TEST_ASSERT(message_eqAddr(get_address(spectator), get_address(game_spectator)));

  // get_players
  player_t** players;
  TRY { players = get_players(game); } ENDTRY;
  TEST_ASSERT(players);

  // both players have the surname "Doe"
  for (int i = 0; i < 2; i++) {
    TEST(strcmp(get_name(players[i]) + strlen("John "), "Doe"), 0);
  }

  // get_player_by_address
  char* player_two_name = get_name(players[1]);
  char* addr_two_name = get_name(get_player_by_address(game, addr_two));

  TEST(strcmp(player_two_name, addr_two_name), 0);
  TEST_ASSERT(is_spectator(get_player_by_address(game, addr_spectator)));

  // get_empty_room_spot (& is_empty_room_spot)
  point_t* empty_room_spot;
  for (int i = 0; i < 10; i++) {
    empty_room_spot = get_empty_room_spot(game);
    TEST_ASSERT(get_symbol(empty_room_spot) == '.');
  }

  // game_delete
  TRY { game_delete(game); } ENDTRY;

  // clean up
  fclose(fp);
  mem_free(mapstring);

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
