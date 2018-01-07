# mancala-bot
Simple bot, which calculates the best move for mancala.

## Commands:

```
START C T
```

C - side of board (W - starting player, B - second player)

T - time limit in seconds for processing the possible moves

returns 1 if all went well

```
MOVE H1 H2 H3 H4 H5 H6 H7 H8 H9 H10 H11 H12 H13 H14
```
H1..H14 - number of marbles in holes
```
Layout:
H14 || H13 | H12 | H11 | H10 | H9 | H8
        H1 | H2  | H3  | H4  | H5 | H6 || H7
```
returns 1-6 for player W, 8-13 for player B

```
STOP
```
ends the program
