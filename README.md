# Stefan MAGIRESCU

## SD - tema 2 - GameTree

In designing the game tree I used an array of pointers to each node's children. Generally, I designed new functions for each of the exercises, but I occasionaly borrowed functions from the other exercises.

### Exercise no. 1:

- **read** reads from input file
- **printTicTac** writes to output file
- **printTree** accesory function, prints to console
- **checkLvl** checks where I am in the tree, by counting spaces in the tic tac toe board
- **checkWin** checks which player wins
- **tictacMaker** generates move
- **newNode** allocates memory for new node in tree
- **insert** builds the tree
- **freeTree** frees allocated memory

### Exercise no. 2:

- **insert2** builds the tree
- **TorF** goes through tree and assigns T or F to each node, judging by its children
- **printTorF** writes to output file

### Exercise 3:

- **freeTreeMin** frees allocated memory
- **parse** parses the input text
- **minimaxTreeBuilder** builds the tree breadth first, using a queue
- **minimax** applies the MiniMax algorithm to each node
- **printMinimax** writes to output file

### General functions:

- **ProbOne** deals with exercise no. 1
- **ProbTwo** deals with exercise no. 2
- **ProbThree** deals with exercise no. 3
- **initQ** allocates memory for root node
- **isEmpty** checks if queue is empty
- **enqueue** adds element to queue
- **dequeue** deletes element from queue

For more on how the functions work, check comments.
