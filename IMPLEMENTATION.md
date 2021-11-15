# CS50 Nuggets
## Implementation Spec
### Palmer's Posse, Fall, 2021



According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes x, y, z modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Plan for division of labor

For our project, we decided to collaborate on the initial aspects of the project, and initialize all modules together. Once we have a common understanding of each data structure and module we will divide between the three of us. 

For testing we are all going to work together and be sure to test code that other people in the group wrote. 

## Server

### Data structures
`struct game`: holds the game information including the grid described in design as well as the game's one spectator, pile structs for each gold pile, total number of players and player structs for each player. 

`struct pile` : holds the location and amount for the gold piles 

 `struct grid`: holds information about the game board including the original map, a map intended to be motified, as well as dimmensions of the grid (functions; including one that turns the map into a string)

`struct point`: holds x and y values 

`struct player`: holds information for each individual player. This includes player's name, type, location (as a point), visibility, address, and purse information.  


### Definition of function prototypes

A function to parse the command-line arguments, initialize the game struct, initialize the message module, and (BEYOND SPEC) initialize analytics module.

```c
static int parseArgs(const int argc, char* argv[]);
```

Takes in a string of a message from client and extracts information 

```c
static char* parseMessage(char* message);
```

A continuous loop that checks and reacts to client input. 

```c
static bool initiateNetwork(void* arg);
```

Takes in a message [and it's origin] and calls parseMessage as well as other helper functions. 

```c
static bool handleMessage(void* arg, const addr_t from, const char* message);
```
Takes in a filename and opens a file for the map. It then reads the map from the file and converts it to a string and then returns it. 
```c
static char* loadMap(char* filename);
```

Will use the global constants minGold and maxGold to randomly disperse the gold. 

```c
static void disperseGold();
```
Takes in a keystroke, performs the associated function, and then updates the game. 

```c
static void updateGame(char stroke);
```
Takes in the game and loops through all the players to print the player information including name and purse. Essentially prints a leaderboard. 
```c
static void printResults(game_t* game);
```



### Detailed pseudo code

#### `parseArgs`:

	validate commandline
	verify map file can be opened for reading
	if seed provided
		verify it is a valid seed number
		seed the random-number generator with that seed
	else
		seed the random-number generator with getpid()

#### `parseMessage`

	if valid Message type 
		if valid Message
			if parameters are valid
				extract parameters 
			concatenate a string from the message
			returns validated string
	else return NULL
	

#### `initiateNetwork`
	initialize the server ports/communication channels
	while there is no client
		return false
	otherwise recieve message and send to handle message
		return true
	

#### `handleMessage`
	calls parseMessage on inputted string
	if message is "PLAY"
		call helper function
	if message is "SPECTATE"
		call helper function
	if message is "KEY" 
		call hhelper function

#### `loadMap`
	create a file and try to open it
	if you're unable to
		return NULL
	read file
	output information to a textfile
	close the original file
	close textfile
	return textfile name 
	
#### `disperseGold`
	pick random number between minGold and maxGold
	populate grid with the random amount of gold

#### `updateGame`
	checks if keystroke is valid
		if so, performs function associated with that key
		sends updated game to all players
	else does nothing


#### `printResults`
	for players in the game
		print name
		print amount of gold

---

## XYZ module

### Definition of function prototypes
Receive a message that a client wants to play and sets them up accordingly

```c
static int handlePlay(char* playMessage);
```

Receive a message that a client wants to spectate and sets them up accordingly

```c
static int handleSpectator(char* spectateMessage);
```

Receive a message that a client input a key and respond with the appropriate function calls

```c
static int handleKey(char* keyMessage);
```

Send the grid to a participating client

```c
static int sendGrid(char* gridMessage, int nrows, int ncols);
```

Send the display information to a participating client

```c
static int sendDisplay(char* displayMessage, char* map);
```

Send the gold information to a participating client

```c
static int sendGold(char* goldMessage, int numNuggets, int purse, int remaining);
```

### Detailed pseudo code
#### `handlePlay`:
	takes in a string message starting with "PLAY"
	ensure max number of players has not been reached
	tells us there is a client we have to give the correct letter assigned name to
	initialize modules to begin game play

#### `handleSpectator`:
	takes in a string message starting with "SPECTATE"
	tells us there is a spectator
	ensure there isn't already a spectator
	if there is, kick them out and let this person spectate
	initialize modules to begin game spectating

#### `handleKey`:
	takes in a char message starting with "KEY"
	ensure the key was one of the ones we watch for
	handle that key by calling a helper function

	*for all possible keys, see the requirments specs

#### `sendGrid`:
	put together a string message that includes GRID nrows ncols
	make sure that the section of the grid being sent considers where the client is
	send the grid the client

#### `sendGold`:
	put together a string message that includes GOLD # nuggets, purse count, remaining gold
	check if has been updated, if so send to clients

#### `sendDisplay`:
	retrieve the text version of the map
	put it into a string message called DISPLAY \nmapString
	sent to client(s)

---

## Testing plan

### unit testing

Our testing plan will be informed while we code, but some preliminary ideas include:
 - practice sending incorrect messages, see how the server responds
 - try to open different types of map files and check for validation
 - try and input incorrect command line arguments
 - have players step on each other
 - play game on multiple maps, several times, to ensure a thourough functionality of the game

---

## Limitations
Currently not relevant, but may be added to as we work on the project.

> This section may not be relevant when you first write your Implementation Plan, but could be relevant after completing the implementation.
