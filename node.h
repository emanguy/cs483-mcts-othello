/*
 * This file contains the method definitions for MCTS node code.
 */

#ifndef NODE_H
#define NODE_H

#include "board.h"

typedef struct Node
{
	int id;
	Node* parent;
	struct board currBoard;
	Node** children;
	int* childMoves;
	int numChildren;
	int numWins;
	int numSimulations;
} node;

void initializeRoot(node* root, struct board *boardState);
node* expand(node *parentNode, int *action, int id);
void deconstructTree(node* rootNode);
void deleteNodes(node* currNode);

#endif

