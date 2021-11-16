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