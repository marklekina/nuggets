# CS50 Nuggets
## Design Spec
### Palmer's Posse, Fall21

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
As a three person group, we only need to implement the server.

We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Server
### User interface

See the requirements spec for the command-line interface. There is no interaction with the user.

### Inputs and outputs
Command-line arguments are not 'input'.

### Functional decomposition into modules
Parameters are not yet set, see Implementation Spec for that.

`main` - sets up modules listed below and runs the game

`parseArgs` - parse the command line and validate arguments

`parseMessage` - parsing the messages from the client and calling the appropriate handling functions

`initiateNetwork` - direct communication with the client, sets up game, calls handleMessage()


`printResults` - print the leaderboard after the game ends

 
### Pseudo code for logic/algorithmic flow

        parseArgs
            validate arg count
	        validate arg type
        	initializes variables
			set random seed if one is not passed
	
        parseMessage 
	        check message type 
            find message parameters
			call appropriate helper
			return true/false for if message was handled
	

        initiateNetwork
			initialize the server ports/communication channels
	        wait for client to connect
			receives messages from the client
			call parseMessage

        printResults
			iterate through the players in the game and print their name and contents in their purse
	        see Requirements Spec for detail on formatting

### Major data structures
 `struct game`: see detail in Implementation Spec; holds all the information about the game including players and gold. Perhaps most importantly handles the computation of visibility.

 `textfile map`: essentially a text file; most provided; must create one more

 `struct grid`: see detail in Implementation Spec; loads the map text file as a string

 `struct player`: see detail in Implementation Spec; holds players, their respective information and functions that will update when players interact with the game

 
---

## XYZ module

This module represents the type of message XYZ that are sent and received.

INPUTS: there are three types of messages that the client can send the server
 - `PLAY` (initiate play)
 - `SPECTATE` (initiate spectating)
 - `KEY` (movement on the grid by the user/quitting)

OUTPUTS:
 - `GRID` nrows ncols (build the grid with the size)
 - `GOLD` n p r (number of nuggets, what is in the players purse and the remaining gold)
 - `DISPLAY` \nstring (a printable string version of the map)
### Functional decomposition

`handlePlay` - receive a message that a client wants to play and sets them up accordingly

`handleSpectator` - receive a message that a client wants to spectate and sets them up accordingly

`handleKey` - receive a message that the client input a key and handle that keystoke

`spot_is_open` - checks if a client can stand on a specific spot on the grid

`update_display` - update all the player locations and gold locations, updating them if needed 

`distribute_gold` - initialize piles, locations of piles, gold per pile and arrya holding all piles

`player_on_gold` - check if a player is standing on gold

`collect_gold` - adds gold to the players purse and takes it out of game play

`sendGrid` - sends a message with grid as well as the rows and columns of the grid to the client

`sendGold` - send the number of nuggets, the amount in the player's purse and the amount of gold remaining to the client

`sendDisplay` - will send a string interpretation of the map, the section of the map depends on the player

### Pseudo code for logic/algorithmic flow

    handlePlay
    	takes in a message
		ensure max number of players has not been reached (if so, quit message)
		add the player to the game and assign a letter (send OK letter message)
		initialize modules to begin game play
		return true/false

    handleSpectator 
		takes in a message and validates it
		add the spectator to the game
			ensure there isn't already a spectator
			if there is, kick them out and let this person spectate
		initialize modules to begin game spectating
		return true/false

    handleKey
		takes in a message
		checks which key was pressed
			if one of the valid ones:
				handle that keys by calling a helper function
				return true
			invalid key
				return false
		*for all possible keys, see the requirments specs	

	spot_is_open
		null checks the parameters
		checks the type of spot at a location
			check if that spot type is valid
		return true/false
	
	update_display
		validate parameters
			initialize copy of original map
			loop through players and update locations
				get point coordinates
				find map spots and update them
			loop through gold piles and update locations
				get point coordinates
				find spot in map string and update it
		refresh game's map

	distribute_gold
		initialize number of piles to be created (should be between GoldMinNumPiles and GoldMaxNumPiles)
  		initialize variable to hold total gold distributed
		initiate an array of gold piles to hold pile locations and gold amounts; and loop through
			find random location to set up a gold pile
			compute random amount of gold to drop
			add location and amount to pile
			add pile to game array
		assign total amount of gold distributed to game

	player_on_gold
		validate parameters
		loop through all piles
			get location of the pile
			compare coordinates and return pile if they match

	collect_gold
		validate parameters
		transfer the gold in the pile to the player's purse
		subtract it from goldRemaining
		send gold message to player
		delete pile

	sendGrid 
	 	put together a message that includes nrows and ncols
		make sure that the section of the grid being sent considers where the client is
		send the grid the client

    sendGold
		put together a message that includes # nuggets, purse count, remaining gold
		check if has been updated, if so send to clients

    sendDisplay
		retrieve the text version of the map (depends on if spectator or client for amount)
		put it into a string
		sent to client(s)
