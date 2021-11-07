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
#include "grid.h"
#include "mem.h"

int handlePlay(char* playMessage);
int handleSpectator(char* spectateMessage);
int handleKey(char* keyMessage);
int sendGrid(char* gridMessage, int nrows, int ncols);
int sendDisplay(char* displayMessage, char* map);
int sendGold(char* goldMessage, int numNuggets, int purse, int remaining);


int handlePlay(char* playMessage){
    // takes in a string message starting with "PLAY"
	// ensure max number of players has not been reached
	// tells us there is a client we have to give the correct letter assigned name to
	// initialize modules to begin game play
}

int handleSpectator(char* spectateMessage){
	// takes in a string message starting with "SPECTATE"
	// tells us there is a spectator
	// ensure there isn't already a spectator
	// if there is, kick them out and let this person spectate
	// initialize modules to begin game spectating
}

int handleKey(char* keyMessage){
	// takes in a char message starting with "KEY"
	// ensure the key was one of the ones we watch for
	// handle that key by calling a helper function

	// *for all possible keys, see the requirments specs
}

int sendGrid(char* gridMessage, int nrows, int ncols){
	// put together a string message that includes GRID nrows ncols
	// make sure that the section of the grid being sent considers where the client is
	// send the grid the client
}

int sendDisplay(char* displayMessage, char* map){
	// put together a string message that includes GOLD # nuggets, purse count, remaining gold
	// check if has been updated, if so send to clients
}

int sendGold(char* goldMessage, int numNuggets, int purse, int remaining){
	// retrieve the text version of the map
	// put it into a string message called DISPLAY \nmapString
	// sent to client(s)
}