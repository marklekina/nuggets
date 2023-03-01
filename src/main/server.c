/*
 * server.c - server program for the Nuggets game
 *
 * see server.h for more information.
 *
 * Mark Lekina Rorat, December 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "file.h"
#include "message.h"
#include "point.h"
#include "pile.h"
#include "grid.h"
#include "player.h"
#include "game.h"
#include "server.h"

// global constants
static const int MaxNameLength = 50;   // max number of chars in playerName
static const int MaxPlayers = 26;      // maximum number of players
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles


int
main(int argc, char const *argv[]) {
 // validate parameters
 const char* program = argv[0];
 if (argc < 2 || argc > 3) {
   printf("usage: %s mapfile [seed]\n", program);
   return(0);
 }

 // set seed for random behavior
 int seed = getpid();
 if (argc == 3) {
   seed = atoi(argv[2]);
 }
 srand(seed);

 // read map string into the game
 const char* mapfile_path = argv[1];
 FILE* fp = fopen(mapfile_path, "r");

 // create game instance
 game_t* game = game_new(fp, MaxPlayers, GoldMaxNumPiles, GoldTotal);
 if (game == NULL) {
   return 2;
 }

 // close map file
 fclose(fp);

 // distribute gold
 bool game_init = distribute_gold(game, GoldMinNumPiles, GoldMaxNumPiles);
 if (!game_init) {
   return 2;
 }

 // initialize the message module
 int myPort = message_init(stderr);
 if (myPort == 0) {
   printf("%s: failed to initialize the message module\n", program);
   return 3;
 }

 // announce port number
 printf("%s: ready for contact at port %d\n", program, myPort);

 // loop and wait for input or messages
 bool ok = message_loop(game, 0, NULL, NULL, handleMessage);

 // free game memory and shut down the messaging module
 message_done();
 game_delete(game);

 // announce end of game session
 printf("%s: game over; terminating session\n", program);
 sleep(1);

 // return successfully
 return ok? 0 : 1;
}


// handler functions
bool
handleMessage(void* arg, const addr_t from, const char* message) {
  // cast arg to game pointer
  game_t* game  = (game_t*) arg;

  // return value for helper handler functions
  bool handler_return;

  // check type of message and handle appropriately
  if (strncmp(message, "PLAY ", strlen("PLAY ")) == 0) {
    // send to PLAY handler
    const char* name = message + strlen("PLAY ");
    handler_return = handle_play(game, from, name);
  }
  else if (strncmp(message, "SPECTATE", strlen("SPECTATE")) == 0) {
    // send to SPECTATE handler
    handler_return = handle_spectate(game, from);
  }
  else if (strncmp(message, "KEY ", strlen("KEY ")) == 0  && strlen(message) > strlen("KEY ")) {
    // send to KEY handler
    const char keystroke = message[strlen("KEY ")];
    handler_return = handle_key(game, from, keystroke);
  }
  else {
    // send error message to correspondent
    send_error(from, "malformatted message");
    handler_return = false;
  }

  // return false to continue message loop, false to terminate
  return handler_return;
}


bool
handle_play(game_t* game, const addr_t from, const char* name) {
  // send QUIT message if no player name is provided
  if (name == NULL || strlen(name) == 0) {
    send_quit(from, "Sorry - you must provide player's name.");
    return false;
  }

  // send QUIT message if the maximum number of players in the game has been reached
  if (get_num_players(game) == MaxPlayers) {
    send_quit(from, "Game is full: no more players can join.");
    return false;
  }

  // truncate player's name to MaxNameLength
  int len = strlen(name) > MaxNameLength? MaxNameLength : strlen(name);
  char* name_copy = strndup(name, len);

  // replace non_printing characters with _
  for (int i = 0; i < len; i++) {
    if (!isgraph(name_copy[i]) && !isblank(name_copy[i])) {
      name_copy[i] = '_';
    }
  }

  // add player to game
  player_t* player = add_player(game, from, name_copy);

  // send player information to the client
  // OK message
  char player_letter = get_letter(player);
  send_ok(from, player_letter);

  // GRID message
  int nrows = get_nrows(get_grid(game));
  int ncols = get_ncols(get_grid(game));
  send_grid(from, nrows, ncols);

  // GOLD message
  int n = 0;                            // gold amount collected
  int p = get_wallet_balance(player);   // gold amount in player's wallet
  int r = get_gold_balance(game);       // gold amount remaining in the game
  send_gold(from, n, p, r);

  // DISPLAY message
  char* visible_map = get_visible_map(player);
  send_display(from, visible_map);

  // announce new player
  printf("player %-3c: new client at address %s\n", player_letter, message_stringAddr(from));

  // return false to keep message_loop running
  return false;
}


bool
handle_key(game_t* game, const addr_t from, const char keystroke) {
  // locate player in the game whose client sent the keystroke
  player_t* player = get_player_by_address(game, from);

  // error locating keystroke sender
  if (player == NULL) {
    send_error(from, "Server is unable to locate the player matching this client.");
    return false;
  }

  // handle quit keystroke i.e, send QUIT message and forget player
  if (keystroke == 'Q') {
    send_quit(from, "Thanks for playing!");
    player_delete(player);
    return false;
  }

  // define valid move keystrokes
  char valid_keys_lower[] = {'y', 'k', 'u', 'h', 'l', 'b', 'j', 'n'};

  // handle move keystroke
  for (int i = 0; i < sizeof(valid_keys_lower); i++) {
    if (keystroke == valid_keys_lower[i]) {
      // move player as specified by keystroke
      move_player(game, player, keystroke);

      // terminate message loop if all gold nuggets have been collected
      if (get_gold_balance(game) == 0) {
        return true;
      }
      // otherwise keep looping
      return false;
    }
  }

  // define valid sprint keystrokes
  char valid_keys_upper[sizeof(valid_keys_lower)];
  for (int i = 0; i < sizeof(valid_keys_lower); i++) {
    valid_keys_upper[i] = toupper(valid_keys_lower[i]);
  }

  // handle sprint keystroke
  for (int i = 0; i < sizeof(valid_keys_upper); i++) {
    if (keystroke == valid_keys_upper[i]) {
      // move player as specified by keystroke
      sprint_player(game, player, keystroke);

      // terminate message loop if all gold nuggets have been collected
      if (get_gold_balance(game) == 0) {
        return true;
      }
      // otherwise keep looping
      return false;
    }
  }

  // handle erroneous keystroke
  send_error(from, "Invalid keystroke detected.");
  return false;
}


bool
handle_spectate(game_t* game, const addr_t from) {
  // send QUIT message to current spectator (if present)
  player_t* spectator = get_spectator(game);

  // add new spectator if absent from the game
  if (!spectator) {
    char* name = strdup("_SPECTATOR_");
    spectator = add_player(game, from, name);
  }
  else {
    addr_t spectator_addr = get_address(spectator);
    if (message_isAddr(spectator_addr)) {
      send_quit(spectator_addr, "You have been replaced by a new spectator.");
    }
  }

  // assign new spectator to the game
  bool new_spectator = update_spectator(spectator, from);
  if (!new_spectator) {
    send_quit(from, "server unable to add new spectator");
    return false;
  }

  // send game information to new spectator
  // GRID message
  int nrows = get_nrows(get_grid(game));
  int ncols = get_ncols(get_grid(game));
  send_grid(from, nrows, ncols);

  // GOLD message
  int n = 0;                            // gold amount collected
  int p = 0;                            // gold amount in spectator's wallet
  int r = get_gold_balance(game);       // gold amount remaining in the game
  send_gold(from, n, p, r);

  // DISPLAY message
  char* visible_map = get_visible_map(spectator);
  send_display(from, visible_map);

  // announce new spectator
  printf("spectator : new client at address %s\n", message_stringAddr(from));

  // return false to keep message_loop running
  return false;
}


// sender functions

void
send_ok(const addr_t to, const char letter) {
  // define string to carry information to client
  char message[message_MaxBytes];

  // format message
  sprintf(message, "OK %c", letter);

  // send message
  message_send(to, message);
}


void
send_grid(const addr_t to, const int nrows, const int ncols) {
  // define string to carry information to client
  char message[message_MaxBytes];

  // format message
  sprintf(message, "GRID %d %d", nrows, ncols);

  // send message
  message_send(to, message);
}


void
send_gold(const addr_t to, const int n, const int p, const int r) {
  // define string to carry information to client
  char message[message_MaxBytes];

  // format message
  sprintf(message, "GOLD %d %d %d", n, p, r);

  // send message
  message_send(to, message);
}


void
send_display(const addr_t to, const char* mapstring) {
  // define string to carry information to client
  char message[message_MaxBytes];

  // format message
  sprintf(message, "DISPLAY\n%s", mapstring);

  // send message
  message_send(to, message);
}


void
send_quit(const addr_t to, const char* explanation) {
  // define string to carry information to client
  char message[message_MaxBytes];

  // format message
  int len = strlen("QUIT ") + strlen(explanation);
  snprintf(message, len, "QUIT %s", explanation);

  // send message
  message_send(to, message);
}


void
send_error(const addr_t to, const char* explanation) {
  // define string to carry information to client
  char message[message_MaxBytes];

  // format message
  int len = strlen("ERROR ") + strlen(explanation);
  snprintf(message, len, "ERROR %s", explanation);

  // send message
  message_send(to, message);
}


bool
collect_gold(game_t* game, player_t* player, pile_t* pile) {
  // 1. fetch gold from gold pile
  int gold_collected = get_gold(pile);
  if (gold_collected == -1) {
    return false;
  }

  // 2. add gold to player's purse
  bool updated_wallet = update_wallet_balance(player, gold_collected);
  if (!updated_wallet) {
    return false;
  }

  // 3. remove pile from game (i.e., set gold balance to zero)
  update_pile_balance(pile);

  // 4. subtract gold from gold_balance
  update_gold_balance(game, gold_collected);

  // announce gold pile collected
  printf("player %-3c: collected %2d nuggets\n", get_letter(player), gold_collected);

  // 5. send gold messages
  addr_t to = get_address(player);
  int wallet_balance = get_wallet_balance(player);
  int gold_balance = get_gold_balance(game);

  // i) send GOLD message to the player that just collected the gold
  send_gold(to, gold_collected, wallet_balance, gold_balance);

  // ii) send GOLD message to everyone else
  // spectator (if present)
  player_t* player_b = get_spectator(game);
  if (player_b) {
    to = get_address(player_b);
    wallet_balance = get_wallet_balance(player_b);  // should always be zero for spectator
    send_gold(to, 0, wallet_balance, gold_balance);
  }

  // players
  // loop through list of players
  int num_players = get_num_players(game);
  player_t** players = get_players(game);

  for (int i = 0; i < num_players; i++) {
    player_b = players[i];

    // skip deleted players
    if (!player_b) {
      continue;
    }

    // send GOLD message to each player
    to = get_address(player_b);
    wallet_balance = get_wallet_balance(player_b);
    send_gold(to, 0, wallet_balance, gold_balance);
  }

  // 6. send QUIT GAME OVER message if all the gold in the game has been collected
  if (get_gold_balance(game) == 0) {
    // prepare GAME OVER message
    char* game_over_report = compile_game_over_report(game);

    // spectator (if present)
    player_b = get_spectator(game);
    if (player_b) {
      to = get_address(player_b);
      send_quit(to, game_over_report);
    }

    // players
    // loop through players list and send QUIT GAME OVER message
    for (int i = 0; i < num_players; i++) {
      player_b = players[i];

      // skip deleted players
      if (!player_b) {
        continue;
      }

      to = get_address(player_b);
      send_quit(to, game_over_report);
    }
  }

  // return successfully
  return true;
}


point_t*
move_player(game_t* game, player_t* player, const char keystroke) {
  // get target location
  point_t* target = get_target_location(game, player, keystroke);

  // move player
  bool moved_player = run_move_sequence(game, player, target);

  // return new location
  if (moved_player) {
    return target;
  }

  // otherwise return old location
  return get_location(player);
}


point_t*
sprint_player(game_t* game, player_t* player, const char keystroke) {
  // pointers to hold player's location
  point_t *prev, *curr;

  // move player in specified location until it is no longer possible
  do {
    prev = get_location(player);
    curr = move_player(game, player, keystroke);
  } while(!is_same_location(prev, curr));

  // return the player's new location
  return curr;
}


bool
run_move_sequence(game_t* game, player_t* player, point_t* target) {
  // indicators of successful execution of various steps in the sequence
  bool switched_location = true;
  bool moved_to_target = true;
  bool collected_gold = true;
  bool updated_mapstring = true;

  // 1. verify that the target location is valid
  if (target == NULL || !is_spot(target)) {
    return false;
  }

  // 2. switch locations if target location is occupied
  int num_players = get_num_players(game);
  player_t** players = get_players(game);
  player_t* player_b;

  // loop through list of players
  for (int i = 0; i < num_players; i++) {
    player_b = players[i];

    // skip deleted players and spectator
    if (player_b == NULL || is_spectator(player_b)) {
      continue;
    }

    // check if target location is occupied
    if (is_same_location(target, get_location(player_b))) {
      // switch the occupant player to the moving player's location
      switched_location = update_location(player_b, get_location(player));
      break;
    }
  }

  // 3. move player to target location
  moved_to_target = update_location(player, target);

  // 4. check for gold in target location and collect gold (and broadcast GOLD messages)
  int num_piles = get_num_piles(game);
  pile_t** piles = get_piles(game);
  pile_t* pile;

  // loop through list of gold piles
  for (int i = 0; i < num_piles; i++) {
    pile = piles[i];

    // skip deleted piles
    if (pile == NULL || get_gold(pile) == 0) {
      continue;
    }

    // check there's a gold pile on the target location
    if (is_same_location(target, get_pile_location(pile))) {
      // collect gold and send GOLD messages
      collected_gold = collect_gold(game, player, pile);
      break;
    }
  }

  // 5. update visible maps and send DISPLAY messages
  addr_t to;

  // spectator (if present)
  player_b = get_spectator(game);
  if (player_b) {
    // update visible map for the spectator
    updated_mapstring = build_visible_mapstring(game, player_b);

    // send updated display
    to = get_address(player_b);
    send_display(to, get_visible_map(player_b));
  }

  // players
  // loop through list of players
  for (int i = 0; i < num_players; i++) {
    player_b = players[i];

    // skip deleted players
    if (player_b == NULL) {
      continue;
    }

    // update display for each player
    updated_mapstring = build_visible_mapstring(game, player_b) && updated_mapstring;

    // send updated display to each player
    to = get_address(player_b);
    send_display(to, get_visible_map(player_b));
  }

  // return true if all steps of the sequence were successful
  return switched_location && moved_to_target && collected_gold && updated_mapstring;
}


char*
compile_game_over_report(game_t* game) {
  // define string to hold report (test: why is this static?)
  static char report[message_MaxBytes];
  strncat(report, "GAME OVER:\n", strlen("GAME OVER:\n"));

  // get list of players in the game
  int num_players = get_num_players(game);
  player_t** players = get_players(game);

  // sort players by wallet balance
  qsort((void*) players, num_players, sizeof(players[0]), compare_player_wallets);

  // variables to hold player info
  player_t* player;
  char player_letter;
  int player_wallet;
  char* player_name;

  // loop through list of players
  for (int i = 0; i < num_players; i++) {
    player = players[i];

    // skip deleted players
    if (!player || is_spectator(player)) {
      continue;
    }

    // get player information
    player_letter = get_letter(player);
    player_wallet = get_wallet_balance(player);
    player_name = get_name(player);

    // construct string representation of player information
    char player_info[MaxNameLength + 12];
    sprintf(player_info, "%-3c %4d  %s\n", player_letter, player_wallet, player_name);

    // concatenate player info to report
    strncat(report, player_info, strlen(player_info));
  }

  // return complete report
  return report;
}
