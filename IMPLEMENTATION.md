# CS50 Nuggets
## Implementation Spec
### Palmer's Posse, Fall, 2021



According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes x, y, z modules.

We only describe the server and its supporting modules because we are a three person group.

We do not describe the `support` library nor the modules that enable features that go beyond the spec.

We avoid repeating information that is provided in the requirements/design spec.

## Plan for division of labor

For our project, we decided to collaborate on the initial aspects of the project, and initialize all modules together. Once we have a common understanding of each data structure and module we will divide between the three of us. 

For testing we are all going to work together and be sure to test code that other people in the group wrote. 

## Server

### Data structures
 `struct game`: holds a grid object and player information functions to: i) add and remove spectators, ii) add players, iii) getter functions the coding is somewhat simple and almost all functions are getters/setters, and iv) MOST IMPORTANTLY, computes the amount of the map a player can see based upon their location/gold and general visibility.

 `struct pile` : holds the location and amount for the gold piles 

 `struct grid`: holds a map string, number of rows and number of columns and functions to: i) load map string into the grid, ii) display the map, and iii) getter functions. The load_map function takes a text file and loads it into a string. Other than that function, almost all are getters/setters.

 `struct point`: holds x and y values 

 `struct player`: player_t struct - holds player information, point_t struct - holds a single (x,y) coordinate, the functions are to: i) create (and delete) the aforementioned structs, ii) getter functions, iii) setter functions, and iv) update player information 


### Definition of function prototypes
sets up modules listed below and runs the game

```c
int main(const int argc, char* argv[]);
```

A function to parse the command-line arguments, initialize the game struct, initialize the message module, and (BEYOND SPEC) initialize analytics module.

```c
void parseArgs(const int argc, char* argv[], game_t* game);
```

Takes in a string of a message from client and extracts information 

```c
bool parseMessage(void* item, const addr_t to, const char* message);
```

A continuous loop that checks and reacts to client input. 

```c
bool initiateNetwork();
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
		find if valid Message by extracting first word (PLAY, SPECTATE, KEY)
			extract correct parameters based on type of message
				extract parameters 
			call the message handler
			return true
		else return false
	

#### `initiateNetwork`
	initialize the server port and announce it
	run the message_loop (returns bool) with message handler functions passed in
	message_done()
	return true/false


#### `printResults`
	retrieve number of players in the game
	loop through each player in the game
		print name
		print amount of gold
		*print with appropriate formatting!

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

checks if a client can stand on a specific spot on the grid

```c
bool spot_is_open(grid_t* grid, int x, int y);
```

update all the player locations and gold locations, updating them if needed 

```c
void update_display(game_t* game);
```

initialize piles, locations of piles, gold per pile and arrya holding all piles
```c
void distribute_gold(game_t* game);
```

check if a player is standing on gold
```c
pile_t* player_on_gold(game_t* game, int x, int y);
```

adds gold to the players purse and takes it out of game play
```c
void collect_gold(game_t* game, player_t* player, pile_t* pile);
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
	validate parameters
	ensure max number of players has not been reached (if so, quit message)	
	add the player to the game and assign a letter (send OK letter message)
		confirm player was added and set the 'to' addr_t
	initialize modules to begin game play
		return true/false

#### `handleSpectator`:
	takes in a message and validates it
	add the spectator to the game
		ensure there isn't already a spectator
		if there is, kick them out and let this person spectate
	initialize modules to begin game spectating
	return true/false

#### `handleKey`:
 	get the address and grid 
		checks which key was pressed
			if one of the valid ones:
				handle that keys by calling a helper function
				return true
			invalid key
				return false
		*for all possible keys, see the requirments specs

#### `spot_is_open`:
	validate parameters
		checks the type of spot at a location (solid rock, horizontal/vertical/corner boundaries)
			find spot in map
			check if that spot type is valid
		return true/false

#### `update_display`:
	* note! same psudeo code can be found in requirement spec. (can ignore below)

	validate parameters (game then grid)
			initialize copy of original map
			loop through players and update locations
				get point coordinates
				find map spots and update them
			loop through gold piles and update locations
				get point coordinates
				find spot in map string and update it
		refresh game's map

#### `distribute_gold`:
	* note! same psudeo code can be found in requirement spec. (can ignore below)
	
	initialize number of piles to be created (should be between GoldMinNumPiles and GoldMaxNumPiles)
  		initialize variable to hold total gold distributed
		initiate an array of gold piles to hold pile locations and gold amounts; and loop through
			find random location to set up a gold pile
			compute random amount of gold to drop
			add location and amount to pile
			add pile to game array
		assign total amount of gold distributed to game

#### `player_on_gold`:
	* note! same psudeo code can be found in requirement spec. (can ignore below)

	validate parameters
		loop through all piles
			get location of the pile
			compare coordinates and return pile if they match

#### `collect_gold`:
	* note! same psudeo code can be found in requirement spec. (can ignore below)

	validate parameters
		transfer the gold in the pile to the player's purse
		subtract it from goldRemaining
		send gold message to player
		delete pile

#### `sendGrid`:
	validate parameters
	pull out the rows and cols form the grid
	put together a string message that includes GRID nrows ncols
	send a message to the client with the grid and address
	return true/false

#### `sendGold`:
	validate parameters
	get gold info from game's grid and player
	build message string message that includes GOLD # nuggets, purse count, remaining gold
	send the client the message
	return true/false

#### `sendDisplay`:
	validate parameters
	fetch the entire map if the player is a spectator
	otherwise fetch only the part of the map visible to the player
	put it into a string message called DISPLAY \nmapString
	sent to client(s)
	return true/false
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
Given the time constraints, we didn't have time to do in depth integration tests or system tests. If we had more time, we would have planned to fully implement these tests as a group to test our code. 
We have all of the common directory files (XYZ.c, grid, game, player) compiling. 
We figured out the math and nuances of visibility and gold distribution.
Unfortunatly, due to time constraints we are struggling to piece these modules together in server (in a way the game can be functional).
