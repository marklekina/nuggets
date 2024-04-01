# Tests

This directory contains unit tests for each of the server modules desribed in [modules](../modules/README.md). Each unit test is named by appending `test` to the module name:

| Module  | Unit test     |
| :------ | :------------ |
| `point` | `pointtest.c` |
| `pile`  | `piletest.c`  |
| `player`| `playertest.c`|
| `grid`  | `gridtest.c`  |
| `game`  | `gametest.c`  |

## Running Tests

To run *unit* tests,

```bash
  make test
```

To run *memory leak* tests,

```bash
  make leaks
```
