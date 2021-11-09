/* 
 * XYZ.c module
 *
 * see XYZ.h for more information.
 *
 * Palmer's Posse
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "message.h"
#include "grid.h"
#include "mem.h"

bool handlePlay(game_t game, char** playerNames, char* playerName, int currentNumPlayers, addr_t to);
bool handleSpectator(game_t* game, addr_t to);
bool handleKey(char key, addr_t to, player_t* player, game_t* game);

bool sendGrid(grid_t* grid);
bool sendGold(player_t* player);
bool sendDisplay(char* map);


static const int MaxNameLength = 50;   // max number of chars in playerName
static const int MaxPlayers = 26;      // maximum number of players
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const intGoldMaxNumPiles = 30; // maximum number of gold piles

// parameters given by handleMessage
bool handlePlay(game_t* game, char** playerNames, char* playerName, int currentNumPlayers, addr_t to){
    // takes in a string message starting with "PLAY"
	// ensure max number of players has not been reached
	if(currentNumPlayers >= MaxPlayers){
		message_send(to, "QUIT Game is full: no more players can join.");
		return false;
	}
	if(playerName == NULL){
		message_send(to, "QUIT Sorry - you must provide player's name.");
		return false;
	}

	int count = 0;
	for(int i = 0; i<MaxNameLength; i++){
		count ++;
		if(isgraph(playerName[i]) == 0 && isblank(playerName[i]) ==0){
			playerName[i] = '_';
		}
	}

	playerNames[currentNumPlayers] = playerName;
	char playerLetter = 'a' + currentNumPlayers;
	message_send(to, "OK %c", playerLetter);
	currentNumPlayers ++;

	// initialize modules to begin game play
	if(sendGrid(game->grid)){
		if(sendGold(player)){
			if(sendDisplay(game->map)){
				return true;
			}
		}
	}
	return true;
}

bool handleSpectator(game_t* game, addr_t to){
	// takes in a string message starting with "SPECTATE"
	// ensure there isn't already a spectator
	// clean out the old spectator
	if(game->spectator == 1){
		message_send(to, "QUIT sorry spectator, your spot was taken.");
		game->spectator = 0;
	}

	// set up the new spectator
	// initialize modules to begin game spectating
	if(sendGrid(game->grid)){
		if(sendGold(player)){
			if(sendDisplay(game->map)){
				// store a current spectator
				game->spectator = 1;
				return true;
			}
		}
	}
}

bool handleKey(char key, addr_t to, player_t* player, game_t* game){
	// takes in a char message starting with "KEY"
	// ensure the key was one of the ones we watch for
	if(key == "Q"){
		if(strcmp(player->type, "player")==0){
			message_send(to, "QUIT Thanks for playing!");
		}
		if(strcmp(player->type, "spectator")==0){
			message_send(to, "QUIT Thanks for watching!");
		}
		return true;
	}
	if(key == "h"){
		// what to do an if() here to see if moement is valid
		player->xPos -= 1;
		sendDisplay(game->map);
		sendGold(player);
		return true;
	}
	if(key == "l"){
		// what to do an if() here to see if moement is valid
		player->xPos += 1;
		sendDisplay(game->map);
		sendGold(player);
		return true;
	}
	if(key == "j"){
		// what to do an if() here to see if moement is valid
		player->yPos -= 1;
		sendDisplay(game->map);
		sendGold(player);
		return true;
	}
	if(key == "k"){
		// what to do an if() here to see if moement is valid
		player->yPos += 1;
		sendDisplay(game->map);
		sendGold(player);
		return true;
	}
	if(key == "y"){
		// what to do an if() here to see if moement is valid
		player->yPos += 1;
		player->xPos -= 1;
		sendDisplay(game->map);
		sendGold(player);
		return true;
	}
	if(key == "u"){
		// what to do an if() here to see if moement is valid
		player->yPos += 1;
		player->xPos += 1;
		sendDisplay(game->map);
		sendGold(player);
		return true;
	}
	if(key == "b"){
		// what to do an if() here to see if moement is valid
		player->yPos -= 1;
		player->xPos -= 1;
		sendDisplay(game->map);
		sendGold(player);
		return true;
	}
	if(key == "n"){
		// what to do an if() here to see if moement is valid
		player->yPos -= 1;
		player->xPos += 1;
		sendDisplay(game->map);
		sendGold(player);
		return true;
	}
	else{
		message_send(to, "ERROR invalid key");
		return false;
	}
}

bool sendGrid(grid_t* grid){
	// put together a string message that includes GRID nrows ncols
	if(grid != NULL){
		int nrows = grid->nrows;
		int ncols = grid->ncols;
		// send the grid the client
		message_send(to, "GRID %d %d", nrows, ncols);	
		return true;
	}
	return false;	
}

// !!
bool sendGold(player_t* player){
	// put together a string message that includes GOLD # nuggets, purse count, remaining gold

	// check if has been updated, if so send to clients
	message_send(to, "GOLD %d %d %d", collected, purse, remaining);	
}

bool sendDisplay(char* map){
	// retrieve the text version of the map and send it to client in a message
	if(map!= NULL){
		message_send(to, "DISPLAY\n %s", map);	
	}
}