# CS50 Nuggets
## Design Spec
### Palmer's Posse, Fall21

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes x, y, z modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.
## Server
### User interface

See the requirements spec for the command-line interface. There is no interaction with the user.
> Because we are a three person group, we do not need to worry about this.

### Inputs and outputs
Command-line arguments are not 'input'.

INPUTS: there are three types of messages that the client can send the server
 - `PLAY` (initiate play)
 - `SPECTATE` (initiate spectating)
 - `KEY` (movement on the grid by the user/quitting)

OUTPUTS:
 - `GRID` nrows ncols (build the grid with the size)
 - `GOLD` n p r (number of nuggets, what is in the players purse and the remaining gold)
 - `DISPLAY` \nstring (a printable string version of the map)

### Functional decomposition into modules
Parameters are not yet set, see Implementation Spec for that.

`main` - sets up modules listed below and runs the game

`parseArgs` - parse the command line and validate arguments, perhaps in a helper function

`parseMessage` - parsing the messages from the client

`initiateNetwork` - direct communication with the client, sets up game, calls handleMessage()

`handleMessage` - will be composed of many helper functions, that will handle the command the client has asked for

`loadMap` - load the map

`disperseGold` - initialize the gold and place it around the map

`updateGame` - after any keystroke is made, update the game

`printResults` - print the leaderboard after the game ends

 
### Pseudo code for logic/algorithmic flow

        parseArgs
            validate arg count
	        validate arg type
        	initializes variables
	
        parseMessage 
	        validate Message type 
            validate Message
	

        initiateNetwork
			initialize the server ports/communication channels
	        wait for client to connect
			receives messages from the client
			call parseMessage

        handleMessage
			receive a message from parseMessage
	        call the correct helper function to deal with the message

        loadMap
        	open text file and read it
        	encode information in the text file to the client

        disperseGold
			use the minGold/maxGold constants
	        determine how the gold will be split up with a random number generator 
	        populate on the grid

        updateGame
			check the gold
			check where user is on grid
			refresh display
			send message across to clients

        printResults
			print the results from the game
	        see Requirements Spec for detail

### Major data structures
 `struct game`: see detail in Implementation Spec

 `textfile map`: essentially a text file; most provided; must create one more

 `struct grid`: see detail in Implementation Spec

 `struct player`: see detail in Implementation Spec
---

## XYZ module

This module represents the type of message XYZ

### Functional decomposition

`handlePlay` - receive a message that a client wants to play and sets them up accordingly

`handleSpectator` - receive a message that a client wants to spectate and sets them up accordingly

`handleKey` - receive a message that the client input a key and handle that keystoke

`sendGrid` - send nrows and ncols (the grid size) to the client participating

`sendGold` - send the number of nuggets, the amount in the player's purse and the amount of gold remaining

`sendDisplay` - will send a string interpretation of the map

### Pseudo code for logic/algorithmic flow

    handlePlay
    	takes in a message
		ensure max number of players has not been reached
		tells us there is a client we have to give the correct letter assigned name to
		initialize modules to begin game play

    handleSpectator 
		takes in a message
		tells us there is a spectator
		ensure there isn't already a spectator
		if there is, kick them out and let this person spectate
		initialize modules to begin game spectating

    handleKey
		takes in a message
		tells us there is a spectator
		ensure the key was one of the ones we watch for
		handle that keys by calling a helper function

		*for all possible keys, see the requirments specs	

    sendGrid 
	 	put together a message that includes nrows and ncols
		make sure that the section of the grid being sent considers where the client is
		send the grid the client

    sendGold
		put together a message that includes # nuggets, purse count, remaining gold
		check if has been updated, if so send to clients

    sendDisplay
		retrieve the text version of the map
		put it into a string
		sent to client(s)
