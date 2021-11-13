/* 
 * server module
 *
 * A module
 *
 * Palmer's Posse, November 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "game.h"
#include "player.h"
#include "grid.h"
#include "message.h"
#include "visibility.c"
#include "XYZ.c"
#include "mem.h"
#include "file.h"

static void parseArgs(const int argc, char* argv[]);
static char* parseMessage(char* message);
static bool initiateNetwork(void* arg);
static bool handleMessage(void* arg, const addr_t from, const char* message);
static char* loadMap(char* filename);
static void disperseGold();
static void updateGame(char stroke);
static void printResults(game_t* game);

static const int MaxNameLength = 50;   // max number of chars in playerName
static const int MaxPlayers = 26;      // maximum number of players
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const intGoldMaxNumPiles = 30; // maximum number of gold piles

/**************** parseArgs ****************/
/* Parse the command line arguments and initialize a game module.
 * 
 * Given:
 *  argc- number of command line arguments
 *  argv- array of command line arguments
 *  game- an empty game module
 * 
 */
static void parseArgs(const int argc, char* argv[], game_t* game){
    if(argc == 2){
        char* map = argv[1];
        
        // file validation
        FILE *fp;
        fp = fopen(map, "r"); // attempt to open for reading

        // if you cannot read into the file, throw error to user
        if(fp == NULL){
            fprintf(stderr, "invalid map; invalid path\n");
            exit(1);
        }
        fclose(fp);
        
        FILE* map = argv[1];
        game->map = map;

        int randSeed = srand(getpid());
        game->seed = randSeed;
    }

    if(argc == 3){
        char* map = argv[1];
        // file validation
        FILE *fp;
        fp = fopen(map, "r"); // attempt to open for reading

        // if you cannot read into the file, throw error to user
        if(fp == NULL){
            fprintf(stderr, "invalid map; invalid path\n");
            exit(2);
        }
        fclose(fp);

        game->map = map;

        int seed;
        sscanf(argv[3], "%d", seed);

        // quick check because zero is an edge case in strcmp
        if(strcmp(argv[3],"0") == 0) {
            fprintf(stderr, "cannot have a seed of zero");
            exit(3);
        }

        // make sure we add this
        game->seed = seed;
    }
    else{
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
static char* parseMessage(addr_t to, game_t* game, char* message){
    if(message == NULL){
        return NULL;
    }

    char* firstWord;
    int i=0;
    while(!isspace(message)){
        firstWord[i] = message[i]; //check if allowed
        i++;
    }

    if(strcmp(firstWord, "PLAY") == 0){
        int nameCount = 0;
        while(strcmp(message[i], '\0') != 0){
            char* name;
            name[nameCount] = message[i];
            i++;
            nameCount++;
        }
        handlePlay(game, to, name);
    }

    if(strcmp(firstWord, "SPECTATE") == 0){
        player_t* spectator = game_addPlayer(game, NULL, "spectator");
        handleSpectator(gmae, to, spectator);
    }

    if(strcmp(firstWord, "KEY") == 0){
        if((player_t* player = findPlayer(game, to)) != NULL){
            int check = 0;
            while((strcmp(message[i], '\0') != 0) && check == 0){
                if(strcmp(message[i], ' ') != 0){
                    char key;
                    key = message[i];
                    check++;
                }
            }
            handleKey(key, to, player, game);
        }        
    }

    else{
        fprintf(stderr, "bad message from client");
        player_t* random = game_addPlayer(game, NULL, "spectator");
        handleKey(4, to, player, game); // sends error message to client
        player_delete(random);
        exit(4);
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
static player_t* findPlayer(game_t* game, addr_t to){
    for(int i = 0; i< game->location; i++){
        player_t* game->players[i];
        if(player->to == to){
            return player_t*;
        }
    }
    return NULL;
}

/**************** loadMap ****************/
/* Basically a wrapper for the load_map function in grid.
 * 
 * Given:
 *  to- a unique client address
 *  game- an game module
 * 
 */
static bool load_grid(game_t* game, nrows, ncols){
    grid_t* grid = grid_new(nrows, ncols);
    
    if((char* map = load_map(grid, game->map)) != NULL){
        game->grid = grid;
        return true;
    }
    return false;
}

/**************** loadMap ****************/
/* MARK?!?!?
 * 
 * 
 */
// static void disperseGold(){
// 	// pick random number between minGold and maxGold
// 	// populate grid with the random amount of gold
// }

static void updateGame(char stroke){
	checks if keystroke is valid
		if so, performs function associated with that key
		sends updated game to all players
	else does nothing

}

static void printResults(game_t* game){
    for players in the game
    print name
    print amount of gold
}

/**************** initiateNetwork ****************/
/* Open the communication port with the client and start a message loop.
 * 
 * Given:
 *  arg- argument to start the loop
 * 
 */
static bool initiateNetwork(void* arg){
    initialize the server ports/communication channels
	while there is no client
		return false
	otherwise recieve message and send to handle message
		return true
}