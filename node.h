/*
 * This file contains the method definitions for MCTS node code.
 */

#ifndef NODE_H
#define NODE_H

#include "board.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct Node
{
	Node* parent;
	struct board board;
	Node** children;
	int* childMoves;
	int numChildren;
	int numWins;
	int numSimulations;
} node;

void initializeRoot(node* root, struct board *boardState);
node* expand(node *parentNode, int *action);
void deconstructTree(node* rootNode);
void deleteNodes(node* currNode);

#endif

