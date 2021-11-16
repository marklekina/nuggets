# Nuggets

This directory contains the common code for the CS50 "Nuggets" game; including the game data structure, player data structure, grid data structure and XYZ message handling module.

Together, these programs enable the functionality of game play.

## Materials provided

See the [support library](support/README.md) for some useful modules. Examples include, message.h, mem.h, file.h

See the [maps](maps/README.md) for some draft maps.

## Assumptions

We also were provided with a client since we were a group of three. 
We  have code for the visiblity and gold distribution, but haven't had time to implement it perfectly.

On a smaller level, we assumed for a few of our getters and setters that the given parameters were not NULL. The greatly reduced pointer errors/warnings.

## Issues
See implementation spec for the issues we ran into during this lab

### Visibility
We have a working and tested visibility function, which we tested by passing it random room spots (which are substitutes for a player's location), and the game's grid. and the function returned what would be visible from that location in a satisfactory manner.

`char* compute_visibility(point_t* point, grid_t* grid);`

NB: the `char*` string returned is of the same length as the game's map, with blanks for spots that are not visible to the player, and appropriate spots for the visible areas.

`point_t* point` refers to a player's location in the grid.
`grid_t* grid` is a struct that holds the player's map and the size of the grid.

Our primary issue with the visibility function is that we haven't integrated it into the `update_dislay` function, which would in turn send the updated display to the player using `send_display`.

Lekina dynamically tested the code throughout the implementation and used this as the line of code to make sure it worked: 
char* display = compute_visibility(get_location(player), get_grid(game));
puts(display); 

### Gold
We have working gold functions that distribute gold randomly, alloting them a random amount within the global variables, and change gold based on the amount recieved. For each pile of gold, we created a pile_t that is touched on briefly in the Implementation spec. This was our way of keeping track of the actual "gold." Within each game, we include an array of the piles in the game as well as a running total for the gold that is distributed and the gold remaining. 

In our XYZ.c, we have function sendGold, distribute_gold, collect_gold, and player_on_gold that help us implement the gold. 

If implemented correctly, sendGold would send the gold collected, amount in player's purse, and total gold remaining and send it to the player.
`bool sendGold(game_t* game, player_t* player, int collected);`

If implemented correctly, distribute gold will randomly distribute gold around the grid and add the gold piles to the game. 
`void distribute_gold(game_t* game)`

If implemented correctly, this would transfer the pile of gold to the player's purse.
`void collect_gold(game_t* game, player_t* player, pile_t* pile)`

If implemented correctly, player_on_gold would take in a player's location and see if the location of the gold is the same as that of the pile's.
`pile_t* player_on_gold(game_t* game, int x, int y) `

In our game, we have add_pile and change_remaining_gold.


If implemented correctly, this would add the pile to the running array of gold piles in the game. 
`bool add_pile(game_t* game, pile_t* pile)`

If implemented correctly, this would subtract the given amount of gold from the game's gold remaining variable. 
`void change_remaining_gold(game_t* game, int amount)`

All of these functions compile successfully and if we had more time, we believe this would be easily implemented into our code and create a game that had a successful gold aspect. This implementation would take place in the XYZ.c and server.c

### Player (population and movement)
We implemented a player data structure;

`player_t* player`: holds player’s name, type, location on the map, visibility string, client address, purse, and letter

At the time of submission for this project, the Nuggets game simply prints a map, logs keys pressed but doesn’t populate the map. This is due to issues with our message handler functions not issues with the player struct.

The player module is mostly for storage. It is composed completely of “new”, delete, getter and setter functions.

In terms of implementation, we mostly refer to where the player moves.

Player Movement:
Movement of the player is primarily handled in the handleKey function in the message (XYZ) module. Essentially, handleKey checks for the key pressed, creates a new location in accordance with that key, and sets the players location to the new location. This logic is where all the getters and setters come in handy from player.
The next aspect of movement requires visibly moving the player. This is done in `update_display(game_t* game)`, also in the XYZ message module. In addition to updating the gold stats, this function loops through all the players in the game and updates their locations if they have moved. Due to time constraint, we were not able to integrate this function into the server (where it should go after each handleKey call). Given more time this is one of the things we would do.

### Game Over Summary 
We made a module called print results which would print the leaderboard once the game ends. Given our player and game module it would be easy to do this. 

### Valgrind
We were very careful to code defensively and look for memory errors. Since we couldn't actually play the game, testing for this was impossible, but if we had more time we would have done this. 