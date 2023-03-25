# Support

 This directory contains external dependencies for the Nuggets *client*, *server* and the server *modules*. These modules are compliled into a common library `support.a`, which is required to compile sources in `main` and `modules`.

| Module  | Function     |
| :------ | :------------ |
| `log` | provides a simple way to write runtime information to a log file. |
| `message`  | provides a message-passing abstraction between clients and the server via UDP. |
| `file`| provides file utilities - reading tokens, lines, or an entire file.|
| `mem`  | provides memory management utilities, *i.e.*, wrappers for `malloc` and `free`.   |
| `tester`  | provides testing macros for unit testing server modules.  |

### Compilation and usage

To compile the support library,
```bash
make support.a
```

See header files `./*.h` for detailed interfaces and the [main](../main/Makefile) & [modules](../modules/Makefile) Makefiles for use cases.

### Bot mode

We also have a `runbots.sh` script that launches the client in **bot** mode. To launch bot mode,
```bash
./runbots.sh bin_dir
```
where `bin_dir` is the location of the `client` binary after compilation.