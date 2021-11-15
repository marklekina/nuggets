/* 
 * Server module
 *
 * A module to set up the Nuggets game. Mainly a wrapper for XYZ.c, grid.c, player.c, visibility.c
 *
 * Palmer's Posse, November 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "game.h"
#include "player.h"
#include "support/message.h"
#include "grid.h"
#include "XYZ.c"
#include "mem.h"
#include "file.h"


void parseArgs(const int argc, char* argv[]);
bool parseMessage(void* item, const addr_t to, const char* client_message);
bool initiateNetwork();
void updateGame(char stroke);
void printResults(game_t* game);
player_t* findPlayer(game_t* game, addr_t to);
game_t* game;             // game object

/**************** parseArgs ****************/
/* Parse the command line arguments and initialize a game module.
 * 
 * Given:
 *  argc- number of command line arguments
 *  argv- array of command line arguments
 *  game- an empty game module
 * 
 */
void parseArgs(const int argc, char* argv[]){
  // ignore seed for now
  if (argc == 2 || argc == 3) {
    // parse filename
    char* map = argv[1];
        
    // attempt to open file for reading
    FILE *fp = fopen(map, "r");

    // if you cannot read into the file, throw error to user
    if(fp == NULL){
      fprintf(stderr, "invalid map; invalid path\n");
      exit(1);
    }

    fclose(fp);
  }
  
  else {
    fprintf(stderr, "Faulty commandline, one or two arguments only.");
    exit(4);
  }
}

/**************** parseMessage ****************/
/* Parse the messages send in by the client.
 * 
 * Given:
 *  message- a string message sent by the client
 * 
 */
bool parseMessage(void* item, const addr_t address, const char* client_message){

  if(client_message == NULL){
    return false;
  }

  // copy pointer to address
  // copy client_message to message
  addr_t* to = (addr_t*) &address;
  char* message = (char*) client_message;

  // convert message to uppercase
  char* ptr = message;
  while (*ptr) {
    *ptr = toupper((unsigned char) *ptr);
    ptr++;
  }

  // handle different message types
  if((strncmp(message, "PLAY ", strlen("PLAY "))) == 0){
    // copy the rest of the message into a string
    int len = strlen(message) - strlen("PLAY");
    char* name = mem_malloc(sizeof(char) * len);
    strcpy(name, &message[strlen("PLAY ")]);

    // initiate player object and set it's address to message source
    player_t* player = player_new(name, "player");
    set_address(player, to);
    
    // call handle player
    handlePlayer(game, to, name);

    // send player game information
    sendGrid(game, player);
    sendGold(game, player, 0);
    sendDisplay(game, player);

    // return true
    return true;
  }

  
  if((strncmp(message, "SPECTATE", strlen("SPECTATE"))) == 0){
    // initiate spectator object and set it's address to message source
    player_t* player = player_new("spectator", "spectator");
    set_address(player, to);
    
    // call handle player
    handleSpectator(game, to);
    // send player game information
    sendGrid(game, player);
    sendGold(game, player, 0);
    sendDisplay(game, player);

    // return true
    return true;
  }

  if((strncmp(message, "KEY ", strlen("KEY "))) == 0){
    // copy the rest of the message into a char
    char key = message[strlen("KEY ")];
    
    // find player that sent the message
    player_t* player = findPlayer(game, *to);
    
    // call handle key
    handleKey(game, player, key);

    return true;
  }

  if((strncmp(message, "Q", strlen("Q"))) == 0){
    // find player that sent the message
    player_t* player = findPlayer(game, *to);

    // call handle key
    handleKey(game, player, 'Q');
    return true;
  }

  else{
    fprintf(stderr, "bad message from client, quitting game");
    message_send(*to, "QUIT invalid message!");
    return false;
  }
}

void
printResults(game_t* game){
  int num_players = get_num_players(game);
  for(int i = 0; i <= num_players; i++){
    player_t* player = get_player(game, i);
    char* name = get_name(player);
    int purse = get_purse(player);
    printf("%s %d\n", name, purse);
  } 
}


/**************** findPlayer ****************/
/* Find a player based on the 'to' address from the client.
 * 
 * Given:
 *  to- a unique client address
 *  game- an game module
 * 
 */
player_t* findPlayer(game_t* game, addr_t to){
  int num_players = get_num_players(game);
  for(int i = 0; i < num_players; i++){
    player_t* player = get_player(game, num_players);
    addr_t* playerAddress = get_address(player);
    
    if(message_eqAddr(*playerAddress, to)){
      return player;
    }
  }
  return NULL;
}


/**************** initiateNetwork ****************/
/* Open the communication port with the client and start a message loop.
 * 
 * Given:
 *  arg- argument to start the loop
 * 
 */
bool initiateNetwork(){
    int ourPort = message_init(stderr); // check what LOG_MESSAGE is?
    fprintf(stderr, "port: %d\n", ourPort);
    
    // bool ok = message_loop(to, 0, NULL, handleInput, handleMessage);
    bool ok = message_loop(&ourPort, 0, NULL, NULL, parseMessage);

    message_done();
    return ok? 0 : 1; // status code depends on result of message_loop
}

int main(const int argc, char* argv[]){
  
  // parse arguments
  parseArgs(argc, argv);

  // load map info
  char* map_filename = argv[1];
  FILE* fp = fopen(map_filename, "r");
  FILE* fp_copy = fp;
  
  // define nrows, ncols and map
  int nrows = file_numLines(fp);
  int ncols = strlen(file_readLine(fp));

  fp = fp_copy;

  // initialize game
  game = game_new(fp, nrows, ncols);

  // distribute gold
  distribute_gold(game);

  // initiate game
  if(initiateNetwork()){
    return 0;
  }
  else{
    return 2;
  }
}
