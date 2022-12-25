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
 // TODO: read mapfile into game
 fclose(fp);

 // initialize the message module
 int myPort = message_init(stderr);
 if (myPort == 0) {
   printf("%s: failed to initialize the message module\n", program);
   return 2;
 }

 // announce port number
 printf("waiting on port %d for contact....\n", myPort);

 // no correspondent yet
 addr_t other = message_noAddr();

 // loop and wait for input or messages
 bool ok = message_loop(&other, 0, NULL, NULL, handleMessage);

 // return successfully
 message_done();
 return ok? 0 : 1;
}

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
    char* error_message = "ERROR malformatted message";
    message_send(from, error_message);
    handler_return = false;
  }

  // return false to continue message loop, false to terminate
  return handler_return;
}

// handler functions

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
  char name_copy[len + 1];
  strncpy(name_copy, name, len);

  // replace non_printing characters with _
  for (int i = 0; i < strlen(name_copy); i++) {
    if (!isgraph(name_copy[i] && !isblank(name_copy[i]))) {
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

  // return false to keep message_loop running
  return false;
}


bool
handle_key(game_t* game, const addr_t from, const char keystroke) {
  // return false to keep message_loop running
  return false;
}


bool
handle_spectate(game_t* game, const addr_t from) {
  // send QUIT message to current spectator (if present)
  player_t* spectator = get_spectator(game);
  addr_t spectator_addr = get_address(spectator);
  if (message_isAddr(spectator_addr)) {
    send_quit(spectator_addr, "You have been replaced by a new spectator.");
  }

  // assign new spectator to the game
  bool new_spectator = update_spectator(game, from);
  if (!new_spectator) {
    send_error(from, "server unable to add new spectator");
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
  sprintf(message, "QUIT %s", explanation);

  // send message
  message_send(to, message);
}


void
send_error(const addr_t to, const char* explanation) {
  // define string to carry information to client
  char message[message_MaxBytes];

  // format message
  sprintf(message, "ERROR %s", explanation);

  // send message
  message_send(to, message);
}
