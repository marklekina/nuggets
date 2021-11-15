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
#include "../support/message.h"
#include "grid.h"
#include "mem.h"
#include "game.h"
#include "player.h"

bool handlePlay(game_t* game, addr_t to, char* playerName);
bool handleSpectator(game_t* game, addr_t to, player_t* player);
bool handleKey(char key, addr_t to, player_t* player, game_t* game);

bool sendGrid(grid_t* grid, addr_t to);
bool sendGold(game_t* game, addr_t to);
bool sendDisplay(char* map, addr_t to);


static const int MaxNameLength = 50;   // max number of chars in playerName
static const int MaxPlayers = 26;      // maximum number of players


// parameters given by handleMessage
bool handlePlay(game_t* game, addr_t to, char* playerName){

	// takes in a string message starting with "PLAY"
	// ensure max number of players has not been reached
	int index = get_location(game);
	if(index >= MaxPlayers){
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
	//use getters instead
	if(!game_addPlayer(game, playerName, "player")){
		return false;
	}


	char* playerLetter = "a" + index;
	char* message = strcat("OK", playerLetter);
	message_send(to, message);

	grid_t* grid = game_getGrid(game);
	char* map = grid_getMap(grid);

	// initialize modules to begin game play
	if(sendGrid(grid, to)){
		if(sendGold(game, to)){
			if(sendDisplay(map, to)){
				return true;
			}
		}
	}
	return true;
}

bool handleSpectator(game_t* game, addr_t to, player_t* player){
	// need to the game functions a player to delete or add
	
	// takes in a string message starting with "SPECTATE"
	// ensure there isn't already a spectator
	// clean out the old spectator
	player_t* spectator = game_getSpectator(game);
	if(spectator != NULL){
		message_send(to, "QUIT sorry spectator, your spot was taken.");
		removeSpectator(game);
	}

	grid_t* grid = game_getGrid(game);
	char* map = grid_getMap(grid);
	// set up the new spectator
	// initialize modules to begin game spectating
	if(sendGrid(grid, to)){
		if(sendGold(game, to)){
			if(sendDisplay(map, to)){
				// store a current spectator
				if(spectatorAdd(game, player)){
					return true;
				}
			}
		}
	}
	return false;
}

bool handleKey(char key, addr_t to, player_t* player, game_t* game){
	// takes in a char message starting with "KEY"
	// ensure the key was one of the ones we watch for
	grid_t* grid = game_getGrid(game);
	char* map = grid_getMap(grid);
	
	if(key == 'Q'){
		if(strcmp(player->type, "player")==0){
			message_send(to, "QUIT Thanks for playing!");
		}
		if(strcmp(player->type, "spectator")==0){
			message_send(to, "QUIT Thanks for watching!");
		}
		return true;
	}
	if(key == 'h'){
		// what to do an if() here to see if moement is valid
		player->xPos -= 1;
		sendDisplay(map, to);
		sendGold(game, to);
		return true;
	}
	if(key == 'l'){
		// what to do an if() here to see if moement is valid
		player->xPos += 1;
		sendDisplay(map, to);
		sendGold(game, to);
		return true;
	}
	if(key == 'j'){
		// what to do an if() here to see if moement is valid
		player->yPos -= 1;
		sendDisplay(map, to);
		sendGold(game, to);
		return true;
	}
	if(key == 'k'){
		// what to do an if() here to see if moement is valid
		player->yPos += 1;
		sendDisplay(map, to);
		sendGold(game, to);
		return true;
	}
	if(key == 'y'){
		// what to do an if() here to see if moement is valid
		player->yPos += 1;
		player->xPos -= 1;
		sendDisplay(map, to);
		sendGold(game, to);
		return true;
	}
	if(key == 'u'){
		// what to do an if() here to see if moement is valid
		player->yPos += 1;
		player->xPos += 1;
		sendDisplay(map, to);
		sendGold(game, to);
		return true;
	}
	if(key == 'b'){
		// what to do an if() here to see if moement is valid
		player->yPos -= 1;
		player->xPos -= 1;
		sendDisplay(map, to);
		sendGold(game, to);
		return true;
	}
	if(key == 'n'){
		// what to do an if() here to see if moement is valid
		player->yPos -= 1;
		player->xPos += 1;
		sendDisplay(map, to);
		sendGold(game, to);
		return true;
	}
	else{
		message_send(to, "ERROR invalid key");
		return false;
	}
}

char* convertInt(int num){
	char* string = "";
	sprintf(string, "%d", num);
	return string;
}

bool sendGrid(grid_t* grid, addr_t to){
	// put together a string message that includes GRID nrows ncols
	if(grid != NULL){
		int nrows = grid_getnRows(grid);
		int ncols = grid_getnCols(grid);

		char* charRows = convertInt(nrows);
		char* charCols = convertInt(ncols);

		// send the grid the client
		char* message = strcat("GRID ", charRows);
		message = strcat(message, " ");
		message = strcat(message, charCols);

		message_send(to, message);	
		return true;
	}
	return false;	
}

// !!
bool sendGold(game_t* game, addr_t to){
	// put together a string message that includes GOLD # nuggets, purse count, remaining gold

	// check if has been updated, if so send to clients
	// message_send(to, "GOLD %d %d %d", collected, purse, remaining);	
	return false;
}

bool sendDisplay(char* map, addr_t to){
	// retrieve the text version of the map and send it to client in a message
	if(map!= NULL){
		char* message = strcat("DISPLAY\n ", map);
		message_send(to, message);	
		return true;
	}
	return false;
}
