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
#include "message.h"
#include "grid.h"
#include "common.c"
#include "XYZ.c"
#include "mem.h"
#include "file.h"


void parseArgs(const int argc, char* argv[], game_t* game);
bool parseMessage(void* item, const addr_t to, const char* message);
player_t* findPlayer(game_t* game, addr_t to);
bool load_grid(game_t* game, int nrows, int ncols);
void disperseGold();

bool initiateNetwork();
char* loadMap(char* filename);
void disperseGold();
void updateGame(char stroke);
void printResults(game_t* game);



/**************** parseArgs ****************/
/* Parse the command line arguments and initialize a game module.
 * 
 * Given:
 *  argc- number of command line arguments
 *  argv- array of command line arguments
 *  game- an empty game module
 * 
 */
void parseArgs(const int argc, char* argv[], game_t* game){
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

        // check that this works and flows!
        // THIS IS WRONG NEED REAL ROWS AND COLS
        game_t* game = game_new(fp, 0, 0);
        fclose(fp);

        srand(getpid());
        int randSeed = rand();
        game_setSeed(game, randSeed);
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
        game_t* game = game_new(fp, 0, 0);
        fclose(fp);

        int* seed = 0;
        sscanf(argv[3], "%d", seed);

        // quick check because zero is an edge case in strcmp
        if(strcmp(argv[3],"0") == 0) {
            fprintf(stderr, "cannot have a seed of zero");
            exit(3);
        }

        game_setSeed(game, *seed);
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
bool parseMessage(void* item, const addr_t to, const char* message){
    game_t* game = item;

    if(message == NULL){
        return false;
    }

    char* firstWord = NULL;
    int i=0;
    while(!isspace(message)){
        firstWord[i] = message[i]; //check if allowed
        i++;
    }

    if((strcmp(firstWord, "PLAY")) == 0){
        int nameCount = 0;
        char* name = NULL;
        while(message[i] != '\0'){
            name[nameCount] = message[i];
            i++;
            nameCount++;
        }
        handlePlay(game, to, name);
        return true;
    }

    if(strcmp(firstWord, "SPECTATE") == 0){
        player_t* player = player_new(NULL, "spectator");

        if(spectatorAdd(game, player)){
            player_t* spectator = game_getSpectator(game);
            handleSpectator(game, to, spectator);
            return true;
        }
        return false;
    }

    if(strcmp(firstWord, "KEY") == 0){
        player_t* player = findPlayer(game, to);
        if(player != NULL){
            int check = 0;
            char key;
            while((message[i] != '\0') && (check == 0)){
                if(isspace(message[i]) == 0){
                    key = message[i];
                    check++;
                }
            }
            handleKey(key, to, player, game);
        }
        return true;
    }

    else{
        fprintf(stderr, "bad message from client, quitting game");
        message_send(to, "QUIT invalid message!");
        return false;
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
    int location = get_location(game);
    for(int i = 0; i < location; i++){
        player_t* player = get_Player(game, location);
        addr_t* playerAddress = getAddress(player);
        
        if(message_eqAddr(*playerAddress, to)){
            return player;
        }
    }
    return NULL;
}

/**************** load_grid ****************/
/* Basically a wrapper for the load_map function in grid.
 * 
 * Given:
 *  to- a unique client address
 *  game- an game module
 * 
 */
// static bool load_grid(game_t* game, int nrows, int ncols){
//     grid_t* grid = grid_new(nrows, ncols);
//     char* map = load_map(grid, game->map);
//     if(map != NULL){
//         if(game_setGrid(game, grid)){
//             return true;
//         }
//     }
//     return false;
// }

// /**************** loadMap ****************/
// /* LEKINA?!?!?
//  * 
//  * 
//  */
// static void disperseGold(){
// 	// pick random number between minGold and maxGold
// 	// populate grid with the random amount of gold
// }

// static void updateGame(char stroke){
// 	checks if keystroke is valid
// 		if so, performs function associated with that key
// 		sends updated game to all players
// 	else does nothing

// }

void printResults(game_t* game){
    int location = get_location(game);
    for(int i =0; i<=location; i++){
        player_t* player = get_Player(game, i);
        char* name = player_getName(player);
        printf("%s", name);
        int purse = player->purse;
        printf("%d", purse);
    }
    
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
    game_t* game = NULL;
    parseArgs(argc, argv, game);
    // might need to pass the LOG_MESSAGE into initiate network
    if(initiateNetwork()){
        return 0;
    }
    else{
        return 2;
    }
}