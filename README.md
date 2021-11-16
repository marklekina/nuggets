# Nuggets

This repository contains the code for the CS50 "Nuggets" game, in which players explore a set of rooms and passageways in search of gold nuggets.
The rooms and passages are defined by a *map* loaded by the server at the start of the game.
The gold nuggets are randomly distributed in *piles* within the rooms.
Up to 26 players, and one spectator, may play a given game.
Each player is randomly dropped into a room when joining the game.
Players move about, collecting nuggets when they move onto a pile.
When all gold nuggets are collected, the game ends and a summary is printed.

## Materials provided

See the [support library](support/README.md) for some useful modules.

See the [maps](maps/README.md) for some draft maps.

## Assumptions

We also were provided with a client since we were a group of three. 
We  ignored implementing the seed portion of the project but did include it in the comments of the server. 
On a smaller level, we assumed for a few of our getters and setters that the given parameters were not NULL. 

## Issues
Note! We have debugged all files in a way such that they compile without warnings/errors. There is a Makefile for the server and one for the modules in the common directory. 

See the README.md in project-palmers-posse/common for an outline of issue and how we would have implemented/fixed them with more time.
Additionally, our group's map file we made can be found in the project-palmers-posse/maps directory and is called palmer-posse-maps. It is an interpretation of the Taj Mahal.
