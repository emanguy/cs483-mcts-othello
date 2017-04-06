/*
 * This file contains the method definitions for MCTS node code.
 */

#ifndef NODE_H
#define NODE_H

#include "board.h"

struct node
{
	struct node *parent;
	struct board board;
	struct board *children;
	int *childMoves;
	int numChildren;
	int numWins;
	int numSimulations;
};

void initNode(struct node *allocatedNode, struct board *boardState);
struct node *expand(struct node *parentNode, int *action);
void deconstructTree(struct node *rootNode);

#endif

