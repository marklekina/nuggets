# Scripts

This sub-directory contains helper shell scripts.

## runbots.sh

This script launches 26 bot player clients as background processes, and a single spectator client in the foreground. The bot clients send random movement keystrokes to the server, which in most cases results in the collection of all gold in the game.

```bash
./runbots.sh $BIN
```

- `$BIN` is the location of the *client* binary after compilation.

## runclient.sh

This script launches a spectator or player clients depending on whether you pass the optional player parameter. The main utility of the script is that, like runbots.sh above, it automates the process of launching the client by parsing the server's log for the port number and running a command to fetch the hostname.

```bash
./runclient.sh $BIN [player]
```

- `$BIN` is also the location of the client binary after compilation.
- The `player` parameter is optional.

***Note***: These scripts only work when the *server* and *client* are on the same host. Also, since they need to parse the server log, the server needs to redirect `stdout` to a log file in `$BIN` for the scripts to detect the log file and run successfully.
