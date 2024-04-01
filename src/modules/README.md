# Modules

 This directory contains information models for the game that are managed by the *server*. Each of these modules contains a `struct` and functions that perform operations on that particular struct. The modules are compliled into a common library `modules.a`, which is required to compile the *server* in `main`. All modules (except `point`) also depend on other modules in this directory to compile.

| Module  | Function     |
| :------ | :------------ |
| `point` | represents a `(x,y)` coordinate on the game's map. |
| `pile`  | holds the location of a 'gold pile' and the amount of gold it contains. |
| `player`| holds player information *i.e.,* name, gold collected, location, etc.|
| `grid`  | represents the game's map, *i.e.,* a map string and a matching array of each grid point in the map.  |
| `game`  | keeps track of all game state, *i.e.,* players, gold piles, and the grid. |

See header files `./*.h` for detailed interfaces and the [main](../main/Makefile) & [modules](Makefile) Makefiles for use cases.
