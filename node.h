#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include "board.h"


struct node{
	struct node* parent;		// Pointer to parent node
	struct board board;	// State of board at this node
	int* childMoves;	// How to reach child nodes
	int numChildren;	// Num children nodes 
	int numWins;		// Num wins from this node
	int numSimulations;	// Num simulations from this node
	struct node** children;		// Pointers to children
};
typedef struct node node;


// Use to create first parent node
 //const node DEFAULT_VALUES = {NULL, {}, {}, 0, 0, 0};

void initializeRoot(node* root, struct board *boardState);

/* Dynamically allocate a new struct node.
 * Attach it to the provided parent node.
 * Do action on board.
 * Return a pointer to the new node.
 * Uses copyBoard() to copy previous board's data into the new node.
 */
node* expand(node* parentNode, int* action);


/* Recursively free all of the tree nodes starting at the provided root */
void deconstructTree(node* currNode);

#endif
