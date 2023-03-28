# Nuggets

Nuggets is a multiplayer exploration game. Players navigate a map of rooms and passages in search of gold nuggets, which are randomly distributed in piles within the rooms. The goal of the game is to collect more gold nuggets than any other player in the game.

A game server maintains all information about the game, and one or more game clients display the game to a user, either as players or spectators.

## Installation

To download and build Nuggets locally, open a Terminal window and run the following commands:

```bash
git clone https://github.com/marklekina/nuggets.git
cd nuggets
make && cd bin
```

This will download the source code and build the client and server binaries. The binaries will be located in the bin subdirectory.

## Usage
### Server

To launch the server, run the following command in a Terminal window:

```bash
./server 2> server.log ../maps/{PATH_TO_MAPFILE} [seed]
```

- Replace `{PATH_TO_MAPFILE}` with the path to a map file located in the `maps` directory.
- The `seed` parameter is optional and can be used to replicate a given game's behavior.
- The server will output a message announcing that it is listening for contact at a specific port.

```
./server: ready for contact at port 43210
```

### Client
To start the client, run the following command in a Terminal window:

```bash
./client 2> client.log host port [player]
```

- Replace `host` with the hostname or IP address of the server.
- Replace `port` with the port number from which the server will be listening.
- The `player` parameter is optional and determines whether the client joins the game as a player or spectator.

This command allows you to run the client and server on the *same* host or *different* machines. The two options listed below allow you to launch the client *only* if both the server and client are on the *same* machine. See [scripts](src/scripts/README.md) for more details.

```bash
../src/scripts/runclient.sh . [player]
```

- This script automatically detects the host name and port number, and starts the client with the optional `player` parameter as a player or spectator. 

```bash
../src/scripts/runbots.sh .
```

- This other script launches the client in `bot-mode`, with 26 bot player clients running in the background and a spectator client on the foreground displaying the game.

**Spectator**

```
Spectator: 250 nuggets unclaimed.
  +----------+
  |..........|                 +---------+
  |..........####              |.........|                +-------+
  |..........|                 +-----#---+                |.*....*|
  +---------#+                       #              #######.......|
            #                        #              #     +---#---+
            #  +-----------+         #     +--------#+        #
            ####.*.........##############  |.........|        #
               |...........|            #  |.........|        #
               +-----------+            ####......*..|        #
                                           +----#----+        #
                                                #    +--------#--+
     +---------------------------------+        #    |...........|
     |......*...*....*.......*.........|        ######...........|
     |.................................|        #    |...........|
     |......+---------------+....*.....|        #    |...........|
     |......|               |..........##########    +-----------+
     |......|               |..........|
     |......|               |..........|
     |......|               |..........|
     +------+               +----------+
```

**Player**

```
Player A has 0 nuggets (250 nuggets unclaimed).












     +------------------
     |......*...*...
     |..........
     |......+
     |..@...|
     |......|
     |......|
     |......|
     +------+

```

## Gameplay

The client plays the game by passing valid keystrokes to the server, to which the server responds appropriately. Below is the list of all valid keys:

**Spectator keystrokes:**
 * `q` quit the game.

**Player keystrokes:**

 * `q` quit the game.
 * `h` move left
 * `l` move right
 * `j` move down
 * `k` move up	
 * `y` move diagonally up and left
 * `u` move diagonally up and right
 * `b` move diagonally down and left
 * `n` move diagonally down and right

Movements are appropriately restricted by room and passage walls. For each move key, the corresponding capitalized character will move automatically and repeatedly in that direction, until a barrier is encountered.

## Acknowledgements

I initially worked on a primitive (and buggier) version of this game for a group project in the Dartmouth *COSC 050: Software Design and Implementation* course. A year later, I decided to rewrite the game from scratch. Many thanks to the course instructors for the inspiration, guidance and material help (see the [support](src/support) and [maps](maps) directories).

## License

This project is licensed under the MIT License - see the `LICENSE` file for details.